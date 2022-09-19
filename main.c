#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SRCLOCK 3
#define RCLOCK 4
#define ADDR_PORT 3
//00010101
//10101000

void ser_writeu16(uint32_t port, uint16_t val) {
	char idx;

	PORTD &= ~SRCLOCK;	
	PORTD &= ~RCLOCK;
	for (idx = 0; idx < 16; idx++) {
		PORTD = (~port & PORTD) | (port * !!(val & (0b1000000000000000 >> idx)));
		PORTD |= SRCLOCK;
		PORTD &= ~SRCLOCK;
	}
	PORTD |= RCLOCK;
}

int read_port(const int8_t port) {
	switch (port) {
	case 1:
		return !!(0b1000 & PORTD);
	case 2:
		return !!(0b10000 & PORTD);
	case 3:
	case 4:
	case 5:
	case 6:
	case 7: /* XT1 */
	case 8: /* XT2 */
		break;
	case 9:
		return !!(0b100000 & PORTD);
	case 10:
		return !!(0b1000000 & PORTD);
	case 11:
		return !!(0b10000000 & PORTD);
		break;
	case 12:
		return !!(0b1 & PORTB);
	case 13:
		return !!(0b10 & PORTB);
		break;
	case 14:
		return !!(0b100 & PORTB);
		break;
	case 15:
		return !!(0b1000 & PORTB);
	case 16:
		return !!(0b10000 & PORTB);
	case 17:
		return !!(0b100000 & PORTB);
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
		break;
	case 23:
		return !!(0b1 & PORTC);
	case 24:
		return !!(0b10 & PORTC);
	case 25:
		return !!(0b100 & PORTC);
	case 26:
		return !!(0b1000 & PORTC);
	case 27:
		return !!(0b10000 & PORTC);
	case 28:
		return !!(0b100000 & PORTC);
	default:
		break;

	}
}

void write_port(const int8_t port, const int b) {
	switch (port) {
	case 1:
		PORTD = (~0b1000 & PORTD) | (!!b * 0b1000);
		break;
	case 2:
		PORTD = (~0b10000 & PORTD) | (!!b * 0b10000);
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7: /* XT1 */
	case 8: /* XT2 */
		break;
	case 9:
		PORTD = (~0b100000 & PORTD) | (!!b * 0b100000);
		break;
	case 10:
		PORTD = (~0b1000000 & PORTD) | (!!b * 0b1000000);
		break;
	case 11:
		PORTD = (~0b10000000 & PORTD) | (!!b * 0b10000000);
		break;
	case 12:
		PORTB = (~0b1 & PORTB) | (!!b * 0b1);
		break;
	case 13:
		PORTB = (~0b10 & PORTB) | (!!b * 0b10);
		break;
	case 14:
		PORTB = (~0b100 & PORTB) | (!!b * 0b100);
		break;
	case 15:
		PORTB = (~0b1000 & PORTB) | (!!b * 0b1000);
		break;
	case 16:
		PORTB = (~0b10000 & PORTB) | (!!b * 0b10000);
		break;
	case 17:
		PORTB = (~0b100000 & PORTB) | (!!b * 0b100000);
		break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
		break;
	case 23:
		PORTC = (~0b1 & PORTC) | (!!b * 0b1);
		break;
	case 24:
		PORTC = (~0b10 & PORTC) | (!!b * 0b10);
		break;
	case 25:
		PORTC = (~0b100 & PORTC) | (!!b * 0b100);
		break;
	case 26:
		PORTC = (~0b1000 & PORTC) | (!!b * 0b1000);
		break;
	case 27:
		PORTC = (~0b10000 & PORTC) | (!!b * 0b10000);
		break;
	case 28:
		PORTC = (~0b100000 & PORTC) | (!!b * 0b100000);
		break;
	default:
		break;

	}
}

#define D13 17

void write_byte(uint16_t address, uint8_t val) {
	write_port(9, val & 0b00000001);	
	write_port(10, val & 0b00000010);
	write_port(11, val & 0b00000100);	
	write_port(12, val & 0b00001000);
	write_port(13, val & 0b00010000);	
	write_port(14, val & 0b00100000);
	write_port(15, val & 0b01000000);	
	write_port(16, val & 0b10000000);
	ser_writeu16(ADDR_PORT, address | 0b1000000000000000);
	write_port(D13, 0);
	write_port(D13, 1);
}

uint8_t read_byte(uint16_t address) {
	uint8_t res = 0;
	ser_writeu16(ADDR_PORT, address & ~0b1000000000000000);
	DDRB = 0;
	DDRD = 0;
	res |= read_port(9) << 0;
	res |= read_port(10) << 1;
	res |= read_port(11) << 2;
	res |= read_port(12) << 3;
	res |= read_port(13) << 4;
	res |= read_port(14) << 5;
	res |= read_port(15) << 6;
	res |= read_port(16) << 7;
	DDRB = 0b11111111;
	DDRD = 0b11111111;
	return res;
}

void write_bytes(uint16_t address, uint8_t *val, uint16_t size) {
	while (size--) {
		write_byte(address, *val);
		val++;
		address++;
	}
} 

int main(void) {
	DDRB = 0b11111111;
	DDRD = 0b11111111;
	DDRC = 0b11111111;

	PORTC = 0;
	PORTB = 0;
	PORTD = 0;

	write_port(D13, 1);
	write_port(23, 0);
	write_byte(0, 0x96);
 	uint8_t b = read_byte(0);

	if (b == 0x56) {
		write_port(23, 1);
		while (1)
			continue;
	}
	while (1) {
		write_port(23, 0);	
		for (int i = 0; i < 8; i++) {
			_delay_ms(100);
			write_port(23, 1);
			_delay_ms(100);
			write_port(23, 0);
			_delay_ms(100);
			write_port(23, b & (1 << i));
			_delay_ms(1000);
			write_port(23, 0);
		}
		_delay_ms(3000);
	}
/*
	if (b == 0x96) {
		for (int i = 0; i < 8; i++) {
	
		write_port(23, 1);
	}
	while (1) {
	}
*/
}
