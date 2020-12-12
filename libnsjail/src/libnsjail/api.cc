#include <sys/mount.h>
#include <memory.h>
#include <sys/resource.h>
#include <libgen.h>
#include "common.h"
#include <nsjail/mnt.h>
#include <nsjail/user.h>
#include <nsjail/util.h>
#include <nsjail/caps.h>
#include <nsjail/cmdline.h>
#include "api.h"


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef NDEBUG
#define debug_printf(msg, ...) printf(msg "\n", ##__VA_ARGS__)
#else
#define debug_printf(msg, ...) do {} while(0)
#endif


int nsjail_argc;
const char **nsjail_argv;


static std::string argFromVec(const std::vector<std::string> &vec, size_t pos) {
	if (pos >= vec.size()) {
		return "";
	}
	return vec[pos];
}


void nsjail_config_add_open_fd(struct nsjconf_t *nsjconf, int fd) {
	nsjconf->openfds.push_back(fd);
}


static void nsjail_addMountSimple(struct nsjconf_t *conf, const std::string path, uintptr_t flags, const std::string &fs_type) {
	if (path.empty()) return;

	// Remove duplicates
	for (const auto &mntpnt: conf->mountpts) {
		if (mntpnt.dst == path)
			return;
	}

	// Add to queue
	if (fs_type == "tmpfs") {
		mnt::addMountPtTail(conf, "", optarg, "tmpfs", "size=4194304", 0, mnt::NS_DIR_YES, true, "", "", "", false);
	} else {
		mnt::addMountPtTail(conf, path, path, "", "", flags, mnt::NS_DIR_MAYBE, true, "", "", "", false);
	}
	debug_printf("[nsjail] %s %s  = %s", flags & MS_RDONLY ? "ro" : "rw", fs_type.c_str(), path.c_str());
}

/**
 * Split "a=b" style parameters (if applicable)
 * @param s
 * @return
 */
static char *splitParameter(char *s) {
	char *eq = strchr(s, '=');
	if (eq) {
		*eq = 0;
		return eq + 1;
	} else {
		return s;
	}
}

/**
 * Get the folder of s, or <null>
 * @param s
 * @return
 */
static char *folder(char *s) {
	return realpath(dirname(s), 0);
}

static void nsjail_addMount(struct nsjconf_t *conf, const char *path, int flags, const std::string &fs_type) {
	if (path[0] == '$') {
		if (strcmp(path, "$ARGV") == 0) {
			for (int i = 1; i < nsjail_argc; i++) {
				char *arg = strdup(nsjail_argv[i]);
				// split by ,
				char *pos = arg;
				char *sep = strchr(pos, ',');
				while (sep) {
					*sep = 0;
					// mount
					pos = splitParameter(pos);
					if (pos[0] != '-') {
						auto final_path = realpath(pos, nullptr);
						if (final_path)
							nsjail_addMountSimple(conf, final_path, flags, fs_type);
						free(final_path);
					}
					pos = sep + 1;
					sep = strchr(pos, ',');
				}
				// mount
				pos = splitParameter(pos);
				if (pos[0] != '-') {
					auto final_path = realpath(pos, nullptr);
					if (final_path)
						nsjail_addMountSimple(conf, final_path, flags, fs_type);
					free(final_path);
				}
				free(arg);
			}
		} else if (strcmp(path, "$ARGV_FOLDERS") == 0) {
			for (int i = 1; i < nsjail_argc; i++) {
				char *arg = strdup(nsjail_argv[i]);
				// split by ,
				char *pos = arg;
				char *sep = strchr(pos, ',');
				while (sep) {
					*sep = 0;
					// mount
					pos = splitParameter(pos);
					if (pos[0] != '-')
						nsjail_addMountSimple(conf, folder(pos), flags, fs_type);
					pos = sep + 1;
					sep = strchr(pos, ',');
				}
				// mount
				pos = splitParameter(pos);
				if (pos[0] != '-')
					nsjail_addMountSimple(conf, folder(pos), flags, fs_type);
				free(arg);
			}
		} else if (strncmp("$HOME", path, strlen("$HOME")) == 0) {
			char *home = getenv("HOME");
			auto newpath = home + std::string(path).substr(5);
			nsjail_addMountSimple(conf, newpath, flags, fs_type);
		} else if (strncmp("$CWD", path, strlen("$CWD")) == 0) {
			char *cwd = getcwd(nullptr, 0);
			auto newpath = cwd + std::string(path).substr(4);
			nsjail_addMountSimple(conf, newpath, flags, fs_type);
			free(cwd);
		} else {
			debug_printf("[nsjail] Error: unknown path \"%s\"", path);
		}
	} else if (path[0] == '/' && path[1] == '\0') {
		nsjail_addMountSimple(conf, "/bin", flags, fs_type);
		nsjail_addMountSimple(conf, "/dev", flags, fs_type);
		nsjail_addMountSimple(conf, "/etc", flags, fs_type);
		nsjail_addMountSimple(conf, "/home", flags, fs_type);
		nsjail_addMountSimple(conf, "/lib", flags, fs_type);
		nsjail_addMountSimple(conf, "/lib64", flags, fs_type);
		nsjail_addMountSimple(conf, "/opt", flags, fs_type);
		nsjail_addMountSimple(conf, "/run", flags, fs_type);
		nsjail_addMountSimple(conf, "/srv", flags, fs_type);
		nsjail_addMountSimple(conf, "/usr", flags, fs_type);
		nsjail_addMountSimple(conf, "/var", flags, fs_type);
	} else {
		nsjail_addMountSimple(conf, path, flags, fs_type);
	}
}

