#include "winchat.h"

signed main(int argc, char* argv[]) {
  char* ip_str;
  int   port;
  if (parse_arg(argc, argv, &ip_str, &port)) exit(1);

  WSADATA wsa_data;
  WSAStartup(MAKEWORD(2, 2), &wsa_data);

  SOCKET clnt_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  u_long argp      = 1;
  if (ioctlsocket(clnt_sock, FIONBIO, &argp)) goto END;

  struct sockaddr_in sock_addr = {.sin_family = AF_INET,
                                  .sin_addr   = {.s_addr = inet_addr(ip_str)},
                                  .sin_port   = htons(port)};
  connect(clnt_sock, (SOCKADDR*) &sock_addr, sizeof(SOCKADDR));
  puts(conn_alert);
  exec_chat(clnt_sock);
END:
  closesocket(clnt_sock);
  WSACleanup();
}
