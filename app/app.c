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

volatile uint32_t delay = 100;

void app_button_interrupt(void){
	if(delay == 100)
		delay = 400;
	else
		delay = 100;
}

void app_tick_1ms(void){
	static uint32_t delay_cnt = 0;
	delay_cnt++;

	if(delay_cnt >= delay){
		delay_cnt = 0;
		hw_led_toggle();
	}

}

void app_init(void){

}

void app_loop(void){
	hw_cpu_sleep();
}
