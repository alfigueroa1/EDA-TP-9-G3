#include <cstdio>
#include <iostream>
#include <windows.h>
#include "ftd2xx.h"
#include "lcd.h"
#include "lcdWriteIR.h"


void lcdWriteIR(FT_HANDLE* deviceHandler, BYTE valor) {
	sendNybble(*deviceHandler, 1, (valor & LCD_NYBBLE_H) & (~LCD_RS_ON));
	Sleep(1);
	sendNybble(*deviceHandler, 1, ((valor & LCD_NYBBLE_L) << 4) & (~LCD_RS_ON));

	return;
}

void lcdWriteDR(FT_HANDLE* deviceHandler, BYTE valor) {
	sendNybble(*deviceHandler, 1, (valor & LCD_NYBBLE_H) | LCD_RS_ON);
	Sleep(1);
	sendNybble(*deviceHandler, 1, ((valor & LCD_NYBBLE_L) << 4) | LCD_RS_ON);

	return;
}

void sendNybble(FT_HANDLE lcdHandle, DWORD sizeSent, unsigned char byte) {
	unsigned char info;
	info = (~LCD_EN_ON) & byte;
	FT_Write(lcdHandle, &info, 1, &sizeSent);
	info = (LCD_EN_ON) | byte;
	FT_Write(lcdHandle, &info, 1, &sizeSent);
	Sleep(1);
	info = (~LCD_EN_ON) & byte;
	FT_Write(lcdHandle, &info, 1, &sizeSent);

	return;
}