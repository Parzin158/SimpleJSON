#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

/* 通过命名空间与其他库进行隔离 */
namespace my{ //一级
namespace json{ //二级
class json
{
public:
    /* 枚举类型Type来包含json中的所有数据类型 */
    enum Type{  
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    json();
    json(bool value);
    json(int value);
    json(double value);
    json(const char * value);
    json(const string & value);
    json(Type type);
    json(const json & other);

    /* 基本类型运算符重载 ，将json对象转换成对应类型 */
    /* 例如:   bool b = v2;    int i = v3;     const string & s = v4; */
    operator bool();
    operator int();
    operator double();
    operator string();

    /* 重载[]运算符
    若操作对象为数组，则使数组通过索引值存放不同类型元素，并通过数组下标获取到json值，动态转换其类型。
    若操作对象为object，传入参数为C++字符串 
    */
    json & operator [] (int index); //返回为引用类型
    json & operator [] (const char * key);
    json & operator [] (const string & key);

    /* 重载"=", "==", "!="运算符 */
    void operator = (const json & other);
    bool operator == (const json & other);
    bool operator != (const json & other);

    /* 通过append函数向数组末尾添加元素 */
    void append(const json & other);

    /* 以字符串形式输出json的内容 */
    string str() const;

    /* 利用copy函数，拷贝重复代码 */
    void copy(const json & other);

    /* 利用clear函数，释放原来内存 */
    void clear();

    /* 使用迭代器，遍历数组 */
    typedef std::vector<json>::iterator iterator;
    iterator begin(){
        return (m_value.m_array)->begin();
    }
    iterator end(){
        return (m_value.m_array)->end();
    }

    /* 类型判断函数 */
    bool isNull() const { return m_type == json_null; }
    bool isBool() const { return m_type == json_bool; }
    bool isInt() const { return m_type == json_int; }
    bool isDouble() const { return m_type == json_double; }
    bool isString() const { return m_type == json_string; }
    bool isArray() const { return m_type == json_array; }
    bool isObject() const { return m_type == json_object; }

    /* 显性类型转换函数，const使函数体内json对象成员变量无法被修改 */
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    string asString() const;

    /* 判断数组中是否有索引，对象中是否有key */
    bool has(int index);
    bool has(const char * key);
    bool has(const string & key);

    /* 删除索引位置的数组元素，对象的某个key */
    void remove(int index);
    void remove(const char * key);
    void remove(const string & key);

    /* 解析函数 */
    void parse(const string & str);

private:
    /* 通过占用内存较小的联合体来定义json的值 */
    union Value{  
        bool m_bool;
        int m_int;
        double m_double;
        std::string * m_string; //string对象无法放入联合体中，必须用指针来存储
        std::vector<json> * m_array; //数组中存放类型为json
        std::map<string, json> * m_object; //键值对来存储json对象，键为string，值为json
    };

    Type m_type; // 记录数据类型
    Value m_value; // 记录数据值
};
}
}
