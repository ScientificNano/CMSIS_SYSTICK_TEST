#include "stm32f407xx.h"
#define SYSCLOCK 168000000U
__IO uint32_t SysTick_CNT = 0;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;//нахуя это нужно?
void RCC_init(void){
RCC->CR|=RCC_CR_HSEON;
while((RCC->CR&(RCC_CR_HSERDY))!=RESET){};
FLASH->ACR&=~FLASH_ACR_PRFTEN;
FLASH->ACR|=FLASH_ACR_PRFTEN;
FLASH->ACR=(((FLASH->ACR) & (~(FLASH_ACR_LATENCY)))|FLASH_ACR_LATENCY_5WS);
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_HPRE))|RCC_CFGR_HPRE_DIV1);
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_PPRE1))|RCC_CFGR_PPRE1_DIV4);
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_PPRE2))|RCC_CFGR_PPRE2_DIV2);
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLSRC))|RCC_PLLCFGR_PLLSRC_HSE);
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLQ))|0x7000000U);//тактирование usb
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLP))|0x00U);// очистка битов не тоже самое??
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLN))|RCC_PLLCFGR_PLLN_7|RCC_PLLCFGR_PLLN_5|RCC_PLLCFGR_PLLN_3);//разные способы включения битов со строкой 29
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLM))|RCC_PLLCFGR_PLLM_4);
RCC->CR|=RCC_CR_PLLON;
while((RCC->CR&(RCC_CR_PLLRDY))!=RCC_CR_PLLRDY){};
RCC->CFGR |= RCC_CFGR_SW_PLL; 					
while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}
void SysTick_Init(void){
SysTick->LOAD=(((SysTick->LOAD)& ~(SysTick_LOAD_RELOAD_Msk))|(SYSCLOCK/4000-1));
SysTick->VAL&=~(SysTick_VAL_CURRENT_Msk);
//SysTick->VAL=(((SysTick->VAL)& ~(SysTick_VAL_CURRENT_Msk))|((SYSCLOCK/1000)-1));
SysTick->CTRL|=SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk;
}

/*void delay(int ms){
SysTick->VAL=(((SysTick->VAL)& ~(SysTick_VAL_CURRENT_Msk))|((SYSCLOCK/1000)-1));
SysTick_CNT = ms;
while(SysTick_CNT){}
}*/
void SysTick_Handler (void){
if(SysTick_CNT)SysTick_CNT--;
}
void delay(uint32_t ms){
SysTick->VAL=(((SysTick->VAL)& ~(SysTick_VAL_CURRENT_Msk))|(SYSCLOCK/4000-1));
SysTick_CNT = ms;
while(SysTick_CNT){}
}
int main(void){
RCC_init();
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_MCO1))|RCC_CFGR_MCO1_1| RCC_CFGR_MCO1_0);
//SysTick_Config(SYSCLOCK/4000);
SysTick_Init();
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
GPIOA->MODER|=GPIO_MODER_MODER6_0;
GPIOA->MODER&=~GPIO_MODER_MODER8;
GPIOA->MODER|=GPIO_MODER_MODER8_1;
while(1){
GPIOA->ODR ^= (1 << 6);
delay(1000);
}
}
///acacac
