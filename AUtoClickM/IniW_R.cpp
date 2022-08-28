#include "IniW_R.h"

IniFile::IniW_R::IniW_R(std::string _path)
{
	std::cout << "  ini wr ����" << std::endl;
	IniFile::FileMain::path = _path;
	ReLoadIni(IniFile::FileMain::path);
	ReLoadPath(IniFile::FileMain::path);
}

IniFile::IniW_R::IniW_R()
{

}

IniFile::IniW_R::~IniW_R()
{

}

void IniFile::IniW_R::AllWriteIni()
{
	
	for (std::map<std::string, std::map<std::string, std::string>>::iterator itr = IniFile::IniReader::root.begin(); itr != IniFile::IniReader::root.end(); ++itr)
	{
		for (std::map<std::string, std::string>::iterator iter = itr->second.begin(); iter != itr->second.end(); ++iter)
		{

			IniFile::IniWriter::SetValue(itr->first, iter->first,iter->second);
		}
	}

	IniFile::IniWriter::WriteIni();
}

void IniFile::IniW_R::WriteIni(std::map<std::string, std::map<std::string, std::string>> newMap)
{

	IniFile::IniWriter::FileWriter.open(IniFile::IniWriter::path.c_str());
	if (!FileWriter.is_open()) {
#ifdef INIDEBUG
		std::cout << "д������������ļ�ʧ�ܣ�������\n" << "Ŀ¼Ϊ��" << IniFile::IniWriter::path << std::endl;
#endif	//INIDEBUG
		throw "�ļ���ʧ�ܣ�";
	}


	for (std::map<std::string, std::map<std::string, std::string>>::iterator itr = newMap.begin(); itr != newMap.end(); ++itr)
	{
		FileWriter << "[" << itr->first << "]" << std::endl;
		for (std::map<std::string, std::string>::iterator iter = itr->second.begin(); iter != itr->second.end(); ++iter)
		{
#ifdef INIDEBUG
			std::cout << "д�룺" << iter->first << " = " << iter->second << std::endl;
#endif	//INIDEBUG
			FileWriter << iter->first << " = " << iter->second << std::endl;
		}
		FileWriter << std::endl;
	}


	FileWriter.close();
	FileWriter.clear();


}
