// TCP_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <fstream>

#pragma comment(lib, "ws2_32")

#pragma warning(disable:4996)

class Receiver
{
public:
	void receive_line(SOCKET & sock);
	void show();
	const char * get_text() const;

private:
	static const int buff_bytes = 2048;
	char buffer[buff_bytes];
};

void Receiver::receive_line(SOCKET & sock)
{
	int total_bytes = 0;
	while (true)
	{
		total_bytes += recv(sock, buffer+total_bytes, buff_bytes-total_bytes-1, 0);
		if (total_bytes < 2) continue;
		if (total_bytes >= buff_bytes - 1)
		{
			break;
		}
		if (buffer[total_bytes - 2] != '\r') continue;
		if (buffer[total_bytes - 1] != '\n') continue;
		break;
	}

	buffer[total_bytes] = 0;
}

void Receiver::show()
{
	puts(buffer);
}

const char * Receiver::get_text() const
{
	return buffer;
}

class MailDownloader
{
public:
	bool connect_to_server();
	int get_mail_quantity() const;
	void download_show_mail(int index);
	void finish();


	~MailDownloader();
private:
	SOCKET sock;
	int mail_quantity;

	bool get_status();
};



bool MailDownloader::connect_to_server()
{
	Receiver receiver;

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
		return false;
	}

	receiver.receive_line(sock);
	receiver.show();

	get_status();


}

int MailDownloader::get_mail_quantity() const
{
	return mail_quantity;
}

void MailDownloader::download_show_mail(int index)
{
	char request[1024];
	sprintf(request, "RETR %u\r\n", index+1);
	send(sock, request, strlen(request), 0);

	Receiver receiver;
	receiver.receive_line(sock);
	receiver.show();
	const char * reply = receiver.get_text();
	if (memcmp(reply, "+OK", 3) != 0)
	{
		puts("メッセージの受信に失敗しました。\n");
		return;
	}

	std::ofstream ofs("log.dat", std::ios::out | std::ios::trunc | std::ios::binary);

	while (true)
	{
		receiver.receive_line(sock);
		receiver.show();
		ofs.write(receiver.get_text(), strlen(receiver.get_text()));
		const char * text = receiver.get_text();
		const int length = strlen(text);
		const char * end3 = text + length - 3;
		if (memcmp(end3, ".\r\n", 3) == 0)
		{
			return;
		}
	}
}

void MailDownloader::finish()
{
	char * quit = "QUIT";
	printf("%s\n", quit);
	char command_quit[1024];
	sprintf(command_quit, "%s%s", quit, "\r\n");
	send(sock, command_quit, strlen(command_quit), 0);

	Receiver receiver;
	receiver.receive_line(sock);
	receiver.show();
}

MailDownloader::~MailDownloader()
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	WSACleanup();
}



bool MailDownloader::get_status()
{
	Receiver receiver;

	char * user = "USER nhs30070";
	printf("%s\n", user);
	char command_user[1024];
	sprintf(command_user, "%s%s", user, "\r\n");
	send(sock, command_user, strlen(command_user), 0);

	receiver.receive_line(sock);
	receiver.show();

	char * pass = "PASS d19941005";
	printf("%s\n", pass);
	char command_pass[1024];
	sprintf(command_pass, "%s%s", pass, "\r\n");
	send(sock, command_pass, strlen(command_pass), 0);

	receiver.receive_line(sock);
	receiver.show();

	puts("user name and password is ok.\n");

	char * stat = "STAT";
	printf("%s\n", stat);
	char command_stat[1024];
	sprintf(command_stat, "%s%s", stat, "\r\n");
	send(sock, command_stat, strlen(command_stat), 0);

	receiver.receive_line(sock);
	receiver.show();

	int damy;
	sscanf(receiver.get_text(), "+OK %d %d", &mail_quantity, &damy);

	return true;
}


/// nhs30070@nagoya.hal.ac.jp


int _tmain(int argc, _TCHAR* argv [])
{
	MailDownloader loader;
	loader.connect_to_server();

	int index;
	if (loader.get_mail_quantity() > 0)
	{
		printf("メールが%d件届いています。\n", loader.get_mail_quantity());
		puts("何番目のメールを受信しますか？\n");
		printf("0～%dの間で指定してください。\n", loader.get_mail_quantity() - 1);
		scanf("%d", &index);
	}

	loader.download_show_mail(index);
	loader.finish();

	getchar();
}
