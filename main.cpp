#include "Communication.hpp"
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char** argv)
{
    Utils utils;
    Communication comm{utils};
    comm.init();
    thread t1(comm.capture,"firesense");
    comm.start();
}
