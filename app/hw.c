/*
 * hw.c
 * Abstrai as funções do HAL e CMSIS
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "app.h"

// a leitura é bit PA0 e PA1, então estão com posições invertidas
#define PA0 1
#define PA1 0

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_0){
		app_button_interrupt(PA0);
	}
	if(GPIO_Pin == GPIO_PIN_1){
		app_button_interrupt(PA1);
	}
}

bool hw_button_state_get(void){
	GPIO_PinState button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);

	if(button_state == GPIO_PIN_RESET)
		return true;
	else
		return false;
}

void hw_led_toggle(void){
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void hw_led_state_set(bool state){
	GPIO_PinState led_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led_state);
}

void hw_delay_ms(uint32_t time_ms){
	HAL_Delay(time_ms);
}

void hw_cpu_sleep(){
	__WFI();
}

uint32_t hw_tick_ms_get(void){
	return HAL_GetTick();
}
