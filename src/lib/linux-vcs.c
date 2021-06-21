#include <linux/uinput.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static bool vcs_quit = true;

static int inp_fd;
static bool is_shifted;
static int mouse_x;
static int mouse_y;

#define delay usleep(500)

void vcs_emit(int fd, int type, int code, int val)
{
  struct input_event ie;
  ie.type = type;
  ie.code = code;
  ie.value = val;
  write(fd, &ie, sizeof(ie));
  delay;
}

void vcs_report()
{
  vcs_emit(inp_fd, EV_SYN, SYN_REPORT, 0);
}

void vcs_press(int code)
{
  vcs_emit(inp_fd, EV_KEY, code, 1);
  vcs_report();
}

void vcs_release(int code)
{
  vcs_emit(inp_fd, EV_KEY, code, 0);
  vcs_report();
}

void vcs_tap(int code)
{
  vcs_press(code);
  vcs_release(code);
}

void vcs_shift()
{
  if(!is_shifted)
  {
    vcs_press(KEY_LEFTSHIFT);
    is_shifted = true;
  }
}

void vcs_unshift()
{
  if(is_shifted)
  {
    vcs_release(KEY_LEFTSHIFT);
    is_shifted = false;
  }
}

static int alpha_mapping['z' - 'a' + 1] = 
{
  KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
  KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
  KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
  KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
};

static int num_mapping['9' - '0' + 1] = 
{
  KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6,
  KEY_7, KEY_8, KEY_9
};

void vcs_type(const char *buf, size_t sz)
{
  for(; sz > 0; buf++, sz--)
  {
    if(*buf >= 'a' && *buf <= 'z')
    {
      vcs_unshift();
      vcs_tap(alpha_mapping[*buf - 'a']);
      continue;
    }
    if(*buf >= 'A' && *buf <= 'Z')
    {
      vcs_shift();
      vcs_tap(alpha_mapping[*buf - 'A']);
      continue;
    }
    if(*buf >= '0' && *buf <= '9')
    {
      vcs_unshift();
      vcs_tap(num_mapping[*buf - '0']);
      continue;
    }
    switch(*buf)
    {
      case '\n':
      {
        vcs_shift();
        vcs_tap(KEY_ENTER);
        break;
      }
      case ' ':
      {
        vcs_unshift();
        vcs_tap(KEY_SPACE);
        break;
      }
      case '!':
      {
        vcs_shift();
        vcs_tap(KEY_1);
        break;
      }
      case '@':
      {
        vcs_shift();
        vcs_tap(KEY_2);
        break;
      }
      case '#':
      {
        vcs_shift();
        vcs_tap(KEY_3);
        break;
      }
      case '$':
      {
        vcs_shift();
        vcs_tap(KEY_4);
        break;
      }
      case '%':
      {
        vcs_shift();
        vcs_tap(KEY_5);
        break;
      }
      case '^':
      {
        vcs_shift();
        vcs_tap(KEY_6);
        break;
      }
      case '&':
      {
        vcs_shift();
        vcs_tap(KEY_7);
        break;
      }
      case '*':
      {
        vcs_shift();
        vcs_tap(KEY_8);
        break;
      }
      case '(':
      {
        vcs_shift();
        vcs_tap(KEY_9);
        break;
      }
      case ')':
      {
        vcs_shift();
        vcs_tap(KEY_0);
        break;
      }
      case ',':
      {
        vcs_unshift();
        vcs_tap(KEY_COMMA);
        break;
      }
      case '<':
      {
        vcs_shift();
        vcs_tap(KEY_COMMA);
        break;
      }
      case '.':
      {
        vcs_unshift();
        vcs_tap(KEY_DOT);
        break;
      }
      case '>':
      {
        vcs_shift();
        vcs_tap(KEY_DOT);
        break;
      }
      case '/':
      {
        vcs_unshift();
        vcs_tap(KEY_SLASH);
        break;
      }
      case '?':
      {
        vcs_shift();
        vcs_tap(KEY_SLASH);
        break;
      }
      case ';':
      {
        vcs_unshift();
        vcs_tap(KEY_SEMICOLON);
        break;
      }
      case ':':
      {
        vcs_shift();
        vcs_tap(KEY_SEMICOLON);
        break;
      }
      case '\'':
      {
        vcs_unshift();
        vcs_tap(KEY_APOSTROPHE);
        break;
      }
      case '\"':
      {
        vcs_shift();
        vcs_tap(KEY_APOSTROPHE);
        break;
      }
      case '[':
      {
        vcs_unshift();
        vcs_tap(KEY_LEFTBRACE);
        break;
      }
      case '{':
      {
        vcs_shift();
        vcs_tap(KEY_LEFTBRACE);
        break;
      }
      case ']':
      {
        vcs_unshift();
        vcs_tap(KEY_RIGHTBRACE);
        break;
      }
      case '}':
      {
        vcs_shift();
        vcs_tap(KEY_RIGHTBRACE);
        break;
      }
      case '\\':
      {
        vcs_unshift();
        vcs_tap(KEY_BACKSLASH);
        break;
      }
      case '|':
      {
        vcs_shift();
        vcs_tap(KEY_BACKSLASH);
        break;
      }
      case '-':
      {
        vcs_unshift();
        vcs_tap(KEY_MINUS);
        break;
      }
      case '_':
      {
        vcs_shift();
        vcs_tap(KEY_MINUS);
        break;
      }
      case '=':
      {
        vcs_unshift();
        vcs_tap(KEY_EQUAL);
        break;
      }
      case '+':
      {
        vcs_shift();
        vcs_tap(KEY_EQUAL);
        break;
      }
      default:
      {
        printf("cannot print char %d aka [%c]\n", *(uint8_t *)buf, *buf);
        return;
        break;
      }
    }
  }
}

