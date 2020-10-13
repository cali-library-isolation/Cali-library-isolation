#ifndef LIBIPC_FILE_DESCRIPTOR_MANAGER_H
#define LIBIPC_FILE_DESCRIPTOR_MANAGER_H

#include <set>

#define FileDescriptor_MAXFD 256
#define FileDescriptor_NOTSET (-1)


#ifdef __cplusplus
extern "C" {
#endif
int internal_receive_fd(void* fileDescriptorMapping, int fd, int pid);
void internal_close_fd(int local_fd, int pid);

int __ipc_replacement_internal_receive_fd(void* fileDescriptorMapping, int fd, int pid);
void __ipc_replacement_internal_close_fd(int local_fd, int pid);
#ifdef __cplusplus
}
#endif


class FileDescriptorMapping {
	static std::set<FileDescriptorMapping *> mappings;

	int local_to_remote_fd[FileDescriptor_MAXFD]{};
	int local_to_remote_pid[FileDescriptor_MAXFD]{}; // 0 = from me

	int sockets[2]{-1, -1};
	int socket = -1;
public:
	FileDescriptorMapping();

	~FileDescriptorMapping();

	void setParent();

	void setLibrary();

	int mapToOtherSide(int fd);

	int fromOtherSide(int remote_fd, int pid);

	void onCloseLocal(int fd, bool propagate=true);

	static void onClose(int fd, bool propagate);
};


int getFreeFiledescriptor();


/**
 * Create a pipe /tmp/stderr and pipe our stderr output there
 */
void stderr_to_pipe();

#endif //LIBIPC_FILE_DESCRIPTOR_MANAGER_H
