# JSON解析器项目

## 项目介绍

本项目是一个基于C++面向对象设计的高性能、跨平台、且不依赖第三方库的JSON数据解析器。

JSON (JavaScript Object Notation) 是一种轻量级的数据交换格式，可以跨语言、跨平台、适合在网络中传输，主要用于在服务器和Web应用程序之间传输数据。

## JSON简介

### JSON表示数据的两种方式

- **Object（对象）**：键/值对（名称/值）的集合，使用花括号`{ }`定义。在每个键/值对中，以键开头，表示数据的名称，必需以字符串的形式定义（在双引号中定义），键后跟一个冒号`:`和值。多个键/值对之间使用逗号`,`分隔。

- **Array（数组）**：值的有序集合，使用方括号`[ ]`定义，数组中每个值之间使用逗号`,`进行分隔。值(value)有字符串、数值、布尔值、null、对象和数组类型。

  

### JSON 中的值的类型：

**数字**：JSON 不区分整型和浮点型，只支持使用 IEEE-754 双精度浮点格式来定义数字。此外，JSON 中不能使用八进制和十六进制表示数字，但可以使用 e 或 E 来表示 10 的指数。

**布尔值**：JSON 中的布尔值与 JavaScript、PHP、Java 等编程语言中相似，有两个值，分别为 true（真）和 false（假）。

**空**：null（空）是 JSON 中的一个特殊值，表示没有任何值，当 JSON 中的某些键没有具体值时，就可以将其设置为 null。

**对象**：JSON 对象由花括号`{ }`以及其中的若干键/值对组成，一个对象中可以包含零个或多个键/值对，每个键/值对之间需要使用逗号`,`分隔。

**数组**：JSON 数组由方括号`[ ]`和其中的若干值组成，值可以是 JSON 中支持的任意类型，每个值之间使用逗号`,`进行分隔，且最后一个值末尾不需要添加逗号。数组中的值可以是 JSON 中的任何类型。

**字符串**：JSON 中的字符串需要使用双引号定义（注意：不能使用单引号），字符串中可以包含零个或多个 Unicode 字符。字符串中也可包含一些转义字符，例如：`\\`反斜线本身；`\/`正斜线；`\"`双引号；`\b`退格；`\f`换页；`\n`换行；`\r`回车；`\t`水平制表符；`\u`四位的十六进制数字。



## 功能模块

### json类

基本类型运算符(bool, int, double, string)重载，将json对象转换成对应类型。

#### "[]" 运算符重载

```c++
/* 重载”[]“运算符的实现 */
json & json::operator [] (int index){
    // 非数组类型则转换成数组，并动态创建数组
    if (m_type != json_array){
        m_type = json_array; 
        m_value.m_array = new vector<json>(); 
    }
    if (index < 0){
        throw new logic_error("array[] index < 0");
    }
    int size = (m_value.m_array)->size(); // 获取vector数组大小
    // 根据index值对数组进行扩容，对新元素添加空json进行扩容
    if (index >= size){
        for (int i = size; i <= index; i++){
            (m_value.m_array)->push_back(json());
        }
    }
    return (m_value.m_array)->at(index); //返回索引值处的数组元素
}
```

#### "=", "==", "!=" 运算符重载

```c++
/* 重载"=", "==", "!="运算符 */
void json::operator = (const json & other){
    clear(); //先清空现有内容，再进行赋值，避免内存泄漏
    copy(other); //拷贝重复代码，将other值传入当前json对象中
}
bool json::operator == (const json & other){
    if(m_type != other.m_type){
        return false;
    }
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
```

#### json类成员函数str()输出json内容

```c++
string json::str() const{
    stringstream ss; // 使用stringstream类实现对多个字符串的拼接
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
        ss << '['; // 从头到尾拼接数组每个元素
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
                ss << ',' ; // 对中间元素用逗号分隔
            }
            ss << '\"' << it->first << '\"' << ':' << it->second.str(); 
            // 输出对象的key和value，其中value为json
        }
        ss << '}';
    }
        break;
    default:
        break;
    }
    return ss.str(); // 递归调用str()输出
}
```

#### 数据类型判断函数

判断json值是否是所需类型，定义为常成员函数，使其无法改变成员变量的值。

- 如：boll isNull() const { return m_type == json_null; }

#### 显性类型转换函数

将json值转换为所需类型。

- 如：bool asBool() const;

#### 解析函数

```c++
// 使用parser类进行解析
void json::parse(const string & str){
    parser p;
    p.load(str); 
    *this = p.parse();
}
```



### parse类

#### 解析函数parser()

```c++
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
```

#### 解析不同类型值函数

- parse_null();
- parse_bool();
- parse_number();
- parse_string();
- parse_array();
- parse_object();



## 如何手动获取网页JSON数据

1. 在浏览器中打开开发者工具。
2. 选择“Network(网络)"选项卡，然后刷新网页，获取网页请求的各种资源，包括JSON。
3. 选择想要查看JSON的请求。一般来说，JSON请求通常是通过AJAX进行的。在Chrome开发者工具中，可选择“XHR”选项卡查看AJAX请求。在这个选项卡中，可查看所有AJAX请求。点击其中一个请求，既可在“Headers”选项卡中看到请求的详细信息及“Response”选项卡中看到响应的内容，包括JSON数据。
4. 使用pre标签将响应中的JSON数据格式化输出。在Chrome开发者工具中，复制JSON响应，在控制台中粘贴，使用JSON.stringify()方法将其格式化成易于阅读的形式。

