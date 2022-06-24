# Blink com interrupções ![Language grade: C](https://img.shields.io/badge/language-C-blue)

Autora: Lesly Montúfar

Código blink com botão parar mudar frequência em que o led pisca, para STM32 utilizando interrupções.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Usando a STM32CubeIDE:
1. Configure a placa para ter dois botões e um led piscando, que serão tratados como interrupções.
2. O led piscará usando como tempo base o SysTick.
3. Pode-se fazer chamadas das funções do HAL e CSIS, para isso a suas funções podem ser abstraídas para a camada hw (de hardware, em `hw.c` e `hw.h`) para facilitar a leitura do código.
4. O desenvolvimento da aplicação deve estar numa pasta da aplicação `app`.
5. Os requisitos da aplicação serão cumpridos em `app.c` e `app.h`. 
6. As duas entradas, `PA0` e `PA1` obterão as seguintes possíveis saídas `S`, que são os delays ON/OFF do led:

| PA0 | PA1 | LED ON/OFF |
|:----------:|:----------:|:-------------:|
| 0 | 0 | 100 / 100 |
| 0 | 1 | 300 / 150 |
| 1 | 0 | 600 / 300 |
| 1 | 1 | 900 / 450 |


## Desenvolvimento

1. Faço a leitura da interrupção sobre-escrevendo a função `HAL_GPIO_EXTI_Callback` do HAL, do arquivo [`hw.c`](https://github.com/LeslyMontufar/blink_int/blob/25bce3939dbe024a494ca429b4ae1250a96a3d67/app/hw.c).
```
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
```

2. A aplicação, em [`app.c`](https://github.com/LeslyMontufar/blink_int/blob/25bce3939dbe024a494ca429b4ae1250a96a3d67/app/app.c), possui a variável `mode_blink` == `PA0 PA1`, para definir o delay em que o led está ON e OFF, considerando as entradas das interrupções dos dois botões.

```
#define APP_DEBOUNCING_TIME_MS 	50
#define DELAY_100_100 			0		// 00 -- PA0-PA1 -- ON - OFF -
#define DELAY_300_150 			1		// 01
#define DELAY_600_300 			2		// 10
#define DELAY_900_450 			3		// 11

#define DELAY_LED_ON(mode_blink)		((mode_blink == 0)? 	100 : 300*mode_blink)
#define DELAY_LED_OFF(mode_blink)		((mode_blink == 0)? 	100 : 150*mode_blink)

int bit_mode; // var aux
volatile uint8_t mode_blink = DELAY_100_100;
volatile uint32_t delay = DELAY_LED_OFF(DELAY_100_100);
bool app_started = false;
```
```
void app_button_interrupt(uint8_t pin_it){

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		bit_mode = !(mode_blink & (1 << pin_it));
		mode_blink &= ~(1 << pin_it); 	// Muda para 0
		mode_blink |= (bit_mode << pin_it);	// Caso contrário, muda para 1

		debouncing_time_ms = hw_tick_ms_get();
	}
}
```

3. As funções abaixo garantem que o led pisque com tempos de delay de ON e OFF diferentes, de acordo com a tabela mostrada.
```
void app_delay_toggle(void){
	delay = (delay == DELAY_LED_ON(mode_blink) )? DELAY_LED_OFF(mode_blink) : DELAY_LED_ON(mode_blink);
}
```
```
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
```

Para se piscar o led por interrupção, utiliza-se o manipulador do SysTick para utilizar o tempo de 1ms base fornecido por ele.
```
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  app_tick_1ms();
  /* USER CODE END SysTick_IRQn 1 */
}
```

4. O problema de debouncing foi tratado na função `app_button_interrupt` utilizando-se:

```
#define APP_DEBOUNCING_TIME_MS 	50
``` 

5. O controle de partida foi implementado na função de inicialização da aplicação:

```
void app_init(void){
	app_started = true;
	hw_led_state_set(false);
}
``` 

6. O modo é de baixa energia, já que não se desperdiça tempo da CPU observando a todo momento se o botão foi pressionado, logo pode se chamar a função da CMSIS para que desativar a CPU, e ativar o processamento somente quando houver interrupção por parte dos botões.

```
void app_loop(void){
	hw_cpu_sleep();
}
```
Sendo essa função declarada em `hw.c`.
```
void hw_cpu_sleep(){
	__WFI();
}
```
