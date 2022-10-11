#include "winchat.h"

signed main(int argc, char* argv[]) {
  char* ip_str;
  int   port;
  if (parse_arg(argc, argv, &ip_str, &port)) exit(1);

  WSADATA wsa_data;
  WSAStartup(MAKEWORD(2, 2), &wsa_data);

  SOCKET serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  struct sockaddr_in sock_addr = {.sin_family = AF_INET,
                                  .sin_addr   = {.s_addr = inet_addr(ip_str)},
                                  .sin_port   = htons(port)};
  bind(serv_sock, (SOCKADDR*) &sock_addr, sizeof(SOCKADDR));
  listen(serv_sock, BACKLOG_SZ);

  SOCKADDR clnt_addr;
  int      clnt_addr_sz = sizeof(SOCKADDR);
  SOCKET   clnt_sock;
  while (clnt_sock = accept(serv_sock, &clnt_addr, &clnt_addr_sz)) {
    u_long argp = 1;
    if (ioctlsocket(clnt_sock, FIONBIO, &argp) == 0) {
      puts(conn_alert);
      exec_chat(clnt_sock);
    }
    closesocket(clnt_sock);
  }

  closesocket(serv_sock);
  WSACleanup();
}
