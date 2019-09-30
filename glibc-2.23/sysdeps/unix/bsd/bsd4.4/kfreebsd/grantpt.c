#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <not-cancel.h>

#include "pty-private.h"

int __syscall_closefrom(int fd);
libc_hidden_proto (__syscall_closefrom)

/* Close all file descriptors except the one specified.  */
static void
close_all_fds (void)
{
    int i;

    for (i = 0 ; i < PTY_FILENO; i++)
        close_not_cancel_no_status (i);
        
    INLINE_SYSCALL(closefrom, 1, PTY_FILENO + 1L);

    int nullfd = open_not_cancel_2 (_PATH_DEVNULL, O_RDONLY);
    assert (nullfd == STDIN_FILENO);
    nullfd = open_not_cancel_2 (_PATH_DEVNULL, O_WRONLY);
    assert (nullfd == STDOUT_FILENO);
    __dup2 (STDOUT_FILENO, STDERR_FILENO);
}
#define CLOSE_ALL_FDS() close_all_fds()

#include <sysdeps/unix/grantpt.c>
