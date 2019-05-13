/*
 * AlexWProject2.c
 *
 * Created: 5/2/2019 10:26:23 PM
 * Author : warho
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU 16000000UL                    // fosc = 16 MHz

ISR(TIMER0_OVF_vect);
ISR(TIMER0_COMPA_vect);
ISR(USART1_RX_vect);
ISR(USART1_UDRE_vect);
void USART_Init(unsigned long);
char USART_RxChar();
void USART_TxChar(char);
void USART_SendString(char*);


void PlayNote(unsigned char invFrequencySet, int setNumCycles);
void rest(int setNumCycles);

void keyBoardNote(unsigned char invFrequencySet, int setNumCycles);

void OcarinaMode();
void KeyBoardGameMode();
void ShufflePlay();

int eponasSong(char notes[]);
int songOfStorms(char notes[]);

void PlaySong1();
void PlaySong2();
void PlayEponasSong()
void PlaySongOfStorms();

long numCycles;
long maxCycles;
int notePlaying;

int t0Used;
unsigned char currInvFrequency;
unsigned char value;

char song1[9] = "ABCAABCA";
char song2[7] = "ECDECD";
char song3[8] = "EDCDEEE";
char selectedSong[10];


int main(void)
{
    DDRC = 0xFF;
	DDRD = 0xFB;
	DDRE = 0x20;
	PORTE = 0xDF;
	
	DDRA = 0x00;
	PORTA = 0xFF;
	
	PORTE = 0x20;
	USART_Init(9600);
	t0Used = 0;
	sei();
    while (1) 
    {
		value = USART_RxChar(); //have to monitor what the value of this is, and add some if statements to chage what mode its in baised off of this
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
			PlaySong1();
		}
		if (PINA == 0xEF)
		{
			PlaySong2();
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
			KeyBoardGameMode();
			return;
		}
		if (eponasSong(notes))
		{
			PlayEponasSong();
			notes[0] = 0;
		}
		if (songOfStorms(notes))
		{
			PlaySongOfStorms();
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
void selsong()
{
	 int x=rand();
	 if (x%3==1)
	 {
		 memcpy(selectedSong, song1, sizeof(song1));
		 USART_SendString(selectedSong);
	 }
	 else if (x%3==2)
	 {
		 memcpy(selectedSong, song2, sizeof(song2));
		 USART_SendString(selectedSong);
	 }
	 else
	 {
		 memcpy(selectedSong, song3, sizeof(song3));
		 USART_SendString(selectedSong);
	 }
	 return;
}
void KeyBoardGameMode()
{
 selsong();
 char currentnotes[10];
 int count =0;
 while (1)
 {
	 value = USART_RxChar();
	 PORTD = PINA;
	 if (PINA == 0xFE)
	 {
		 PlayNote(120, 10); //C
		 currentnotes[count]='C';
		 currentnotes[count+1]='\0';
		 count++;
		 
	 }
	 if (PINA == 0xFD)
	 {
		 PlayNote(63, 10); //B
		 currentnotes[count]='C';
		 currentnotes[count+1]='\0';
		 count++;
		 
	 }
	 if (PINA == 0xFB)
	 {
		 PlayNote(71, 10); //A
		 currentnotes[count]='C';
		 currentnotes[count+1]='\0';
		 count++;
		 
	 }
	 if (PINA == 0xF7)
	 {
		 PlayNote(71, 10); //G
		 currentnotes[count]='C';
		 currentnotes[count+1]='\0';
		 count++;
		 
	 }
	 if (PINA == 0xEF)
	 {
		 PlayNote(71, 10); //F
		 currentnotes[count]='C';
		 currentnotes[count+1]='\0';
		 count++;
		 
	 }
	 if (PINA == 0xDF)
	 {
		PlayNote(71, 10); //E
		currentnotes[count]='C';
		currentnotes[count+1]='\0';
		count++;
		
	 }
	 if (PINA == 0xBF) //Alex: I am assuming that this was supposed to be a BF so that the button would constantly increase
	 {
		PlayNote(71, 10); //D
		currentnotes[count]='C';
		currentnotes[count+1]='\0';
		count++;
		
	 }
	 if(PINA == 0x7F)
	 {
		 PlayNote(120, 10); //C
		 currentnotes[count]='C';
		 currentnotes[count+1]='\0';
		 count++;
		 
	 }
	 if(PINE == 0xBF)//button on port e is pushed
	 {
		//don't need to call main, return will exit this function then ocarinaMode, going back to main anyway
		return; 
	 }
	 if(strcmp(currentnotes,selectedSong))//current note string equals song
	 {
		 USART_SendString("Success!");
		 selsong();
		 memcpy(selectedSong, "", sizeof(""));
		 count =0;
	 }
	 if(strlen(currentnotes)>strlen(selectedSong))// current note list is longer than the goal reset it
	 {
		  USART_SendString("Try Again");
		  memcpy(selectedSong, "", sizeof(""));
		  count =0;
	 }
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

ISR(USART1_RX_vect)
{
	value = UDR1;
}

ISR(USART1_UDRE_vect)
{
	UDR1 = 'x';
}

void USART_Init(unsigned long BAUDRATE)
{
	int BAUD_PRESCALE = (((F_CPU/(BAUDRATE *16UL)))-1);
	UCSR1B |= (1<<RXEN)|(1<<TXEN);
	UCSR1C |= (1<<UCSZ0)|(1<<UCSZ1);	
	UBRR1L=BAUD_PRESCALE;
	UBRR1H=(BAUD_PRESCALE>>8);
}

char USART_RxChar()
{
	if((UCSR1A&(1<<RXC)))
		return(UDR1);
	else
		return '\0';
}

void USART_TxChar(char data)
{
	UDR1 =data;
	while(!(UCSR1A &(1<<UDRE)));
}

void USART_SendString(char *str)
{
	int i=0;
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);
		i++;
	}
}

void PlaySong1()
{
	USART_SendString("Twinkle Twinkle Little Star");
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
	
	return;
}

void PlaySong2()
{
	USART_SendString("Dunno the name of this one");
	PlayNote(71, 563);  //A, triplet
	PlayNote(67, 597);	//B flat, triplet
	PlayNote(63, 635);	//B, triplet
	PlayNote(60, 2000);	//C, half note
	
	return;
}

void PlayEponasSong()
{
	USART_SendString("Epona's Song");
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
	
	return;
}

void PlaySongOfStorms()
{
	USART_SendString("Song of Storms");
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
	
	return;
}

void ShufflePlay()
{
	int x= rand();
	
	if (x % 4 == 0)
	{
		PlaySong1();
	}
	else if(x % 4 == 1)
	{
		PlaySong2();
	}
	else if (x % 4 == 2)
	{
		PlayEponasSong();
	}
	else
	{
		PlaySongOfStorms();
	}
	
	return;
}