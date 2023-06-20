
#include "Fire.h"


using namespace std;

Fire::Fire(string theName, string theDate,string theLocation,double TheLatitude,double TheLongitude,string TheDescription):name(theName), date(theDate),location(theLocation),latitude(TheLatitude),longitude(TheLongitude),description(TheDescription) {
}

string Fire::get_name()  {
    return name;
}

void Fire::add_mission(Mission newMission){	
	missions.push_back(newMission);
}

vector<Mission> Fire::get_missions(){
	return missions;

}

json Fire::get_json(){
	json j;
	j["name"] = name;
	j["date"] = date;
	j["location"] = location;	
	j["latitude"] = latitude;
	j["longitude"] = longitude;
	j["description"] = description;

	vector<json> jMissions;
	for(int i = 0; i<missions.size();i++){
		jMissions.push_back(missions[i].get_json());

       // Mission mission =  fire.get_missions()[j];
    }
    j["misions"] = jMissions;
	return j;
}
