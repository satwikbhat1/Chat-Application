#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<conio.h>

int main()
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      std::cout << "\n Socket creation error \n";
      return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
      std::cout << "\nInvalid address/ Address not supported \n";
      return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      std::cout << "\nConnection Failed \n";
      return -1;
  }

  std::string message;
  while (true)
  {
      std::cout << "Enter message (type 'exit' to close): ";
      std::getline(std::cin, message);

      if (message == "exit")
      {
          break;
      }

      send(sock, message.c_str(), message.length(), 0);

      valread = read(sock, buffer, 1024);
      std::cout << "Server: " << buffer << std::endl;
      memset(buffer, 0, sizeof(buffer)); // Clear the buffer
  }

  close(sock);
  getch();
  return 0;
}
