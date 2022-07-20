#include "IRC.h"

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
IRC::IRC() : IModule(0, Category::OTHER, "Toggles the ability to see the IRC Chat") {
}

const char* IRC::getModuleName() {
    return "IRC";
}

SOCKET sock;
string ipAddress = "8.tcp.ngrok.io";
int port = 13718;

void IRC::onEnable() {
    //startConnection();
    toggled = true;
}

void IRC::onTick(GameMode* gm) {
    shouldHide = true;
}

void IRC::onDisable() {
    //endConnection(); // crashes
    toggled = false;
}

/* IRC Connections */

void IRC::startConnection() {
    clientMessageF("Attempting to join IRC Chat...");

    sock = socket(AF_INET, SOCK_STREAM, 0); WSAData data; WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) {
        clientMessageF("%sCouldn't start IRC Connection", RED);
        return;
    }

    // Create socket
    if (sock == INVALID_SOCKET) {
        clientMessageF("%sCouldn't create IRC Connection", RED);
        WSACleanup();
        return;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        clientMessageF("%sCouldn't connect IRC Chat, Error: %s", RED, to_string(WSAGetLastError()).c_str());
        closesocket(sock);
        WSACleanup();
        return;
    }
    else clientMessageF("%sSuccessfully joined the IRC Chat!", GREEN);
}

void IRC::sendMessage(string message) {
    char buf[4096];
    do {
        if (message.size() > 0) {
            // Send the text
            int sendResult = send(sock, message.c_str(), message.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                // wait for response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0) {
                    clientMessageF("IRC >> %s", string(buf, 0, bytesReceived));
                }
            }
        }

    } while (message.size() > 0);
}

void IRC::endConnection() {
    clientMessageF("Successfully left the IRC Chat!");
    closesocket(sock);
    WSACleanup();
}