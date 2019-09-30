/* Copyright (C) 1999-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Jaeger <aj@suse.de>, 1999 and
		  Jakub Jelinek <jakub@redhat.com>, 1999.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */


int process_elf32_file (const char *file_name, const char *lib, int *flag,
			unsigned int *osversion, char **soname,
			void *file_contents, size_t file_length);
int process_elf64_file (const char *file_name, const char *lib, int *flag,
			unsigned int *osversion, char **soname,
			void *file_contents, size_t file_length);

/* Read an unsigned leb128 value from P, store the value in VAL, return
   P incremented past the value.  We assume that a word is large enough to
   hold any value so encoded; if it is smaller than a pointer on some target,
   pointers should not be leb128 encoded on that target.  */
static unsigned char *
read_uleb128 (unsigned char *p, unsigned long *val)
{
  unsigned int shift = 0;
  unsigned char byte;
  unsigned long result;

  result = 0;
  do
    {
      byte = *p++;
      result |= (byte & 0x7f) << shift;
      shift += 7;
    }
  while (byte & 0x80);

  *val = result;
  return p;
}

#define ATTR_TAG_FILE          1
#define ABI_VFP_args          28
#define VFP_ARGS_IN_VFP_REGS   1

/* Check the ABI in the ARM attributes. Search through the section
   headers looking for the ARM attributes section, then check the
   VFP_ARGS attribute. */
static int is_library_hf(const char *file_name, void *file_contents, size_t file_length)
{
  unsigned int i;
  ElfW(Ehdr) *ehdr = (ElfW(Ehdr) *) file_contents;
  ElfW(Shdr) *shdrs;

  shdrs = file_contents + ehdr->e_shoff;
  for (i = 0; i < ehdr->e_shnum; i++)
    {
      if (SHT_ARM_ATTRIBUTES == shdrs[i].sh_type)
        {
	  /* We've found a likely section. Load the contents and
	   * check the tags */
	  unsigned char *p = (unsigned char *)file_contents + shdrs[i].sh_offset;
	  unsigned char * end;

	  /* Sanity-check the attribute section details. Make sure
	   * that it's the "aeabi" section, that's all we care
	   * about. */
	  if (*p == 'A')
            {
	      unsigned long len = shdrs[i].sh_size - 1;
	      unsigned long namelen;
	      p++;

	      while (len > 0)
                {
		  unsigned long section_len = p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
		  if (section_len > len)
		    section_len = len;

		  p += 4;
		  len -= section_len;
		  section_len -= 4;

		  if (0 != strcmp((char *)p, "aeabi"))
                    {
		      p += section_len;
		      continue;
                    }
		  namelen = strlen((char *)p) + 1;
		  p += namelen;
		  section_len -= namelen;

		  /* We're in a valid section. Walk through this
		   * section looking for the tag we care about
		   * (ABI_VFP_args) */
		  while (section_len > 0)
                    {
		      unsigned long val = 0;
		      unsigned long tag;
		      unsigned long size;

		      end = p;
		      tag = (*p++);

		      size = p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
		      if (size > section_len)
			size = section_len;
		      p += 4;

		      section_len -= size;
		      end += size;
		      if (ATTR_TAG_FILE != tag)
                        {
			  /* ignore, we don't care */
			  p = end;
			  continue;
                        }
		      while (p < end)
                        {
			  p = read_uleb128 (p, &tag);
			  /* Handle the different types of tag. */
			  if ( (tag == 4) || (tag == 5) || (tag == 67) )
                            {
			      /* Special cases for string values */
			      namelen = strlen((char *)p) + 1;
			      p += namelen;
                            }
			  else
                            {
			      p = read_uleb128 (p, &val);
                            }
			  if ( (tag == ABI_VFP_args) && (val == VFP_ARGS_IN_VFP_REGS) )
			    return 1;
                        }
                    }
                }
            }
        }
    }
  return 0;
}

/* Returns 0 if everything is ok, != 0 in case of error.  */
int
process_elf_file (const char *file_name, const char *lib, int *flag,
		  unsigned int *osversion, char **soname, void *file_contents,
		  size_t file_length)
{
  ElfW(Ehdr) *elf_header = (ElfW(Ehdr) *) file_contents;
  int ret;

  if (elf_header->e_ident [EI_CLASS] == ELFCLASS32)
    {
      Elf32_Ehdr *elf32_header = (Elf32_Ehdr *) elf_header;

      ret = process_elf32_file (file_name, lib, flag, osversion, soname,
				file_contents, file_length);

      if (!ret && EF_ARM_EABI_VERSION (elf32_header->e_flags) == EF_ARM_EABI_VER5)
	{
	  if (elf32_header->e_flags & EF_ARM_ABI_FLOAT_HARD)
	    *flag = FLAG_ARM_LIBHF|FLAG_ELF_LIBC6;
	  else if (elf32_header->e_flags & EF_ARM_ABI_FLOAT_SOFT)
	    *flag = FLAG_ARM_LIBSF|FLAG_ELF_LIBC6;
	  else if (is_library_hf(file_name, file_contents, file_length))
	    *flag = FLAG_ARM_LIBHF|FLAG_ELF_LIBC6;
	  else
	    /* We must assume the unmarked objects are compatible
	       with all ABI variants. Such objects may have been
	       generated in a transitional period when the ABI
	       tags were not added to all objects.  */
	    *flag = FLAG_ELF_LIBC6;
	}
    }
  else
    {
      ret = process_elf64_file (file_name, lib, flag, osversion, soname,
				file_contents, file_length);
      /* AArch64 libraries are always libc.so.6+.  */
      if (!ret)
	*flag = FLAG_AARCH64_LIB64|FLAG_ELF_LIBC6;
    }
  return ret;
}

#undef __ELF_NATIVE_CLASS
#undef process_elf_file
#define process_elf_file process_elf32_file
#define __ELF_NATIVE_CLASS 32
#include "elf/readelflib.c"

#undef __ELF_NATIVE_CLASS
#undef process_elf_file
#define process_elf_file process_elf64_file
#define __ELF_NATIVE_CLASS 64
#include "elf/readelflib.c"
