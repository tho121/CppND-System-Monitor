#include <string>

#include "format.h"

using namespace std;
using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 

    int minutes = seconds / 60;
    int hours = minutes / 60;

    seconds = seconds % 60;
    minutes = minutes % 60;
    //hours = hours % 24;

    string hh = hours < 10 ? "0" + to_string(hours) : to_string(hours);
    string mm = minutes < 10 ? "0" + to_string(minutes) : to_string(minutes);
    string ss = seconds < 10 ? "0" + to_string(seconds) : to_string(seconds);

    return hh + ":" + mm + ":" + ss; 
}