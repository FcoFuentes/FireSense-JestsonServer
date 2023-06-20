#include "Communication.hpp"
#include "nlohmann/json.hpp"
#include "utils.hpp"
using json = nlohmann::json;

//Communication::Communication(Utils theUtils,int argc, char** argv): utils(theUtils),argc(argc),argv(argv){  
Communication::Communication(Utils theUtils) : utils(theUtils)
{
}

void
Communication::execSystemCommand2(const std::string &commandstr)
{
  const char *command = commandstr.c_str();
  system(command);
}

void Communication::init()
{
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  //
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  //
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

string Communication::convertToString(char* a, int size){ 
  int i; 
  string s = ""; 
  for (i = 0; i < size; i++) { 
    if ('\0' == a[i])
      break;
    s = s + a[i]; 
  } 
  return s; 
} 

void Communication::start()
{
  bool flag = false;
  printf("start\n");
  while (1)
  {
    int sock = (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen));
    //printf("sock: %d\n", sock);
    if (sock < 0)
    {
      exit(EXIT_FAILURE);
    }
    else if (new_socket != 7)
    { //revisar
   //   cout << new_socket << endl;
      valread = read(new_socket, buffer, 1024);
      if (valread > 0)
      {
        jsonIn = convertToString(buffer, 1024);
        printf("json: %s\n", buffer);
        json j = parse();
        send(new_socket, j.dump().c_str(), strlen(j.dump().c_str()), 0);
        close(new_socket);
        memset(buffer, 0, sizeof buffer);
        printf("\n");
        j_complete = json::parse(jsonIn);
        string command = string(j_complete.at("command"));
        if ((command.compare(string("CREATEMISSION"))) == 0)
        {
          std::cout << "inicio.\n";
          //ruta de la mision, dentro de esta ruta estan las carpetas RGB, THERMAL y MOSAICO
          string path = "/var/www/html/FIRES/"+string(j_complete.at("data").at("fire"))+"/"+string(j_complete.at("data").at("name"));
          //createFolder("/opt/firesense/environment-variables.txt", path);
          initEnvFile("/opt/firesense/environment-variables.txt");
          updateEnvFile("/opt/firesense/environment-variables.txt", "FLIGHTPATH", path+"/");
          execSystemCommand2("sudo systemctl start server-telemetry.service &");
          flagThread = true;
        }
        else if ((command.compare(string("REBOOT"))) == 0)// || (command.compare(string("SHUTDOWN"))) == 0)
          {
            printf("REBOOTREBOOT");
            system("shutdown -r");
            exit(0);
            // flagThread = true;
            // cout << " json: " << j_complete << "..." << endl;
            // jsonData = json::parse(string(j_complete.at("data")));
            // json mission
          }
          else if((command.compare(string("SHUTDOWN"))) == 0)
          {
            printf("APAGARAPGAR");
            system("shutdown -h");
            exit(0);

          }
      }
    }
    printf("end while\n");
  }
}

constexpr unsigned int string2int(const char* str, int h = 0){
  return !str[h] ? 5381 : (string2int(str, h+1)*33) ^ str[h];
}


json Communication::create_fire(json jFire)
{
  json j;
  j["command"] = "CREATEFIRE";
  if (utils.create_fireDir(jFire))
    j["state"] = "success";
  else
    j["state"] = "error";
  return j;
}

json Communication::create_mission(json jMission)
{
  json j;
  j["command"] = "CREATEMISSION";
  j["fire"] = jMission.at("fire");
  j["name"] = jMission.at("name");

  if (utils.create_MissionDir(jMission))
  {
    j["state"] = "success";
    //capture(jMission); //llama al algoritmo
  }
  else
    j["state"] = "error";
  return j;
}


//inicia la captura 
void Communication::capture(string msg){
  while (true)
  {
    if (flagThread)
    {      
      // execSystemCommand("sudo systemctl stop server-telemetry.service &");
      //std::cout << "\nfin.\n";
      //flagThread = false;
      // return 1;
   }
  }
}

json Communication::get_fires()
{
  vector<Fire> fires = utils.read_fires();
  json j;
  j["command"] = "GETFIRES";
  vector<json> jFires;

  for (int i = 0; i < fires.size(); i++)
  {
    Fire fire = fires[i];
    jFires.push_back(fire.get_json());
   // cout << fire.get_name() << endl;
  }
  j["fires"] = jFires;
  //cout << " jsoni: " << j << endl;
  return j;
}


json Communication::export_usb(json jsonData)
{
  cout << "export_usb" << endl;
  json j;
  j["command"] = "EXPORTUSB";
  string fire = jsonData.at("fire");
  string mission = jsonData.at("mission");
  utils.export_usb(fire, mission);
  //cout << fire << " - " << mission << endl;
  j["state"] = "success";
  j["msj"] = "Se exporto con exito";
  return j;
}

