
#pragma once
#include <qdebug.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
typedef struct 
{
	int x;
	int y;
	int time;
}click_Node;

//INI文件结点存储结构
class ININode
{
public:
	ININode(std::string root, std::string key, std::string value)
	{
		this->root = root;
		this->key = key;
		this->value = value;
	}
	std::string root;
	std::string key;
	std::string value;
};

//键值对结构体
class SubNode
{
public:
	void InsertElement(std::string key, std::string value)
	{
		sub_node.insert(std::pair<std::string, std::string>(key, value));
	}
	std::map<std::string, std::string> sub_node;
};

//INI文件操作类
class CMyINI
{
public:
	CMyINI(std::string path);
	~CMyINI();

public:
	
	std::string GetValue(std::string root, std::string key);									//由根结点和键获取值
	std::vector<ININode>::size_type GetSize() { return map_ini.size(); }				//获取INI文件的结点数
	std::vector<ININode>::size_type SetValue(std::string root, std::string key, std::string value);	//设置根结点和键获取值
	
	void Clear() { map_ini.clear(); }	//清空
	void Travel();						//遍历打印INI文件
	bool startEmpty;
	static std::vector<std::string> vStringSplit(const  std::string& s, const std::string& delim = ",");
	static click_Node StringSplit(const std::string &str, const std::string& delim = ",");
	void deleteForRoot(std::string);
private:
	std::string path;
	int ReadINI();						//读取INI文件
	int WriteINI();						//写入INI文件
	std::map<std::string, SubNode> map_ini;		//INI文件内容的存储变量
};


