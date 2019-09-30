#ifndef LIBIPC_IPC_COMMUNICATION_H
#define LIBIPC_IPC_COMMUNICATION_H

#include <unistd.h>
#include <sys/sem.h>
#include <vector>
#include <map>
#include "ipc_constants.h"
#include "file_descriptor_manager.h"


typedef void (*ipc_call_processor)(void*);
typedef void (*nsjail_configurator)(struct nsjconf_t*);
#define process_local


class CallbackEntry {
public:
	void* callback;
	void (*wrapper)(void* cb, void* receiving, void* sending);

	CallbackEntry(void *callback, void (*wrapper)(void *, void *, void *)) : callback(callback), wrapper(wrapper) {}

};

class CallbackManager {
private:
	// callback pointer => callback index in table
	std::map<void*, uintptr_t> callback_numbers;
	// callback table (index => callback function)
	std::vector<CallbackEntry> callback_functions;
	void* trampolines = 0;
	int remainingTrampolinBytes = 0;

public:
	CallbackManager();

	/**
	 * Wraps a callback address that is going to leave this process (and is going to be invoked by ipc in this process' context).
	 * Creates an entry in the internal callback table, and returns the index.
	 * @param callback
	 * @return
	 */
	void* wrap_outgoing_callback(void* callback, void (*wrapper)(void*, void*, void*));

	/**
	 * Wraps an incoming callback address (from another process), and returns an IPC wrapper that invokes the given callback in another process.
	 * @param callback
	 * @param replacement a function taking the exactly same parameters as the callback, which triggers the ipc procedure.
	 * @return
	 */
	void* wrap_incoming_callback(void* callback, void* replacement);

	void execute(void* callback, void* receiving_data, void* sending_data);
};


extern process_local CallbackManager global_callback_manager;



class IpcCommunication {
private:
	key_t sending_semaphore;
	key_t receiving_semaphore;
	int ipc_count = 0;
	bool is_library = false;

	ipc_call_processor processor;
	ipc_call_processor library_processor;
	nsjail_configurator configurator;

	FileDescriptorMapping fileDescriptors;

public:
	IpcCommunication(size_t sending_size, size_t receiving_size, ipc_call_processor processor, ipc_call_processor library_processor)
			: IpcCommunication(sending_size, receiving_size, processor, library_processor, nullptr, false){};
	IpcCommunication(size_t sending_size, size_t receiving_size, ipc_call_processor processor, ipc_call_processor library_processor, nsjail_configurator configurator, bool fork_now);
	~IpcCommunication();

	void* sending_data;
	void* receiving_data;

	/**
	 * Dispatches an IPC call, but does not wait for it to return. A call to trigger_ipc_call() should be followed by
	 * a call to wait_for_return().
	 */
	void trigger_ipc_call();

	static IpcCommunication* last_instance;
	/**
	 * Forks a new process (in nsjail) that runs an eventloop.
	 */
	void fork_receiving_process();
	void child_process_main();

	void switch_sender_receiver();

	/**
	 * Waits for the last dispatched call to return. Handles all method calls until this occurs.
	 * @return a pointer to the received data (containing return values)
	 */
	void* wait_for_return();

	inline int share_fd(int fd) {
		return fileDescriptors.mapToOtherSide(fd);
	}
};


#endif //LIBIPC_IPC_COMMUNICATION_H
