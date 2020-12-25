#include <winsock.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "wsock32.lib")

int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET sock;
	sockaddr_in server; 

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	struct hostent* ghbn = gethostbyname("zerodium.com");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	server.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)ghbn->h_addr_list[0]))); 

	if (connect(sock, (sockaddr*)&server, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	char send_data[] = "GET /images/slide-2-bg.jpg HTTP/1.0\r\nConnection: close\r\nUser-agent: Mozilla/4.0\r\nAccept: text/html, image/gif, image/jpeg\r\n\r\n";
	char recv_data[1024];
	ZeroMemory(recv_data, 1024);

	send(sock, send_data, strlen(send_data), 0);
	recv(sock, recv_data, 1024, 0);

	char* temp = strstr(recv_data, "Content-Length: ");
	if (temp != NULL)
	{
		while (*temp != '\n') printf("%c", *temp++);
		printf("\n");
	}
	else
		printf("No such information!\n");
	closesocket(sock);
	WSACleanup();

	return 0;
}