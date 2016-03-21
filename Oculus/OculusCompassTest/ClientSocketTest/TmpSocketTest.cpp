

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>

#include <OVR_CAPI.h>
#include <OVR_Math.h>



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8888"
#define DEFAULT_ADDRES "192.168.42.1"

#define TARGET_HEIGHT 200

#define STX '\002'
#define ETX '\003'

void readCompass(ovrSession session, SOCKET ConnectSocket)
{
	using namespace OVR;
	int iResult;
	char sendbuf[10];
	OVR::Posef pose;

	while (1)
	{
		//Change ovr time to predict future
		ovrTrackingState ts = ovr_GetTrackingState(session, ovr_GetTimeInSeconds(), ovrFalse);

		float yaw, pitch, roll;
		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{

			pose = ts.HeadPose.ThePose;
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &roll, &pitch);
			yaw = yaw * 180 / 3.1416;
			yaw += 200;
			printf("compass: %f\n", yaw);
			printf("target height: %d\n", TARGET_HEIGHT);
			//sprintf_s(sendbuf, "%6.1f", yaw);
			sendbuf[0] = STX;
			sendbuf[1] = '0' + (((int)(yaw / 100)) % 10);
			sendbuf[2] = '0' + (((int)(yaw / 10)) % 10);
			sendbuf[3] = '0' + (((int)yaw) % 10);
			sendbuf[4] = '0' + (((int)(TARGET_HEIGHT / 1000)) % 10);
			sendbuf[5] = '0' + (((int)(TARGET_HEIGHT / 100)) % 10);
			sendbuf[6] = '0' + (((int)(TARGET_HEIGHT / 10)) % 10);
			sendbuf[7] = '0' + (((int)TARGET_HEIGHT) % 10);
			sendbuf[8] = ETX;
			sendbuf[9] = '\0';

			iResult = send(ConnectSocket, sendbuf, 10, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
			}

		}
		Sleep(500);

	}

}

// Include the OculusVR SDK
void Application()
{
	ovrResult ovrResult = ovr_Initialize(nullptr);
	if (OVR_FAILURE(ovrResult))
		return;

	ovrSession session;
	ovrGraphicsLuid luid;
	ovrResult = ovr_Create(&session, &luid);
	if (OVR_FAILURE(ovrResult))
	{
		ovr_Shutdown();
		return;
	}

	ovrHmdDesc desc = ovr_GetHmdDesc(session);
	ovrSizei resolution = desc.Resolution;



	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	//char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("192.168.42.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

	readCompass(session, ConnectSocket);

	ovr_Destroy(session);
	ovr_Shutdown();
}
int __cdecl main(int argc, char **argv)
{
	Application();
	return 0;
}


/*int __cdecl main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	//char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("192.168.42.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	char sendbuf[5];
	sendbuf[0] = '|';
	sendbuf[1] = '1';
	sendbuf[2] = '2';
	sendbuf[3] = '3';
	sendbuf[4] = '|';
	while (1) {
		iResult = send(ConnectSocket, sendbuf, 5, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
		}
		Sleep(1000);
	}

	printf("Bytes Sent: %ld\n", iResult);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
*/