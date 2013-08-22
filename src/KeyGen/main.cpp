
#include <stdio.h>
#include "../Ktmfc/md5.h"
#include <Windows.h>

void main()
{
	printf("请输入10位的用户码:");
	char s[100];
	do
	{
		gets_s(s,100);
	}
	while(strlen(s) != 10);

	printf("\n许可证密码为：%s\n",MD5String(s));
	system("pause");

}