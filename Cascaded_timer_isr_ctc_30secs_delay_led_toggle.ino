#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t t1c = 0;
uint8_t t2c = 0;

int main ()
{
  DDRB = 0xFF;
  PORTB = 0xFF;
  TCNT2 = 0;
  TCCR1A = 0x00;
  TCCR1B = (1<<CS12) | (1 << WGM12); ;
  OCR1A = 62500 ;
  TIMSK1 |= (1 << OCIE1A);
  TCCR2A |= (1 << WGM21);
  OCR2A = 60 ; // this genarates 30 secs  delay , if u want 1 min give value as 120
  TIMSK2 |= (1 << OCIE2A);

  sei();

  while(1)
  {

  }

}

ISR(TIMER1_COMPA_vect) {
        TCCR2B = (1<<CS20);
        TCCR2B = (0<<CS20);
}

ISR(TIMER2_COMPA_vect) {
      TCNT2 = 0;
      PORTB ^= (1<<PB5);
}
