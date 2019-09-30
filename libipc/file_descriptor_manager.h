#ifndef LIBIPC_FILE_DESCRIPTOR_MANAGER_H
#define LIBIPC_FILE_DESCRIPTOR_MANAGER_H

#include <set>

#define FileDescriptor_MAXFD 256
#define FileDescriptor_NOTSET (-1)


#ifdef __cplusplus
extern "C" {
#endif
int internal_receive_fd(void* fileDescriptorMapping, int fd);
void internal_close_fd(int local_fd);

int __ipc_replacement_internal_receive_fd(void* fileDescriptorMapping, int fd);
void __ipc_replacement_internal_close_fd(int local_fd);
#ifdef __cplusplus
}
#endif


class FileDescriptorMapping {
	static std::set<FileDescriptorMapping *> mappings;

	int local_to_remote_fd[FileDescriptor_MAXFD]{};

	int sockets[2]{-1, -1};
	int socket = -1;
public:
	FileDescriptorMapping();

	~FileDescriptorMapping();

	void setParent();

	void setLibrary();

	int mapToOtherSide(int fd);

	int fromOtherSide(int remote_fd);

	void onCloseLocal(int fd, bool propagate=true);

	static void onClose(int fd, bool propagate);
};

#endif //LIBIPC_FILE_DESCRIPTOR_MANAGER_H
