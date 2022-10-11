#ifndef _WINCHAT_H_
#define _WINCHAT_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>

#define BACKLOG_SZ 20
#define MX_MSG_SZ  65536

int parse_arg(int argc, char* argv[], char** ip_str, int* port) {
  if (argc != 2 && argc != 3) return 1;
  *ip_str = argc == 2 ? "127.0.0.1" : argv[2];
  *port   = atoi(argv[argc - 1]);
  if (*port < 0 || *port > 65535) return 1;
  return 0;
}

void exec_chat(SOCKET sock) {
  static char msg[MX_MSG_SZ];
  while (1) {
    time_t t   = time(NULL);
    int    len = sprintf(msg, "%s", ctime(&t));
    if (len < 0) continue;
    putchar('>');
    fgets(msg + len, MX_MSG_SZ - len, stdin);
    putchar('\n');
    if (msg[len] != '\n') send(sock, &msg, MX_MSG_SZ, 0);

    while (1) {
      msg[0] = '\0';
      if (recv(sock, &msg, MX_MSG_SZ, 0) <= 0) break;
      if (msg[0] != '\0') printf("%s\n\n", msg);
    }
  }
}

#endif