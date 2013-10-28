#include <afxwin.h>
#include "city.h"
#include "libxml.h"
#include "libxml\xpath.h"

#pragma comment (lib, "libxml2.lib")
#pragma comment (lib, "iconv.lib")

CCity::CCity(void)
{
}

CCity::~CCity(void)
{
}

CString	GetCurAppFilePath() 
{
	TCHAR szPath[MAX_PATH] ;
	GetModuleFileName(NULL, szPath, MAX_PATH) ;
	PathRemoveFileSpec(szPath) ;
	return	szPath ;
}

//http://stackoverflow.com/questions/8308236/performing-arithmetic-on-systemtime
SYSTEMTIME add( SYSTEMTIME s, double seconds ) {

	FILETIME f;
	SystemTimeToFileTime( &s, &f );

	ULARGE_INTEGER u  ; 
	memcpy( &u  , &f , sizeof( u ) );

	const double c_dSecondsPer100nsInterval = 100. * 1.E-9;
	u.QuadPart += seconds / c_dSecondsPer100nsInterval; 

	memcpy( &f, &u, sizeof( f ) );

	FileTimeToSystemTime( &f, &s );
	return s;
}

char* CCity::getLocalTimeString()
{
	SYSTEMTIME st = {0};
	GetSystemTime(&st); //UTC time

	// Convert to local time provided timezone information.
	SYSTEMTIME localTime = add(st, timezone*60*60);
	char* csLocalTime = new char[32];
	sprintf(csLocalTime, "%02d:%02d", localTime.wHour,
		localTime.wMinute/*,localTime.wSecond*/);
	return csLocalTime;
}

char* CCity::getTimezoneDiffString(CCity city1, CCity city2)
{
	char* timeDiffString = new char[64];
	if (city2.timezone > city1.timezone)
		sprintf(timeDiffString, "%s的时间比%s早%2.1f小时", city2.displayname, city1.displayname, abs(city2.timezone - city1.timezone));
	else if (city2.timezone < city1.timezone) 
		sprintf(timeDiffString, "%s的时间比%s晚%2.1f小时", city2.displayname, city1.displayname, abs(city2.timezone - city1.timezone));
	else  //float point comparison?
		sprintf(timeDiffString, "%s和%s处在同一个时区", city2.displayname, city1.displayname);
	return timeDiffString;
}

char* CCity::getTimezoneDiffString(CCity city1)
{
	char* timeDiffString = new char[64];
	sprintf(timeDiffString, "%s和北京的时差为%2.1f小时", city1.displayname, city1.timezone - 8);
	return timeDiffString;
}

int code_convert(char* from_charset, char* to_charset, char* inbuf,
	int inlen, char* outbuf, int outlen)
{
	iconv_t cd;
	char** pin = &inbuf;  
	char** pout = &outbuf;
	cd = iconv_open(to_charset,from_charset);  
	if(cd == 0)
		return -1;

	memset(outbuf,0,outlen);  

	if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen) == -1)
		return -1;  

	iconv_close(cd);

	return 0;  
}

char* u2g(char *inbuf)  
{
	if (strlen(inbuf) == 0)
	{
		return "";
	}

	int nOutLen = 2 * strlen(inbuf) - 1;
	char* szOut = (char*)malloc(nOutLen);
	if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
	{
		free(szOut);
		szOut = NULL;
	}

	return szOut;
}  

bool CCity::updateXml()
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.lpVerb = TEXT("runas"); //以管理员身份运行，如果XML文件放在C:\Program Files下， 需要以管理员运行才能修改文件。正确的做法是把需要写权限的文件移到AppData或者ProgramData目录下面去。	
	CString file = GetCurAppFilePath() + TEXT("\\UpdateFolderStructure.exe");
	sei.lpFile = file;//使用全路径
	//sei.lpParameters = TEXT("");
	sei.nShow = SW_SHOWNORMAL;
	if (!ShellExecuteEx(&sei))
	{
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED)
		{
			printf("user cancelled the elevated request!");
		}
		else if (dwStatus == ERROR_FILE_NOT_FOUND)
		{
			printf("File not found!");			
		}
		return false;
	}
	else
	{
		WaitForSingleObject(sei.hProcess,INFINITE);
		DWORD dwRet;
		GetExitCodeProcess(sei.hProcess, &dwRet);
		if (dwRet == 0)
			return true;
		else
		{
			printf("Failed to update xml, please check log.");
			return false;
		}
	}
}

vector<CCity> CCity::getCities()
{
	//return populatetestdata();

	vector<CCity> cities;
	//
	xmlInitParser();
	xmlDocPtr doc = xmlParseFile("cities.xml");
	if (doc)
	{
		xmlXPathContext *xpathCtx = xmlXPathNewContext( doc );
		xmlXPathObject * xpathObj =
			xmlXPathEvalExpression( (xmlChar*)"/cities/city", xpathCtx );
		xmlNode *node = xpathObj->nodesetval->nodeTab[0];
		while (node)
		{
			if (!xmlStrcmp(node->name, (const xmlChar *)"city")) //deal with white space between nodes
			{
				CCity city;
				xmlAttr *attr = node->properties;
				while ( attr )
				{
					char* val = (char*)(attr->children->content);

					if ((!xmlStrcmp(attr->name, (const xmlChar *)"id"))){
						city.id = atoi(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"name"))){
						city.name = u2g(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"displayname"))){
						city.displayname = u2g(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"country"))){
						city.country = u2g(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"displaycountry"))){
						city.displaycountry = (strcmp(val, "true") == 0);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"latitude"))){
						city.latitude = atof(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"longitude"))){
						city.longitude = atof(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"timezone"))){
						city.timezone = atof(val);
					}
					else if ((!xmlStrcmp(attr->name, (const xmlChar *)"path"))){
						city.folder = u2g(val);
					}
					attr = attr->next;
				}

				xmlNode *categoriesNode = node->children;
				if (categoriesNode != NULL)
				{
					while (xmlStrcmp(categoriesNode->name, (const xmlChar *)"categories")) //deal with white space between nodes
					{
						categoriesNode = categoriesNode->next;
						continue;
					}

					xmlNode *categoryNode = categoriesNode->children;
					while (categoryNode != NULL) {
						if (!xmlStrcmp(categoryNode->name, (const xmlChar *)"category")) //deal with white space between nodes
						{
							vector<char*> vCategory;
							attr = categoryNode->properties;
							if ((!xmlStrcmp(attr->name, (const xmlChar *)"name"))){
								vCategory.push_back(u2g((char*)(attr->children->content)));
							}
							xmlNode *imgNode = categoryNode->children;
							while (imgNode != NULL) {
								if (!xmlStrcmp(imgNode->name, (const xmlChar *)"img")) //deal with white space between nodes
								{
									attr = imgNode->properties;
									if ((!xmlStrcmp(attr->name, (const xmlChar *)"path"))){
										vCategory.push_back(u2g((char*)(attr->children->content)));
									}
								}
								imgNode = imgNode->next;
							}
							city.images.push_back(vCategory);
						}
						categoryNode = categoryNode->next;
					}
				}
				cities.push_back(city);
			}
			node = node->next;
		}
		xmlFreeDoc(doc);
	}
	return cities;
}