#pragma once
#include <string>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/*ifstream – 从已有的文件读

ofstream – 向文件写内容

fstream - 打开文件供读写*/

namespace IniFile {//ini文件类
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
