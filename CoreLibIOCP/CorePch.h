#pragma once
#include "Types.h"
#include "CoreGlobal.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <assert.h>
#pragma comment(lib, "ws2_32.lib")

void CrashFunc(bool result);
