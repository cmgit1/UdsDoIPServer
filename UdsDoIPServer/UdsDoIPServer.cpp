#include "DoIPServer.h"
#include "iso14229.h"
#include "uds_doip_app_interfaces.h"

#include<iostream>
#include<iomanip>
#include<thread>

using namespace std;

DoIPServer server;
UDSTp_t tp_doip;
UDSServer_t uds_srv;
unique_ptr<DoIPConnection> connection(nullptr);
std::vector<std::thread> doipReceiver;
bool serverActive = false;

void DoIPServerSendDiagnosticPayload(unsigned short sourceAddress, unsigned char* data, int length) {
    connection->sendDiagnosticPayload(sourceAddress, data, length);
}
/**
 * Is called when the doip library receives a diagnostic message.
 * @param address   logical address of the ECU sending the diagnostic message
 * @param data      message which was received
 * @param length    length of the message
 */
void ReceiveFromLibrary(unsigned short address, unsigned char* data, int length) {
    cout << "DoIP Message received from 0x" << hex << address << ": ";
    for(int i = 0; i < length; i++) {
        cout << hex << setw(2) << (int)data[i] << " ";
    }
    cout << endl;

    recv_uds_len = length;
}

/**
 * The DoIP library code will call this function when it receives a DoIP diagnostic message.
 * The library notifies the application(i.e., the server code in our case) about the message.
 * The application(i.e., the server code in our case) checks if the target address is equal to this server's logical address
 * @param targetAddress     logical address to the targeted ecu
 * @return                  If a positive or negative ACK should be send to the client
 */
bool DiagnosticMessageReceived(unsigned short targetAddress) {
    (void)targetAddress;
    unsigned char ackCode;

    cout << "Received Diagnostic message" << endl;

    //send positiv ack
    ackCode = 0x00;
    cout << "-> Send positive diagnostic message ack" << endl;
    connection->sendDiagnosticAck(SERVER_LOGICAL_ADDRESS, true, ackCode);

    return true;
}

/**
 * Closes the connection of the server by ending the listener threads
 */
void CloseConnection() {
    cout << "Connection closed" << endl;
    //serverActive = false;
}

/*
 * Check permantly if udp message was received
 */
void listenUdp() {

    while(serverActive) {
        server.receiveUdpMessage();
    }
}

/*
 * Check permantly if tcp message was received
 */
void listenTcp() {

    server.setupTcpSocket();

    while(true) {
        connection = server.waitForTcpConnection();
        connection->setCallback(ReceiveFromLibrary, DiagnosticMessageReceived, CloseConnection);
        connection->setGeneralInactivityTime(50000);

        InitializeUDSServer(&uds_srv, &tp_doip);

         while(connection->isSocketActive()) {
            UDSServerPoll(&uds_srv);
             connection->receiveTcpMessage();
         }
    }
}

void ConfigureDoipServer() {
    // VIN needs to have a fixed length of 17 bytes.
    // Shorter VINs will be padded with '0'
    server.setVIN("VECT0RVEH1CLE0001");
    server.setLogicalGatewayAddress(SERVER_LOGICAL_ADDRESS);
    server.setGID(0);
    server.setFAR(0);
    server.setEID(0);

    // doipserver->setA_DoIP_Announce_Num(tempNum);
    // doipserver->setA_DoIP_Announce_Interval(tempInterval);

}

int main() {
    ConfigureDoipServer();

    server.setupUdpSocket();

    serverActive = true;
    doipReceiver.push_back(thread(&listenUdp));
    doipReceiver.push_back(thread(&listenTcp));

    server.sendVehicleAnnouncement();

    doipReceiver.at(0).join();
    doipReceiver.at(1).join();
    return 0;
}
