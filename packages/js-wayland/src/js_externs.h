#include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <limits.h>
#include <syscall.h>
#include <sys/mman.h>
#include <errno.h>
#include "xdg-shell-client-protocol.h"


#ifndef JS_EXTERNS_H_
#define JS_EXTERNS_H_

int connect_to_env();
int draw_window();

#endif // JS_EXTERNS_H_
