#include<avr/interrupt.h>
#include<avr/io.h>
#include<util/delay.h>

void Timer1_init(void){
	TCCR1A=(1<<FOC1A);
	TCCR1B=(1<<WGM12)|(1<<CS12);
	TCNT1=0;
	OCR1A=3900;
	TIMSK|=(1<<OCIE1A);
}
char sec=0;
char min=0;
char hour=0;

ISR(TIMER1_COMPA_vect){
	sec++;
	if(sec==60){
		min++;
		sec=0;
	}
	if (min==60){
		hour++;
		min=0;
	}
}

void INT0_init(void){
	SREG&=~(1<<7);
	DDRD&=~(1<<2);
	PORTD|=(1<<2);
	MCUCR&=~(1<<ISC00);
	MCUCR|=(1<<ISC01);//FALLING EDGE
	GICR|=(1<<INT0);//INTERRUPT 0 ENABLE
	SREG|=(1<<7);
}

void INT1_init(void){
	SREG&=~(1<<7);
	DDRD&=~(1<<3);
	MCUCR|=(1<<ISC10)|(1<<ISC11);
	GICR|=(1<<INT1);
	SREG|=(1<<7);
}

void INT2_init(void){
	SREG&=~(1<<7);
	DDRB&=~(1<<2);
	MCUCSR&=~(1<<ISC2);
	GICR|=(1<<INT2);
	SREG|=(1<<7);
}

ISR(INT0_vect){
	TCCR1B|=(1<<CS12);
	SREG|=(1<<7);
	PORTC&=0XF0;
	PORTA|=0X3F;
	sec=0;
	min=0;
	hour=0;
	_delay_ms(500);
}

ISR(INT1_vect){
	TCCR1B&=~(1<<CS12); //stop clock
	PORTA|=0x3F;
	SREG|=(1<<7);
}

ISR(INT2_vect){
	TCCR1B|=(1<<CS12); //resume clock
	PORTA|=0x3F;
	SREG|=(1<<7);
}

int main(void){
	SREG|=(1<<7);
	DDRA |= 0x3F;
	PORTA&=0xC0;
	DDRC|=0x0F;
	PORTC&=0xF0;
	INT0_init();
	INT1_init();
	INT2_init();
	Timer1_init();
	while(1){
		//print second
		PORTA&=0xC0;
		PORTA|=(1<<0);
		PORTC=(PORTC&0xF0)|(sec%10);
		_delay_ms(5);
		PORTA&=0xC0;
		PORTA|=(1<<1);
		PORTC=(PORTC&0xF0)|(sec/10);
		_delay_ms(5);
		PORTA&=0xC0;
		PORTA|=(1<<2);
		PORTC=(PORTC&0xF0)|(min%10);
		_delay_ms(5);
		PORTA&=0xC0;
		PORTA|=(1<<3);
		PORTC=(PORTC&0xF0)|(min/10);
		_delay_ms(5);
		PORTA&=0xC0;
		PORTA|=(1<<4);
		PORTC=(PORTC&0xF0)|(hour%10);
		_delay_ms(5);
		PORTA&=0xC0;
		PORTA|=(1<<5);
		PORTC=(PORTC&0xF0)|(hour/10);
		_delay_ms(5);



		/*int i;
		while(1){
			for(i=0;i<6;i++){
			PORTA=PORTA&~0x3F;
			PORTA|=(1<<i);
			_delay_ms(5);
			}
		}*/
	}
}
