#pragma once

#include <WS2spi.h>
#include <winsock2.h>
#include <process.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <direct.h>
using namespace std;
#pragma warning(disable : 4996)
#pragma comment(lib, "Sporder.lib")
#pragma comment(lib, "Ws2_32.lib")
#include <sporder.h>
#pragma warning(disable : 4996)

class ProtocolTraversestheExperiment1
{
public:
	void ShowAllProtocol()
	{
		int ProtocolsCount = 0;
		LPWSAPROTOCOL_INFO info = GetProvider(&ProtocolsCount);
		if (ProtocolsCount != 0)
		{
			for (int i = 0; i < ProtocolsCount; i++)
			{
				wprintf(_T("Protocol:%s \r\n"), info[i].szProtocol);
				wprintf(_T("CatalogEntryId:%d         ChainLen:%d \n\n"), info[i].dwCatalogEntryId, info[i].ProtocolChain.ChainLen);
			}
			FreeProvider(info);
		}
	}

public:
	ProtocolTraversestheExperiment1()
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ProtocolTraversestheExperiment1()
	{
		WSACleanup();
	}

private:
	LPWSAPROTOCOL_INFO GetProvider(LPINT lpnTotalProtocols)
	{
		DWORD dwSize = 0;
		LPWSAPROTOCOL_INFO pProtoInfo = NULL;
		if (WSAEnumProtocols(NULL, pProtoInfo, &dwSize) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAENOBUFS)
				return NULL;
		}
		pProtoInfo = (LPWSAPROTOCOL_INFO) new WSAPROTOCOL_INFO[dwSize / sizeof(WSAPROTOCOL_INFO)];
		if (!pProtoInfo)
			return NULL;
		ZeroMemory(pProtoInfo, dwSize);
		*lpnTotalProtocols = WSAEnumProtocols(NULL, pProtoInfo, &dwSize);
		return pProtoInfo;
	}
	void FreeProvider(LPWSAPROTOCOL_INFO pProtoInfo)
	{
		delete[] pProtoInfo;
	}
};

class ProtocolTraversestheExperiment2
{
public:
	void ShowAllProtocol()
	{
		int ProtocolsCount = 0;
		LPWSAPROTOCOL_INFOW info = GetProvider(&ProtocolsCount);
		if (ProtocolsCount != 0)
		{
			for (int i = 0; i < ProtocolsCount; i++)
			{
				wprintf(_T("Protocol:%s \r\n"), info[i].szProtocol);
				wprintf(_T("CatalogEntryId:%d         ChainLen:%d \n\n"), info[i].dwCatalogEntryId, info[i].ProtocolChain.ChainLen);
			}
			FreeProvider(info);
		}
	}

public:
	ProtocolTraversestheExperiment2()
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ProtocolTraversestheExperiment2()
	{
		WSACleanup();
	}

private:
	LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
	{
		int nError = 0;
		DWORD dwSize = 0;
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		if (WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
		{
			if (nError != WSAENOBUFS)
				return NULL;
		}
		pProtoInfo = (LPWSAPROTOCOL_INFOW) new WSAPROTOCOL_INFOW[dwSize / sizeof(WSAPROTOCOL_INFOW)];
		if (!pProtoInfo)
			return NULL;
		ZeroMemory(pProtoInfo, dwSize);
		*lpnTotalProtocols = WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
		return pProtoInfo;
	}
	void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
	{
		delete[] pProtoInfo;
	}
};

class installLsp
{ /*
	 SetConsoleTitle(_T("��װLSP�ṩ�߳���ʵ��"));
	 ProtocolTraversestheExperiment2 s;
	 printf("��װLSPǰ������Э��:\r\n");
	 s.ShowAllProtocol();
	 installLsp LSP;
	 LSP.InstallProvider(PATH);
	 printf("��װLSP�������Э��:\r\n");
	 s.ShowAllProtocol();
	 LSP.RemoveProvider();
	 system("color 3a");
	 printf("���LSP���\r\n");
  */
public:
	installLsp()
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
		CoCreateGuid(&this->Layered_guid);
		CoCreateGuid(&this->AgreementChain_guid);
	}
	~installLsp()
	{
		WSACleanup();
	}
	int InstallProvider(WCHAR *wszDllPath)
	{
		WCHAR wszLSPName[] = _T("AaronLSP");
		int nError = NO_ERROR;
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		int nProtocols = 0;
		WSAPROTOCOL_INFOW UDPLayeredInfo, UDPChainInfo;
		DWORD dwUdpOrigCatalogId = 0;
		DWORD dwLayeredCatalogId = 0;

		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return -1;
		for (int i = 0; i < nProtocols; i++)
		{
			if (pProtoInfo[i].iAddressFamily == AF_INET && pProtoInfo[i].iProtocol == IPPROTO_UDP)
			{

				memcpy(&UDPChainInfo, &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));

				UDPChainInfo.dwServiceFlags1 &= (~XP1_IFS_HANDLES);

				dwUdpOrigCatalogId = pProtoInfo[i].dwCatalogEntryId;
				break;
			}
		}
		memcpy(&UDPLayeredInfo, &UDPChainInfo, sizeof(WSAPROTOCOL_INFOW));

