#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

mutex fileMutex;
map<string, string> sharedFiles; // FileName -> FilePath

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    read(clientSocket, buffer, BUFFER_SIZE);
    string request(buffer);
    
    if (request.find("GET ") == 0) {
        string fileName = request.substr(4);
        fileMutex.lock();
        if (sharedFiles.find(fileName) != sharedFiles.end()) {
            ifstream file(sharedFiles[fileName], ios::binary);
            if (file) {
                stringstream ss;
                ss << file.rdbuf();
                string fileData = ss.str();
                write(clientSocket, fileData.c_str(), fileData.size());
            }
        }
        fileMutex.unlock();
    }
    close(clientSocket);
}

void startServer() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    cout << "[SERVER] Listening on port " << PORT << "..." << endl;
    
    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        thread(handleClient, clientSocket).detach();
    }
}

void requestFile(const string& serverIP, const string& fileName) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
    
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
        string request = "GET " + fileName;
        write(clientSocket, request.c_str(), request.size());
        
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        read(clientSocket, buffer, BUFFER_SIZE);
        ofstream outFile("downloaded_" + fileName, ios::binary);
        outFile.write(buffer, strlen(buffer));
        cout << "[CLIENT] File downloaded as downloaded_" << fileName << endl;
    }
    close(clientSocket);
}

int main() {
    cout << "1. Start as Server" << endl;
    cout << "2. Request a File" << endl;
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        cout << "Enter file to share (format: filename filepath): ";
        string fileName, filePath;
        cin >> fileName >> filePath;
        fileMutex.lock();
        sharedFiles[fileName] = filePath;
        fileMutex.unlock();
        
        thread serverThread(startServer);
        serverThread.join();
    } else if (choice == 2) {
        cout << "Enter server IP and file name: ";
        string serverIP, fileName;
        cin >> serverIP >> fileName;
        requestFile(serverIP, fileName);
    }
    return 0;
}
