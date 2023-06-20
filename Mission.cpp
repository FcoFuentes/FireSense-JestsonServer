
#include "Mission.h"
using namespace std;
	//string name;
    //string date;
    //string alt;
    //string state;

Mission::Mission(string theName, string theDate,int theAlt,string theState,double TheLatitude,double TheLongitude,string TheFireName,string TheDescription) : name(theName), date(theDate),alt(theAlt), state(theState) ,latitude(TheLatitude),longitude(TheLongitude),fireName(TheFireName),description(TheDescription){
 
}

string Mission::get_name()  {
    return name;
}

json Mission::get_json(){
	json j;
	j["name"] = name;
	j["date"] = date;
	j["alt"] = alt;
	j["latitude"] = latitude;
	j["longitude"] = longitude;
	j["state"] = state;
	j["fireName"] = fireName;
	j["description"] = description;
	
	return j;
}

