#include"./iniReader.h"
#include "IniReader.h"

IniFile::IniReader::IniReader(std::string path) :FileMain(path) {
#ifdef INIDEBUG
	std::cout << "����·����" << path  << std::endl;
#endif	//INIDEBUG
	IsEmpty = false;
	ReadIniFile();
}

IniFile::IniReader::IniReader() 
{
	std::cout << "  ini r ����" << std::endl;
	IsEmpty = false;
}

IniFile::IniReader::~IniReader()
{
}

void IniFile::IniReader::ReLoadIni(std::string path)
{
	this->path = path;
	if (root.size()>0)
		root.clear();
	ReadIniFile();
}

void IniFile::IniReader::ReadIniFile()
{
	FileReader.open(path.c_str());

	if (!FileReader.is_open()) {
		std::ofstream ofs(path.c_str());
		IsEmpty = true;
		ofs.close();
#ifdef INIDEBUG
		std::cout << "�ļ�"<< path <<"��ʧ�� " << std::endl;
#endif	//INIDEBUG
		//std::cout << "�ļ�" << path << "��ʧ�� " << std::endl;
		//throw(path+"�ļ���ʧ��");
		return;
	}
	std::string str_line;
	std::string str_root;
#ifdef INIDEBUG
	std::cout << "��ȡ�ɹ���\n" << FileReader.is_open()<< std::endl;
#endif	//INIDEBUG
	
	while (std::getline(FileReader, str_line))
	{
		std::map<std::string, std::string> rongqi;
		std::string::size_type left_pos = 0;
		std::string::size_type right_pos = 0;
		std::string::size_type equal_div_pos = 0;
		std::string key ;
		std::string value ;
		str_line=trim(str_line);
		//���ע��
		if (str_line.npos != (equal_div_pos = str_line.find("//"))) {
#ifdef INIDEBUG
			std::cout << "ע����ʼλ��:" << equal_div_pos+1 << std::endl;
#endif	//INIDEBUG
			str_line.erase(equal_div_pos, str_line.length());
		}
			
		//�ҵ���
		if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
		{
			str_root = str_line.substr(left_pos + 1, right_pos - 1);
			
#ifdef INIDEBUG
			std::cout << "��ȡ����:"<< str_root << std::endl;
#endif	//INIDEBUG
		}
		//�ҵ���ֵ��
		if (str_line.npos != (equal_div_pos = str_line.find("="))|| str_line.npos != (equal_div_pos = str_line.find(":")))
		{
			key = str_line.substr(0, equal_div_pos);
			value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
			key = TrimString(key);
			value = TrimString(value);
			Node node(str_root, key, value);
			ini_Node.push_back(node);
#ifdef INIDEBUG
			std::cout << "��Ӽ�ֵ:" << key<<"="<< value << std::endl;
#endif	//INIDEBUG
		}

	}


	std::map<std::string, std::string> map_tmp;
	for (std::vector<Node>::iterator itr = ini_Node.begin(); itr != ini_Node.end(); ++itr)
	{
		map_tmp.insert(std::pair<std::string, std::string>(itr->root, ""));

	}	//��ȡ�����ڵ�

	for (std::map<std::string, std::string>::iterator itr = map_tmp.begin(); itr != map_tmp.end(); ++itr)
	{
#ifdef INIDEBUG
		std::cout << "���ڵ㣺 " << itr->first << std::endl;
#endif	//INIDEBUG
		std::map<std::string, std::string> forge;
		for (std::vector<Node>::iterator sub_itr = ini_Node.begin(); sub_itr != ini_Node.end(); ++sub_itr)
		{
			if (sub_itr->root == itr->first)
			{
#ifdef INIDEBUG
				std::cout << "��ֵ�ԣ� " << sub_itr->key << " = " << sub_itr->value << std::endl;
#endif	//INIDEBUG
				forge.insert(std::pair<std::string, std::string>(sub_itr->key, sub_itr->value));
			}
		}
		root.insert(std::pair<std::string, std::map<std::string, std::string>>(itr->first, forge));
	}
	FileReader.close();
	FileReader.clear();
#ifdef INIDEBUG

	std::cout << "��ȡ����������ɣ�������\n" << "Ŀ¼Ϊ��" << path << std::endl;
#endif	//INIDEBUG

	if (ini_Node.size()<=0)
	{
		IsEmpty = true;
	}
}

std::string IniFile::IniReader::GetValue(std::string _root, std::string key)
{
	std::map<std::string, std::string>::iterator iter;
	iter =root[_root].find(key);
#ifdef INIDEBUG
	std::cout << "����ֵ��" << iter->second << std::endl;
#endif	//INIDEBUG
	std::string ref = iter->second;
	if (!(iter->second).empty())
		return iter->second;
	return "0";
}


std::string IniFile::IniReader::toString()
{
	std::string temp;
	for (std::map<std::string, std::map<std::string, std::string>>::iterator itr = root.begin(); itr != root.end(); ++itr)
	{
		temp += "[" + itr->first + "]\n";
		for (std::map<std::string,std::string>::iterator iter  = itr->second.begin(); iter != itr->second.end(); ++iter)
		{
#ifdef INIDEBUG
			std::cout << "ToString��" << iter->second << std::endl;
#endif	//INIDEBUG
			temp += iter->first+" = " + iter->second + "\n";
		}
		temp += "\n\n";
	}
	return temp;
}

int IniFile::IniReader::GetInt32(std::string root, std::string key)
{
	try
	{
		return atoi(GetValue(root, key).c_str());
	}
	catch (const std::exception& )
	{
		std::cout << "ֵ��Ϊ���Σ�" << std::endl;
		throw "ֵ��Ϊ���Σ�";
	}
}

double IniFile::IniReader::GetDouble(std::string root, std::string key)
{
	try
	{
		return atof(GetValue(root, key).c_str());
	}
	catch (const std::exception&)
	{
		std::cout << "ֵ��Ϊ���㣡" << std::endl;
		throw "ֵ��Ϊ���㣡";
	}
}

std::string IniFile::IniReader::GetString(std::string root, std::string key)
{
	return GetValue(root, key);
}

bool IniFile::IniReader::GetBool(std::string root, std::string key)
{
	std::string temp = GetValue(root, key);
	int c = atof(temp.c_str());
	if (c ==0)
	{
		return false;
	}
	else if (c == 1)
	{
		return true;
	}
	return false;
}

std::vector<double> IniFile::IniReader::GetNumArray(std::string, std::string)
{
	std::vector<double> ref;
	std::vector<std::string> temp;
	for (std::vector<std::string>::iterator itr = temp.begin(); itr != temp.end(); ++itr)
	{
		ref.push_back(atof(itr->c_str()));
	}
	return ref;
}

std::vector<std::string> IniFile::IniReader::GetArray(std::string root, std::string key)
{
	try
	{
		return split(GetValue(root, key), ",");
	}
	catch (const std::exception&)
	{
		std::cout << "ֵ��Ϊ���飡" << std::endl;
		throw "ֵ��Ϊ���飡";
	}
	
}


std::string& IniFile::IniReader::trim(std::string& s)
{
	if (s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

std::string& IniFile::IniReader::TrimString(std::string& str)
{
	std::string::size_type pos = 0;
	while (str.npos != (pos = str.find(" ")))
		str = str.replace(pos, pos + 1, "");
	return str;
}

std::vector<std::string> IniFile::IniReader::split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + (int)pattern.size() - 1;
		}
	}
	return result;
}
