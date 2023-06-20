
#include <string>
#include <iostream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

class Mission {
//private:
    string name;
    string date;
    int alt;
    string state;
    double latitude;
    double longitude;
    string fireName;
    string description;

public:
    Mission(string theName, string theDate,int theAlt,string theState,double TheLatitude,double TheLongitude,string TheFireName,string TheDescription);

    // accessor
    string get_name() ;
    json get_json();

};


