
#include <string>
#include <iostream>
#include <vector>
#include "Mission.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

class Fire {
//private:
    string name;
    string date;
    string location;
    double latitude;
    double longitude;
    string description;
    vector<Mission> missions;	

public:
    Fire(string theName, string theDate,string theLocation,double theLatitude,double theLongitude,string TheDescription);
    string get_name();
    void add_mission(Mission newMission);
    vector<Mission> get_missions();
    json get_json();



};


