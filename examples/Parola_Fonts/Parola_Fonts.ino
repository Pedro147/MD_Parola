// Program to exercise the MD_Parola library

#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#include "Parola_Fonts_data.h"

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may 
// need to be adapted
#define	MAX_DEVICES	10

#define	CLK_PIN		13
#define	DATA_PIN	11
#define	CS_PIN		10

// HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// GENERIC SPI
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define	PAUSE_TIME		1000

// Turn on debug statements to the serial output
#define  DEBUG  0

#if  DEBUG
#define	PRINT(s, x)	{ Serial.print(F(s)); Serial.print(x); }
#define	PRINTS(x)	Serial.print(F(x))
#define	PRINTX(x)	Serial.println(x, HEX)
#else
#define	PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

// Global variables
typedef struct 
{
	uint8_t PROGMEM *	pFont;
	char *				pMsg;
} Message_t;

Message_t	M[] = 
{ 
	{ NULL, "Arduino Uno" }, 
	{ fontKatakana, "\x0b1\x0b0\x0c2\x0b2\x0c9 \x0a5 \x0b3\x0c9" },
	{ fontArabic, "\x0b1\x0b0\x0c2\x0b2\x0c9 \x0b3\x0c9" },		// this is nonsense as I don't do arabic!
	{ fontGreek, "\x080\x0a8\x0a4\x0ab\x0a6\x0ac\x0a0\x0a4\x0a6 \x08e\x0ac\x0a4\x0a6" }
};
#define	MAX_MESG  (sizeof(M)/sizeof(M[0]))

uint8_t	curM = 0;		// current message definition to use
	
void setup(void)
{
  Serial.begin(57600);
  PRINTS("\n[Parola Demo]");

  P.begin();
  P.setFont(M[curM].pFont);
  P.displayText(M[curM].pMsg, MD_Parola::CENTER, P.getSpeed(), PAUSE_TIME, MD_Parola::SCROLL_LEFT, MD_Parola::SCROLL_LEFT);
}

void loop(void)
{
  if (P.displayAnimate()) 
  {
	curM = (curM + 1) % MAX_MESG;
	P.setFont(M[curM].pFont);
    P.setTextBuffer(M[curM].pMsg);

	PRINT("\nChanging font to ", curM);
	PRINTS("\n");
	for (uint8_t i=0; i<strlen(M[curM].pMsg); i++)
	{
		PRINT(" ", (uint8_t) M[curM].pMsg[i]);
	}
    P.displayReset();
  }
}
