#include "lcd.h"

unsigned char seed = 0;
unsigned char state = 0;


void startLcd() {
	unsigned char ScreenBuffer[8][128];
	InitScreenBuffer(ScreenBuffer);

	GLCD_Line(ScreenBuffer, 0, 38, 63, 38);
	GLCD_Line(ScreenBuffer, 58, 38, 58, 127);
	display_string(0, 0, "Score");
	display_string(1, 0, "000");
	display_string(3, 0, "Speed");
	display_string(4, 0, "000");
	display_string(0, 7, "1 - START");
	display_string(1, 7, "2 - RESET");
	display_string(2, 7, "3 - LANE 1");
	display_string(3, 7, "4 - LANE 2");
	display_string(4, 7, "5 - LANE 3");
	display_string(5, 7, "6 - LANE 4");
}

int main(void)
{
	//Initialization
	DDRA = 0xFF;
	PORTB = 0xFF; DDRB = 0xFF;
	DDRD = 0x0;
	DDRE = 0xFF;
	PORTG = 0x0; DDRG = 0xFF;

	cmd_lr( 0x3F ); //DISP ON
	cmd_lr( 0xB8 );
	cmd_lr( 0x40 );
		
	lcd_clear();
	startLcd();
		
	TCNT1H = 0xB;
	TCNT1L = 0xDC;
	TCCR1A = 0x0;
	TCCR1B = 0x04;
		
	TIMSK = (1<<TOIE1);
	sei();

	while(1) {
	}
}

ISR (TIMER1_OVF_vect) {
	seed++;
	if(seed == 10) {
		seed = 0;
	}
}

/*
ISR (INT0_vect) {
	if (state == 0) {
		state = 1;
	}
	else {
		state = 0;
	}
}
*/