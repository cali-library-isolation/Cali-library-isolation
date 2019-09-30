#ifndef LLVMREWRITER_ARCHIVE_WRAPPER_H
#define LLVMREWRITER_ARCHIVE_WRAPPER_H

#include <string>
#include <archive.h>
#include <stdexcept>
#include <archive_entry.h>
#include <vector>
#include <assert.h>
#include <iostream>

namespace libarchive {

	class ArchiveHeaderIterator;

	class Archive {
	public:
		struct archive *archive;

		explicit Archive(const std::string &filename);

		~Archive() {
			archive_read_free(archive);
		}

		ArchiveHeaderIterator begin();

		ArchiveHeaderIterator end();

	};


	class ArchiveHeader {
		Archive &archive;
		struct archive_entry *header = nullptr;

	public:
		explicit ArchiveHeader(Archive &archive) : archive(archive) {}

		std::string name() const {
			return std::string(archive_entry_pathname(header));
		}

		std::string read() {
			auto size = (size_t) archive_entry_size(header);
			std::vector<char> buffer(size);
			size_t pos = 0;
			size_t read = 0;
			while ((read = archive_read_data(archive.archive, buffer.data() + pos, size - pos)) > 0) {
				pos += read;
				// std::cout << "read: " << pos << " / " << size << " (" << read << " now)" << std::endl;
			}
			return std::string(buffer.data(), buffer.size());
		}

		friend class ArchiveHeaderIterator;
	};


	class ArchiveHeaderIterator : std::iterator<std::forward_iterator_tag, ArchiveHeader> {
		ArchiveHeader archiveHeader;

	public:
		explicit ArchiveHeaderIterator(Archive &archive) : archiveHeader(archive) {}

		// Pre-increment
		ArchiveHeaderIterator &operator++() {
			// assert(header != nullptr && "Out-of-bounds iterator increment!");
			int r = archive_read_next_header(archiveHeader.archive.archive, &archiveHeader.header);
			if (r == ARCHIVE_EOF) archiveHeader.header = nullptr;
			else if (r != ARCHIVE_OK) throw std::runtime_error("Can't read header!");
			return *this;
		}

		// two-way comparison: v.begin() == v.cbegin() and vice versa
		bool operator==(const ArchiveHeaderIterator &rhs) const {
			return archiveHeader.header == rhs.archiveHeader.header;
		}

		bool operator!=(const ArchiveHeaderIterator &rhs) const {
			return archiveHeader.header != rhs.archiveHeader.header;
		}

		ArchiveHeader operator*() const {
			return archiveHeader;
		}
	};

}

#endif //LLVMREWRITER_ARCHIVE_WRAPPER_H
