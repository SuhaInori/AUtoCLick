#include "CokFileR_W.h"

CokFile::CokFileR_W::CokFileR_W()
{
	isEnable = false;
}

CokFile::CokFileR_W::CokFileR_W(std::string path)
{
	isEnable = false;
	this->path = path;
	isEmpty = false;
	ReLoadPath(path);
	
}

CokFile::CokFileR_W::~CokFileR_W()
{
	WriteIn();
}
/// <summary>
/// �ض�ȡ·��
/// </summary>
/// <param name="path"></param>
void CokFile::CokFileR_W::ReLoadPath(std::string path)
{
	if (fileProcessor.is_open())
		fileProcessor.close();
#ifdef DEBUG
	std::cout << "����Ŀ¼��" <<path<< std::endl;
#endif	//INIDEBUG
	std::ifstream ifs(path.c_str());
	if (!ifs) {
#ifdef DEBUG
		std::cout <<"���ļ�ʧ�ܣ�" << std::endl;
#endif	//INIDEBUG
		std::ofstream ofs(path);
		isEmpty = true;
		ofs.close();
	}
	fileProcessor.open(path);
	isEnable = true;
}
/// <summary>
/// ��ȡ�ļ���
/// </summary>
/// <returns></returns>
std::vector<CokFile::CokCC> CokFile::CokFileR_W::GetCokFile()
{
	std::string line;
	std::string::size_type pos = 0;
	while (getline(fileProcessor, line)) {
		CokFile::CokCC line_ccc;
		std::string temp;
		if (line.npos != (pos = line.find(","))) {
			temp = line.substr(0, pos );
			line = line.substr(pos + 1, line.size() - 1);
			line_ccc.nub = atoi(temp.c_str());
		}
		if (line.npos != (pos = line.find(",")))
		{
			temp = line.substr(0, pos );
			line = line.substr(pos + 1, line.size() - 1);
			line_ccc.xadd= atoi(temp.c_str());
		}
		if (line.npos != (pos = line.find(",")))
		{
			temp = line.substr(0, pos);
			line = line.substr(pos + 1, line.size() - 1);
			line_ccc.yadd = atoi(temp.c_str());
		}
		if (line.npos != (pos = line.find(",")))
		{
			temp = line.substr(0, pos );
			line = line.substr(pos + 1, line.size() - 1);
			line_ccc.sleepTime = atoi(temp.c_str());
			line_ccc.another = line;
		}
#ifdef DEBUG
		qDebug() << line_ccc.nub << ',' << line_ccc.xadd << ',' << line_ccc.yadd << ',' << line_ccc.sleepTime << ',' << QString::fromStdString(line_ccc.another) ;
#endif	//INIDEBUG
		lic.push_back(line_ccc);
	}

	return lic;
}
/// <summary>
/// ��ӽڵ�
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="sleep"></param>
/// <param name="another"></param>
void CokFile::CokFileR_W::AddPoint(int x, int y, int sleep, std::string another)
{
	CokFile::CokCC temp;
	temp.nub = lic.size();
	temp.xadd = x;
	temp.yadd = y;
	temp.sleepTime = sleep;
	temp.another = another;
#ifdef DEBUG
	qDebug() << temp.nub << ',' << temp.xadd << ',' << temp.yadd << ',' << temp.sleepTime << ',' << QString::fromStdString(temp.another) ;
#endif	//INIDEBUG
	lic.push_back(temp);
#ifdef DEBUG
	qDebug() << "LENTH" << lic.size() ;
#endif	//INIDEBUG
}

void CokFile::CokFileR_W::AddCokCC(CokCC c)
{
	lic.push_back(c);
}

/// <summary>
/// �����ļ�
/// </summary>
void CokFile::CokFileR_W::WriteIn()
{
	WriteNewFile(this->lic, this->path);
}
/// <summary>
/// ���ⲿ��д���ļ�
/// </summary>
/// <param name="tcc">�ⲿ��</param>
/// <param name="path">·��</param>
void CokFile::CokFileR_W::WriteNewFile(std::vector<CokCC> tcc, std::string path)
{
#ifdef DEBUG
	qDebug()  << "LENTH" << tcc.size() ;
	qDebug()  << "��ʼд���ļ��� д�뵽��"  ;
#endif	//INIDEBUG
	if (fileProcessor.is_open())
		fileProcessor.close();
	fileProcessor.open(path);
	if (!fileProcessor)
	{
		qDebug() << "�ļ���ʧ�ܣ�";
	}
	for(std::vector<CokCC>::iterator iter = tcc.begin(); iter != tcc.end(); ++iter)
	{
#ifdef DEBUG
		qDebug() << "д���ļ��� ";
#endif	//INIDEBUG
		fileProcessor << iter->nub << ',' << iter->xadd << ',' << iter->yadd << ',' << iter->sleepTime << ',' << iter->another << std::endl;
#ifdef DEBUG
		qDebug() << iter->nub << ',' << iter->xadd << ',' << iter->yadd << ',' << iter->sleepTime << ',' << QString::fromStdString(iter->another);
#endif	//INIDEBUG
	}
	fileProcessor.close();
}

bool CokFile::CokFileR_W::is_open()
{
	return fileProcessor.is_open();
}
