#include "Ini_Read_Write.h"


//#define INIDEBUG

CMyINI::CMyINI(std::string path)
{
	this->path = path;
	startEmpty = false;
	ReadINI();
}


CMyINI::~CMyINI()
{
	WriteINI();
}

//************************************************************************
// ��������: 	TrimString
// ����˵��:		ȥ���ո�
// ��������: 	string & str	������ַ���
// �� �� ֵ: 	std::string &	����ַ���
//************************************************************************
std::string& TrimString(std::string& str)
{
	std::string::size_type pos = 0;
	while (str.npos != (pos = str.find(" ")))
		str = str.replace(pos, pos + 1, "");
	return str;
}

//************************************************************************
// ��������: 	ReadINI
// ����˵��:		��ȡINI�ļ��������䱣�浽map�ṹ��
// ��������: 	string path	INI�ļ���·��
// �� �� ֵ: 	int
//************************************************************************

/// <summary>
/// ReadINI :��ȡINI�ļ��������䱣�浽map�ṹ��
/// </summary>
/// <param name="path">�ļ�·��</param>
/// <returns>string path	INI�ļ���·��</returns>
int CMyINI::ReadINI()
{
	back:std::ifstream in_conf_file(path.c_str());
	if (!in_conf_file)
	{
		std::ofstream ofs(path.c_str());
		startEmpty = true;
		ofs.close();
#ifdef INIDEBUG
		std::cout << "�����ļ� " << std::endl;
#endif	//INIDEBUG
		goto back;
	}
	std::string str_line = "";
	std::string str_root = "";
	std::vector<ININode> vec_ini;
	while (getline(in_conf_file, str_line))
	{
		std::string::size_type left_pos = 0;
		std::string::size_type right_pos = 0;
		std::string::size_type equal_div_pos = 0;
		std::string str_key = "";
		std::string str_value = "";

		if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
		{
			//cout << str_line.substr(left_pos+1, right_pos-1) << endl;
			str_root = str_line.substr(left_pos + 1, right_pos - 1);
		}

		if (str_line.npos != (equal_div_pos = str_line.find("=")))
		{
			str_key = str_line.substr(0, equal_div_pos);
			str_value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
			str_key = TrimString(str_key);
			if (str_key != "path")
				str_value = TrimString(str_value);
			else
			{
				qDebug() << "�ҵ�path��" << str_value.c_str();
				qDebug() << "root :" << str_root.c_str();
				str_root = "main";
			}
				
			//cout << str_key << "=" << str_value << endl;
		}

		if ((!str_root.empty()) && (!str_key.empty()) && (!str_value.empty()))
		{
			ININode ini_node(str_root, str_key, str_value);
			vec_ini.push_back(ini_node);
			//cout << vec_ini.size() << endl;
		}
	}
	in_conf_file.close();
	in_conf_file.clear();

	//vector convert to map
	std::map<std::string, std::string> map_tmp;
	for (std::vector<ININode>::iterator itr = vec_ini.begin(); itr != vec_ini.end(); ++itr)
	{
		map_tmp.insert(std::pair<std::string, std::string>(itr->root, ""));
	}	//��ȡ�����ڵ�
	for (std::map<std::string, std::string>::iterator itr = map_tmp.begin(); itr != map_tmp.end(); ++itr)
	{
#ifdef INIDEBUG
		std::cout << "���ڵ㣺 " << itr->first << std::endl;
#endif	//INIDEBUG
		SubNode sn;
		for (std::vector<ININode>::iterator sub_itr = vec_ini.begin(); sub_itr != vec_ini.end(); ++sub_itr)
		{
			if (sub_itr->root == itr->first)
			{
#ifdef INIDEBUG
				std::cout << "��ֵ�ԣ� " << sub_itr->key << "=" << sub_itr->value << std::endl;
#endif	//INIDEBUG
				sn.InsertElement(sub_itr->key, sub_itr->value);
			}
		}
		map_ini.insert(std::pair<std::string, SubNode>(itr->first, sn));
	}
	return 1;
}

//************************************************************************
// ��������: 	GetValue
// ����˵��:		���ݸ����ĸ����ͼ�ֵ�����������ֵ
// ��������: 	string root		������ĸ����
// ��������: 	string key		������ļ�
// �� �� ֵ: 	std::string		�������ֵ
//************************************************************************

/// <summary>
/// GetValue:���ݸ����ĸ����ͼ�ֵ�����������ֵ
/// </summary>
/// <param name="root">������ĸ����</param>
/// <param name="key">������ļ�</param>
/// <returns>std::string:�������ֵ</returns>
std::string CMyINI::GetValue(std::string root, std::string key)
{
	std::map<std::string, SubNode>::iterator itr = map_ini.find(root);
	if (itr == map_ini.end())
		return "null root";
	std::map<std::string, std::string>::iterator sub_itr = itr->second.sub_node.find(key);
	if (!(sub_itr->second).empty())
		return sub_itr->second;
	return "null key";
}

