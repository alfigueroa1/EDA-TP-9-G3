//#pragma once
//
//#include "basicLCD.h"
//
////estructura del display Hitachi HD44780.
//
//class hitachiLCD : public basicLCD
//{
//public:
//	hitachiLCD();
//	virtual ~hitachiLCD();
//	virtual bool lcdInitOk();
//	virtual FT_STATUS lcdGetError();
//	virtual bool lcdClear();
//	virtual bool lcdClearToEOL();
//	virtual basicLCD& operator<<(const unsigned char c);
//	virtual basicLCD& operator<<(const unsigned char* c);
//	virtual bool lcdMoveCursorUp();
//	virtual bool lcdMoveCursorDown();
//	virtual bool lcdMoveCursorRight();
//	virtual bool lcdMoveCursorLeft();
//	virtual bool lcdSetCursorPosition(const cursorPosition pos);
//	virtual cursorPosition lcdGetCursorPosition();
//
//private:
//	bool canInit;
//	FT_HANDLE * device_handler;
//	virtual void lcdUpdateCursor();
//};