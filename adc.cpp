#include "adc.hpp"

#include <avr/io.h>
#include <avr/interrupt.h>

const Adc adc;

volatile uint16_t temperatureAdc;
volatile uint16_t keyboardAdc;

/**
 * Obsługa przerwania pomiaru ADC.
 */
ISR(ADC_vect)
{
	keyboardAdc = adc.measure();
}

void Adc::initialize() const
{
	ADMUX = _BV(REFS0);
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint16_t Adc::measure() const
{
	uint16_t value = ADCL;
	return value + (ADCH << 8);
}
