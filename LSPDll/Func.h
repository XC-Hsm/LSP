#pragma once
#include "stdafx.h"
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
	*lpnTotalProtocols = WSAEnumProtocols(NULL, pProtoInfo, &dwSize);
	return pProtoInfo;
}
void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo, int i)
{
	if (i == 1)
		delete pProtoInfo;
	else
		delete[] pProtoInfo;
}