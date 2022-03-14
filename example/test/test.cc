#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char* argv[]) {
  int sock;
  struct sockaddr_in serv_addr;
  char message[30] = "test";
  int str_len;

  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    error_handling("socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ==
      -1)  // connect 함수호출을 통해서 서버 프로그램에 연결 요청
    error_handling("connect() error");

  for (auto i = 0 ; i < 10 ; ++i) {
    printf("Send %d\n", i);
    str_len = send(sock, message, sizeof(message), MSG_NOSIGNAL);
    if (str_len == -1)
      error_handling("read() error");
  }

  printf("Message from server:%s\n", message);
  close(sock);
  return 0;
}
