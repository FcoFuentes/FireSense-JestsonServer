#include <arpa/inet.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include "Utils.hpp"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace std; 
#define PORT 65321 
#define PORTC 10000 

class Communication{
public:

	//int argc; 
	//char** argv;

	/*enum COMMANDS{   
		CREATEFIRE = 2931118425,
		GETFIRES = 3653579992,
		CREATEMISSION = 3543534797
		
	};*/

	//Communication(Utils theUtils,int argc,char** argv);
	Communication(Utils theUtils);
		string PATH_THERMAL = "/dev/video0";
		string PATH_RGB = "/dev/video1";
		string convertToString(char* a, int size);
		void execSystemCommand2(const std::string &commandstr);
		void init();
		void start();
		json parse();
		json create_mission(json jMission);
		static void capture(string msg);
		json get_fires();
		json create_fire(json jFire);
		json export_usb(json jsonData);
		json json_images(json jsonData);
		json import(json jsonData);
		json test_conn();
		json del_fire(string fire, string mission);
		json info();
		void setFlagThread();
		json shut_down();
		json reboot();
		

private: 
	Utils utils;
	 //int argc;
	//char** argv;
	//inline static int argc; 
	//inline static char** argv;
	 int new_socket;
	 int server_fd, valread; 
	 int opt= 1; 
	 struct sockaddr_in address; 
	 int addrlen = sizeof(address); 
	 char buffer[1024] = {'0'};
	 string jsonIn="";
	 char strJsonResp[100000]= {0}; 
	 inline static bool flagThread = false;
	 inline static bool flagExit = false;
	 inline static json j_complete;

};
