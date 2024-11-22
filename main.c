#include "include.h"
#include "common.h"

#define N 8
#define RIT_PERIOD_MS 50U

// Private Variables
static volatile uint32_t Sys_Tick = 0;

// Local Functions Prototypes
static void InitSysTick(void);
void SysTick_Handler(void);
void Delay_SysTick(uint32_t SysTicks);

// Imported Variables
extern uint8_t btn_flag;

int main (void) {
	// Imperative Inits
	SystemInit();
	InitSysTick();
	
	LCD_Initialization();
		
	// BUTTON_init:
	//		1: which button
	//		2: priority of the associated interrupt
	BUTTON_init(BUTTON_0, PRIO_3); // INT0
	BUTTON_init(BUTTON_1, PRIO_3); // KEY1
	BUTTON_init(BUTTON_2, PRIO_3); // KEY2
	
	joystick_init();
		
	// RIT WORKS WITH CLOCK = 100MHZ
	// ONE INTERRUPT EACH 50ms
	
	init_RIT(RIT_MS_TO_TICKS(RIT_PERIOD_MS)); enable_RIT();
	
	// power control register
	LPC_SC->PCON |= 0x1;					// PM0=1
	LPC_SC->PCON &= 0xFFFFFFFFD;	// PM1=0
	//execution of wfi or wfe assembly enters Power-Down mode when SLEEPDEEP is on
		
	start_game();
	
	__ASM("wfi");
}

/* Initialize SysTick using CMSIS Core_CM4 function */
static void InitSysTick(void){
	SysTick_Config(SystemFrequency/1000U); /* Configure the SysTick timer */
}
/* SysTick Interrupt Handler */
void SysTick_Handler(void){
	Sys_Tick++; /* increment timer */
}
/* Delay Function based on SysTick Counter */
void Delay_SysTick(uint32_t SysTicks){
	uint32_t DelayTimer_SysTick = Sys_GetTick() + SysTicks; /* Get End Tick */
	while(Sys_GetTick() < DelayTimer_SysTick); 				/* wait for timer */
}
/*Get Current Elapsed Ticks*/
uint32_t Sys_GetTick(void){
	return Sys_Tick;
}
