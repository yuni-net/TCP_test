// TCP_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32")

#pragma warning(disable:4996)

int _tmain(int argc, _TCHAR* argv[])
{
	SOCKET sock;
	SOCKADDR_IN ServAddr;
	u_short ServPort = 110;
	char servIP[256] = "192.168.120.11";
	char sendString[1024];
	char receiveBuffer[1024];
	int totalBytesRcvd = 0;	// 受信バイト数

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// TCPソケット作成
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("ソケット生成失敗\n");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(servIP);
	ServAddr.sin_port = htons(ServPort);

	// 接続
	if (connect(sock, (PSOCKADDR) &ServAddr,
		sizeof(ServAddr)) == SOCKET_ERROR) {
		printf("接続エラー\n");
		getchar();
		return -1;
	}

	totalBytesRcvd = 0;
	while (true)
	{
		totalBytesRcvd += recv(sock, receiveBuffer, 1024, 0);
		if (totalBytesRcvd < 2) continue;
		if (receiveBuffer[totalBytesRcvd - 2] != '\r') continue;
		if (receiveBuffer[totalBytesRcvd - 1] != '\n') continue;
		break;
	}

	receiveBuffer[totalBytesRcvd] = 0;
	printf("%s\n", receiveBuffer);


	char * user = "USER nhs30070";
	printf("%s\n", user);
	char command_user[1024];
	sprintf(command_user, "%s%s", user, "\r\n");
	send(sock, command_user, strlen(command_user), 0);

	totalBytesRcvd = 0;
	while (true)
	{
		totalBytesRcvd += recv(sock, receiveBuffer, 1024, 0);
		if (totalBytesRcvd < 2) continue;
		if (receiveBuffer[totalBytesRcvd - 2] != '\r') continue;
		if (receiveBuffer[totalBytesRcvd - 1] != '\n') continue;
		break;
	}

	receiveBuffer[totalBytesRcvd] = 0;
	printf("%s\n", receiveBuffer);

	char * pass = "PASS a19941005";
	printf("%s\n", pass);
	char command_pass[1024];
	sprintf(command_pass, "%s%s", pass, "\r\n");
	send(sock, command_pass, strlen(command_pass), 0);

	totalBytesRcvd = 0;
	while (true)
	{
		totalBytesRcvd += recv(sock, receiveBuffer, 1024, 0);
		if (totalBytesRcvd < 2) continue;
		if (receiveBuffer[totalBytesRcvd - 2] != '\r') continue;
		if (receiveBuffer[totalBytesRcvd - 1] != '\n') continue;
		break;
	}

	receiveBuffer[totalBytesRcvd] = 0;
	printf("%s\n", receiveBuffer);

	char * stat = "STAT";
	printf("%s\n", stat);
	char command_stat[1024];
	sprintf(command_stat, "%s%s", stat, "\r\n");
	send(sock, command_stat, strlen(command_stat), 0);

	totalBytesRcvd = 0;
	while (true)
	{
		totalBytesRcvd += recv(sock, receiveBuffer, 1024, 0);
		if (totalBytesRcvd < 2) continue;
		if (receiveBuffer[totalBytesRcvd - 2] != '\r') continue;
		if (receiveBuffer[totalBytesRcvd - 1] != '\n') continue;
		break;
	}

	receiveBuffer[totalBytesRcvd] = 0;
	printf("%s\n", receiveBuffer);

	char * quit = "QUIT";
	printf("%s\n", quit);
	char command_quit[1024];
	sprintf(command_quit, "%s%s", quit, "\r\n");
	send(sock, command_quit, strlen(command_quit), 0);

	totalBytesRcvd = 0;
	while (true)
	{
		totalBytesRcvd += recv(sock, receiveBuffer, 1024, 0);
		if (totalBytesRcvd < 2) continue;
		if (receiveBuffer[totalBytesRcvd - 2] != '\r') continue;
		if (receiveBuffer[totalBytesRcvd - 1] != '\n') continue;
		break;
	}

	receiveBuffer[totalBytesRcvd] = 0;
	printf("%s\n", receiveBuffer);

	shutdown(sock, SD_BOTH);
	closesocket(sock);

	WSACleanup();

	getchar();

	return 0;
}

