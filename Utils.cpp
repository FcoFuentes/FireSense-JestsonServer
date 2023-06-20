#include "Utils.hpp"
using namespace std;
#include "nlohmann/json.hpp"
using json = nlohmann::json;

Utils::Utils()   {	
}

string Utils::getPath(){
	return PATH_FIRES;
}

int Utils::create_MissionDir(json mission){
	int check; 
	//string dirFire = PATH_FIRES+"/"+(string(mission.at("fire")));
	string dirMission = PATH_FIRES+"/"+(string(mission.at("fire")))+"/"+(string(mission.at("name")));

    //check = mkdir(dirFire.c_str(),0777); 
    if (!mkdir(dirMission.c_str(),0777)) {
    	mkdir((dirMission+"/RGB").c_str(),0777); 
    	mkdir((dirMission+"/THERMAL").c_str(),0777); 
    	mkdir((dirMission+"/MOSAICO").c_str(),0777); 
    	FILE * fPtr;
    	fPtr = fopen((dirMission+"/json.json").c_str(), "w");
    	fputs(mission.dump().c_str(), fPtr);
    	fclose(fPtr); 
	    
    	ofstream file;
	file.open("/opt/firesense/telemetry/server/src/mision");
	file << dirMission+"/json.json\n";
	file.close();

    	return 1;
    }
    else {
    	printf("Unable to create directory\n"); 
    	return 0;
    }
}

//agregar a lista fires y responder json
int Utils::create_fireDir(json fire){
	//int check; 
	string dirname = PATH_FIRES+"/"+(string(fire.at("name")));
	//check = mkdir(dirname.c_str(),0777); 
	if (!mkdir(dirname.c_str(),0777)) {
		//printf("Directory created\n"); 
		FILE * fPtr;
		//snprintf(buf1, sizeof buf1, "%s%s", buf, "/json.json");
		fPtr = fopen((dirname+"/json.json").c_str(), "w");
		fputs(fire.dump().c_str(), fPtr);
		fclose(fPtr); 
		return 1;
	}
	else {
		printf("Unable to create directory\n"); 
		return 0;
	}

}

string Utils::captueImage(string str){ 
	/*char ch = '\\';  

    for(int i = 0; i < str.size(); i++){  
        if(string[i] == ' ')  
            string[i] = ch;  
            }  */
    //str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
}

void Utils::addBackSlash(char *str){
	char auxStr[500];
	int j =0;
	snprintf(auxStr, sizeof auxStr, "%s", str);
	for(int i = 0;i< strlen(auxStr);i++){
		if(auxStr[i] == 32){
			str[j]   = '\\';
			j=j+1;
		}
		str[j]= auxStr[i];
		j++;
	}
}

/*
int Utils::zipping(string fire, string mission){

	}*/

string Utils::getUsbName(){
	printf("getUsbName \n");
	DIR *d;
	struct dirent *dir;
	d = opendir(PATH_USB.c_str());
	char nameDir[256];
	if (d){
		while ((dir = readdir(d)) != NULL){
			snprintf(nameDir, sizeof nameDir, "%s", dir->d_name);
			//printf("USB: \"%s\" \n ", dir->d_name);
			//printf(".: \"%d\" \n ", strcmp(nameDir, "."));
			//printf("..: \"%d\" \n ", strcmp(nameDir, ".."));
			if( (strcmp(nameDir, ".") != 0) && (strcmp(nameDir, "..") != 0) ){
				//snprintf(nameDir, sizeof nameDir, "%s", "FAIL");
				//printf("%s(%ld) - %s(%ld)\n", dir->d_name,strlen(dir->d_name),nameDir,strlen(nameDir));
				break;
			}                 
		}
		closedir(d);
	}
	//printf(" - nombre USB: \"%s\" \n ", nameDir);
	return string(nameDir);

}


int Utils::export_usb(string fire, string mission){
	DIR *d;
	struct dirent *dir;
	d = opendir(PATH_USB.c_str());
	char nameDir[256];
	if (d){
		while ((dir = readdir(d)) != NULL){
			snprintf(nameDir, sizeof nameDir, "%s", dir->d_name);
			if( (strcmp(nameDir, ".") != 0) && (strcmp(nameDir, "..") != 0) ){
				//printf("%s(%ld) - %s(%ld)\n", dir->d_name,strlen(dir->d_name),nameDir,strlen(nameDir));
				break;
			}                 
		}
		closedir(d);
	}
	//printf(" - nombre USB: \"%s\" \n ", nameDir);
	char fireTemp[50];
	char missionTemp[50];
	strncpy(fireTemp, fire.c_str(), sizeof(fireTemp));
	strncpy(missionTemp, mission.c_str(), sizeof(missionTemp));
	addBackSlash(fireTemp);
	addBackSlash(missionTemp);
	string fireName = string(fireTemp);
	string missionName = string(missionTemp);
	//cout << " export "<<fireTemp <<  " - "<<missionTemp << endl;
	string commandZip = "zip -r "+fireName+"_"+missionName+".zip "+PATH_FIRES+"/"+fireName+"/"+missionName;
	system(commandZip.c_str());
	string commandMv = "mv "+fireName+"_"+missionName+".zip "+PATH_USB+"/"+nameDir;
	system(commandMv.c_str());
	//cout <<"Command: " << commandZip << endl;
	return 0 ;
}