		wcscpy(UDPLayeredInfo.szProtocol, wszLSPName);

		UDPLayeredInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;

		UDPLayeredInfo.dwProviderFlags = PFL_HIDDEN;

		if (SOCKET_ERROR == WSCInstallProvider(&Layered_guid, wszDllPath, &UDPLayeredInfo, 1, &nError))
		{
			FreeProvider(pProtoInfo);
			return nError;
		}
		FreeProvider(pProtoInfo);

		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return -1;
		for (int i = 0; i < nProtocols; i++)
		{
			if (memcmp(&pProtoInfo[i].ProviderId, &Layered_guid, sizeof(GUID)) == 0)
			{

				dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
				break;
			}
		}

		WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
		swprintf(wszChainName, _T("%s over %s"), wszLSPName, UDPChainInfo.szProtocol);
		wcscpy(UDPChainInfo.szProtocol, wszChainName);
		if (UDPChainInfo.ProtocolChain.ChainLen == 1)
		{
			UDPChainInfo.ProtocolChain.ChainEntries[1] = dwUdpOrigCatalogId;
		}
		else
		{
			for (int i = UDPChainInfo.ProtocolChain.ChainLen; i > 0; i--)
			{

				UDPChainInfo.ProtocolChain.ChainEntries[i] = UDPChainInfo.ProtocolChain.ChainEntries[i - 1];
			}
		}
		UDPChainInfo.ProtocolChain.ChainLen++;
		UDPChainInfo.ProtocolChain.ChainEntries[0] = dwLayeredCatalogId;

		if (SOCKET_ERROR == WSCInstallProvider(&AgreementChain_guid, wszDllPath, &UDPChainInfo, 1, &nError))
		{
			FreeProvider(pProtoInfo);
			return nError;
		}

		FreeProvider(pProtoInfo);
		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return -1;
		DWORD dwIds[20];
		int nIndex = 0;

		for (int i = 0; i < nProtocols; i++)
		{
			if (pProtoInfo[i].ProtocolChain.ChainLen > 1 && pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId)
				dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}

		for (int i = 0; i < nProtocols; i++)
		{
			if (pProtoInfo[i].ProtocolChain.ChainLen <= 1 || pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId)

				dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}

		nError = WSCWriteProviderOrder(dwIds, nIndex);
		FreeProvider(pProtoInfo);
		return nError;
	}
	void RemoveProvider()
	{
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		int nProtocols = 0;
		DWORD dwLayeredCatalogId = 0;

		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return;
		int nError = 0;
		int i = 0;
		for (i = 0; i < nProtocols; i++)
		{
			if (memcmp(&Layered_guid, &pProtoInfo[i].ProviderId, sizeof(GUID)) == 0)
			{
				dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
				break;
			}
		}
		if (i < nProtocols)
		{
			for (i = 0; i < nProtocols; i++)
			{
				if (pProtoInfo[i].ProtocolChain.ChainLen > 1 && pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId)
				{
					WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &nError);
					break;
				}
			}
			WSCDeinstallProvider(&Layered_guid, &nError);
		}
	}

private:
	LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
	{
		int nError = 0;
		DWORD dwSize = 0;
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		if (WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
		{
			if (nError != WSAENOBUFS)
				return NULL;
		}
		pProtoInfo = (LPWSAPROTOCOL_INFOW) new WSAPROTOCOL_INFOW[dwSize / sizeof(WSAPROTOCOL_INFOW)];
		if (!pProtoInfo)
			return NULL;
		ZeroMemory(pProtoInfo, dwSize);
		*lpnTotalProtocols = WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
		return pProtoInfo;
	}
	void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
	{
		delete[] pProtoInfo;
	}

private:
	GUID Layered_guid;
	GUID AgreementChain_guid;
};

