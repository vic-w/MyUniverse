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

    xmlNodeSetPtr nodeset = NULL;  
    xmlChar* uri;  

    xmlXPathRegisterNs(context,(const xmlChar *)"ns", (const xmlChar *)"http://localhost/ImageryDescriptionDocumentFile.xsd");

    xmlChar* szXpath = (xmlChar*)"/ns:ImageryDescription";
    xmlXPathObjectPtr result = xmlXPathEvalExpression(szXpath, context);


    if(NULL != result) {  
        nodeset = result->nodesetval;  
        int i = 0;  
        for(i = 0; i < nodeset->nodeNr; i ++) {  
            uri = xmlGetProp(nodeset->nodeTab[i],(const xmlChar*)"href");  
            printf("link address:%s\n",uri);  
            xmlFree(uri);  
        }  
        xmlXPathFreeObject(result);  
    }  


    int a=0;
    xmlCleanupParser();
    return;

}