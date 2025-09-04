#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

struct ServerConfig {
    int port;
    int connection_backlog;
    size_t buffer_size;
};

int main(int argc, char **argv) {
    ServerConfig config;
    config.port = 6379; 
    config.connection_backlog = 5;
    config.buffer_size = 1024;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(config.port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, config.connection_backlog) != 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Server listening on port " << config.port << "...\n";

    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    while (true) {
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            continue; 
        }

        std::string partial_message;
        char buffer[1024];

        while (true) {
            int bytes_rec = recv(client_fd, buffer, config.buffer_size - 1, 0);
            if (bytes_rec <= 0) {
                break;
            }

            buffer[bytes_rec] = '\0';
            partial_message += buffer;

            std::cout << "Client sent: " << partial_message << std::endl;

            size_t pos;
            while ((pos = partial_message.find("\r\n")) != std::string::npos) {
                std::string command = partial_message.substr(0, pos);
                partial_message.erase(0, pos + 2);

                if (command == "PING") {
                    const char *response = "+PONG\r\n";
                    send(client_fd, response, strlen(response), 0);
                } else {
                    const char *response = "-ERR unknown command\r\n";
                    send(client_fd, response, strlen(response), 0);
                }
            }
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}

