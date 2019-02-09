#include <LPC17xx.h>
int a=1;
void TIMER2_IRQHandler (void)  // تابه زیر روال وقفه تایمر کانتر 
{
	LPC_TIM2->IR=1; // پاک کردن پرچم وقفه
	LPC_GPIO2->FIOSET=a; // روشن کردن ال ای دی
	a*=2; 
}

void TimerSet(void){

 LPC_GPIO2->FIODIR=0Xff; // خروجی کردن ال ای دی
 LPC_GPIO2->FIOCLR=0Xff;
	LPC_SC->PCONP|= (1<<22);  // روشن کردن تایمر 2
	LPC_TIM2->TCR=0X2;  // ریست کردن و خاموش کردن تایمر کانتر  
	LPC_TIM2->PR=0X00;  // شریب تقسیم 0 است
	LPC_TIM2->MR0=24999999;  // عدد 25 ملیون رو با مقدار تایمر/کانتر مقایشه کن
	LPC_TIM2->MCR=0X3;  // فعال کردن وقفه و ریست کردن مقدار تایمر/کانتر بعد از تساوی رجیستر بالا با مقدار تایمر کانتر
	LPC_TIM2->TC=0X0; // مقدار اولیه تایمر/کانتر رو 0 قرار بده
	NVIC_EnableIRQ(TIMER2_IRQn);  // فعال کردن وقفه ی تایمر کانتر 2
	LPC_TIM2->TCR=0X01;  // روشن کردن تایمر کانتر و شروع کردن به شمارش

	
}




