#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
    // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
      perror("accept");
      exit(EXIT_FAILURE);
  }

  while (true)
  {
      memset(buffer, 0, sizeof(buffer)); // Clear the buffer
      int bytes_read = read(new_socket, buffer, sizeof(buffer));
      if (bytes_read == 0)
      {
          std::cout << "Client disconnected" << std::endl;
          break;
      }

      std::cout << "Client: " << buffer << std::endl;

      // Echo the message back to the client
      send(new_socket, buffer, strlen(buffer), 0);

      // Check if the message is a command to close the connection
      if (strcmp(buffer, "exit") == 0)
      {
          std::cout << "Exit command received. Closing connection." << std::endl;
          break;
      }
  }

  close(new_socket);
  close(server_fd);
  return 0;
}

