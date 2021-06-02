#include "lcd.h"

#define READY 0
#define PLAY 1
#define GAMEOVER 2

unsigned char seed = 0;
unsigned char state = READY;
unsigned char score = 0, speed = 0;
unsigned char ScreenBuffer[8][128];

void startLcd();
void drawScore();
void drawSpeed();
void drawNote(unsigned char[4], unsigned char);

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
	
	TCNT1H = 0xB;
	TCNT1L = 0xDC;
	TCCR1A = 0x0;
	TCCR1B = 0x01;

	EICRA = 0x66;
	EICRB = 0x66;
	EIMSK = 0x03;
	sei();

	InitScreenBuffer(ScreenBuffer);

	while (1) {
		if (state == READY) {
			TIMSK = (1<<TOIE1);
			lcd_clear();
			startLcd();
			
			while (state == READY) {
				_delay_us(1);
			}
		}
		else if (state == PLAY) {
			unsigned char timeSet = 0;
			srand(seed);
			
			unsigned char NoteBuffer[4];
			InitNoteBuffer(NoteBuffer);
			
			while (state == PLAY) {
				if (timeSet == 0) {
					for (char i = 3; i > 0; --i) {
						NoteBuffer[i] = NoteBuffer[i - 1];
					}
					
					NoteBuffer[0] = rand() % 4 + 1;
				}
				
				drawNote(NoteBuffer, timeSet);
				
				++speed;
				++timeSet;
				timeSet %= 16;
			}
		}
		else {
			while (state == GAMEOVER) {
				_delay_us(1);
				// fill in
			}
		}
	}
}

void startLcd() {
	GLCD_Line(ScreenBuffer, 0, 38, 63, 38);
	GLCD_Line(ScreenBuffer, 58, 39, 58, 127);
	
	display_string(0, 0, "Score");
	drawScore();
	
	display_string(3, 0, "Speed");
	drawSpeed();
	
	if (state == READY) {
		display_string(0, 7, "1 - START");
		display_string(1, 7, "2 - RESET");
		display_string(2, 7, "3 - LANE 1");
		display_string(3, 7, "4 - LANE 2");
		display_string(4, 7, "5 - LANE 3");
		display_string(5, 7, "6 - LANE 4");
	}
}

void drawScore() {
	display_char(1, 0, score / 100 + 0x30);
	display_char(1, 1, score % 100 / 10 + 0x30);
	display_char(1, 2, score % 10 + 0x30);
}

void drawSpeed() {
	display_char(4, 0, speed / 100 + 0x30);
	display_char(4, 1, speed % 100 / 10 + 0x30);
	display_char(4, 2, speed % 10 + 0x30);
}

void drawNote(unsigned char NoteBuffer[4], unsigned char timeSet) {
	drawScore();
	drawSpeed();

	for (char i = 0; i < 4; ++i) {
		if (NoteBuffer[i] != 0) {
				GLCD_RectangleBlack(ScreenBuffer, timeSet + 16 * i, (NoteBuffer[i] - 1) * 22 + 39, timeSet + 16 * i + 4, NoteBuffer[i] * 22 + 39);
		}
	}
}

ISR (TIMER1_OVF_vect) {
	++seed;
}

// START
ISR (INT0_vect) {
	if (state == READY) {
		state = PLAY;
	}
	
	TIMSK = (0<<TOIE1);
}

// RESET
ISR (INT1_vect) {
	if (state == GAMEOVER) {
		state = READY;
	}
}