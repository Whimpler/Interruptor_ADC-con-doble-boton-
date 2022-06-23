#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "uart.h"
#include <stdlib.h>
volatile bool aux = true; //bandera

//FUNCIONES
void interrupt_INT0_Init(){
 cli(); //DESH INT
 EICRA = 0b1111; // se habilitan int 1 y 2
 EIMSK = 3; 
 sei(); //HAB INT
}
void interrupt_TIMER0_Init(){
 cli();
 TCCR0B = 0b101; //PREESCALADOR 1024
 TIMSK0 = 1; //HB TIMER POR DESBORDAMIENTO
 sei();
 TCNT0 = 12; //250ms
}
void ADC_Init(){
 ADMUX = 0b000; //ADC0 
 ADCSRA = 0b10101011; //ADC activado modo automático y preescalador 8
 //ADCSRB= 0b10; //Modo interrupción externa //ADIE(3) en 1 para habilitar la interrupción por la terminación de una conversión analógica.
 ADCSRB= 0b100; //Modo Timer0 (250ms)
}
//INTERRUPCIONES
ISR(TIMER0_OVF_vect){
 TCNT0 = 12;
}
ISR(INT0_vect){
 //EIFR &= ~(1<<INTF0);
}
 ISR(INT1_vect){
    aux=!aux;
      if(aux){
	 ADCSRB=0b100;
      }else{
	 ADCSRB=0b010;
      }
}
ISR(ADC_vect){ 
 //PORTB =ADCL;
 //PORTC = ADCH;
float r = ADC/1023.0*5.0;
   
   
unsigned int val =(int) r;//Variable auxiliar para guardar datos
 
 //PORTB=0B0100000;
//PORTB = 0b11111111;
  int intervalo = (val>=0) + (val>=1)  + (val>=2) +  (val>=3)+  (val>=4)+  (val==5) ;//Se suman las condiciones, cuando es 5 - intervalo es 6 y se va al caso 6 del switch
  switch(intervalo){
      case 1:PORTB= 0B0111111;//Cuando el numero es 0
	 break;
	case 2:PORTB = 0B0000110;//Cuando el numero es 1
	   break;
	  case 3:PORTB =0B1011011 ;//Cuando el numero es 2
	     break;
	    case 4:PORTB = 0B1001111;//Cuando el numero es 3
	       break;
	      case 5:PORTB = 0B1100110;//Cuando el numero es 4
		 break;
	      case 6:PORTB = 0B1101101;//Cuando el numero es 5
	      default: break;
		 break; 
      };
      
char rec1[20] ;
dtostrf(r, 2, 4, rec1);
serial_println_str(rec1);
      
//dtostrf(r,1,0,auxPB);
}
//MAIN

int main()
{
   DDRB=DDRB|0B1111111;
 PORTD |= (1<<PD2); //Activar resistencia de PULLUP en PIN0 del puerto D
 ADC_Init();
 interrupt_INT0_Init();
 interrupt_TIMER0_Init(); //250ms de retardo
serial_begin(9600);
while(1){
;
}
return 0;
}
