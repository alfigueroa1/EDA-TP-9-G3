//#include <iostream>
//#include "hitachiLCD.h"
//#include "lcd.h"
//#include "lcdInit.h"
//#include "lcdWriteIR.h"
//
//#define START_FIRST_LINE 1
//#define END_FIRST_LINE 16
//#define START_SECOND_LINE 65
//#define END_SECOND_LINE 80
//#define COL_MIN 1
//#define COL_MAX 16
//#define GAP_LINE (START_SECOND_LINE-END_FIRST_LINE)
//#define ON_FIRST_LINE(x) (((x >= START_FIRST_LINE) && (x <= END_FIRST_LINE)))
//#define ON_SECOND_LINE(x) (((x >= START_SECOND_LINE) && (x <= END_SECOND_LINE)))
//
//hitachiLCD::hitachiLCD()
//{
//	this->canInit = false;
//	this->error = true;
//	device_handler = lcdInit(44780); //HD44780 (????)
//	if (device_handler != nullptr)
//	{
//		this->canInit = true;
//		this->error = false;
//	}
//	cadd = START_FIRST_LINE;
//}
//
//hitachiLCD:: ~hitachiLCD()
//{
//	if(this->canInit == true)
//	{
//		FT_Close(*device_handler);
//		delete device_handler;
//	}
//}
//
//bool hitachiLCD::lcdInitOk()
//{
//	return this->canInit;
//}
//
//FT_STATUS hitachiLCD::lcdGetError()
//{
//	FT_STATUS state = FT_GetStatus(*device_handler, nullptr, nullptr, nullptr);
//	if (state != FT_OK)
//		this->error = true;
//	return state;
//}
//
//bool hitachiLCD::lcdClear()
//{
//	bool canClear = false;
//	FT_STATUS state = FT_OK;
//	lcdWriteIR(device_handler, LCD_CLEAR);
//	if (state == FT_OK)
//	{
//		this->cadd = START_FIRST_LINE;
//		canClear = true;
//	}
//	else
//		this->error = true;
//	return canClear;
//}
//
//bool hitachiLCD::lcdClearToEOL()
//{
//	bool canClearToEOL = false;
//	int caddaux = cadd;
//	int lim = 0;
//	if (ON_FIRST_LINE(cadd))
//		lim = END_FIRST_LINE;
//	else if (ON_SECOND_LINE(cadd))
//		lim = END_SECOND_LINE;
//	else
//	{
//		std::cout << "Cursor Overflow!" << std::endl;
//		this->error = true;
//	}
//
//	if (this->error == false)
//	{
//		canClearToEOL = true;
//
//		while (cadd <= lim)
//		{
//			lcdWriteDR(device_handler, ' ');
//			cadd++;
//		}
//		cadd = caddaux;
//		lcdUpdateCursor();
//	}
//	return canClearToEOL;
//}
//
//basicLCD& hitachiLCD::operator<<(const unsigned char c)
//{
//	lcdWriteDR((this->device_handler), c);
//	cadd++;
//
//	if (cadd == (END_FIRST_LINE + 1))
//		cadd = START_SECOND_LINE;
//	if (cadd == (END_SECOND_LINE + 1))
//		cadd = START_FIRST_LINE;
//	lcdUpdateCursor();
//	return *this;
//}
//
//basicLCD& hitachiLCD::operator<<(const unsigned char* c)
//{
//	for (int i = 0; c[i] != '\0'; i++)
//	{
//		lcdWriteDR((this->device_handler), c[i]);
//		cadd++;
//		if (cadd == (END_FIRST_LINE + 1))
//			cadd = START_SECOND_LINE;
//		if (cadd == (END_SECOND_LINE + 1))
//			cadd = START_FIRST_LINE;
//		lcdUpdateCursor();
//	}
//	return *this;
//}
//
//bool hitachiLCD::lcdMoveCursorUp()
//{
//	if (ON_FIRST_LINE(cadd))
//		return false;
//	else if (ON_SECOND_LINE(cadd))
//	{
//		cursorPosition pos;
//		pos.row = 1;
//		pos.column = cadd - (GAP_LINE + COL_MAX) + 1;
//		lcdSetCursorPosition(pos);
//		return true;
//	}
//	else
//		return false;
//}
//
//bool hitachiLCD::lcdMoveCursorDown()
//{
//	if (ON_FIRST_LINE(cadd))
//	{
//		cursorPosition pos;
//		pos.row = 2;
//		pos.column = cadd - 1;
//		lcdSetCursorPosition(pos);
//		return true;
//	}
//	else if (ON_SECOND_LINE(cadd))
//		return false;
//	else
//		return false;
//}
//
//bool hitachiLCD::lcdMoveCursorRight()
//{
//	bool retVal = true;
//	cadd++;
//
//	if (cadd == (END_FIRST_LINE + 1))
//		cadd = START_SECOND_LINE;
//	if (cadd == (END_SECOND_LINE + 1))
//	{
//		cadd = END_SECOND_LINE;
//		retVal = false;
//	}
//	lcdUpdateCursor();
//	return retVal;
//}
//bool hitachiLCD::lcdMoveCursorLeft()
//{
//	bool retVal = true;
//	if (cadd == START_FIRST_LINE)
//		retVal = false;
//	else if (cadd == START_SECOND_LINE)
//		cadd = END_FIRST_LINE;
//	else
//		cadd--;
//	lcdUpdateCursor();
//	return retVal;
//}
//
//bool hitachiLCD::lcdSetCursorPosition(const cursorPosition pos)
//{
//	bool retVal = true;
//	switch (pos.row)
//	{
//		case 1:
//			cadd = pos.column;
//		break;
//		case 2:
//			cadd = pos.column + (GAP_LINE + COL_MAX);
//		break;
//		default:
//			retVal = false;
//	}
//
//	if (pos.column > COL_MAX || pos.column < COL_MIN)
//		retVal = false;
//	lcdUpdateCursor();
//	return retVal;
//}
//
//cursorPosition hitachiLCD::lcdGetCursorPosition()
//{
//	cursorPosition pos;
//
//	if (ON_FIRST_LINE(cadd))
//	{
//		pos.row = 1;
//		pos.column = cadd;
//	}
//	else if (ON_SECOND_LINE(cadd))
//	{
//		pos.row = 2;
//		pos.column = cadd - (GAP_LINE + COL_MAX);
//	}
//	else
//	{
//		pos.row = -1;
//		pos.column = -1;
//	}
//	return pos;
//}
//
//void hitachiLCD::lcdUpdateCursor()
//{
//	char caddaux = 0;
//	
//	if (ON_FIRST_LINE(cadd) || ON_SECOND_LINE(cadd))
//		caddaux = ((char)cadd) - 1;
//	else
//		caddaux = (char)0xFF;
//	lcdWriteIR(device_handler, LCD_SET_DDRAM | caddaux);
//}