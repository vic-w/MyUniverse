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
				};
				attr = attr->next;
			}
			cities.push_back(city);
			node = node->next->next; //TODO: fix this
		}
	}
    
	xmlFreeDoc(doc);
	return cities;
}