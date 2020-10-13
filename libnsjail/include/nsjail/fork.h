#ifndef FORKING_NSJAIL_FORK_H
#define FORKING_NSJAIL_FORK_H

#include <memory>
#include "nsjail/common.h"


typedef void (*callback_function_ptr)(void);

std::unique_ptr<struct nsjconf_t> nsjail_default_config();
void nsjail_print_default_config(void);
void nsjail_run_jailed(struct nsjconf_t *nsjconf, callback_function_ptr process);
pid_t nsjail_fork(struct nsjconf_t *nsjconf, callback_function_ptr child_process, void(*on_parent_exit_callback)(int));

#endif //FORKING_NSJAIL_FORK_H
