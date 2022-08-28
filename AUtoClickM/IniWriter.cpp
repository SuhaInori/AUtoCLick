#include "IniWriter.h"

IniFile::IniWriter::IniWriter(std::string path):FileMain(path)
{

}

IniFile::IniWriter::IniWriter() 
{
	std::cout << "  iniw ����" << std::endl;
}

IniFile::IniWriter::~IniWriter()
{
}

void IniFile::IniWriter::SetValue(std::string _root, std::string _key ,std::string _value)
{
	std::map<std::string, std::map<std::string, std::string>>::iterator itr = root.find(_root);	//����
	if (root.end() != itr)
	{
		itr->second.insert(std::pair<std::string, std::string>(_key, _value));
		//itr->second.sub_node[key] = value;
	}	//���ڵ��Ѿ������ˣ�����ֵ
	else
	{
		std::map<std::string, std::string> tp;
		tp.insert(std::pair<std::string, std::string >(_key,_value));
		root.insert(std::pair<std::string, std::map<std::string, std::string>>(_root, tp));
	}	//���ڵ㲻���ڣ����ֵ
}


void IniFile::IniWriter::WriteIni()
{
	FileWriter.open(path.c_str());
	if (!FileWriter.is_open()) {
#ifdef INIDEBUG
		std::cout << "д������������ļ�ʧ�ܣ�������\n"<<"Ŀ¼Ϊ��"<< path << std::endl;
#endif	//INIDEBUG
		throw "�ļ���ʧ�ܣ�";
	}
		
		
	for (std::map<std::string, std::map<std::string, std::string>>::iterator itr = root.begin(); itr != root.end(); ++itr)
	{
		FileWriter << "[" << itr->first << "]" << std::endl;
		for (std::map<std::string, std::string>::iterator iter = itr->second.begin(); iter != itr->second.end(); ++iter)
		{
#ifdef INIDEBUG
			std::cout << "д�룺" << iter->first << " = " << iter->second << std::endl;
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

