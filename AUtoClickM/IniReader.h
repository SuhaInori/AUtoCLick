#pragma once

#include "FileMain.h"

struct Node
{
	std::string root;
	std::string key;
	std::string value;

	Node(std::string a, std::string b, std::string c) :root(a), key(b), value(c) {};
};

namespace IniFile {
	class IniReader :public FileMain
	{
	public:
		IniReader(std::string);

		IniReader();

		~IniReader();

		bool IsEmpty;
		/// <summary>
		/// 重读文件
		/// </summary>
		/// <param name="path">文件路径</param>
		void ReLoadIni(std::string);
		/// <summary>
		/// 将ini所有信息存入string中
		/// </summary>
		/// <returns></returns>
		std::string toString();
		/// <summary>
		/// 获取int型值
		/// </summary>
		/// <param name="root">根节点</param>
		/// <param name="key">键</param>
		/// <returns></returns>
		int GetInt32(std::string, std::string);
		/// <summary>
		/// 获取double型值
		/// </summary>
		/// <param name="root">根节点</param>
		/// <param name="key">键</param>
		/// <returns></returns>
		double GetDouble(std::string, std::string);
		/// <summary>
		/// 获取string型值
		/// </summary>
		/// <param name="root">根节点</param>
		/// <param name="key">键</param>
		/// <returns></returns>
		std::string GetString(std::string, std::string);
		/// <summary>
		/// 获取bool型
		/// </summary>
		/// <param name="">根节点</param>
		/// <param name="">键</param>
		/// <returns></returns>
		bool GetBool(std::string, std::string);
		/// <summary>
		/// 获取数字数组型值
		/// </summary>
		/// <param name="root">根节点</param>
		/// <param name="key">键</param>
		/// <returns></returns>
		std::vector<double>GetNumArray(std::string, std::string);
		/// <summary>
		/// 获取数组型值
		/// </summary>
		/// <param name="root">根节点</param>
		/// <param name="key">键</param>
		/// <returns></returns>
		std::vector<std::string> GetArray(std::string, std::string);
	protected:
		std::fstream FileReader;

	private:
		std::vector<Node> ini_Node;

		void ReadIniFile();

		std::string GetValue(std::string, std::string);

		static std::string& trim(std::string& s);

		static std::string& TrimString(std::string& str);

		static std::vector<std::string> split(std::string str, std::string pattern);
	};
}