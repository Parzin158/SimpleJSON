#include "json.h"
#include "parser.h"
#include <sstream>
using namespace std;

using namespace my::json;

/* 根据json传入的参数value的类型，将value值赋给对应类型的m_value */
json::json() : m_type(json_null){ 
    //空值
}
json::json(bool value) : m_type(json_bool){
    m_value.m_bool = value;
}
json::json(int value) : m_type(json_int){
    m_value.m_int = value;
}
json::json(double value) : m_type(json_double){
    m_value.m_double = value;
}
json::json(const char * value) : m_type(json_string){
    m_value.m_string = new string(value);
}
json::json(const string & value) : m_type(json_string){
    m_value.m_string = new string(value);
}


/* 通过值的不同类型来构造 */
json::json(Type type) : m_type(type){ 
    switch (m_type){
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        m_value.m_string = new string("");
        break;
    case json_array:
        m_value.m_array = new std::vector<json>();
        break;
    case json_object:
        m_value.m_object = new std::map<string, json>();
        break;
    default:
        break;
    }
}

/* 通过其他的json来构造 */
json::json(const json & other){
    copy(other);//拷贝重复代码，将other值传入当前json对象中
}

/* 利用copy函数，拷贝重复代码，实现将other值传入当前json对象中 */
void json::copy(const json & other){
    m_type = other.m_type;
    switch (m_type){
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = other.m_value.m_bool;
        break;
    case json_int:
        m_value.m_int = other.m_value.m_int;
        break;
    case json_double:
        m_value.m_double = other.m_value.m_double;
        break;
    case json_string:
        m_value.m_string = other.m_value.m_string; //将指针复制，进行浅拷贝
        break;
    case json_array:
        m_value.m_array = other.m_value.m_array; //浅拷贝
        break;
    case json_object:
        m_value.m_object = other.m_value.m_object; //浅拷贝
        break;
    default:
        break;
    }
}
/* 进行浅拷贝时，需要释放原来other中的动态分配的指针，否则会发生内存泄漏 */


/* 基本类型的运算符重载的实现 */
json::operator bool(){
    if (m_type != json_bool){ //类型非bool则抛出异常
        throw new logic_error("Type error, not a bool value. ");
    }
    return m_value.m_bool;
}
json::operator int(){
    if (m_type != json_int){ //类型非int则抛出异常
        throw new logic_error("Type error, not a int value. ");
    }
    return m_value.m_int;
}
json::operator double(){
    if (m_type != json_double){ //类型非double则抛出异常
        throw new logic_error("Type error, not a double value. ");
    }
    return m_value.m_double;
}
json::operator string(){
    if (m_type != json_string){ //类型非string则抛出异常
        throw new logic_error("Type error, not a string value. ");
    }
    return *(m_value.m_string); //对指针解引用取其内容
}


/* 重载”[]“运算符的实现 */
json & json::operator [] (int index){
    if (m_type != json_array){
        m_type = json_array; //非数组转换成数组类型
        m_value.m_array = new vector<json>(); //动态创建数组
    }
    if (index < 0){ //index值错误，则抛出异常
        throw new logic_error("array[] index < 0");
    }
    int size = (m_value.m_array)->size(); //size函数获取vector数组大小
    if (index >= size){ //对数组进行扩容
        for (int i = size; i <= index; i++){
            (m_value.m_array)->push_back(json()); //对数组添加空json进行扩容
        }
    }
    return (m_value.m_array)->at(index); //返回索引值处的数组元素
}
    
/* 重载"[]"运算符，操作对象为object，传入参数为C++字符串 */
json & json::operator [] (const char * key){
    string name(key); //实例化string类的对象name，传入key进行构造
    return ((*this))[name];
}
json & json::operator [] (const string & key){
    if (m_type != json_object){ //若传入类型非other，先清空再转成other
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<string, json>(); //动态创建
    }
    return (*(m_value.m_object))[key]; //返回map键值对中的key
} 


/* append函数向数组末尾添加元素 */
void json::append(const json & other){
    if (m_type != json_array){ //传入类型非数组，先清理再转成数组
        clear();
        m_type = json_array; //转换成数组类型
        m_value.m_array = new vector<json>(); //动态创建数组
    }
    (m_value.m_array)->push_back(other); //调用stl模板库push_back函数添加元素
}


