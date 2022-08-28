#pragma once
#include "FileMain.h"

namespace IniFile {
	class IniWriter :public FileMain
	{
	public:
		IniWriter(std::string);

		IniWriter();

		~IniWriter();
		/// <summary>
		/// 设置键值对
		/// </summary>
		/// <param name="">根</param>
		/// <param name="">键</param>
		/// <param name="">值</param>
		void SetValue(std::string , std::string, std::string);
		/// <summary>
		/// 文件保存
		/// </summary>
		void WriteIni();

		void ReLoadPath(std::string);
	protected:
		std::ofstream FileWriter;

	private:

		
	};

}