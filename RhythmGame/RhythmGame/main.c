#include "lcd.h"

unsigned char sec0, sec1;

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
	display_string(0, 0, "Clock Interrupt Test");
		
	TCNT1H = 0xB;
	TCNT1L = 0xDC;
	TCCR1A = 0x0;
	TCCR1B = 0x04;
		
	TIMSK = (1<<TOIE1);
	sei();
		

	sec1 = 0;
	sec0 = 0;

	while(1) {
		display_char(1, 3, sec1+0x30);
		display_char(1, 4, sec0+0x30);
	}
}

ISR(TIMER1_OVF_vect){
	sec0++;
	if(sec0 == 10){
		sec0 = 0;
		sec1++;
		if(sec1 == 6){
			sec1 = 0;
		}
	}
}