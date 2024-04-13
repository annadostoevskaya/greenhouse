#include <avr/io.h>
#include <util/delay.h>

int main() {
  DDRB = DDRB | (1 << DDB5);

  for (;;) {
    PORTB = PORTB | (1 << PORTB5);
    _delay_ms(1000.0f);
    PORTB = PORTB & ~(1 << PORTB5);
    _delay_ms(1000.0f);
  }

  return 0;
}
