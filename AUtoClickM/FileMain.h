#pragma once
#include <string>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/*ifstream �C �����е��ļ���

ofstream �C ���ļ�д����

fstream - ���ļ�����д*/

namespace IniFile {//ini�ļ���
	class FileMain
	{
	public:
		
		void ReAddPath(std::string _path);

	protected:
		std::string path;

		FileMain(std::string);

		FileMain();

		std::map<std::string, std::map<std::string, std::string>> root;

	private:
	};
}
