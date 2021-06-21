#include <vcs.h>

#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <cstdio>

bool daemon_exited = false;

static void handle_signal(int sig)
{
  daemon_exited = true;
}

int main()
{
  vcs_init();
  signal(SIGINT, handle_signal);
  sleep(5);

  printf("starting mouse movement\n");
  sleep(1);
  vcs_mouse_reset();
  printf("mouse is reset\n");
  sleep(1);
  vcs_mouse_abs(300, 300);
  printf("mouse is at 300, 300\n");
  sleep(1);
  vcs_mouse_rel(100, -100);
  printf("top right\n");
  sleep(1);
  printf("done\n");
  while(!daemon_exited)
  {
    sleep(1);
  }
  vcs_destroy();
  return 0;
}
