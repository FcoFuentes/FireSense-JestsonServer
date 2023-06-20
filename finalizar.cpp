using namespace std;
#include "nlohmann/json.hpp"
using json = nlohmann::json;
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
int main(int argc, char** argv) {
	//usleep(1000);

	string nombreArchivo = "mision";
    ifstream archivo(nombreArchivo.c_str());
    string linea;
    getline(archivo, linea);
	cout << linea << endl;

	/*string nombreArchivoAlt = "altura";
    ifstream archivoAlt(nombreArchivoAlt.c_str());
    string lineaAlt;
    getline(archivoAlt, lineaAlt);
	cout << lineaAlt << endl;*/

	ifstream ifsMission(linea.c_str());
	string contentMission((istreambuf_iterator<char>(ifsMission)),
		(istreambuf_iterator<char>()));	

	json jsonMission =json::parse(contentMission);
	cout << jsonMission << endl << endl;
	jsonMission["state"] = "FINALIZED";
	//jsonMission["alt"] = lineaAlt;
	cout << jsonMission.at("state") << endl << endl;

	FILE * fPtr;
	fPtr = fopen(linea.c_str(), "w");
	fputs(jsonMission.dump().c_str(), fPtr);
	fclose(fPtr); 
	remove( nombreArchivo.c_str() );
	//remove( nombreArchivoAlt.c_str() );
}
