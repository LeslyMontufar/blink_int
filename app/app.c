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

void app_init(void){

}

void app_loop(void){
	uint32_t delay;
	bool button_state = hw_button_state_get();

	if(button_state)
		delay = 2000;
	else
		delay = 500;

	hw_led_state_set(true);
	hw_delay_ms(delay);
	hw_led_state_set(false);
	hw_delay_ms(delay);

}
