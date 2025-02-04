#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <winsock2.h>
#include <thread>
#include <map>

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

const string USER_DB = "C:/Users/wiewk/Documents/c++/TestMain/users.csv";
const string CHAT_DB = "C:/Users/wiewk/Documents/c++/TestMain/chat_history.csv";
string TEMP_DB = "C:/Users/wiewk/Documents/c++/temp/temp.csv";
map<string, SOCKET> clients;
map<string, bool> online_status; // 온라인 상태 확인 flag

void saveUser(const string& username, const string& password) {
    ofstream file(USER_DB, ios::app);

    if (!file) {
        cerr << "회원가입 실패: " << USER_DB << " 파일을 열 수 없음!" << endl;
        return;
    }

    file << username << "," << password << endl;
    file.close();
    cout << "회원가입 성공: " << username << endl;
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
void deleteUser(const string& username) {
    ifstream file(USER_DB);
    ofstream temp(TEMP_DB);
    string line, user, pass;
    bool found = false;
    while (getline(file,line))
    {
        stringstream ss(line);
        getline(ss, user, ',');
        getline(ss, pass, ',');
        if (user != username)
        {
            temp << line << endl;
        }
    }
    file.close();
    temp.close();
    
    if (remove(USER_DB.c_str())!=0)
    {
        perror("Error deleting original file");
    }
    if (rename("C:/Users/wiewk/Documents/c++/temp/temp.csv", USER_DB.c_str()) != 0)
    {
        perror("Error renaming file");
    }

    if (found)
    {
        cout << "User" << username << " deleted successfully" << endl;
    }
    else
    {
        cout << "User not found" << endl;
    }
}

void saveChat(const string& sender, const string& receiver, const string& message) {
    ofstream file(CHAT_DB, ios::app);
    if (file.is_open()) {
        file << sender << "," << receiver << "," << message << endl;
        file.close();
    }
}
void searchChatHistory(const string& username) {
    ifstream file(CHAT_DB);
    string line;
    cout << "Chat history for : " << username << endl;
    while (getline(file,line))
    {
        if (line.find(username) != string::npos)
        {
            cout << line << endl;
        }
    }
}
void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    string username;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "클라이언트 연결 종료" << endl;
            break;
        }

        string message(buffer);
        cout << "Received: " << message << endl;

        stringstream ss(message);
        string command, user, pass;
        getline(ss, command, ' ');

        if (command == "LOGIN") {
            getline(ss, user, ' ');
            getline(ss, pass, ' ');
            if (authenticateUser(user, pass)) {
                send(clientSocket, "LOGIN_SUCCESS", strlen("LOGIN_SUCCESS"), 0);  // ✅ 응답 보내기
            }
            else {
                send(clientSocket, "LOGIN_FAIL", strlen("LOGIN_FAIL"), 0);  // ✅ 응답 보내기
            }
        }
        else if (command == "REGISTER") {
            getline(ss, user, ' ');
            getline(ss, pass, ' ');
            //saveUser(user, pass);
            send(clientSocket, "REGISTERED", strlen("REGISTERED"), 0);  // ✅ 응답 보내기
        }
        else {
            send(clientSocket, "UNKNOWN_COMMAND", strlen("REGISTERED"), 0);  // ✅ 응답 보내기
        }
    }
    closesocket(clientSocket);
}
int main() {
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (result != 0) {
        cerr << "WSAStartup 실패! 오류 코드: " << result << endl;
        return 1; // 프로그램 종료
    }
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);// 핸들
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN); // 접속 대기 상태

    cout << "Server started on port 54000..." << endl;
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        thread(handleClient, clientSocket).detach();
    }
    WSACleanup();
}