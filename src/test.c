#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

int main() {
  //初始化 DLL
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  //创建套接字
  SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  //绑定套接字
  struct sockaddr_in sockAddr;
  memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
  sockAddr.sin_family      = AF_INET;      //使用IPv4地址
  sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
  sockAddr.sin_port        = htons(7777);             //端口
  bind(servSock, (SOCKADDR*) &sockAddr, sizeof(SOCKADDR));

  //进入监听状态
  listen(servSock, 20);

  //接收客户端请求
  SOCKADDR clntAddr;
  int      nSize    = sizeof(SOCKADDR);
  SOCKET   clntSock = accept(servSock, (SOCKADDR*) &clntAddr, &nSize);

  char RecvData[MAX_PATH] = {0};  //定义变量存储接收到的数据
  while (TRUE) {
    //接收到的数据如果小于等于0 失败返回，否则把接收到的数据打印出来
    if (recv(servSock, RecvData, MAX_PATH, 0) <= 0) {
      printf("client已退出本次聊天");
      break;
    }
    printf("client：%s\n", RecvData);
  }

  //向客户端发送数据

  char SendData[MAX_PATH] = {0};
  while (TRUE) {
    gets(SendData);  //获取输入的字符串
    send(clntSock, SendData, strlen(SendData) + sizeof(char), 0);
  }
  //关闭套接字
  closesocket(clntSock);
  closesocket(servSock);

  //终止 DLL 的使用
  WSACleanup();

  return 0;
}