void nsjail_addMountRO(struct nsjconf_t *conf, const char *path) {
	nsjail_addMount(conf, path, MS_BIND | MS_REC | MS_PRIVATE | MS_RDONLY, "");
}

void nsjail_addMountRW(struct nsjconf_t *conf, const char *path) {
	nsjail_addMount(conf, path, MS_BIND | MS_REC | MS_PRIVATE, "");
}

void nsjail_addMountTmp(struct nsjconf_t *conf, const char *path) {
	nsjail_addMount(conf, path, 0, "tmpfs");
}

void nsjail_setUID(struct nsjconf_t *conf, char *uid) {
	debug_printf("[nsjail] uid = %s", uid);
	std::vector<std::string> subopts = util::strSplit(optarg, ':');
	std::string i_id = argFromVec(subopts, 0);
	std::string o_id = argFromVec(subopts, 1);
	std::string cnt = argFromVec(subopts, 2);
	size_t count = strtoul(cnt.c_str(), nullptr, 0);
	if (!user::parseId(conf, i_id, o_id, count, false, false)) {
		fprintf(stderr, "Failed to set UID");
	}
}

void nsjail_setGID(struct nsjconf_t *conf, char *gid) {
	debug_printf("[nsjail] gid = %s", gid);
	std::vector<std::string> subopts = util::strSplit(optarg, ':');
	std::string i_id = argFromVec(subopts, 0);
	std::string o_id = argFromVec(subopts, 1);
	std::string cnt = argFromVec(subopts, 2);
	size_t count = strtoul(cnt.c_str(), nullptr, 0);
	if (!user::parseId(conf, i_id, o_id, count, true, false)) {
		fprintf(stderr, "Failed to set GID");
	}
}

void nsjail_setNetwork(struct nsjconf_t *conf, const char *mode) {
	if (strcmp(mode, "all") == 0 || strcmp(mode, "host") == 0) {
		conf->clone_newnet = false;
		conf->iface_lo = true;
	} else if (strcmp(mode, "local") == 0) {
		conf->clone_newnet = true;
		conf->iface_lo = true;
	} else {
		// none
		conf->clone_newnet = true;
		conf->iface_lo = false;
	}
}

void nsjail_setSeccomp(struct nsjconf_t *conf, char *seccomp) {
	conf->kafel_string = seccomp;
}

void nsjail_addCapability(struct nsjconf_t *conf, const char *cap) {
	int num = caps::nameToVal(cap);
	if (num == -1) {
		fprintf(stderr, "Invalid capability: %s\n", cap);
	} else {
		conf->caps.push_back(num);
	}
}

void nsjail_setMaxCPU(struct nsjconf_t *conf, int cpu_count) {
	conf->max_cpus = cpu_count;
}

void nsjail_setSilent(struct nsjconf_t *conf) {
	conf->is_silent = true;
}

void nsjail_set_rlimit_as(struct nsjconf_t *conf, const char *value){
	conf->rl_as = cmdline::parseRLimit(RLIMIT_AS, value, (1024 * 1024));
}

void nsjail_set_rlimit_core(struct nsjconf_t *conf, const char *value){
	conf->rl_core = cmdline::parseRLimit(RLIMIT_CORE, value, (1024 * 1024));
}

void nsjail_set_rlimit_cpu(struct nsjconf_t *conf, const char *value){
	conf->rl_cpu = cmdline::parseRLimit(RLIMIT_CPU, value, 1);
}

void nsjail_set_rlimit_fsize(struct nsjconf_t *conf, const char *value){
	conf->rl_fsize = cmdline::parseRLimit(RLIMIT_FSIZE, value, (1024 * 1024));
}

void nsjail_set_rlimit_nofile(struct nsjconf_t *conf, const char *value){
	conf->rl_nofile = cmdline::parseRLimit(RLIMIT_NOFILE, value, 1);
}

void nsjail_set_rlimit_nproc(struct nsjconf_t *conf, const char *value){
	conf->rl_nproc = cmdline::parseRLimit(RLIMIT_NPROC, value, 1);
}

void nsjail_set_rlimit_stack(struct nsjconf_t *conf, const char *value){
	conf->rl_stack = cmdline::parseRLimit(RLIMIT_STACK, value, (1024 * 1024));
}

void nsjail_set_cgroup_mem_max(struct nsjconf_t *conf, ssize_t value){
	conf->cgroup_mem_max = value;
}

void nsjail_set_cgroup_pids_max(struct nsjconf_t *conf, ssize_t value){
	conf->cgroup_pids_max = value;
}

void nsjail_set_cgroup_cpu_ms_per_sec(struct nsjconf_t *conf, ssize_t value){
	conf->cgroup_cpu_ms_per_sec = value;
}



#pragma clang diagnostic pop