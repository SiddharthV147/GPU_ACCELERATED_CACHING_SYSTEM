#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
  
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  
  if(sock < 0) {
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(6379);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    // std::cerr << "Connection failed\n";
    return 1;
  }

  const char* message = "PING\r\nPING\r\nPING\r\n";
  for(int i=0; i<5; i++){
    send(sock, message, strlen(message), 0);

    char buffer[1024];
    int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
    if(bytes > 0){
      buffer[bytes] = '\0';
      std::cout << "Received : " << buffer;
    }
  }

  close(sock);

  return 0;

}
