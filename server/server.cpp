#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

using namespace std;

mutex mtx;
map<string, string> user_credentials; // username -> password
map<string, bool> online_users;       // username -> online status
vector<SOCKET> clients;               // 현재 접속 중인 클라이언트 리스트

// 사용자 데이터 로드
void loadUsers() {
    ifstream file("C:/Users/wiewk/Documents/c++/TestMain/users.csv");
    string username, password;
    while (file >> username >> password) {
        user_credentials[username] = password;
    }
    file.close();
}

// 새로운 사용자 저장
void saveUser(const string& username, const string& password) {
    ofstream file("C:/Users/wiewk/Documents/c++/TestMain/users.csv", ios::app);
    if (file.is_open()) {
        file << username << " " << password << "\n";
        file.close();
    }
    user_credentials[username] = password;
}

// 사용자 삭제
void deleteUser(const string& username) {
    user_credentials.erase(username);
    ofstream file("C:/Users/wiewk/Documents/c++/TestMain/users.csv");
    for (const auto& user : user_credentials) {
        file << user.first << " " << user.second << "\n";
    }
    file.close();
}

// 사용자 상태 업데이트
void updateUserStatus(const string& username, bool online) {
    online_users[username] = online;
}

// 채팅 메시지 저장
void saveMessage(const string& sender, const string& message) {
    ofstream file("C:/Users/wiewk/Documents/c++/TestMain/chat_history.csv", ios::app);
    if (file.is_open()) {
        file << sender << "," << message << "\n";
        file.close();
    }
}

// 특정 사용자의 채팅 내역 검색
string retrieveChatHistory(const string& username) {
    ifstream file("C:/Users/wiewk/Documents/c++/TestMain/chat_history.csv");
    string line, history = "=== Chat History ===\n";

    while (getline(file, line)) {
        if (line.find(username) != string::npos) {
            history += line + "\n";
        }
    }
    file.close();
    return history.empty() ? "No chat history found.\n" : history;
}

// 메시지를 모든 클라이언트에게 브로드캐스트 (보낸 클라이언트를 제외)
void broadcastMessage(const string& sender, const string& message, SOCKET senderSocket) {
    lock_guard<mutex> lock(mtx);
    cout<< "[" + sender + "] [" + (online_users[sender] ? "Online" : "Offline") + "] : " + message << endl;

    for (SOCKET client : clients) {
        if (client != senderSocket) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

// 클라이언트 처리
void handleClient(SOCKET client_socket) {
    if (clients.size() >=2)
    {
        send(client_socket, "Server full", 12, 0);
        closesocket(client_socket);
        return;
    }
    clients.push_back(client_socket);
    char buffer[1024];
    string username, password;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        string choice(buffer);
        choice.erase(choice.find_last_not_of(" \n\r\t") + 1);

        if (choice == "1") { // 회원가입
            send(client_socket, "Enter new username: ", strlen("Enter new username: "), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            username = buffer;
            username.erase(username.find_last_not_of(" \n\r\t") + 1);

            send(client_socket, "Enter new password: ", strlen("Enter new password: "), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            password = buffer;
            password.erase(password.find_last_not_of(" \n\r\t") + 1);

            saveUser(username, password);
            send(client_socket, "Registration successful\n", 25, 0);
        }
        else if (choice == "2") { // 로그인
            send(client_socket, "Enter username: ", 16, 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            username = buffer;
            username.erase(username.find_last_not_of(" \n\r\t") + 1);

            send(client_socket, "Enter password: ", 16, 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            password = buffer;
            password.erase(password.find_last_not_of(" \n\r\t") + 1);

            cout << "[DEBUG] Received Username: " << username << ", Password: " << password << endl;
            cout << "[DEBUG] Stored Password for " << username << ": " << user_credentials[username] << endl;

            if (user_credentials.find(username) != user_credentials.end() && user_credentials[username] == password) {
                send(client_socket, "Login successful\n", strlen("Login successful\n"), 0);
                updateUserStatus(username, true);
                send(client_socket, "You can now start chatting\n", 28, 0); // 로그인 후 즉시 메시지 전송
                
                while (true) {
                    memset(buffer, 0, sizeof(buffer));
                    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
                    if (bytesReceived <= 0) break;

                    string message(buffer);
                    if (message == "exit")
                    {   
                        cout << message << endl;
                        send(client_socket, "Loged out", strlen("Loged out"), 0);
                        break;
                    }
                    broadcastMessage(username, message, client_socket);
                    saveMessage(username, message);
                }
            }
            else {
                send(client_socket, "Invalid credentials\n", 21, 0);
            }
        }
        else if (choice == "3") { // 회원탈퇴
            send(client_socket, "Enter username to delete: ", 27, 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            username = buffer;
            username.erase(username.find_last_not_of(" \n\r\t") + 1);
            deleteUser(username);
            send(client_socket, "Account deleted\n", 17, 0);
        }
        else if (choice == "4") { // 채팅 내역검색
            send(client_socket, "Enter username to search chat history: ",strlen("Enter username to search chat history: "),0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            username = buffer;
            username.erase(username.find_last_not_of(" \n\r\t") + 1);
            string history = retrieveChatHistory(username);
            send(client_socket, history.c_str(), history.size(), 0);
        }
        else if (choice == "5" ) // 종료
        {
            closesocket(client_socket);
            return;
        }
        else
        {
            send(client_socket,"invalid command!!", strlen("invalid command!!"),0);
            continue;
        }
    }


    updateUserStatus(username, false);
    closesocket(client_socket);
    clients.erase(remove(clients.begin(), clients.end(), client_socket), clients.end());
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup 실패! 오류 코드: " << result << endl;
        return 1; // 프로그램 종료
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 2);
    cout << "Server listening on port 8080...\n";

    loadUsers(); // 사용자 정보 불러오기
    //
    while (true) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        thread(handleClient, client_socket).detach();
    }

    WSACleanup();
    return 0;
}
