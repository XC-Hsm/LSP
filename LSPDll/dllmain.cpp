#include "Func.h"
#include "stdafx.h"
#include <string>

WSPUPCALLTABLE g_pUpCallTable;
WSPPROC_TABLE g_NextProcTable;
TCHAR g_szCurrentApp[MAX_PATH];

#define FILE_MAX_SIZE (1024 * 1024)

void get_local_time(char *buffer)
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
			(timeinfo->tm_year + 1900), timeinfo->tm_mon, timeinfo->tm_mday,
			timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

long get_file_size(char *filename)
{
	long length = 0;
	FILE *fp = NULL;

	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
	}

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	return length;
}

void write_log_file(char *filename, long max_size, char *buffer, unsigned buf_size)
{
	if (filename != NULL && buffer != NULL)
	{

		long length = get_file_size(filename);

		if (length > max_size)
		{
			unlink(filename);
		}

		{
			FILE *fp;
			fp = fopen(filename, "at+");
			if (fp != NULL)
			{
				char now[32];
				memset(now, 0, sizeof(now));
				get_local_time(now);
				fwrite(now, strlen(now) + 1, 1, fp);
				fwrite(buffer, buf_size, 1, fp);

				fclose(fp);
				fp = NULL;
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{

		GetModuleFileName(NULL, g_szCurrentApp, MAX_PATH);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

int WSPAPI WSPConnect(
	SOCKET s,
	const struct sockaddr FAR *name,
	int namelen,
	LPWSABUF lpCallerData,
	LPWSABUF lpCalleeData,
	LPQOS lpSQOS,
	LPQOS lpGQOS,
	LPINT lpErrno)
{
	sockaddr_in *info = (sockaddr_in *)name;
	USHORT port = ntohs(info->sin_port);

	return g_NextProcTable.lpWSPConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
}
int WSPAPI WSPSendTo(
	SOCKET s,
	LPWSABUF lpBuffers,
	DWORD dwBufferCount,
	LPDWORD lpNumberOfBytesSent,
	DWORD dwFlags,
	const struct sockaddr FAR *lpTo,
	int iTolen,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID lpThreadId,
	LPINT lpErrno)
{
	sockaddr_in *info = (sockaddr_in *)lpTo;
	USHORT port = ntohs(info->sin_port);

	if (inet_ntoa(info->sin_addr) == std::string("118.180.60.20") || inet_ntoa(info->sin_addr) == std::string("125.74.108.110"))
	{
		for (int i = 0; i < dwBufferCount; i++)
		{
			strcpy(lpBuffers[i].buf, "12354");

			printf(lpBuffers[i].buf);
		}

		int nError = 0;
		SetConsoleTitle(_T("sorry,we shutdown you udp protocol port<8888>!"));

		*lpErrno = WSAECONNABORTED;

		return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags,
										   lpTo, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
	}
	return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags,
									   lpTo, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}
void str_replace(char *str1, char *str2, char *str3)
{
	int i, j, k, done, count = 0, gap = 0;
	char temp[2000];
	for (i = 0; i < strlen(str1); i += gap)
	{
		if (str1[i] == str2[0])
		{
			done = 0;
			for (j = i, k = 0; k < strlen(str2); j++, k++)
			{
				if (str1[j] != str2[k])
				{
					done = 1;
					gap = k;
					break;
				}
			}
			if (done == 0)
			{
				for (j = i + strlen(str2), k = 0; j < strlen(str1); j++, k++)
				{
					temp[k] = str1[j];
				}
				temp[k] = '\0';
				for (j = i, k = 0; k < strlen(str3); j++, k++)
				{
					str1[j] = str3[k];
					count++;
				}
				for (k = 0; k < strlen(temp); j++, k++)
				{
					str1[j] = temp[k];
				}
				str1[j] = '\0';
				gap = strlen(str2);
			}
		}
		else
		{
			gap = 1;
		}
	}
	if (count == 0)
	{
		printf("Can't find the replaced string!\n");
	}
	return;
}
int WSPAPI WSPSend(
	SOCKET s,
	LPWSABUF lpBuffers,
	DWORD dwBufferCount,
	LPDWORD lpNumberOfBytesSent,
	DWORD dwFlags,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID lpThreadId,
	LPINT lpErrno)
{
	struct sockaddr_in cliaddr;
	int clilen = sizeof(cliaddr);
	getpeername(s, (struct sockaddr *)&cliaddr, &clilen);
	sockaddr_in *info = &cliaddr;
	USHORT port = ntohs(info->sin_port);

	//http://api.qqahao.cn/api.php?qq=
	//if (inet_ntoa(info->sin_addr) == std::string("118.180.60.20") || inet_ntoa(info->sin_addr) == std::string("125.74.108.110"))
	//{
		for (int i = 0; i < dwBufferCount; i++)
		{
			std::string buf = lpBuffers[i].buf;
			int result = buf.find(std::string("http://api.qqahao.cn/"));

			if (buf.find(std::string("http://api.qqahao.cn/")) != -1) {

				str_replace(lpBuffers[i].buf, "2323345595", "2063884978");
				//str_replace(lpBuffers[i].buf, "2557063315", "2063884978");
			}
			/*if (lpBuffers[i].len >= 36)
			{
				char posttype[33] = {0};
				memcpy(posttype, lpBuffers[i].buf, 33);*/
				//if (strcmp(posttype, "POST /ajax.php?act=query HTTP/1.1") == 0)
				//{

				//	write_log_file(".\\log.txt", FILE_MAX_SIZE, lpBuffers[i].buf, strlen(lpBuffers[i].buf));
					

				//	write_log_file(".\\log.txt", FILE_MAX_SIZE, lpBuffers[i].buf, strlen(lpBuffers[i].buf));
				//}
			//}
		}

		//return g_NextProcTable.lpWSPSend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
	//}
	return g_NextProcTable.lpWSPSend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags,
									 lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPStartup(
	WORD wVersionRequested,
	LPWSPDATA lpWSPData,
	LPWSAPROTOCOL_INFO lpProtocolInfo,
	WSPUPCALLTABLE UpcallTable,
	LPWSPPROC_TABLE lpProTable)
{

	if (lpProtocolInfo->ProtocolChain.ChainLen <= 1)
	{
		return WSAEPROVIDERFAILEDINIT;
	}

	g_pUpCallTable = UpcallTable;

	WSAPROTOCOL_INFOW NextProtocolInfo;
	int nTotalProtols;
	LPWSAPROTOCOL_INFOW pProtoInfo = GetProvider(&nTotalProtols);
	int flag = 0;

	for (int chaini = 0; chaini < lpProtocolInfo->ProtocolChain.ChainLen; chaini++)
	{

		DWORD dwBaseEntryId = lpProtocolInfo->ProtocolChain.ChainEntries[chaini];

		int i = 0;
		for (; i < nTotalProtols; i++)
		{
			if (pProtoInfo[i].dwCatalogEntryId == dwBaseEntryId)
			{
				memcpy(&NextProtocolInfo, &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}

	if (flag == 0)
	{
		return WSAEPROVIDERFAILEDINIT;
	}

	int nError = 0;
	TCHAR szBaseProviderDll[MAX_PATH];
	int nLen = MAX_PATH;

	if (WSCGetProviderPath(&NextProtocolInfo.ProviderId, szBaseProviderDll, &nLen, &nError) == SOCKET_ERROR)
		return WSAEPROVIDERFAILEDINIT;

	if (!ExpandEnvironmentStrings(szBaseProviderDll, szBaseProviderDll, MAX_PATH))
		return WSAEPROVIDERFAILEDINIT;

	HMODULE hModdule = LoadLibrary(szBaseProviderDll);
	if (hModdule == NULL)
		return WSAEPROVIDERFAILEDINIT;

	LPWSPSTARTUP pfnWSPStartup = (LPWSPSTARTUP)GetProcAddress(hModdule, "WSPStartup");
	if (NULL == pfnWSPStartup)
		return WSAEPROVIDERFAILEDINIT;
	LPWSAPROTOCOL_INFOW pInfo = lpProtocolInfo;
	if (NextProtocolInfo.ProtocolChain.ChainLen == BASE_PROTOCOL)
		pInfo = &NextProtocolInfo;

	int nRet = pfnWSPStartup(wVersionRequested, lpWSPData, lpProtocolInfo, UpcallTable, lpProTable);

	if (nRet != ERROR_SUCCESS)
		return nRet;

	g_NextProcTable = *lpProTable;

	lpProTable->lpWSPSendTo = WSPSendTo;
	lpProTable->lpWSPConnect = WSPConnect;
	lpProTable->lpWSPSend = WSPSend;
	FreeProvider(pProtoInfo, nTotalProtols);
	return nRet;
}
