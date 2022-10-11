#ifndef _WINCHAT_H_
#define _WINCHAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>

#define BACKLOG_SZ    20
#define MX_MSG_SZ     2048
#define MX_MSG_BUF_SZ (MX_MSG_SZ + 256)

const char conn_alert[] = "==> session established";

int parse_arg(int argc, char* argv[], char** ip_str, int* port) {
  if (argc != 2 && argc != 3) return 1;
  *ip_str = argc == 2 ? "127.0.0.1" : argv[1];
  *port   = atoi(argv[argc - 1]);
  if (*port < 0 || *port > 65535) return 1;
  return 0;
}

void exec_chat(SOCKET sock) {
  static char msg[MX_MSG_BUF_SZ];
  int         eol = 1;
  while (1) {
    if (eol) printf("-> ");
    fgets(msg, MX_MSG_SZ, stdin);
    eol = 1;
    if (msg[0] == '\n') puts("==> no message to send");
    else {
      size_t len = strlen(msg);
      if (msg[len - 1] != '\n') {
        msg[len++] = '\n';
        msg[len]   = '\0';
        eol        = 0;
      }
      strcat(msg, "==> time: ");
      time_t t = time(NULL);
      strcat(msg, ctime(&t));
      printf("%s", msg + len);
      send(sock, msg, strlen(msg) + 1, 0);
    }

    memset(msg, 0, sizeof(msg));
    int first_alert = 1;
    while (1) {
      if (recv(sock, msg, MX_MSG_BUF_SZ, 0) <= 0) break;
      static const char new_msg_alert[] = "==> new message";
      if (first_alert) {
        puts(new_msg_alert);
        first_alert = 0;
      }
      for (char* i = msg; i < msg + MX_MSG_BUF_SZ; ++i) {
        if (*i) {
          putchar(*i);
          *i = '\0';
        } else {
          if (i + 1 >= msg + MX_MSG_BUF_SZ || *(i + 1) == '\0') break;
          puts(new_msg_alert);
        }
      }
    }
  }
}

#endif