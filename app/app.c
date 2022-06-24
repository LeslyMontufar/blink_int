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
#define DELAY_100_100 			0		// ON - OFF - 00 -- PA0-PA1
#define DELAY_300_150 			1		// 01
#define DELAY_600_300 			2		// 10
#define DELAY_900_450 			3		// 11

#define DELAY_LED_ON(mode_blink)		((mode_blink == 0)? 	100 : 300*mode_blink)
#define DELAY_LED_OFF(mode_blink)		((mode_blink == 0)? 	100 : 1500*mode_blink)

volatile uint8_t mode_blink = DELAY_100_100;
volatile uint32_t delay = DELAY_LED_OFF(DELAY_100_100);
bool app_started = false;

void app_delay_toggle(void){
	delay = (delay == DELAY_LED_ON(mode_blink) )? DELAY_LED_OFF(mode_blink) : DELAY_LED_ON(mode_blink);
}

void app_button_interrupt(uint8_t pin_it){

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){

		if((mode_blink & (1 << pin_it))) 	// Se 1, no pin_it
			mode_blink &= ~(1 << pin_it); 	// Muda para 0
		else
			mode_blink |= (1 << pin_it);	// Caso contrário, muda para 1

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
		app_delay_toggle();
	}

}

void app_init(void){
	app_started = true;
	hw_led_state_set(false);
}

void app_loop(void){
	hw_cpu_sleep();
}
