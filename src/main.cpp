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

//Function ran on Channel 0 Interrupt Trigger
void chan_0_callback(void)
{
  if(LED_State == HIGH) //Change LED State
      LED_State = LOW;
  else
      LED_State = HIGH;
  timer.clearEvent(0); //Clear the Triggered Event
  timer.restartTimer(); //Restart the counter on the timer
}

void setup() {
  timer.setTimer(3,9);  //Configure the BitMode (3 - 32 Bit) and Prescalar (9: 2 ^ 9 = 512)
  timer.setChannel(0,TIM_COMPARE_CALC(2)); //Set Channel 2 to trigger on 2 Seconds
  timer.RegisterCallback(chan_0_callback, 0); //Register the Callback Function for the Interrupt
  timer.begin(); //Start the Timer
  pinMode(LED_BUILTIN,OUTPUT); //Set LED Pin to Output
}

void loop() {
  digitalWrite(LED_BUILTIN,LED_State);  //Set LED to LED_State
}

//This isn't ideal, but was an easy way to make this work.
//This basically says: On the Timer0 Interrupt, run the timer objects function.
extern "C"
{
    void TIMER0_IRQHandler_v (void)  //Interrupt Handler for Timer0
    {
        timer.TIMER0_ISR(); //Call the API's Interrupt Routine, so it can choose the function to run.
    }
}