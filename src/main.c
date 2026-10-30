#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"

#define HSE_STARTUP_TIMEOUT 0x5000

void SystemClock_Config()
{
    uint32_t Time_Counter = 0;

    RCC -> CR |= RCC_CR_HSEON; // включаем HSE
    while (!(RCC -> CR & RCC_CR_HSERDY)) { // проверяем готовность HSE
        if (Time_Counter++ > HSE_STARTUP_TIMEOUT)
            return;
    }

    FLASH -> ACR |= FLASH_ACR_LATENCY_2; // настраиваем ожидание Flash-памяти
    FLASH -> ACR |= FLASH_ACR_PRFTBE; // настраиваем кеш

    RCC -> CFGR |= RCC_CFGR_HPRE_DIV1; // настраиваем делитель шины AHB
    RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2; // настраиваем делитель шины APB1
    RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1; // настраиваем делитель шины APB2

    RCC -> CFGR |= RCC_CFGR_PLLSRC; //
    RCC -> CFGR &= ~RCC_CFGR_PLLXTPRE;
    RCC -> CFGR |= RCC_CFGR_PLLMULL9;

    RCC -> CR |= RCC_CR_PLLON;
    Time_Counter = 0;
    while (!(RCC -> CR & RCC_CR_PLLRDY)) {
        if (Time_Counter++ > HSE_STARTUP_TIMEOUT)
            return;
    }

    RCC -> CFGR &= ~RCC_CFGR_SW;
    RCC -> CFGR |=  RCC_CFGR_SW_PLL;

    Time_Counter = 0;
    while ((RCC -> CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
        if (Time_Counter++ > HSE_STARTUP_TIMEOUT)
            return;
    }
}

void USART1_Config() 
{
    RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

    RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN;

    USART2 -> CR1 |= USART_CR1_TE; 
    USART2 -> CR1 |= USART_CR1_RE;
    USART2 -> CR1 &= ~USART_CR1_M; // Message length, 8 bit
    
    USART2 -> BRR = 0x139;

    USART2 -> CR1 |= USART_CR1_UE; // UART Enable
}

void GPIOA_Config()
{
    RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN; //IO Port A Enable
    
    GPIOA -> CRL &= ~(0xF << 8); // Output mode 50 MHz
    GPIOA -> CRL |= (0xB << 8); // Alternate function push-pull

    GPIOA -> CRL &= ~(0xF << 12); // Input mode
    GPIOA -> CRL |= (0x4 << 12); // Floating input
}

int main()
{
    SystemClock_Config();
    GPIOA_Config();
    USART1_Config();

    char* message = "Hello from UART2 STM32 NUCLEO F103RB!!!\r\n";

    while(1)
    {
        char *p = message;
        while (*p)
        {
            while (!(USART2 -> SR & USART_SR_TXE));

            USART2 -> DR = *p++;
        }

        for (volatile int i = 0; i < 5000000; i++);
    }
}