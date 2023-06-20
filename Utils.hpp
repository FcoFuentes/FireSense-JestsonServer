#include <string>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h> 
#include <sys/types.h> 
#include "Fire.h"
using namespace std;

class Utils {
	
public:
	//string PATH_FIRES = "/home/francisco/fires";
	//string PATH_USB = "/media/francisco";
	string PATH_FIRES = "/var/www/html/FIRES";
    string PATH_SERVER = "/var/www/html";
	string PATH_USB = "/media/nvidia";
    Utils();
    vector<Fire> read_fires();
    int create_MissionDir(json fire);
    string getPath();
    vector<Fire> fires;	
    int create_fireDir(json fire);
    int export_usb(string fire, string mission);
    void addBackSlash(char *str);
    string removeSpaces(string str);
    string captueImage(string str);
    vector<string> get_json_images(string fire,string mission,string folder);
    int import(string fire, string mission);
    int del_Fire(string fire, string mission);
    string getUsbName();

};