class installLSP
{
public:
	installLSP()
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
		CoCreateGuid(&this->Layered_guid);
		CoCreateGuid(&this->AgreementChain_guid);
	}
	~installLSP()
	{
		WSACleanup();
	}
	BOOL InstallProvider(WCHAR *wszDllPath)
	{
		WCHAR wszLSPName[] = _T("AaronLSP");
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		int nProtocols = 0;
		WSAPROTOCOL_INFOW OriginalProtocolInfo[3];
		DWORD dwOrigCatalogId[3];
		int nArrayCount = 0;
		DWORD dwLayeredCatalogId;
		int nError;
		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return FALSE;
		BOOL bFindUdp = FALSE;
		BOOL bFindTcp = FALSE;
		BOOL bFindRaw = FALSE;
		for (int i = 0; i < nProtocols; i++)
		{
			if (pProtoInfo[i].iAddressFamily == AF_INET)
			{
				if (!bFindUdp && pProtoInfo[i].iProtocol == IPPROTO_UDP)
				{
					memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));

					OriginalProtocolInfo[nArrayCount].dwServiceFlags1 &= (~XP1_IFS_HANDLES);

					dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
					bFindUdp = TRUE;
				}
				if (!bFindTcp && pProtoInfo[i].iProtocol == IPPROTO_TCP)
				{
					memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));

					OriginalProtocolInfo[nArrayCount].dwServiceFlags1 &= (~XP1_IFS_HANDLES);

					dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
					bFindTcp = TRUE;
				}
				if (!bFindRaw && pProtoInfo[i].iProtocol == IPPROTO_IP)
				{
					memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));

					OriginalProtocolInfo[nArrayCount].dwServiceFlags1 &= (~XP1_IFS_HANDLES);

					dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
					bFindRaw = TRUE;
				}
			}
		}
		if (nArrayCount == 0)
		{
			FreeProvider(pProtoInfo);
			return FALSE;
		}

		WSAPROTOCOL_INFOW LayeredProtocolInfo;

		memcpy(&LayeredProtocolInfo, &OriginalProtocolInfo[0], sizeof(WSAPROTOCOL_INFOW));

		wcscpy(LayeredProtocolInfo.szProtocol, wszLSPName);

		LayeredProtocolInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;

		LayeredProtocolInfo.dwProviderFlags = PFL_HIDDEN;

		if (SOCKET_ERROR == WSCInstallProvider(&Layered_guid, wszDllPath, &LayeredProtocolInfo, 1, &nError))
		{
			FreeProvider(pProtoInfo);
			return FALSE;
		}
		FreeProvider(pProtoInfo);

		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return FALSE;
		for (int i = 0; i < nProtocols; i++)
		{
			if (memcmp(&pProtoInfo[i].ProviderId, &Layered_guid, sizeof(GUID)) == 0)
			{

				dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
				break;
			}
		}

		WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
		for (int i = 0; i < nArrayCount; i++)
		{
			swprintf(wszChainName, _T("%s over %s"), wszLSPName, OriginalProtocolInfo[i].szProtocol);
			wcscpy(OriginalProtocolInfo[i].szProtocol, wszChainName);
			if (OriginalProtocolInfo[i].ProtocolChain.ChainLen == 1)
			{
				OriginalProtocolInfo[i].ProtocolChain.ChainEntries[1] = dwOrigCatalogId[i];
			}
			else
			{
				for (int j = OriginalProtocolInfo[i].ProtocolChain.ChainLen; j > 0; j--)
					OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j] = OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j - 1];
			}

			OriginalProtocolInfo[i].ProtocolChain.ChainLen++;
			OriginalProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogId;
		}

		if (SOCKET_ERROR == WSCInstallProvider(&AgreementChain_guid, wszDllPath, OriginalProtocolInfo, nArrayCount, &nError))
		{
			FreeProvider(pProtoInfo);
			return FALSE;
		}

		FreeProvider(pProtoInfo);
		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return FALSE;
		DWORD dwIds[40];
		int nIndex = 0;

		for (int i = 0; i < nProtocols; i++)
		{
			if (pProtoInfo[i].ProtocolChain.ChainLen > 1 && pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId)
				dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}

		for (int i = 0; i < nProtocols; i++)
		{
			if (pProtoInfo[i].ProtocolChain.ChainLen <= 1 || pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId)
				dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
		}

		if (WSCWriteProviderOrder(dwIds, nIndex) != ERROR_SUCCESS)
			return FALSE;
		FreeProvider(pProtoInfo);
		return TRUE;
	}
	void RemoveProvider()
	{
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		int nProtocols = 0;
		DWORD dwLayeredCatalogId = 0;

		pProtoInfo = GetProvider(&nProtocols);
		if (nProtocols < 1 || pProtoInfo == NULL)
			return;
		int nError = 0;
		int i = 0;
		for (i = 0; i < nProtocols; i++)
		{
			if (memcmp(&Layered_guid, &pProtoInfo[i].ProviderId, sizeof(GUID)) == 0)
			{
				dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
				break;
			}
		}
		if (i < nProtocols)
		{
			for (i = 0; i < nProtocols; i++)
			{
				if (pProtoInfo[i].ProtocolChain.ChainLen > 1 && pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId)
				{
					WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &nError);
					break;
				}
			}
			WSCDeinstallProvider(&Layered_guid, &nError);
		}
	}

private:
	LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
	{
		int nError = 0;
		DWORD dwSize = 0;
		LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
		if (WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
		{
			if (nError != WSAENOBUFS)
				return NULL;
		}
		pProtoInfo = (LPWSAPROTOCOL_INFOW) new WSAPROTOCOL_INFOW[dwSize / sizeof(WSAPROTOCOL_INFOW)];
		if (!pProtoInfo)
			return NULL;
		ZeroMemory(pProtoInfo, dwSize);
		*lpnTotalProtocols = WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
		return pProtoInfo;
	}
	void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
	{
		delete[] pProtoInfo;
	}

private:
	GUID Layered_guid;
	GUID AgreementChain_guid;
};