#include "ServerNet.h"

int main(int argc, char **argv)
{
	CServerNet serverNet;
	int iRlt = serverNet.Init("127.0.0.1",1235);
	if (iRlt == 0)
	{
		printf("init ok...\n");
		serverNet.Run();
	}
	else
		printf("serverNet init failed with error : %d\n",iRlt);
	system("pause");
}