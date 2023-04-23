#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#pragma comment (lib,"ws2_32.lib")
#include<winsock2.h>
#include<iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>

#pragma warning (disable:4996)

void send_file(SOCKET* sock, const std::string& file_name) {
	std::fstream file;
	file.open(file_name, std::ios_base::in | std::ios_base::binary);


	if (file.is_open()) {

		int file_size = std::experimental::filesystem::file_size(file_name) + 1;

		char* bytes = new char[file_size];

		file.read(bytes, file_size);

		std::cout << "size: " << file_size << std::endl;
		std::cout << "name: " << file_name << std::endl;
		std::cout << "data: " << bytes << std::endl;

		send(*sock, std::to_string(file_size).c_str(), 16, 0);
		send(*sock, file_name.c_str(), 32, 0);
		send(*sock, bytes, file_size, 0);
		delete[]bytes;
	}
	else {
		std::cout << "Error in file opening \n";
	}
	file.close();
}
int main() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "error" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error 2 \n";
	}
	else {
		std::cout << " Client connected \n";
		std::string path;
		std::cin >> path;
		send_file(&newConnection, path);
	}


	system("pause");
	return 0;
}