#pragma once

#include <Windows.h>
#include "udpType.h"

DWORD WINAPI UdpThread(LPVOID lpParam);

void DespatchCmd(void* buf, LPVOID lpParam);
