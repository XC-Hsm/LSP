#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#include <WinSock2.h>
#include <WS2spi.h>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")
