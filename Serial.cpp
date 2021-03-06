// SensorWebPC.cpp
// Pass the port name in the command line - e.g. sensorwebpc comm2
//#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <windows.h>
#include "Serial.h"

Serial::Serial(std::string device) {
	Serial::device = device;
}

void Serial::PrintError( LPCSTR str) {
	LPVOID lpMessageBuffer;
	int error = GetLastError();
	//sublang_default = default language
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMessageBuffer, 0, NULL);
	printf("%s: (%d) %s\n\n",str,error,lpMessageBuffer);
	LocalFree( lpMessageBuffer );
}

//int main(int argc, char* argv[]) {
int Serial::writeData(const char* data) {
	// open port for I/O
//	HANDLE h = CreateFile(argv[1], GENERIC_READ|GENERIC_WRITE, 0,NULL,	OPEN_EXISTING,0,NULL);
	HANDLE h = CreateFile(device.c_str(), GENERIC_READ|GENERIC_WRITE, 0,NULL,	OPEN_EXISTING,0,NULL);
	if(h == INVALID_HANDLE_VALUE) {
		PrintError("E012_Failed to open port");
	}
	else {

		std::stringstream s;
		s<<data;
		std::string data = s.str();
		// set timeouts
		COMMTIMEOUTS cto = { 1, 100, 1000, 0, 0 };
		DCB dcb;
		if(!SetCommTimeouts(h,&cto))
		PrintError("E013_SetCommTimeouts failed");

		// set DCB
		memset(&dcb,0,sizeof(dcb));
		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = 19200;
		dcb.fBinary = 1;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		// dcb.fOutxCtsFlow = 1;
		// dcb.fRtsControl = DTR_CONTROL_HANDSHAKE;

		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;

		if(!SetCommState(h,&dcb)) {
			PrintError("E014_SetCommState failed");
		}

//		char buf[7];
//		DWORD read = 0;
//		DWORD write=1; // Number of bytes to write to serial port
//		buf[0] = 72; // Decmial value to write to serial port
		DWORD write = sizeof(data);

		WriteFile(h,data.c_str(),write,&write,NULL); // write is updated with the number of bytes written

//		ReadFile(h,buf,sizeof(buf),&read,NULL); // read is updated with the number of bytes read
//		DWORD i;
//		for (i=0; i<read; i++) {
//			printf("%i ", (unsigned char)buf[i]);
//		}

		CloseHandle(h);
	}

	return 0;
}
