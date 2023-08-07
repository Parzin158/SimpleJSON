#include "parser.h"
using namespace my::json;

/* 默认构造函数，空值初始化 */
parser::parser() : m_str(""), m_idx(0){ 

}

/* 加载解析的字符串 */
void parser::load(const string & str){
    m_str = str;
    m_idx = 0;
}

/* 跳过空白字符 */
void parser::skip_white_space(){
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\r' || m_str[m_idx] == '\t'){
        m_idx++; //当前字符为 空格、换行、回车、制表符 ，跳过忽略
    }
}

/* 获取下一个字符 */
char parser::get_next_token(){
    skip_white_space();
    if (m_idx == m_str.size())
        throw std::logic_error("unexpected end of input");
    return m_str[m_idx++]; //拷贝构造，获取下一个字符的索引
}

json parser::parse(){
    char ch = get_next_token();
    switch (ch)
    {
    case 'n':
        m_idx--;
        return parse_null();
    case 't':
    case 'f':
        m_idx--;
        return parse_bool();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        m_idx--;
        return parse_number();
    case '"':
        return json(parse_string()); //通过返回的字符串构造json对象
    case '[':
        return parse_array();
    case '{':
        return parse_object();
    default:
        break;
    }
    throw std::logic_error("unexpacted char");
}

/* 解析空值 */
json parser::parse_null(){
    if (0 == m_str.compare(m_idx, 4, "null")){
        m_idx += 4; //索引跳过4个字节到null末尾
        return json();
    }
    throw std::logic_error("parse null error");
}

/* 解析布尔值 */
json parser::parse_bool(){
    if (0 == m_str.compare(m_idx, 4, "true")){
        m_idx += 4; //索引跳过4个字节到true末尾
        return true;
    }
    else if (0 == m_str.compare(m_idx, 5, "false")){
        m_idx += 5; //索引跳过5个字节到false末尾
        return false;
    }
    throw std::logic_error("parse bool error");
}

/* 解析数字 */
json parser::parse_number(){        
    int pos = m_idx; //记录原始位置
    if ('-' == m_str[m_idx]){
        m_idx++; //负号后移一位
    }
    //非数字抛出异常
    if (m_str[m_idx] < '0' || m_str[m_idx] > '9'){
        throw std::logic_error("parse number error"); 
    }

    while (1){
        m_idx++; //属于数字则后移        
        //浮点数
        if ('.' == m_str[m_idx]){
            m_idx++;
            //小数点后非数字抛出异常
            if (m_str[m_idx] < '0' || m_str[m_idx] > '9'){
                throw std::logic_error("parse number error"); 
            }
            //数字则继续
            while(m_str[m_idx] >= '0' && m_str[m_idx] <= '9'){
                m_idx++;
            }
            return (std::atof(m_str.c_str() + pos));
        }
        //整数
        if (m_str[m_idx] < '0' || m_str[m_idx] > '9'){
            return (std::atoi(m_str.c_str() + pos));
        }
    }
}

/* 解析字符串 */
string parser::parse_string(){
    string out;
    while (1){
        //检查下一个字符的值
        char ch = m_str[m_idx++]; 
        if ('"' == ch){
            break; 
        } //是双引号则结束

        //反斜杠开头的转义字符
        if ('\\' == ch){
            ch = m_str[m_idx++];
            switch (ch)
            {
            case '\n':
                out += '\n'; //字符串后追加换行符
                break;
            case '\r':
                out += '\r'; //追加回车
                break;;
            case '\t':
                out += '\t'; //追加制表符
                break;
            case '\b':
                out += '\b'; //追加退格符
                break;
            case '\f':
                out += '\f'; //追加换页符
                break;
            case '"':
                out += '\"'; //追加双引号
                break;
            case '\\':
                out += '\\'; //追加反斜杠
                break;
            case 'u':
                out += 'u'; //unicode字符(4字节)
                for (int i=0; i < 4; i++){
                    out += m_str[m_idx++];
                }
                break;

            default:
                break;
            }
        }
        else {
            out += ch;
        }
    }
    return out;
}

/* 解析数组 */
json parser::parse_array(){
    json arr(json::json_array); //声明一个数组类型
    char ch = get_next_token(); //获取下一个字符
    if (']' == ch){
        return arr;
    }
    m_idx--;
    while (1){ //循环解析数组元素
        arr.append(parse()); //递归解析当前指向的数组元素，并追加到arr数组末尾
        ch = get_next_token();
        if (']' == ch){
            break; //到数组末尾结束
        }
        if (',' != ch){
            throw std::logic_error("prase array error");
        }
        //m_idx++; //当前字符为','则跳过，继续解析
    }
    return arr;
}

/* 解析对象 */
json parser::parse_object(){
    json obj(json::json_object); //声明一个对象类型
    char ch = get_next_token();
    if ('}' == ch){
        return obj; //空对象直接返回
    }
    m_idx--;
    while (1)
    {
        ch = get_next_token();
        if ('"' != ch){
            throw std::logic_error("prase object error");
        }
        string key = parse_string(); //解析出key
        ch = get_next_token();
        if (':' != ch){
            throw std::logic_error("prase object error");
        }
        obj[key] = parse();
        ch = get_next_token(); //解析出value
        if ('}' == ch){
            break; //解析完成退出循环
        }
        if (',' != ch){
            throw std::logic_error("prase object error");
        }
        //进入下一轮解析
    }
    return obj;
}
