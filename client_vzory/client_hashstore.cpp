#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const std::string HOST = "127.0.0.1";
const int PORT = 9000;
const std::string HASH = "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824";

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST.c_str(), &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Pripojené na " << HOST << ":" << PORT << std::endl;

    /* ===========================================
     * --- UKAZKA KOMUNIKACIE GET ---
     *  ========================================== */ 
    // posli GET
    std::string cmd = "GET " + HASH + "\n";
    send(sock, cmd.c_str(), cmd.size(), 0);

    // precitaj riadok hlavičky
    std::string header;
    char c;
    while (recv(sock, &c, 1, 0) == 1 && c != '\n') {
        header += c;
    }
    std::cout << "Hlavička servera: " << header << std::endl;

    if (header.substr(0,3) != "200") {
        std::cerr << "Server vrátil chybu" << std::endl;
        close(sock);
        return 1;
    }

    // jednoduché parsovanie: 200 OK <length> <description>
    size_t first_space = header.find(' ');
    size_t second_space = header.find(' ', first_space + 1);
    size_t third_space = header.find(' ', second_space + 1);

    int length = std::stoi(header.substr(second_space + 1, third_space - second_space -1));
    std::string description = header.substr(third_space + 1);

    // precitaj obsah a vypíš na konzolu
    char* buffer = new char[length];
    int received = 0;
    while (received < length) {
        int n = recv(sock, buffer + received, length - received, 0);
        if (n <= 0) break;
        received += n;
    }

    std::cout << "Obsah súboru:" << std::endl;
    std::cout.write(buffer, received);
    std::cout << std::endl;

    delete[] buffer;
    // --- KONIEC UKAZKY ---
    
    // SEM IMPLEMENTOVAT KOMUNIKACIU DALEJ


    
    close(sock);
    return 0;
}
