/*
 * Lesly Montúfar
 * app.c
 * parece um pouco o arduino esse .c
 *
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "hw.h"

#define APP_DEBOUNCING_TIME_MS 	50
#define DELAY_100_100 			0		// ON - OFF
#define DELAY_300_150 			1
#define DELAY_600_300 			2
#define DELAY_900_450 			3

#define DELAY_BUTTON_ON(mode_blink)		(mode_blink == 0)? 100 : 300*mode_blink
#define DELAY_BUTTON_OFF(mode_blink)	(mode_blink == 0)? 100 : 150*mode_blink

volatile uint32_t delay = 100;
bool app_started = false;

void app_button_interrupt(uint8_t pin_it){
	static uint8_t mode_blink = DELAY_100_100;

	mode_blink |= (1 << pin_it);

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		delay = (delay == DELAY_BUTTON_ON(mode_blink) )? DELAY_BUTTON_OFF(mode_blink) : DELAY_BUTTON_ON(mode_blink);

		debouncing_time_ms = hw_tick_ms_get();
	}
}

void app_tick_1ms(void){
	static uint32_t delay_cnt = 0;

	if(!app_started)
		return;

	delay_cnt++;

	if(delay_cnt >= delay){
		delay_cnt = 0;
		hw_led_toggle();
	}

}

void app_init(void){
	app_started = true;
}

void app_loop(void){
	hw_cpu_sleep();
}
