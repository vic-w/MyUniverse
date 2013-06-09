#include "opencv.hpp"
#include "libxml\tree.h"
#include "libxml\parser.h"

void main()
{
    xmlDocPtr doc; //文件指针
   xmlNodePtr cur;//节点指针
   doc = xmlParseFile("b.xml");
   cur = xmlDocGetRootElement(doc);
   if(xmlStrcmp(cur->name, (const xmlChar*)"root") )
   {
     xmlFreeDoc(doc);
     return;
   }
   cur = cur->xmlChildrenNode;
   //parseNode(doc, cur);//处理节点
   //xmlSaveFormatFileEnc("tconfig.xml", doc, "UTF-8", 1 );//保存修改
   xmlFreeDoc(doc); //释放内存
   xmlCleanupParser();
   xmlMemoryDump();
   return;
	system("pause");
}