//************************************************************************
// ��������: 	WriteINI
// ����˵��:		����XML����Ϣ���ļ���
// ��������: 	string path	INI�ļ��ı���·��
// �� �� ֵ: 	int
//************************************************************************

/// <summary>
/// WriteINI:����XML����Ϣ���ļ���
/// </summary>
/// <param name="path"></param>
/// <returns>int:INI�ļ��ı���·��</returns>
int CMyINI::WriteINI()
{
	std::ofstream out_conf_file(path.c_str());
	if (!out_conf_file)
		return -1;
	//cout << map_ini.size() << endl;
	for (std::map<std::string, SubNode>::iterator itr = map_ini.begin(); itr != map_ini.end(); ++itr)
	{
		//cout << itr->first << endl;
		out_conf_file << "[" << itr->first << "]" << std::endl;
		for (std::map<std::string, std::string>::iterator sub_itr = itr->second.sub_node.begin(); sub_itr != itr->second.sub_node.end(); ++sub_itr)
		{
			//cout << sub_itr->first << "=" << sub_itr->second << endl;
			out_conf_file << sub_itr->first << "=" << sub_itr->second << std::endl;
		}
	}

	out_conf_file.close();
	out_conf_file.clear();

	return 1;
}


//************************************************************************
// ��������: 	SetValue
// ����˵��:		�����������ֵ
// ��������: 	string root		������ĸ��ڵ�
// ��������: 	string key		������ļ�
// ��������: 	string value	�������ֵ
// �� �� ֵ: 	std::vector<ININode>::size_type	
//************************************************************************

/// <summary>
/// SetValue:�����������ֵ
/// </summary>
/// <param name="root">������ĸ��ڵ�</param>
/// <param name="key">������ļ�</param>
/// <param name="value">�������ֵ</param>
/// <returns>std::vector<ININode>::size_type</returns>
std::vector<ININode>::size_type CMyINI::SetValue(std::string root, std::string key, std::string value)
{
	std::map<std::string, SubNode>::iterator itr = map_ini.find(root);	//����
	if (map_ini.end() != itr)
	{
		//itr->second.sub_node.insert(pair<string, string>(key, value));
		itr->second.sub_node[key] = value;
	}	//���ڵ��Ѿ������ˣ�����ֵ
	else
	{
		SubNode sn;
		sn.InsertElement(key, value);
		map_ini.insert(std::pair<std::string, SubNode>(root, sn));
	}	//���ڵ㲻���ڣ����ֵ

	return map_ini.size();
}

//************************************************************************
// ��������: 	Travel
// ����˵��:		������ӡINI�ļ�
// �� �� ֵ: 	void
//************************************************************************

/// <summary>
/// ������ӡINI�ļ�
/// </summary>
void CMyINI::Travel()
{
	for (std::map<std::string, SubNode>::iterator itr = this->map_ini.begin(); itr != this->map_ini.end(); ++itr)
	{
		//root
		std::cout << "[" << itr->first << "]" << std::endl;
		for (std::map<std::string, std::string>::iterator itr1 = itr->second.sub_node.begin(); itr1 != itr->second.sub_node.end();
			++itr1)
		{
			std::cout << " " << itr1->first << " = " << itr1->second << std::endl;
		}
	}

}
/// <summary>
/// �ָ��ַ���
/// </summary>
/// <param name="s">�ָ���ַ���</param>
/// <param name="delim">�ָ��־</param>
/// <returns>������������</returns>
std::vector<std::string> CMyINI::vStringSplit(const std::string& s, const std::string& delim)
{
	std::vector<std::string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}
/// <summary>
/// �ָ��ַ���
/// </summary>
/// <param name="s">�ָ���ַ���</param>
/// <param name="delim">�ָ��־</param>
/// <returns>����click_Node</returns>
click_Node CMyINI::StringSplit(const std::string& str, const std::string& delim)
{

	click_Node tec = {0,0};
	size_t pos = 0;
	size_t len = str.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return tec;
	int find_pos = str.find(delim, pos);

	if (find_pos < 0)
	{
		tec.x= std::stoi(str.substr(pos, len - pos));
		return tec;
	}
	tec.x = std::stoi(str.substr(pos, find_pos - pos));
	pos = find_pos + delim_len;
	find_pos = str.find(delim, pos);

	if (find_pos < 0)
	{
		tec.y = std::stoi(str.substr(pos, len - pos));
		return tec;
	}
	tec.y= std::stoi(str.substr(pos, find_pos - pos));
	pos = find_pos + delim_len;
	find_pos = str.find(delim, pos);

	if (find_pos < 0)
	{
		tec.time = std::stoi(str.substr(pos, len - pos));
		return tec;
	}
	tec.time = std::stoi(str.substr(pos, find_pos - pos));
	return tec;

}

void CMyINI::deleteForRoot(std :: string root)
{
	map_ini.erase(root);
}