/* 重载"=", "==", "!="运算符 */
void json::operator = (const json & other){
    clear(); //先清空现有内容，再进行赋值，避免内存泄漏
    copy(other); //拷贝重复代码，将other值传入当前json对象中
}
bool json::operator == (const json & other){
    if(m_type != other.m_type){
        return false;
    } //先判断类型是否相等
    switch (m_type){
    case json_null:
        return true;
    case json_bool:
        return m_value.m_bool == other.m_value.m_bool;
    case json_int:
        return m_value.m_int == other.m_value.m_int;
    case json_double:
        return m_value.m_double == other.m_value.m_double;
    case json_string:
        return *(m_value.m_string) == *(other.m_value.m_string);
    case json_array:
        return m_value.m_array == other.m_value.m_array;
    case json_object:
        return m_value.m_object == other.m_value.m_object;
    default:
        break;
    }
    return false;
}

bool json::operator != (const json & other){
    return !((*this) == other); // 利用重载==的结果取反
}


/* 利用clear函数，释放原来内存 */
void json::clear(){
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:{
        delete m_value.m_string; //释放字符串指针
        break;
    }
    case json_array:{
        for(auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++){
            it->clear(); //删除数组内对象，递归调用clear函数
        }
        delete m_value.m_array;
        break;
    }
    case json_object:{
        for(auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++){
              (it->second).clear(); //删除map中存放的json，递归调用clear函数  
        }
        delete m_value.m_object;
        break;
    }
    default:
        break;
    }
    m_type = json_null; //设置为空类型
}


/* 输出数组类型json的内容 */
string json::str() const{
    stringstream ss; //使用stringstream类实现对多个字符串的拼接
    switch (m_type)
    {
    case  json_null:
        ss << "null";
        break;
    case json_bool:
        if(m_value.m_bool){
            ss << "true";
        }
        else ss << "false";
        break;
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        ss << '\"' << *(m_value.m_string) << '\"' ;
        break;
    case json_array:{
        ss << '['; //从头到尾拼接数组每个 元素
        for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++){
            if (it != (m_value.m_array)->begin()){
                ss << ',' ; //对中间元素用逗号分隔
            }
            ss << it->str(); //递归调用str()函数
        }
        ss << ']';
    }
        break;
    case json_object:{
        ss << '{'; //从头到尾拼接对象
        for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++){
            if (it != (m_value.m_object)->begin()){
                ss << ',' ; //对中间元素用逗号分隔
            }
            ss << '\"' << it->first << '\"' << ':' << it->second.str(); 
            //输出对象的key和value，其中value为json
        }
        ss << '}';
    }
        break;
    default:
        break;
    }
    return ss.str();
}


/* 显性类型转换函数，const使函数体内json对象无法被修改 */
bool json::asBool() const{
    if (m_type != json_bool){
        throw new std::logic_error("type error, not bool value");
    }
    return m_value.m_bool;
}
int json::asInt() const{
    if (m_type != json_int){
        throw new std::logic_error("type error, not int value");
    }
    return m_value.m_int;
}
double json::asDouble() const{
    if (m_type != json_double){
        throw new std::logic_error("type error, not double value");
    }
    return m_value.m_double;
}
string json::asString() const{
    if (m_type != json_string){
        throw new std::logic_error("type error,not string value");
    }
    return *(m_value.m_string);
}


/* 判断数组中是否有索引，对象中是否有key */
bool json::has(int index){
    if(m_type != json_array){
        return false;
    }
    int size = m_value.m_array->size();
    return (index >=0 && index > size); //返回index值是否合法
}
bool json::has(const char * key){
    string name(key); //此处使用string类的拷贝构造
    return has(name);
}
bool json::has(const string & key){
    if(m_type != json_object){
        return false;
    }
    return ((m_value.m_object)->find(key) != (m_value.m_object)->end()); //返回寻找key结果
}


/* 删除索引位置的数组元素，对象的某个key */
void json::remove(int index){
    if(m_type != json_array){
        return;
    } //类型错误，返回
    int size = (m_value.m_array)->size();
    if(index < 0 || index >= size){
        return;
    } //index值非法，返回
    (m_value.m_array)->at(index).clear(); //内存清零
    (m_value.m_array)->erase((m_value.m_array)->begin() + index); //释放内存
}
void json::remove(const char * key){
    string name(key);
    remove(name);
}
void json::remove(const string & key){
    auto it = (m_value.m_object)->find(key);
    if(it == (m_value.m_object)->end()){
        return;
    }
    (*(m_value.m_object))[key].clear(); //内存清零
    (m_value.m_object)->erase(key); //释放内存
}


/* 解析函数 */
void json::parse(const string & str){
    parser p;
    p.load(str); //使用parser类的load方法解析str
    *this = p.parse();
}