//elimina 
int Utils::del_Fire(string fire,string mission){
	char fireTemp[50];
	char missionTemp[50];
	strncpy(fireTemp, fire.c_str(), sizeof(fireTemp));
	strncpy(missionTemp, mission.c_str(), sizeof(missionTemp));
	addBackSlash(fireTemp);
	addBackSlash(missionTemp);
	string fireName = string(fireTemp);
	string missionName = string(missionTemp);
//PATH_FIRES+"/"+string(dir->d_name)
	string commandDel = "rm "+PATH_FIRES+"/"+fireName+"/"+missionName+" -R";
	//cout <<"del_Fire: " << commandDel << endl;
	system(commandDel.c_str());
	return 0;
}

//verifica si hay una memoria usb si la hay exporta el incendio/mision
int Utils::import(string fire, string mission){
	DIR *d;
	struct dirent *dir;
	d = opendir(PATH_USB.c_str());
	char nameDir[256];
	if (d){
		while ((dir = readdir(d)) != NULL){
			snprintf(nameDir, sizeof nameDir, "%s", dir->d_name);
			if( (strcmp(nameDir, ".") != 0) && (strcmp(nameDir, "..") != 0) ){
				//printf("%s(%ld) - %s(%ld)\n", dir->d_name,strlen(dir->d_name),nameDir,strlen(nameDir));
				break;
			}                 
		}
		closedir(d);
	}
	//printf(" - nombre USB: \"%s\" \n ", nameDir);

	char fireTemp[50];
	char missionTemp[50];
	strncpy(fireTemp, fire.c_str(), sizeof(fireTemp));
	strncpy(missionTemp, mission.c_str(), sizeof(missionTemp));
	addBackSlash(fireTemp);
	addBackSlash(missionTemp);

	string fireName = string(fireTemp);
	string missionName = string(missionTemp);

	//cout << " export "<<fireTemp <<  " - "<<missionTemp << endl;

	string commandZip = "zip -r "+fireName+"_"+missionName+".zip "+PATH_FIRES+"/"+fireName+"/"+missionName;
	system(commandZip.c_str());

	string commandMv = "mv "+fireName+"_"+missionName+".zip "+PATH_SERVER;
	system(commandMv.c_str());
	//cout <<"Command: " << commandMv << endl;
	return 0 ;
}

vector<Fire> Utils::read_fires(){
	DIR *d;
	struct dirent *dir;
	d = opendir(PATH_FIRES.c_str());
	if (d){
		fires.clear();
		while ((dir = readdir(d)) != NULL){
			string fireName = dir->d_name;
			if( ("." != fireName) && (".." != fireName) ){
				ifstream ifsFire(PATH_FIRES+"/"+string(dir->d_name)+"/json.json");				
				string contentFire((istreambuf_iterator<char>(ifsFire)),(istreambuf_iterator<char>()));	
				json jsonFire =json::parse(contentFire);				
				Fire f1 {jsonFire.at("name"), jsonFire.at("fecha"),jsonFire.at("localidad"),jsonFire.at("latitude"),jsonFire.at("longitude"),jsonFire.at("description") };				
				DIR *m;
				struct dirent *mision;
				string missionPath = PATH_FIRES+"/"+string(dir->d_name);
				m = opendir(missionPath.c_str());
				if (m){            
					while ((mision = readdir(m)) != NULL){						
						string missionName = mision->d_name;
						if( ("." != missionName) && (".." != missionName) && ("json.json" != missionName)){
							ifstream ifsMission(missionPath+"/"+missionName+"/json.json");
							string contentMission((istreambuf_iterator<char>(ifsMission)),
								(istreambuf_iterator<char>()));	
							json jsonMission =json::parse(contentMission);
							Mission m1 {jsonMission.at("name"), jsonMission.at("date"),jsonMission.at("alt"),jsonMission.at("state"),jsonMission.at("latitude"),jsonMission.at("longitude"),jsonMission.at("fireName"),jsonMission.at("description")};
							f1.add_mission(m1);
						}			                         
					}
				}
				fires.push_back(f1);
			}			
		}              
	}
	return fires;
}

vector<string> Utils::get_json_images(string fire,string mission,string folder){
	vector<string> imageList;
	DIR *d;
	struct dirent *dir;
	string path = PATH_FIRES+"/"+fire+"/"+mission+"/"+folder;
	d = opendir(path.c_str());
	if (d){
		while ((dir = readdir(d)) != NULL){
			string image = dir->d_name;
			
			if( ("." != image) && (".." != image) ){
				imageList.push_back(image);
				//cout << "Incendio: " << image << endl;
			}
			/*if( ("." != fireName) && (".." != fireName) ){
				ifstream ifsFire(PATH_FIRES+"/"+string(dir->d_name)+"/json.json");
				string contentFire((istreambuf_iterator<char>(ifsFire)),(istreambuf_iterator<char>()));	
				json jsonFire =json::parse(contentFire);
				Fire f1 {jsonFire.at("name"), jsonFire.at("fecha"),jsonFire.at("localidad") };				

				DIR *m;
				struct dirent *mision;
				string missionPath = PATH_FIRES+"/"+string(dir->d_name);
				m = opendir(missionPath.c_str());
				if (m){            
					while ((mision = readdir(m)) != NULL){
						string missionName = mision->d_name;
						if( ("." != missionName) && (".." != missionName) && ("json.json" != missionName)){
							ifstream ifsMission(missionPath+"/"+missionName+"/json.json");
							string contentMission((istreambuf_iterator<char>(ifsMission)),
								(istreambuf_iterator<char>()));	

							json jsonMission =json::parse(contentMission);
							Mission m1 {jsonMission.at("name"), jsonMission.at("date"),jsonMission.at("alt"),jsonMission.at("state") };
							f1.add_mission(m1);
						}			                         
					}
				}
				fires.push_back(f1);
			}		*/	
		}              
	}
return imageList;

}

