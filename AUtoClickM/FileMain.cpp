#include "FileMain.h"

void IniFile::FileMain::ReAddPath(std::string _path)
{

	this->path = _path;
}

IniFile::FileMain::FileMain(std::string _PATH):path(_PATH)
{
}

IniFile::FileMain::FileMain() 
{
}
