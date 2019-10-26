#pragma once
#define FTD2XX_EXPORTS

#include <windows.h>
#include "ftd2xx.h"
#include "lcd.h"


#define MY_LCD_DESCRIPTION "EDA LCD 3 B" // CAMBIAR LA DESCRIPCION SEGUN EL GRUPO

#define CONNECTING_TIME 5 //in seconds

FT_HANDLE* lcdInit(int iDevice);		//Inicializa la escritura

