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
/// 重读取路径
/// </summary>
/// <param name="path"></param>
void CokFile::CokFileR_W::ReLoadPath(std::string path)
{
	if (fileProcessor.is_open())
		fileProcessor.close();
#ifdef DEBUG
	std::cout << "加载目录：" <<path<< std::endl;
#endif	//INIDEBUG
	std::ifstream ifs(path.c_str());
	if (!ifs) {
#ifdef DEBUG
		std::cout <<"打开文件失败！" << std::endl;
#endif	//INIDEBUG
		std::ofstream ofs(path);
		isEmpty = true;
		ofs.close();
	}
	fileProcessor.open(path);
	isEnable = true;
}
/// <summary>
/// 获取文件表
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
/// 添加节点
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
/// 保存文件
/// </summary>
void CokFile::CokFileR_W::WriteIn()
{
	WriteNewFile(this->lic, this->path);
}
/// <summary>
/// 将外部表写入文件
/// </summary>
/// <param name="tcc">外部表</param>
/// <param name="path">路径</param>
void CokFile::CokFileR_W::WriteNewFile(std::vector<CokCC> tcc, std::string path)
{
#ifdef DEBUG
	qDebug()  << "LENTH" << tcc.size() ;
	qDebug()  << "开始写入文件！ 写入到："  ;
#endif	//INIDEBUG
	if (fileProcessor.is_open())
		fileProcessor.close();
	fileProcessor.open(path);
	if (!fileProcessor)
	{
		qDebug() << "文件打开失败！";
	}
	for(std::vector<CokCC>::iterator iter = tcc.begin(); iter != tcc.end(); ++iter)
	{
#ifdef DEBUG
		qDebug() << "写入文件！ ";
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
