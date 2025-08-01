#include <avr/io.h>
#include <avr/interrupt.h>

volatile int c=0;
int main()
{
  DDRB |= 0xFF;
  PORTB &= ~(1 << PB5);
  PRR &= ~(1 << PRADC); 
  ADCSRB &= ~(1 << ACME); 
  ADCSRA &= ~(1 << ADATE); 
  DIDR1 |= (1 << AIN0D) | (1 << AIN1D); 
  ACSR &= ~(1 << ACBG);
  ACSR |= (1 << ACIE); 
  ACSR |= (1 << ACIS1) ; 
  ACSR |= (1 << ACIS0) ;

  sei();
  while(1)
  {
    if(c>=1)
    {
      _delay_ms(1500);
      PORTB &= ~(1 << PB5);
      c=0;
    }
  }
}

ISR(ANALOG_COMP_vect)
{
  c=1;
  PORTB |= (1<<PB5);
  ACSR |= (1 << ACI); 
}

