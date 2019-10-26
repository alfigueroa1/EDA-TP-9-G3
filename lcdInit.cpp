#include <cstdio>
#include <iostream>
#include <windows.h>
#include <chrono>
#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "lcd.h"
#include "lcdInit.h"
#include "lcdWriteIR.h"
#include "hitachiLCD.h"

FT_HANDLE* hitachiLCD::lcdInit(int iDevice) {

	FT_STATUS status = !FT_OK;
	FT_HANDLE* device_handler = new FT_HANDLE();

	unsigned char info = 0x00;
	DWORD sizeSent = 0;

	std::chrono::seconds MaxTime(CONNECTING_TIME);/*The display has a settling time after the physical connection so the attempt to connect
	will be done for a few seconds*/

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> current = start;

	while (status != FT_OK && ((current - start) < MaxTime))//loop till succesful connection o max connecting time is exceeded
	{
		status = FT_OpenEx((void*)MY_LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, device_handler);

		if (status == FT_OK)
		{
			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			if (FT_SetBitMode(*device_handler, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
			{

				//Example to write 0xf0 to the display 

				//(E=0, RS=0, D4-D7=f)...
				info = 0xf0;

				//Finally executes the action "write to LCD"...
				if (FT_Write(*device_handler, &info, 1, &sizeSent) == FT_OK)
				{
					//If success continue with the program (...)
					//std::cout << "Succesful initiation in LCD\n" << std::endl;
					/*printf("Succesful initiation in LCD\n");
					fflush(stdout);*/
					sendNybble(*device_handler, sizeSent, (LCD_RS_OFF) | (LCD_FUNCTION_SET) | (LCD_DL_8_BIT));
					Sleep(5);

					sendNybble(*device_handler, sizeSent, (LCD_RS_OFF) | (LCD_FUNCTION_SET) | (LCD_DL_8_BIT));
					Sleep(1);

					sendNybble(*device_handler, sizeSent, (LCD_RS_OFF) | (LCD_FUNCTION_SET) | (LCD_DL_8_BIT));
					Sleep(1);

					sendNybble(*device_handler, sizeSent, (LCD_RS_OFF) | (LCD_FUNCTION_SET) | (LCD_DL_4_BIT));
					Sleep(1);

					info = ((LCD_FUNCTION_SET) | (LCD_DL_4_BIT) | (LCD_N_2_LINES) | (LCD_FONT_5X8));
					lcdWriteIR(device_handler, info);
					Sleep(1);

					lcdWriteIR(device_handler, LCD_DISPLAY_CTRL);
					Sleep(1);

					lcdWriteIR(device_handler, LCD_CLEAR);
					Sleep(10);

					lcdWriteIR(device_handler, LCD_ENTRY_MODE_SET);
					Sleep(1);

					lcdWriteIR(device_handler, LCD_DISPLAY_CTRL | LCD_CURSOR_ON | LCD_DISPLAY_ON);
					Sleep(1);

					lcdWriteIR(device_handler, (LCD_ENTRY_MODE_SET) | (LCD_CURSOR_R));
					Sleep(1);
				}
				else
					printf("Error initiating the LCD\n");
			}
			else
				printf("Couldn't configure LCD\n");

			//FT_Close(*device_handler);
		}
		current = std::chrono::system_clock::now();
	}

	if (status != FT_OK)
		printf("Couldn't open LCD\n");

	return device_handler;
}
