#include"./iniReader.h"
#include "IniReader.h"

IniFile::IniReader::IniReader(std::string path) :FileMain(path) {
#ifdef INIDEBUG
	std::cout << "设置路径：" << path  << std::endl;
#endif	//INIDEBUG
	IsEmpty = false;
	ReadIniFile();
}

IniFile::IniReader::IniReader() 
{
	std::cout << "  ini r 构造" << std::endl;
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
		std::cout << "文件"<< path <<"丢失！ " << std::endl;
#endif	//INIDEBUG
		//std::cout << "文件" << path << "丢失！ " << std::endl;
		//throw(path+"文件丢失！");
		return;
	}
	std::string str_line;
	std::string str_root;
#ifdef INIDEBUG
	std::cout << "读取成功！\n" << FileReader.is_open()<< std::endl;
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
		//清除注释
		if (str_line.npos != (equal_div_pos = str_line.find("//"))) {
#ifdef INIDEBUG
			std::cout << "注释起始位置:" << equal_div_pos+1 << std::endl;
#endif	//INIDEBUG
			str_line.erase(equal_div_pos, str_line.length());
		}
			
		//找到跟
		if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
		{
			str_root = str_line.substr(left_pos + 1, right_pos - 1);
			
#ifdef INIDEBUG
			std::cout << "读取到根:"<< str_root << std::endl;
#endif	//INIDEBUG
		}
		//找到键值对
		if (str_line.npos != (equal_div_pos = str_line.find("="))|| str_line.npos != (equal_div_pos = str_line.find(":")))
		{
			key = str_line.substr(0, equal_div_pos);
			value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
			key = TrimString(key);
			value = TrimString(value);
			Node node(str_root, key, value);
			ini_Node.push_back(node);
#ifdef INIDEBUG
			std::cout << "添加键值:" << key<<"="<< value << std::endl;
#endif	//INIDEBUG
		}

	}


	std::map<std::string, std::string> map_tmp;
	for (std::vector<Node>::iterator itr = ini_Node.begin(); itr != ini_Node.end(); ++itr)
	{
		map_tmp.insert(std::pair<std::string, std::string>(itr->root, ""));

	}	//提取出根节点

	for (std::map<std::string, std::string>::iterator itr = map_tmp.begin(); itr != map_tmp.end(); ++itr)
	{
#ifdef INIDEBUG
		std::cout << "根节点： " << itr->first << std::endl;
#endif	//INIDEBUG
		std::map<std::string, std::string> forge;
		for (std::vector<Node>::iterator sub_itr = ini_Node.begin(); sub_itr != ini_Node.end(); ++sub_itr)
		{
			if (sub_itr->root == itr->first)
			{
#ifdef INIDEBUG
				std::cout << "键值对： " << sub_itr->key << " = " << sub_itr->value << std::endl;
#endif	//INIDEBUG
				forge.insert(std::pair<std::string, std::string>(sub_itr->key, sub_itr->value));
			}
		}
		root.insert(std::pair<std::string, std::map<std::string, std::string>>(itr->first, forge));
	}
	FileReader.close();
	FileReader.clear();
#ifdef INIDEBUG

	std::cout << "读取操作：：完成！！！！\n" << "目录为：" << path << std::endl;
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
	std::cout << "返回值：" << iter->second << std::endl;
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
			std::cout << "ToString：" << iter->second << std::endl;
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
		std::cout << "值不为整形！" << std::endl;
		throw "值不为整形！";
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
		std::cout << "值不为浮点！" << std::endl;
		throw "值不为浮点！";
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
		std::cout << "值不为数组！" << std::endl;
		throw "值不为数组！";
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
	str += pattern;//扩展字符串以方便操作
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
