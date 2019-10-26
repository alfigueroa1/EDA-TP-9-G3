#pragma once
#define FTD2XX_EXPORTS
#include <windows.h>
#include "ftd2xx.h"
#include "lcd.h"

void lcdWriteIR(FT_HANDLE* deviceHandler, BYTE valor);
void lcdWriteDR(FT_HANDLE* deviceHandler, BYTE valor);
void sendNybble(FT_HANDLE lcdHandle, DWORD sizeSent, unsigned char byte);		//Env�a un byte al bus de datos
