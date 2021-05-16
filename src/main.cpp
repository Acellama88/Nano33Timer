#include <Arduino.h>
#include "ArduinoNanoBLE33_Timer0.h"

Nano33TIMER timer; //Timer
bool LED_State = HIGH;
uint32_t counter = 0;
bool SerialOut = false;

#define TIM_SCALAR 512 //Used for the Calculation of  (2^PRESCALAR: 2 ^ 9 = 512)
#define TIM_CLK 16000000 //Clock Frequency for the Timer (16 MHz)
#define TIM_SEC (15*60) //Amount of time to wait in Seconds for TIM_COMPARE Calculation
#define TIM_COMPARE ((TIM_CLK / TIM_SCALAR) * TIM_SEC) //Calculate a TIM_Compare
#define TIM_COMPARE_CALC(seconds) ((TIM_CLK / TIM_SCALAR) * seconds) //Macro for Comparator for Timer

void chan_0_callback(void)
{
  if(LED_State == HIGH)
      LED_State = LOW;
  else
      LED_State = HIGH;
  timer.clearEvent(0);
  timer.restartTimer();
}

void setup() {
  timer.setTimer(3,9);
  timer.setChannel(0,TIM_COMPARE_CALC(2));
  timer.RegisterCallback(chan_0_callback, 0);
  timer.begin();
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN,LED_State);
}

extern "C"
{
    void TIMER0_IRQHandler_v (void)
    {
        timer.TIMER0_ISR();
    }
}