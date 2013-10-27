#include "menu_icon.h"


CMenuIcon::CMenuIcon()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	string dir("menu\\");
	string path = dir + "*.bmp";
	hFind = FindFirstFile(path.c_str(), &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox (_T("Invalid file handle.\n"));
	}
	else
	{
		do
		{
            string imageFileName = FindFileData.cFileName;
			path = dir + imageFileName;
			GlbSize size;
			GlbImage img = glbLoadImage(path.c_str(), size);
			string key = imageFileName.substr(0,imageFileName.size()-4);
			m_icons.insert(map< string, GlbImage >::value_type(key, img)); 
			m_size.insert(map< string, GlbSize >::value_type(key, size)); 

		}while (FindNextFile(hFind, &FindFileData) != 0);
	}
}

CMenuIcon::~CMenuIcon()
{
	for(map<string,GlbImage>::iterator it = m_icons.begin(); it!=m_icons.end(); ++it)
	{
		glbReleaseImage(&(it->second));
	}
}

GlbImage CMenuIcon::getImage(string name, GlbSize &size)
{
	size = m_size.find(name)->second;
	return m_icons.find(name)->second;
}