json Communication::import(json jsonData)
{
  json j;
  j["command"] = "IMPORT";
  string fire = jsonData.at("fire");
  string mission = jsonData.at("mission");
  utils.import(fire, mission);
  cout << fire << " - " << mission << endl;
  j["state"] = "success";
  j["msj"] = "Se importó con exito";
  return j;
}

json Communication::json_images(json jsonData)
{
  json j;
  string fire = jsonData.at("fire");
  string mission = jsonData.at("mission");
  string folder = jsonData.at("folder");
  j["command"] = "GETIMAGES";
  vector<string> imageList = utils.get_json_images(fire, mission, folder);
  for (int i = 0; i < imageList.size(); i++)
  {
    cout << "Incendio: " << imageList.at(i) << endl;
  }
  j["data"] = imageList;
  return j;
}

json Communication::del_fire(string fire, string mission)
{
  json j;
  //string fire = jsonData.at("fire");
  j["command"] = "DELFIRE";
  cout << "del_fire: " << fire << "mission: " << mission << endl;
  utils.del_Fire(fire, mission);
  /*string commandZip = "zip -r "+fireName+"_"+missionName+".zip "+PATH_FIRES+"/"+fireName+"/"+missionName;
  system(commandZip.c_str());
  string mission = jsonData.at("mission");
  string folder = jsonData.at("folder");
  
  j["command"] = "GETIMAGES";
  vector<string> imageList = utils.get_json_images(fire,mission,folder);
  for(int i = 0; i< imageList.size();i++){
    cout << "Incendio: " << imageList.at(i) << endl;
    }*/
  j["state"] = "success";
  return j;
}

json Communication::test_conn()
{
  json j;
  j["command"] = "TESTCONN";
  //  utils.import(fire,mission);
  //cout <<fire <<  " - "<<mission << endl;
  j["state"] = "success";
  j["msj"] = "conexion exitosa";
  return j;
}

json Communication::info()
{
  json j;
  j["command"] = "GETINFO";
  fs::space_info devi = fs::space("/");
  j["state"] = "success";
  j["capacity"] = devi.capacity / 1000000;
  j["free"] = devi.free / 1000000;
  j["available"] = devi.available / 1000000;
  string usb = utils.getUsbName();
  j["usb"] = usb;
  ifstream myfile;
  myfile.open(PATH_RGB);
  if (myfile)
  {
    j["rgb"] = "OK";
  }
  else
  {
    j["rgb"] = "FAIL";
  }

  ifstream myfile1;
  myfile1.open(PATH_THERMAL);
  if (myfile1)
  {
    // cout<<"file exists";
    j["thermal"] = "OK";
  }
  else
  {
    j["thermal"] = "FAIL";
  }
  return j;
}

json Communication::shut_down()
{
  cout << "Communication::shut_down(): "  << endl;
  json j;
  j["command"] = "SHUTDOWN";
  j["state"] = "success";
  //system("shutdown -h +1");
  return j;
}

json Communication::reboot()
{
  json j;
  j["command"] = "REBOOT";
  j["state"] = "success";
 // system("shutdown -r");
  return j;
}

json Communication::parse()
{
  json j_complete = json::parse(jsonIn);
  string command = j_complete.at("command");
  unsigned int commandJson = string2int(command.c_str());
  cout << "Comando: " << command << endl;
  json j;
  switch (commandJson)
  {
  case string2int("GETFIRES"):
    // cout << "GETFIRES: " << commandJson << endl;
    j = get_fires();
    break;
  case string2int("CREATEMISSION"):
    j = create_mission(j_complete.at("data"));
    break;
  case string2int("CREATEFIRE"):
    j = create_fire(j_complete.at("data"));
    break;
  case string2int("EXPORTUSB"):
    j = export_usb(j_complete.at("data"));
    break;
  case string2int("GETIMAGES"):
    j = json_images(j_complete.at("data"));
    break;
  case string2int("IMPORT"):
    j = import(j_complete.at("data"));
    break;
  case string2int("TESTCONN"):
    j = test_conn();
    break;
  case string2int("DELFIRE"):
    j = del_fire(j_complete.at("fire"), j_complete.at("mission"));
    break;
  case string2int("GETINFO"):
    j = info();
    break;
  case string2int("SHUTDOWN"):
    j = shut_down();
    break;
  case string2int("REBOOT"):
    j = reboot();
    break;
    /*case string2int("DELMISSION"):
       j = del_mission(j_complete.at("name"));
       break;*/
  default:
    cout << "default!";
  }
  return j;
}
