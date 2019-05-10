/*
 * AlexWProject2.c
 *
 * Created: 5/2/2019 10:26:23 PM
 * Author : warho
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect);
ISR(TIMER0_COMPA_vect);

void PlayNote(unsigned char invFrequencySet, int setNumCycles);
void rest(int setNumCycles);

void keyBoardNote(unsigned char invFrequencySet, int setNumCycles);

void OcarinaMode();

int eponasSong(char notes[]);
int songOfStorms(char notes[]);

long numCycles;
long maxCycles;
int notePlaying;

int t0Used;
unsigned char currInvFrequency;


int main(void)
{
    DDRC = 0xFF;
	DDRD = 0xFF;
	DDRE = 0xFF;
	
	DDRA = 0x00;
	PORTA = 0xFF;
	
	PORTE = 0x20;
	t0Used = 0;
    while (1) 
    {
		PORTD = PINA;
		if (PINA == 0xFE)
		{
			PlayNote(120, 10);
		}
		if (PINA == 0xFD)
		{
			PlayNote(80, 10);
		}
		if (PINA == 0xFB)
		{
			PlayNote(71, 10);
		}
		if (PINA == 0xF7)
		{
			PlayNote(120, 1000); //Low C, quarter
			rest(100);
			PlayNote(120, 1000);
			rest(100);
			PlayNote(80, 1500); //G, quarter
			rest(100);
			PlayNote(80, 1500);
			rest(100);
			PlayNote(72, 1667); //A, quarter
			rest(100);
			PlayNote(72, 1667);
			rest(100);
			PlayNote(80, 3000); //G, half
		}
		if (PINA == 0xEF)
		{
			PlayNote(71, 563);  //A, triplet
			PlayNote(67, 597);	//B flat, triplet
			PlayNote(63, 635);	//B, triplet
			PlayNote(60, 2000);	//C, half note
		}
		if (PINA == 0xDF)
		{
			if (!t0Used)
			{
				notePlaying = 1;
			
				OCR0A = 80;
				TCNT0 = 0;
				TCCR0A = 0xC0;
				TCCR0B = 0x03;
				TIMSK0 = 0x02;
			
				sei();
				t0Used = 1;
			}
		}
		else
		{
			cli();
			
			TCCR0A = 0x00;
			TCCR0B = 0x00;
			TIMSK0 = 0x00;
			t0Used = 0;
		}
		if (PINA == 0x7F)
		{
			while (PINA == 0x7F) {PORTD = PINA;}
			OcarinaMode();
		}
    }
}

void PlayNote(unsigned char invFrequency, int setNumCycles)
{
	notePlaying = 1;
	currInvFrequency = invFrequency;
	
	TCNT0 = -currInvFrequency;
	numCycles = 0;
	maxCycles = setNumCycles;
	TCCR0A = 0x00;
	TCCR0B = 0x03;
	TIMSK0 = 0x01;
	
	sei();
	while(numCycles < maxCycles) {PORTD = PINA;};
	cli();
	
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TIMSK0 = 0x00;
	return;
}

void rest(int setNumCycles)
{
	notePlaying = 0;
	currInvFrequency = 100;
	
	TCNT0 = -currInvFrequency;
	numCycles = 0;
	maxCycles = setNumCycles;
	TCCR0A = 0x00;
	TCCR0B = 0x03;
	TIMSK0 = 0x01;
	
	sei();
	while(numCycles < maxCycles) {PORTD = PINA;};
	cli();
	
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TIMSK0 = 0x00;
	return;
}

void OcarinaMode()
{
	char notes[6] = {0};
	while(1)
	{
		PORTD = PINA;
		if (PINA == 0xFE)
		{
			for (int i = 4; i >= 0; i--)
			{
				notes[i+1] = notes[i];
			}
			notes[0] = 1;
			while(PINA == 0xFE)
			{
				PlayNote(106,10);	//low D
				PORTD = PINA;
			}
		}
		if (PINA == 0xFD)
		{
			for (int i = 4; i >= 0; i--)
			{
				notes[i+1] = notes[i];
			}
			notes[0] = 2;
			while(PINA == 0xFD)
			{
				PlayNote(90,10);	//F
				PORTD = PINA;
			}
		}
		if (PINA == 0xFB)
		{
			for (int i = 4; i >= 0; i--)
			{
				notes[i+1] = notes[i];
			}
			notes[0] = 3;
			while(PINA == 0xFB)
			{
				PlayNote(71,10);	//A
				PORTD = PINA;
			}
		}
		if (PINA == 0xF7)
		{
			for (int i = 4; i >= 0; i--)
			{
				notes[i+1] = notes[i];
			}
			notes[0] = 4;
			while(PINA == 0xF7)
			{
				PlayNote(63,10);	//B
				PORTD = PINA;
			}
		}
		if (PINA == 0xEF)
		{
			for (int i = 4; i >= 0; i--)
			{
				notes[i+1] = notes[i];
			}
			notes[0] = 5;
			while(PINA == 0xEF)
			{
				PlayNote(53,10);	//high D
				PORTD = PINA;
			}
		}
		if (PINA == 0x7F)
		{
			while(PINA == 0x7f) {PORTD = PINA;}
			return;
		}
		if (eponasSong(notes))
		{
			rest(1200);
			PlayNote(71, 563);
			PlayNote(67, 597);
			PlayNote(63, 635);
			PlayNote(60, 2000);
			
			rest(2400);
			
			PlayNote(53, 1132);	//high D, eighth
			PlayNote(63, 952);	//B, eighth
			PlayNote(71, 3380);	//A, half
			PlayNote(53, 1132);
			PlayNote(63, 952);
			PlayNote(71, 3380);
			
			PlayNote(53, 1132);
			PlayNote(63, 952);
			PlayNote(71, 1690);	//A, quarter
			PlayNote(63, 1905);	//B, quarter
			PlayNote(71, 5070);	//A, dotted half
			notes[0] = 0;
		}
		if (songOfStorms(notes))
		{
			rest(1200);
			PlayNote(71, 563);
			PlayNote(67, 597);
			PlayNote(63, 635);
			PlayNote(60, 2000);
			
			rest(2400);
			
			PlayNote(106, 377);	//low D, eighth
			PlayNote(90, 444);	//F, eighth
			PlayNote(53, 3019);	//high D, half
			PlayNote(106, 377);
			PlayNote(90, 444);
			PlayNote(53, 3019);
			
			PlayNote(47, 1277);	//high E, quarter, shortened
			rest(600);	//eighth rest, lenghtened to account for shortened not above
			PlayNote(45, 889);	//high F, eighth
			PlayNote(47, 851);	//high E, eighth
			PlayNote(45, 889);
			PlayNote(47, 851);
			PlayNote(60, 667);	//high C, eighth
			PlayNote(71, 2254);	//A, half
			notes[0] = 0;
		}
	}
}

int eponasSong(char notes[])
{
	if (3 == notes[0] &&
		4 == notes[1] &&
		5 == notes[2] &&
		3 == notes[3] &&
		4 == notes[4] &&
		5 == notes[5])
	{
		return 1;
	}
	return 0;
}

int songOfStorms(char notes[])
{
	if (5 == notes[0] &&
		2 == notes[1] &&
		1 == notes[2] &&
		5 == notes[3] &&
		2 == notes[4] &&
		1 == notes[5])
	{
		return 1;
	}
	return 0;
}

ISR(TIMER0_OVF_vect)
{
	numCycles++;
	if (notePlaying)
	{
		PORTE ^= 0x10;
	}
	if (numCycles < maxCycles)
	{
		TCNT0 = -currInvFrequency;
	}
}

ISR(TIMER0_COMPA_vect)
{
	TCNT0 = 0;
	if (notePlaying)
	{
		PORTE ^= 0x10;
	}
}