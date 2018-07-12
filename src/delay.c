#include <stdio.h>
#include <math.h>
#include "library.h"
#include "delay.h"

void Delay(uint16_t s){
	uint64_t us = 1000000 * s;
	while(us--){
		__nop();
	}
}

void DelayMillis(uint32_t ms){
	unsigned long long us = 1000 * ms;
	while(us--){
		__nop();
	}
}

void DelayMicros(uint32_t us){
	while(us--){
		__nop();
	}
}
