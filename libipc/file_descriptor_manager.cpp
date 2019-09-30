#include "file_descriptor_manager.h"
#include "file_descriptor_send.h"
#include "hook.hpp"
#include "shm_malloc.h"
#include "debug.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

real_function(open, int(
		const char *, int, mode_t));
real_function(open64, int(
		const char *, int, mode_t));
real_function(close, int(int));

int open(const char *pathname, int flags, mode_t mode) asm("open");

int open64(const char *pathname, int flags, mode_t mode) asm("open64");

int close(int fd) asm("close");


int open(const char *pathname, int flags, mode_t mode) {
	int fd = real(open)(pathname, flags, mode);
	shm_debug("open(\"%s\", o%o, %d);  // from %d  => %d\n", pathname, flags, mode, getpid(), fd);
	O_RDONLY;
	return fd;
}

int open64(const char *pathname, int flags, mode_t mode) {
	int fd = real(open64)(pathname, flags, mode);
	shm_debug("open64(\"%s\", o%o, %d);  // from %d  => %d\n", pathname, flags, mode, getpid(), fd);
	return fd;
}

int close(int fd) {
	int result = real(close)(fd);
	FileDescriptorMapping::onClose(fd, true);
	return result;
}



std::set<FileDescriptorMapping *> FileDescriptorMapping::mappings;


FileDescriptorMapping::FileDescriptorMapping() {
	for (int &fd : local_to_remote_fd) {
		fd = FileDescriptor_NOTSET;
	}
	local_to_remote_fd[0] = 0;
	local_to_remote_fd[1] = 1;
	local_to_remote_fd[2] = 2;

	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, sockets) < 0) {
		perror("socketpair");
	}

	mappings.insert(this);
}

FileDescriptorMapping::~FileDescriptorMapping() {
	if (socket >= 0) real(close)(socket);
	mappings.erase(this);
}

int FileDescriptorMapping::mapToOtherSide(int fd) {
	if (local_to_remote_fd[fd] != FileDescriptor_NOTSET)
		return local_to_remote_fd[fd];
	// fprintf(stderr, "[%d] Send %d to remote...\n", socket, fd);
	// move to remote end
	sendfd(socket, fd);
	int remote_fd = __ipc_replacement_internal_receive_fd(this, fd);
	local_to_remote_fd[fd] = remote_fd;
	debug_printf("[%d] Local %d Remote %d.\n", socket, fd, remote_fd);
	return remote_fd;
}

int FileDescriptorMapping::fromOtherSide(int remote_fd) {
	// fprintf(stderr, "[%d] recvfd...\n", socket);
	int local_fd = recvfd(socket);
	// fprintf(stderr, "[%d] recvfd => %d\n", socket, local_fd);
	local_to_remote_fd[local_fd] = remote_fd;
	// fprintf(stderr, "[%d] Local %d Remote %d (received)\n", socket, local_fd, remote_fd);
	return local_fd;
}

int internal_receive_fd(void* fileDescriptorMapping, int fd) {
	return ((FileDescriptorMapping*) fileDescriptorMapping)->fromOtherSide(fd);
}

void FileDescriptorMapping::onCloseLocal(int fd, bool propagate) {
	if (fd >= 0 && local_to_remote_fd[fd] != FileDescriptor_NOTSET) {
		int remote_fd = local_to_remote_fd[fd];
		local_to_remote_fd[fd] = FileDescriptor_NOTSET;
		debug_printf("[%d] Close %d (remote %d) ...\n", socket, fd, remote_fd);
		// close and unset on remote end
		if (propagate) {
			__ipc_replacement_internal_close_fd(remote_fd);
		}
	}
}

void FileDescriptorMapping::onClose(int fd, bool propagate) {
	if (fd >= 0)
		for (auto mapping: mappings)
			mapping->onCloseLocal(fd, propagate);
}

void FileDescriptorMapping::setParent() {
	socket = sockets[0];
	real(close)(sockets[1]);
}

void FileDescriptorMapping::setLibrary() {
	socket = sockets[1];
	real(close)(sockets[0]);
}

void internal_close_fd(int local_fd) {
	FileDescriptorMapping::onClose(local_fd, false);
	real(close)(local_fd);
}
