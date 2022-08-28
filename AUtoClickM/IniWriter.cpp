#include "IniWriter.h"

IniFile::IniWriter::IniWriter(std::string path):FileMain(path)
{

}

IniFile::IniWriter::IniWriter() 
{
	std::cout << "  iniw 构造" << std::endl;
}

IniFile::IniWriter::~IniWriter()
{
}

void IniFile::IniWriter::SetValue(std::string _root, std::string _key ,std::string _value)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator itr = root.find(_root);	//查找
	if (root.end() != itr)
	{
		itr->second.insert(std::pair<std::string, std::string>(_key, _value));
		//itr->second.sub_node[key] = value;
	}	//根节点已经存在了，更新值
	else
	{
		std::map<std::string, std::string> tp;
		tp.insert(std::pair<std::string, std::string >(_key,_value));
		root.insert(std::pair<std::string, std::map<std::string, std::string>>(_root, tp));
	}	//根节点不存在，添加值
}


void IniFile::IniWriter::WriteIni()
{
	FileWriter.open(path.c_str());
	if (!FileWriter.is_open()) {
#ifdef INIDEBUG
		std::cout << "写入操作：：打开文件失败！！！！\n"<<"目录为："<< path << std::endl;
#endif	//INIDEBUG
		throw "文件打开失败！";
	}
		
		
	for (std::map<std::string, std::map<std::string, std::string>>::iterator itr = root.begin(); itr != root.end(); ++itr)
	{
		FileWriter << "[" << itr->first << "]" << std::endl;
		for (std::map<std::string, std::string>::iterator iter = itr->second.begin(); iter != itr->second.end(); ++iter)
		{
#ifdef INIDEBUG
			std::cout << "写入：" << iter->first << " = " << iter->second << std::endl;
#endif	//INIDEBUG
			FileWriter << iter->first << " = " << iter->second << std::endl;
		}
		FileWriter <<std::endl;
	}


	FileWriter.close();
	FileWriter.clear();
}


void IniFile::IniWriter::ReLoadPath(std::string _path)
{
	this->path = _path;
}

