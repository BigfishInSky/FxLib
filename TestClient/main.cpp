//#include <WinSock2.h>
#include "SocketSession.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <unistd.h>
#include "lua_engine.h"
#include "fxtimer.h"

int main()
{
	CLuaEngine::CreateInstance();

	CLuaEngine::Instance()->Reload();

	GetTimeHandler()->Init();
	GetTimeHandler()->Run();

	IFxNet* pNet = FxNetGetModule();

	UINT32 dwIP = inet_addr("127.0.0.1");

	FxSession* pSession = oSessionFactory.CreateSession();
	pNet->Connect(pSession, dwIP, 10000, true);

	FxSleep(1000);

	char szMsg[1024] = "";
	int i = 0;
	while (true)
	{
		pNet->Run(0xffffffff);
		if (pSession->IsConnected())
		{
			sprintf(szMsg, "%s", "select * from role");
			++i;
			//sprintf(szMsg, "%d", ++i);
			pSession->Send(szMsg, 1024);
			i %= 20;
			if (i == 0)
			{
				pSession->Close();
			}
			FxSleep(500);
		}
		else
		{
			pSession->Reconnect();
			FxSleep(1000);
		}
	}
}