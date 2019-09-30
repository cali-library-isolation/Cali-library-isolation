/* Mutex attribute type.  Generic version.
   Copyright (C) 2002, 2008 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef _BITS_MUTEX_ATTR_H
#define _BITS_MUTEX_ATTR_H	1

enum __pthread_mutex_protocol;
enum __pthread_process_shared;
enum __pthread_mutex_type;

/* This structure describes the attributes of a POSIX mutex
   attribute.  */
struct __pthread_mutexattr
{
  int __prioceiling;
  enum __pthread_mutex_protocol __protocol;
  enum __pthread_process_shared __pshared;
  enum __pthread_mutex_type __mutex_type;
};

/* Attributes for a recursive mutex.  */
extern const struct __pthread_mutexattr __pthread_errorcheck_mutexattr;
extern const struct __pthread_mutexattr __pthread_recursive_mutexattr;

#endif /* bits/mutex-attr.h */
