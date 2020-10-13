#include "file_descriptor_manager.h"
#include "file_descriptor_send.h"
#include "shared_memory_manger.h"
#include "hook.hpp"
#include "shm_malloc.h"
#include "debug.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <nsjail/api.h>

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
	//bzero(local_to_remote_pid, sizeof(int) * FileDescriptor_MAXFD);
	for (int &pid: local_to_remote_pid) {
		pid = 0;
	}

	// Create socket and move the file descriptors to high numbers
	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, sockets) < 0) {
		perror("socketpair");
	}
	int fd;
	if ((fd = dup3(sockets[0], getFreeFiledescriptor(), O_CLOEXEC)) > 0){
		real(close)(sockets[0]);
		sockets[0] = fd;
	}
	if ((fd = dup3(sockets[1], getFreeFiledescriptor(), O_CLOEXEC)) > 0){
		real(close)(sockets[1]);
		sockets[1] = fd;
	}
	debug_printf("FileDescriptor socketpair: %d <-> %d", sockets[0], sockets[1]);

	mappings.insert(this);
}

FileDescriptorMapping::~FileDescriptorMapping() {
	if (socket >= 0) real(close)(socket);
	mappings.erase(this);
}

int FileDescriptorMapping::mapToOtherSide(int fd) {
	if (local_to_remote_fd[fd] != FileDescriptor_NOTSET) {
		return local_to_remote_fd[fd];
	}
	// debug_printf(stderr, "[%d] Send %d to remote...", socket, fd);
	// move to remote end
	if (sendfd(socket, fd) < 0)
		return fd;
	int remote_fd = __ipc_replacement_internal_receive_fd(this, fd, getpid());
	local_to_remote_fd[fd] = remote_fd;
	local_to_remote_pid[fd] = 0;
	debug_printf("[%d] Local FD %d Remote %d.", getpid(), fd, remote_fd);
	return remote_fd;
}

int FileDescriptorMapping::fromOtherSide(int remote_fd, int pid) {
	// fprintf(stderr, "[%d] recvfd...\n", socket);
	int local_fd = recvfd(socket);
	// fprintf(stderr, "[%d] recvfd => %d\n", socket, local_fd);
	// fprintf(stderr, "[%d] Local FD %d Remote %d (received from pid %d)\n", getpid(), local_fd, remote_fd, pid);
	local_to_remote_fd[local_fd] = remote_fd;
	local_to_remote_pid[local_fd] = pid;
	return local_fd;
}

int internal_receive_fd(void* fileDescriptorMapping, int fd, int pid) {
	return ((FileDescriptorMapping*) fileDescriptorMapping)->fromOtherSide(fd, pid);
}

void FileDescriptorMapping::onCloseLocal(int fd, bool propagate) {
	if (fd >= 0 && local_to_remote_fd[fd] != FileDescriptor_NOTSET) {
		int remote_fd = local_to_remote_fd[fd];
		local_to_remote_fd[fd] = FileDescriptor_NOTSET;
		debug_printf("[%d] Close %d (remote %d) once received from %d ...", getpid(), fd, remote_fd, local_to_remote_pid[fd]);
		// close and unset on remote end
		if (propagate) {
			// fprintf(stderr, "[%d] trigger __ipc_replacement_internal_close_fd %d (local %d)\n", getpid(), remote_fd, fd);
			__ipc_replacement_internal_close_fd(remote_fd, local_to_remote_pid[fd]);
		}
	}
}

void FileDescriptorMapping::onClose(int fd, bool propagate) {
	// fprintf(stderr, "[%d] close %d (propagate=%d)\n", getpid(), fd, propagate);
	if (shared_memory_has_forked() && fd >= 0)
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

void internal_close_fd(int local_fd, int opened_by_pid) {
	if (shared_memory_has_forked() && local_fd >= 0 && opened_by_pid > 0 && opened_by_pid != getpid()) {
		fprintf(stderr, "[%d] ERR: I should close FD %d, but I'm not %d!\n", getpid(), local_fd, opened_by_pid);
	}
	// fprintf(stderr, "[%d] internal_close_fd %d\n", getpid(), local_fd);
	FileDescriptorMapping::onClose(local_fd, false);
	real(close)(local_fd);
}


static int next_free_filedescriptor = SHARED_MEMORY_FD -1;

int getFreeFiledescriptor() {
	return next_free_filedescriptor--;
}

void stderr_to_pipe() {
	mkfifo("/tmp/stderr", 0666);
	int fd = open("/tmp/stderr", O_WRONLY);
	if (fd < 0) {
		perror("open pipe");
	} else {
		if (dup2(fd, 2) < 0) perror("pipe dup2");
		real(close)(fd);
		fprintf(stderr, "[%d] ----- Execution of %s -----\n>", getpid(), program_invocation_name);
		for (auto i = 0; i < nsjail_argc; i++) {
			fprintf(stderr, " %s", nsjail_argv[i]);
		}
		fprintf(stderr, "\n");
	}
}
