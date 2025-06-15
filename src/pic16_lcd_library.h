#include <xc.h>
#include <stdint.h>

#define LCD_DATA        D   //Port RD0-RD3 are connected to D4-D7
#define LCD_DATA_POS    0

#define LCD_RS_PORT     B
#define LCD_RS_POS      3

#define LCD_RW_PORT     B
#define LCD_RW_POS      2

#define LCD_E_PORT      D
#define LCD_E_POS       4

#define LCD_TYPE_162	//For 16 Chars by 2 lines

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010
#define LS_NONE	 0B00000000

void LCDInit(uint8_t style);
void LCDWriteString(const char *msg);
void LCDWriteInt(int val,int8_t field_length);
void LCDGotoXY(uint8_t x,uint8_t y);

void LCDByte(uint8_t,uint8_t);
#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

void LCDBusyLoop();

#define LCDClear() LCDCmd(0b00000001)
#define LCDHome()  LCDCmd(0b00000010)

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
 LCDGotoXY(x,y);\
 LCDWriteInt(val,fl);\
}

#define _CONCAT(a,b) a##b
#define _CONCAT2(port,pos)  TRIS##port##bits.TRIS##port##pos
#define _CONCAT3(port,pos)  R##port##pos
#define PORT(x) _CONCAT(PORT,x)
#define TRIS(x) _CONCAT(TRIS,x)
#define TRISBIT(__port,__pos) _CONCAT2(__port,__pos)
#define PORTBIT(__port,__pos) _CONCAT3(__port,__pos)

unsigned char __cgram[]=
{
	0x0C, 0x12, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, //Char0 dergee symbol
	0x00, 0x04, 0x0E, 0x1F, 0x0E, 0x04, 0x00, 0x00, //Char1
	0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04, 0x00, //Char2
	0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00, 0x00, //Char3
	0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, //Char4
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char5
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char6
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char7
};