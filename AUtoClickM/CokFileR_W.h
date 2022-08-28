#pragma once
#include <string>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <qdebug.h>
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
namespace CokFile {
	struct CokCC
	{
		int nub;
		int xadd;
		int yadd;
		unsigned int sleepTime;
		std::string another;
	};

	class CokFileR_W
	{
	public:
		CokFileR_W();
		CokFileR_W(std::string path);

		~CokFileR_W();

		void ReLoadPath(std::string path);

		std::vector<CokCC> GetCokFile();

		void AddPoint(int x,int y,int sleep,std::string another= " ");

		void AddCokCC(CokCC);

		void WriteIn();

		void WriteNewFile(std::vector<CokCC> tcc, std::string path);

		bool is_open();

		bool isEmpty;

		bool isEnable;

	private:
		std::string path;
		std::fstream fileProcessor;
		std::vector<CokCC> lic;

	};
}


