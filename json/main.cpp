#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
using namespace std;

#include "json.h"
using namespace my::json;

int main(){

    ifstream fin("./main.json");
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
    
    const string & type = my_json["course"][0]["type"][0];
    std::cout << type << std::endl;

    my_json.clear();
    fin.close();
    fin.clear();

    system("pause");
    return 0;
}