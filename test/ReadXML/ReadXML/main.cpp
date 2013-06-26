#include "libxml.h"

void main()
{
    xmlInitParser();
    xmlDocPtr doc;
    doc = xmlParseFile("default.xml"); //读取文件

    xmlNodePtr curNode;
    curNode = xmlDocGetRootElement(doc);

    xmlXPathContextPtr context = xmlXPathNewContext(doc);//建立context
    if (context == NULL)
    {  
       printf("context is NULL\n");
       return;
    }

    xmlXPathRegisterNs(context,(const xmlChar *)"ns", (const xmlChar *)"http://localhost/ImageryDescriptionDocumentFile.xsd");

    xmlChar* szXpath = (xmlChar*)"/ns:ImageryDescription/ns:imageryDescription[@id=0]";

    xmlXPathObjectPtr result = xmlXPathEvalExpression(szXpath, context);

    if (result == NULL || xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        printf("can't find element!\n");
    }
    else
    {
        xmlNodePtr pNode = result->nodesetval->nodeTab[0];

        xmlChar* id_char = xmlGetProp(pNode,(xmlChar*) "id");
        int id = atoi((char*)id_char);
        printf("id = %d\n", id);
        xmlChar* href = xmlGetProp(pNode,(xmlChar*) "href");
        printf("href = %s\n", (char*)href);
        xmlChar* imageName = xmlGetProp(pNode,(xmlChar*) "imageName");
        printf("imageName = %s\n", (char*)imageName);
    }



    xmlCleanupParser();
    return;

}