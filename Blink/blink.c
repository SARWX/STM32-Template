#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
void Delay(uint32_t nTime);
int Wait_for(uint32_t nTime);
int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

    // Configure Pins
    GPIO_StructInit(&GPIO_InitStructure);       // Сбрасываем к начальным GPIO_InitStructure

   /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    */

    // Конфигурируем кнопку
    GPIOA->CRL=0x04;            // GPIOA - это указатель на структуру, т.е. он указывает на место в памяти, в котором хранится структура GPIOA со всеми своими параметрами
    // Конфигурируем светодиоды
    GPIOC->CRH=0x22;            // Аналогично CRH - параметр и ничего особенно большего



    // Configure SysTick Timer
    if (SysTick_Config(SystemCoreClock / 1000))     // SysTick_Config настраивает таймер на генерацию исключений каждые SystemCoreClock / 1000 тактов
    while (1);      // если все ок и генерациянастроена, то SysTick_Config вернет 0, а если возникли ошибки, то вернет 1 и запустится бесконечный цикл 
    static int ledval = 0;
    while (1) {
        // toggle led
        // GPIO_WriteBit(GPIOC , GPIO_Pin_9 , (ledval) ? Bit_SET : Bit_RESET);
        // if (GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0)) {
        if (GPIOA->IDR & 0x01) {
            ( (ledval) ? (GPIOC->BSRR = (0x100 | 0x2000000) ) : (GPIOC->BSRR = (0x1000000 | 0x200) ) );
            ledval = 1 - ledval;
            Delay(100);
            if (Wait_for(500)) {
                for(int i = 0; i < 10; i++) {
                    ( (ledval) ? (GPIOC->BSRR = (0x100 | 0x2000000) ) : (GPIOC->BSRR = (0x1000000 | 0x200) ) );
                    ledval = 1 - ledval;
                    Delay(100);
                }
            }
           // Delay(200); // wait 250ms
        }
    }
}
// Timer code
static __IO uint32_t TimingDelay;
void Delay(uint32_t nTime){
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

int Wait_for(uint32_t nTime){
    TimingDelay = nTime;
    int a = 0;
    while(TimingDelay != 0) {
         a |= (GPIOA->IDR & 0x01);
    }
    return(a);
}

void SysTick_Handler(void){
    if (TimingDelay != 0x00)
    TimingDelay --;
}
#ifdef USE_FULL_ASSERT
    void assert_failed(uint8_t* file, uint32_t line)
    {
        /* Infinite loop */
        /* Use GDB to find out why we're here */
        while (1);
    }
#endif   
