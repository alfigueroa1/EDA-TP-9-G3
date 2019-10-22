#pragma once
//1-a) Port Names
#define PORT_P0	0
#define PORT_P1	1
#define PORT_P2	2
#define PORT_P3	3
#define PORT_P4	4
#define PORT_P5	5
#define PORT_P6	6
#define PORT_P7	7
//1-b) Control Ports
#define LCD_EN	(1 << PORT_P0)
#define LCD_RS	(1 << PORT_P1)
#define LCD_D4	(1 << PORT_P4)
#define LCD_D5	(1 << PORT_P5)
#define LCD_D6	(1 << PORT_P6)
#define LCD_D7	(1 << PORT_P7)
//1-c) Masks
#define LCD_NYBBLE_H	0xF0
#define LCD_NYBBLE_L	0x0F
#define MASK_CTRL		(LCD_EN | LCD_RS)
#define NOT_MASK_CTRL	~(MASK_CTRL)
#define MASK_DATA		(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)
#define NOT_MASK_DATA	~(MASK_DATA)
//1-d) Port MACROS
//Hardware is ACTIVE HIGH
#define LCD_EN_ON	(LCD_EN)
#define LCD_EN_OFF	(LCD_EN ^ LCD_EN)
#define LCD_RS_ON	(LCD_RS)
#define LCD_RS_OFF	(LCD_RS ^ LCD_RS)
#define LCD_D4_ON	(LCD_D4)
#define LCD_D4_OFF	(LCD_D4 ^ LCD_D4)
#define LCD_D5_ON	(LCD_D5)
#define LCD_D5_OFF	(LCD_D5 ^ LCD_D5)
#define LCD_D6_ON	(LCD_D6)
#define LCD_D6_OFF	(LCD_D6 ^ LCD_D6)
#define LCD_D7_ON	(LCD_D7)
#define LCD_D7_OFF	(LCD_D7 ^ LCD_D7)

//2) LCD Instructions
#define LCD_CLEAR	0x01
#define LCD_HOME	0x02
//LCD ENTRY MODE SET
#define	LCD_ENTRY_MODE_SET	0x04
#define LCD_SHIFT			0x01
#define LCD_CURSOR_R		0x03
#define LCD_CURSOR_L		0x02
//LCD DISPLAY CONTROL
#define LCD_DISPLAY_CTRL	0x08
#define LCD_DISPLAY_ON		0x04
#define LCD_DISPLAY_OFF		0x00
#define LCD_CURSOR_ON		0x02
#define LCD_CURSOR_OFF		0x00
#define LCD_BLINK_ON		0x01
#define LCD_BLINK_OFF		0x00
//CURSOR OR DISPLAY SHIFT
#define LCD_SHIFT_CURSOR_L	0x10
#define LCD_SHIFT_CURSOR_R	0x14
#define LCD_SHIFT_DISPLAY_L	0x18
#define LCD_SHIFT_DISPLAY_R	0x1C
//FUNCTION SET
#define LCD_FUNCTION_SET	0x20
#define LCD_DL_8_BIT		0x10
#define LCD_DL_4_BIT		0x00
#define LCD_N_2_LINES		0x08
#define LCD_N_1_LINE		0x00
#define LCD_FONT_5X10		0x04
#define LCD_FONT_5X8		0x00
//SET CGRAM ADDRESS
#define LCD_SET_CGRAM		0x4O
//SET DDRAM ADDRESS
#define LCD_SET_DDRAM		0x80
//READ BUSY FLAG
#define LCD_READ_BUSY_FLAG	0x0100
