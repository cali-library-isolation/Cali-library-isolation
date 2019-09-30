#include "archive-wrapper.h"

libarchive::Archive::Archive(const std::string &filename) {
	archive = archive_read_new();
	archive_read_support_filter_all(archive);
	archive_read_support_format_all(archive);
	int rc = archive_read_open_filename(archive, filename.c_str(), 409600);
	if (rc != ARCHIVE_OK)
		throw std::runtime_error("Could not open archive");
}

libarchive::ArchiveHeaderIterator libarchive::Archive::begin() {
	return ++ArchiveHeaderIterator(*this);
}

libarchive::ArchiveHeaderIterator libarchive::Archive::end() {
	return ArchiveHeaderIterator(*this);
}

