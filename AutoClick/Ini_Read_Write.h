
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

//INI�ļ����洢�ṹ
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

//��ֵ�Խṹ��
class SubNode
{
public:
	void InsertElement(std::string key, std::string value)
	{
		sub_node.insert(std::pair<std::string, std::string>(key, value));
	}
	std::map<std::string, std::string> sub_node;
};

//INI�ļ�������
class CMyINI
{
public:
	CMyINI(std::string path);
	~CMyINI();

public:
	
	std::string GetValue(std::string root, std::string key);									//�ɸ����ͼ���ȡֵ
	std::vector<ININode>::size_type GetSize() { return map_ini.size(); }				//��ȡINI�ļ��Ľ����
	std::vector<ININode>::size_type SetValue(std::string root, std::string key, std::string value);	//���ø����ͼ���ȡֵ
	
	void Clear() { map_ini.clear(); }	//���
	void Travel();						//������ӡINI�ļ�
	bool startEmpty;
	static std::vector<std::string> vStringSplit(const  std::string& s, const std::string& delim = ",");
	static click_Node StringSplit(const std::string &str, const std::string& delim = ",");
	void deleteForRoot(std::string);
private:
	std::string path;
	int ReadINI();						//��ȡINI�ļ�
	int WriteINI();						//д��INI�ļ�
	std::map<std::string, SubNode> map_ini;		//INI�ļ����ݵĴ洢����
};


