#include <signal.h>
#include <pthread.h>

#define gai_start_notify_thread __gai_start_notify_thread
#define gai_create_helper_thread __gai_create_helper_thread

extern inline void
__gai_start_notify_thread (void)
{
  sigset_t ss;
  sigemptyset (&ss);
  sigprocmask(SIG_SETMASK, &ss, NULL);
}

extern inline int
__gai_create_helper_thread (pthread_t *threadp, void *(*tf) (void *),
			    void *arg)
{
  pthread_attr_t attr;

  /* Make sure the thread is created detached.  */
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

  /* The helper thread needs only very little resources.  */
  (void) pthread_attr_setstacksize (&attr, 0x10000);

  /* Block all signals in the helper thread.  To do this thoroughly we
     temporarily have to block all signals here.  */
  sigset_t ss;
  sigset_t oss;
  sigfillset (&ss);
  sigprocmask(SIG_SETMASK, &ss, &oss);

  int ret = pthread_create (threadp, &attr, tf, arg);

  /* Restore the signal mask.  */
  sigprocmask(SIG_SETMASK, &oss, NULL);

  (void) pthread_attr_destroy (&attr);
  return ret;
}

#include_next <gai_misc.h>
