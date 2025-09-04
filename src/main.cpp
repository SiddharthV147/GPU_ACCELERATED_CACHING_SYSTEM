#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {
  
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   std::cerr << "Failed to create server socket\n";
   return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(6379);
  
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 6379\n";
    return 1;
  }
  
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  while(true){
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    if(client_fd < 0) {
      std::cerr << "Error while connecting to the client" << std::endl;
      return -1;
    }

    std::string partial_message;
    char buffer[1024];
    while(true){
      int bytes_rec = recv(client_fd, buffer, sizeof(buffer), 0);
      if(bytes_rec <= 0) {
        break;
      }
      if(bytes_rec > 0) {
        buffer[bytes_rec] = '\0';
        partial_message+=buffer;
        
      std::cout << partial_message << std::endl;
      size_t pos;
        while((pos = partial_message.find("\r\n")) != std::string::npos) {
          std::string command = partial_message.substr(0, pos);
          partial_message.erase(0, pos+2);
          if(command == "PING") {
            const char* response = "+PONG\r\n";
            send(client_fd, response, strlen(response), 0);
          }
          else {
            const char* response = "-ERR unknown command\r\n";
            send(client_fd, response, strlen(response), 0);
          }
        }
      }
    }
    close(client_fd);
  }

  close(server_fd);

  return 0;
}
