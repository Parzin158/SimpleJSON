#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

#include "json.h"
using namespace my::json;

int main(){
    ifstream fin("./Sample.json");
    char buffer[256];
    char *val = getcwd(buffer, sizeof(buffer));
    if (val) {
        std::cout << buffer << std::endl;
    }

    if(!fin.is_open()){
        std::cout << "Open file failed" << std::endl;
        return 0;
    }
    stringstream ss;
    ss << fin.rdbuf();
    const string & data = ss.str();

    json my_json;
    my_json.parse(data);

    std::cout << my_json.str() << std::endl;    
    
    // const string & type = my_json["clients"][1];
    // std::cout << type << std::endl;

    my_json.clear();
    fin.close();
    fin.clear();

    return 0;
}
