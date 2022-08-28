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
		/// �ض��ļ�
		/// </summary>
		/// <param name="path">�ļ�·��</param>
		void ReLoadIni(std::string);
		/// <summary>
		/// ��ini������Ϣ����string��
		/// </summary>
		/// <returns></returns>
		std::string toString();
		/// <summary>
		/// ��ȡint��ֵ
		/// </summary>
		/// <param name="root">���ڵ�</param>
		/// <param name="key">��</param>
		/// <returns></returns>
		int GetInt32(std::string, std::string);
		/// <summary>
		/// ��ȡdouble��ֵ
		/// </summary>
		/// <param name="root">���ڵ�</param>
		/// <param name="key">��</param>
		/// <returns></returns>
		double GetDouble(std::string, std::string);
		/// <summary>
		/// ��ȡstring��ֵ
		/// </summary>
		/// <param name="root">���ڵ�</param>
		/// <param name="key">��</param>
		/// <returns></returns>
		std::string GetString(std::string, std::string);
		/// <summary>
		/// ��ȡbool��
		/// </summary>
		/// <param name="">���ڵ�</param>
		/// <param name="">��</param>
		/// <returns></returns>
		bool GetBool(std::string, std::string);
		/// <summary>
		/// ��ȡ����������ֵ
		/// </summary>
		/// <param name="root">���ڵ�</param>
		/// <param name="key">��</param>
		/// <returns></returns>
		std::vector<double>GetNumArray(std::string, std::string);
		/// <summary>
		/// ��ȡ������ֵ
		/// </summary>
		/// <param name="root">���ڵ�</param>
		/// <param name="key">��</param>
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