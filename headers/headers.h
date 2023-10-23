#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <termios.h>

#include "prompt.h"
#include "cmd_parser.h"
#include "proclore.h"
#include "warp.h"
#include "exec_process.h"
#include "peek.h"
#include "seek.h"
#include "pastevents.h"
#include "misc.h"
#include "iman.h"
#include "activities.h"
#include "ping.h"
#include "redirect.h"
#include "fg.h"
#include "bg.h"
#include "neonate.h"
#include "pipe.h"

#endif