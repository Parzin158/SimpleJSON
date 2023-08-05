#pragma once

#include <string>
#include "json.h"
using namespace std;

namespace my{
namespace json{

class parser{
public:
    parser();

    /* 加载解析的字符串 */
    void load(const string & str); 
    
    json parse();

private:

    /* 跳过空白字符 */
    void skip_white_space();

    /* 获取下一个字符 */
    char get_next_token();
    
    /* 解析值函数 */
    json parse_null();
    json parse_bool();
    json parse_number();
    string parse_string();
    json parse_array();
    json parse_object();

private:
    string m_str; //存放数据
    int m_idx; //存放索引

};

}
}
