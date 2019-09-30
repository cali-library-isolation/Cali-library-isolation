#ifndef LIBNSJAIL_API_H
#define LIBNSJAIL_API_H

#ifdef __cplusplus
extern "C" {
#endif

struct nsjconf_t;

extern int nsjail_argc;
extern const char **nsjail_argv;

void nsjail_config_add_open_fd(struct nsjconf_t *nsjconf, int fd);

void nsjail_addMountRO(struct nsjconf_t *conf, const char *path);

void nsjail_addMountRW(struct nsjconf_t *conf, const char *path);

void nsjail_addMountTmp(struct nsjconf_t *conf, const char *path);

void nsjail_setUID(struct nsjconf_t *conf, char *uid);

void nsjail_setGID(struct nsjconf_t *conf, char *gid);

void nsjail_setNetwork(struct nsjconf_t *conf, const char *mode);

void nsjail_setSeccomp(struct nsjconf_t *conf, char *seccomp);

void nsjail_addCapability(struct nsjconf_t *conf, const char *cap);

void nsjail_setMaxCPU(struct nsjconf_t *conf, int cpu_count);

void nsjail_setSilent(struct nsjconf_t *conf);

void nsjail_set_rlimit_as(struct nsjconf_t *conf, const char *value);
void nsjail_set_rlimit_core(struct nsjconf_t *conf, const char *value);
void nsjail_set_rlimit_cpu(struct nsjconf_t *conf, const char *value);
void nsjail_set_rlimit_fsize(struct nsjconf_t *conf, const char *value);
void nsjail_set_rlimit_nofile(struct nsjconf_t *conf, const char *value);
void nsjail_set_rlimit_nproc(struct nsjconf_t *conf, const char *value);
void nsjail_set_rlimit_stack(struct nsjconf_t *conf, const char *value);

void nsjail_set_cgroup_mem_max(struct nsjconf_t *conf, ssize_t value);
void nsjail_set_cgroup_pids_max(struct nsjconf_t *conf, ssize_t value);
void nsjail_set_cgroup_cpu_ms_per_sec(struct nsjconf_t *conf, ssize_t value);

#ifdef __cplusplus
};
#endif

#endif //LIBNSJAIL_API_H
