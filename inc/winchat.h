#ifndef _WINCHAT_H_
#define _WINCHAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>

#define BACKLOG_SZ    20
#define MX_MSG_SZ     2048
#define MX_TIME_SZ    256
#define MX_MSG_BUF_SZ (MX_MSG_SZ + MX_TIME_SZ)

const char conn_alert[] = "==> session begin";

int parse_arg(int argc, char* argv[], char** ip_str, int* port) {
  if (argc != 2 && argc != 3) return 1;
  *ip_str = argc == 2 ? "127.0.0.1" : argv[1];
  *port   = atoi(argv[argc - 1]);
  if (*port < 0 || *port > 65535) return 1;
  return 0;
}

void exec_chat(SOCKET sock) {
  static char send_buf[MX_MSG_BUF_SZ];
  int         eol = 1;
  while (1) {
    if (eol) printf("-> ");
    fgets(send_buf, MX_MSG_SZ, stdin);
    eol = 1;
    if (send_buf[0] == '\n') puts("==> no message to send");
    else {
      size_t len = strlen(send_buf);
      if (send_buf[len - 1] != '\n') {
        send_buf[len++] = '\n';
        send_buf[len]   = '\0';
        eol             = 0;
      }
      strcat(send_buf, "==> time: ");
      time_t t = time(NULL);
      strcat(send_buf, ctime(&t));
      printf("%s", send_buf + len);
      send(sock, send_buf, strlen(send_buf) + 1, 0);
    }

    static char recv_buf[MX_MSG_BUF_SZ];
    int         first_alert = 1;
    while (1) {
      int ret = recv(sock, recv_buf, MX_MSG_BUF_SZ, 0);
      if (ret < 0) break;
      else if (ret == 0) {
        puts("==> session end");
        return;
      }
      static const char new_msg_alert[] = "==> new message";
      if (first_alert) {
        puts(new_msg_alert);
        first_alert = 0;
      }
      for (char* i = recv_buf; i < recv_buf + MX_MSG_BUF_SZ; ++i) {
        if (*i) {
          putchar(*i);
          *i = '\0';
        } else {
          if (i + 1 >= recv_buf + MX_MSG_BUF_SZ || *(i + 1) == '\0') break;
          puts(new_msg_alert);
        }
      }
    }
  }
}

#endif