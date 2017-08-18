#include <stdlib.h>
#include <avr/io.h>
#include "lcd.h"   //" = Lib in akt Projektverzeichnis
#include <avr/interrupt.h>


#ifndef F_CPU
#define F_CPU 8000000L
#endif
#include <util/delay.h>

#define ATaster !(PINB & (1<<PB2))
#define SSchalter !(PINB & (1<<PB1))
#define CTaster !(PINB & (1<<PB0))

#define XSchalter !(PINC & (1<<PC0))
#define YSchalter !(PINC & (1<<PC1))
#define ZSchalter !(PINC & (1<<PC2))

#define AUS 1
#define SCHARF 2
#define ALARM 3

uint16_t ms100;
uint16_t verz_ms_100;
uint16_t ISR_zaehler;
uint16_t sec;
uint16_t restart_countdown = 2000;
uint8_t state;
uint8_t min;

ISR (TIMER0_OVF_vect)		//Overflow Interrupt Service Routine von Timer0
{		
		TCNT0 = 0;		// Startwert des Zaehlers nach Interrupt.
		ISR_zaehler++;
		if(ISR_zaehler==12)	//entspricht ca 100ms Sekunden (8M / 256 / 256 = 122/10)
		{
			ISR_zaehler=0;
			ms100++;
			verz_ms_100++;
		}
		if(verz_ms_100==1)
		{
			verz_ms_100 = 0;
		}
		if(ms100==10)
		{
			ms100 = 0;
			sec++;
			restart_countdown--;
		}
		
		if(sec==60)
		{
			sec = 0;
			
		}
		if(restart_countdown==0)
		{
			restart_countdown = 60;
			
		}
} // end of ISR 

uint8_t state;

uint8_t x;
uint8_t y;


int main(void)
{
	
			
	int state = 1;
	
	int ziffer1 = 1;
	int ziffer2 = 2;
	int ziffer3 = 3;
	
		//Konfiguration Timer Overflow
		TCCR0A	= 0x00;			//normal mode
		TCCR0B	= 0x04;			//clk/256
		TIMSK0	|= (1<<TOV0);	//Time0 frei
		TIFR0	|= (1<<TOV0);	//Interrupt Timeroverflow einschalten
		sei();
		
		DDRB = 0x0F;
		PORTB = 0x07;
		
		DDRC = 0x0F;
		PORTC = 0x00;
		
		DDRD=0xFF;
		PORTD = 0xFF;
		
		lcd_init(LCD_DISP_ON_CURSOR_BLINK);  // initialisieren

	
	while(1)
	{
		switch(state)
		{
			case AUS:		lcd_gotoxy(1,2);
							lcd_puts("Alarmanlage:");
							lcd_gotoxy(2,3);
							lcd_puts("AUS");
				
							if(SSchalter)
							{
								state = SCHARF;
								
								lcd_gotoxy(1,2);
								lcd_puts("Alarmanlage:");
								lcd_gotoxy(2,3);
								lcd_puts("SCHARF");
							}
							break;
						
			case SCHARF:	if(ATaster)
							{
								state = ALARM;
								
								PORTC |=(1<<PC4);		//Beeper einschalten
								
								lcd_gotoxy(0,0);
								lcd_puts("!!ALARM!!");

								lcd_gotoxy(0,2);
								lcd_puts("Geben Sie den");
								lcd_gotoxy(0,3);
								lcd_puts("Code ein:");
								
							}
							
							if(CTaster)
							{
								state = AUS;
								
								lcd_clrscr();
							}
							break;
			
			case ALARM: 	if(restart_countdown>60)
							{
								restart_countdown = 60;
							}
							
								if(restart_countdown<10)
								{
									lcd_gotoxy(12,0);
									lcd_count_16(restart_countdown);
								}
								
								else
								{
									lcd_gotoxy(11,0);
									lcd_count_16(restart_countdown);
								}
							
							if(CTaster)
							{
								state = AUS;
								
								PORTC &= ~(1<<PC4);		//Beeper ausschalten
								
								lcd_clrscr();
							}
							break;	
							
							switch(ziffer1)
							{
								case XSchalter: lcd_count_16(verz_ms_100);
												switch(ziffer2)
												{
													case XSchalter:lcd_count_16(verz_ms_100);
																switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																					lcd_gotoxy(0,3);
																					lcd_puts("Korrekter Code");
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
																
													case YSchalter:lcd_count_16(verz_ms_100);
																switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
																
													case ZSchalter:lcd_count_16(verz_ms_100);
																switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
												}break;
												
								case YSchalter: switch(ziffer2)
												{
													case XSchalter:switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
																
													case YSchalter:switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
																
													case ZSchalter:switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
												}break;
												
								case ZSchalter: switch(ziffer2)
												{
													case XSchalter:switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
																
													case YSchalter:switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
																
													case ZSchalter:switch(ziffer3)
																{
																	case XSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case YSchalter:lcd_count_16(verz_ms_100);
																				break;
																	case ZSchalter:lcd_count_16(verz_ms_100);
																				break;
																}break;
												}break;
							}break;		
							
						
		}
		
	}
	return 0;
}//end of main