void vcs_restore()
{
  vcs_unshift();
}

void vcs_mouse_reset()
{
  vcs_emit(inp_fd, EV_REL, REL_X, -20000);
  vcs_emit(inp_fd, EV_REL, REL_Y, -20000);
  vcs_report();
  mouse_x = 0;
  mouse_y = 0;
}

void vcs_mouse_abs(int x, int y)
{
  vcs_emit(inp_fd, EV_REL, REL_X, x - mouse_x);
  vcs_emit(inp_fd, EV_REL, REL_Y, y - mouse_y);
  vcs_report();
  mouse_x = x;
  mouse_y = y;
}

void vcs_mouse_rel(int x, int y)
{
  vcs_emit(inp_fd, EV_REL, REL_X, x);
  vcs_emit(inp_fd, EV_REL, REL_Y, y);
  vcs_report();
  mouse_x += x;
  mouse_y += y;
}

void vcs_init()
{
  struct uinput_setup usetup;
  int i;

  if(!vcs_quit)
  {
    printf("vcs is already loaded\n");
    return;
  }
  vcs_quit = false;
  is_shifted = false;

  printf("vcs loading\n");

  // create driver
  inp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  // specify which events can be registered by the driver
  // keyboard
  ioctl(inp_fd, UI_SET_EVBIT, EV_KEY);
  for(i = 1; i < KEY_MAX; i++)
  {
    if(ioctl(inp_fd, UI_SET_KEYBIT, i) < 0)
    {
      printf("failed ioctl SET_KEYBIT %d aka %x\n", i, i);
    }
  }
  // mouse
  ioctl(inp_fd, UI_SET_EVBIT, EV_REL);
  ioctl(inp_fd, UI_SET_RELBIT, REL_X);
  ioctl(inp_fd, UI_SET_RELBIT, REL_Y);

  // attach driver information
  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234;
  usetup.id.product = 0x5678;
  strcpy(usetup.name, "VCS driver");

  // bind driver
  ioctl(inp_fd, UI_DEV_SETUP, &usetup);
  ioctl(inp_fd, UI_DEV_CREATE);

  printf("vcs loaded\n");
}

void vcs_destroy()
{
  if(vcs_quit)
  {
    printf("vcs is not loaded\n");
    return;
  }
  vcs_quit = true;

  ioctl(inp_fd, UI_DEV_DESTROY);
  close(inp_fd);

  printf("vcs unloaded\n");
}
