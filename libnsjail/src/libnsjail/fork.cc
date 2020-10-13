#include <stdlib.h>
#include <sys/prctl.h>
#include <signal.h>
#include <string.h>
#include <nsjail/logs.h>

#include "fork.h"
#include "nsjail/cmdline.h"
#include "nsjail/sandbox.h"
#include "nsjail/subproc.h"


static const char *default_commandline[] = {
		"???", "-Mo",
		// "--chroot", "/", "--rw",
		"--chroot", "/dev/shm",
		// "-R", "/usr", "-R", "/lib64", "-R", "/lib", "-R", "/bin", "-R", "/etc", "-R", "/dev/urandom", "-B", "/dev/shm", "-B", "/tmp",
		// "--user", "65534", "--group", "65534",
		"--disable_clone_newipc",
		"--keep_caps", "--keep_env",
		// "--disable_clone_newnet", /*"--disable_clone_newcgroup",*/ "--disable_clone_newuts", "--disable_clone_newns", "--skip_setsid",
		"--time_limit", "0",
		"--", "/usr/bin/id", NULL
}; // */


void nsjail_print_default_config(void) {
	printf("NSJail config:");
	const char **it = default_commandline;
	while (*it != 0) {
		printf(" %s", *it);
		it++;
	}
	printf("\n");
}


std::unique_ptr<struct nsjconf_t> nsjail_default_config() {
	// const char* params[] = {"???", "-Mo", "--chroot", "/", "--user", "65534", "--group", "65534", "--", "/usr/bin/id", 0};

	//char **params_heap = (char**) malloc(sizeof default_commandline);
	//memcpy(params_heap, default_commandline, sizeof default_commandline);
	// printf("Params: %zu\n", sizeof params / sizeof params[0] -1);
	//auto conf = cmdline::parseArgs(sizeof default_commandline / sizeof default_commandline[0] - 1, params_heap);

	auto conf = cmdline::parseArgs(0, nullptr);
	if (!conf) fprintf(stderr, "Error parsing fake cmd!");
	conf->mode = MODE_STANDALONE_ONCE;
	conf->chroot = "/dev/shm";
	conf->clone_newipc = false;
	conf->keep_caps = true;
	conf->keep_env = true;
	conf->tlimit = 0;
	conf->execute_function = nullptr;
	return conf;
}


void addParentDependency() {
	if (prctl(PR_SET_PDEATHSIG, SIGTERM) == -1) perror("prctl() - no parent dependency set");
}

void nsjail_run_jailed(struct nsjconf_t *nsjconf, callback_function_ptr process) {
	logs::logLevel(logs::WARNING);
	nsjconf->execute_function = process;

	if (!nsjconf->clone_newuser && geteuid() != 0) {
		fprintf(stderr, "--disable_clone_newuser requires root() privs");
	}

	/*
	if (nsjconf->daemonize && (daemon(0, 0) == -1)) {
		fprintf(stderr, "daemon");
	}
	cmdlineLogParams(nsjconf);
	if (nsjailSetSigHandlers() == false) {
		exit(1);
	}
	if (nsjailSetTimer(nsjconf) == false) {
		exit(1);
	}

	//nsjailStandaloneMode(&nsjconf);
	subprocRunChild(nsjconf, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	*/

	if (!sandbox::preparePolicy(nsjconf)) {
		fprintf(stderr, "Couldn't prepare sandboxing policy");
	}
	subproc::runChild(nsjconf, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
}


void attach_gdb(pid_t pid) {
	char buffer[1024];
	sprintf(buffer, "sudo gnome-terminal -- gdb %s %d", program_invocation_name, pid);
	if (system(buffer))
		perror("system gdb");
}


pid_t nsjail_fork(struct nsjconf_t *nsjconf, callback_function_ptr process, void(*on_parent_exit_callback)(int)) {
	//nsjail_test();
	pid_t child = fork();
	if (child == -1) perror("fork()");
	else if (child == 0) {
		// Child process
		addParentDependency();
		if (on_parent_exit_callback)
			signal(SIGTERM, on_parent_exit_callback);
		nsjail_run_jailed(nsjconf, process);
		on_parent_exit_callback(0);
		/*if (!nsjconf->pids.empty() && getenv("CALI_ATTACH_GDB")) {
			attach_gdb(nsjconf->pids[0].pid);
		}*/
		exit(0);
	}
	return child;
}
