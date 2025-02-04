#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <winsock2.h>
#include <thread>
#include <map>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const string USER_DB = "users.csv";
const string CHAT_DB = "chat_history.csv";
map<string, SOCKET> clients;

void saveUser(const string& username, const string& password) {
    ofstream file(USER_DB, ios::app);
    if (file.is_open()) {
        file << username << "," << password << endl;
        file.close();
    }
}

bool authenticateUser(const string& username, const string& password) {
    ifstream file(USER_DB);
    string line, user, pass;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, user, ',');
        getline(ss, pass, ',');
        if (user == username && pass == password) {
            return true;
        }
    }
    return false;
}

void saveChat(const string& sender, const string& receiver, const string& message) {
    ofstream file(CHAT_DB, ios::app);
    if (file.is_open()) {
        file << sender << "," << receiver << "," << message << endl;
        file.close();
    }
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    string username;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        string message(buffer);
        cout << "Received: " << message << endl;

        stringstream ss(message);
        string command, user, pass, receiver, text;
        getline(ss, command, ' ');

        if (command == "LOGIN") {
            getline(ss, user, ' ');
            getline(ss, pass, ' ');
            if (authenticateUser(user, pass)) {
                username = user;
                clients[user] = clientSocket;
                send(clientSocket, "LOGIN_SUCCESS", 13, 0);
            }
            else {
                send(clientSocket, "LOGIN_FAIL", 10, 0);
            }
        }
        else if (command == "REGISTER") {
            getline(ss, user, ' ');
            getline(ss, pass, ' ');
            saveUser(user, pass);
            send(clientSocket, "REGISTERED", 10, 0);
        }
        else if (command == "SEND") {
            getline(ss, receiver, ' ');
            getline(ss, text);
            saveChat(username, receiver, text);
            if (clients.find(receiver) != clients.end()) {
                send(clients[receiver], text.c_str(), text.size(), 0);
            }
        }
    }
    closesocket(clientSocket);
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(54000);
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    cout << "Server started on port 54000..." << endl;
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        thread(handleClient, clientSocket).detach();
    }
    WSACleanup();
}