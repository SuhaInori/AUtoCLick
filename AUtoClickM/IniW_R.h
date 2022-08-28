#pragma once
#include "IniReader.h"
#include "IniWriter.h"
namespace IniFile {
	class IniW_R :public IniReader, public IniWriter
	{
	public:
		IniW_R(std::string);

		IniW_R();

		~IniW_R();

		void AllWriteIni();

	private:
		void WriteIni(std::map<std::string, std::map<std::string, std::string>>);
	};

}

