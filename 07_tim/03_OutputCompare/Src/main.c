#include "stm32f030x8.h"
#include "tim.h"



volatile int timestamp=0;

int main (void){

	tim1_pa8_output_compare();
	tim3_pa6_input_capture();

	while(1){
		/*Wait until edge is captured*/
		while(!(TIM3->SR & TIM3_SR_CC1IF)){}

		//Read the data
		timestamp = TIM3->CCR1;
		TIM3->SR &= ~TIM3_SR_CC1IF;             // explicit clear
}
}
