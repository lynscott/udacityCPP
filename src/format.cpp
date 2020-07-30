#include <string>
#include <vector>
#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

    int hours = seconds / 3600;
    int minutes = seconds % 3600 / 60;
    int calcSeconds = seconds % 3600 % 60;
    
    std::vector<int> output{hours, minutes, calcSeconds};
    
    std::string str;
    for (std::size_t i = 0; i < output.size(); i++){
        
        string strTemp = std::to_string(output[i]);

        if(strTemp.length() < 2) {
            strTemp = "0"+ strTemp;
        }
        str += strTemp;
        if(i != (output.size()-1)) {
            str += ":";
        }
        
    }
    return str;
}