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
		/// ���ü�ֵ��
		/// </summary>
		/// <param name="">��</param>
		/// <param name="">��</param>
		/// <param name="">ֵ</param>
		void SetValue(std::string , std::string, std::string);
		/// <summary>
		/// �ļ�����
		/// </summary>
		void WriteIni();

		void ReLoadPath(std::string);
	protected:
		std::ofstream FileWriter;

	private:

		
	};

}