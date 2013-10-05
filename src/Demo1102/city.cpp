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
	sprintf(csLocalTime, "%ld:%ld:%ld", localTime.wHour,
							 localTime.wMinute,
							 localTime.wSecond);
	return csLocalTime;
}

char* CCity::getTimezoneDiffString(CCity city1, CCity city2)
{
	char* timeDiffString = new char[64];
	sprintf(timeDiffString, "%s和%s的时差为%2.1f小时", city2.displayname, city1.displayname, city2.timezone - city1.timezone);
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
		return inbuf;
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

vector<CCity> CCity::getCities()
{
	vector<CCity> cities;

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
					bool northsphere = (val[strlen(val)-1] == 'N');
					val[strlen(val)]='\0';
					city.latitude = atof(val);
					if (!northsphere)
					{
						city.latitude = -city.latitude;
					}
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"longitude"))){
					bool eastsphere = (val[strlen(val)-1] == 'E');
					val[strlen(val)]='\0';
					city.longitude = atof(val);
					if (!eastsphere)
					{
						city.longitude = -city.longitude;
					}
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"timezone"))){
					city.timezone = atof(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"path"))){
					city.path = u2g(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"population"))){
					city.population = atoi(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"description"))){
					city.description = u2g(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"temprature"))){
					city.temprature = u2g(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"weatherCondition"))){
					city.weatherCondition = u2g(val);
				};
				attr = attr->next;
			}
			cities.push_back(city);
			node = node->next->next; //TODO: fix this
		}
		xmlFreeDoc(doc);
	}
    
	return cities;
}