/*
 * MAIN PROJECT CODE
 * ECE 266, Spring 2023
 *
 * Created by Berkay Tuysuzogullari and Daniel Ounsy
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "light_sensor.h"
#include "clockupdate.h"
#include <buzzer.h>
#include <sys/_stdint.h>
#include "pwmled.h"
#include <math.h>


// The state of the 7-segment display. Initially it shows "00:00" with the colon turned on.
Seg7Display seg7 = { { 10, 10, 10, 10 }, false /* colon on */};


typedef struct
{
    float red;
    float blue;
    float green;
} LEDColorSetting;

typedef struct {
    bool buzzer_on;
} SysState;
static SysState sys =
{
    false /* not activated */,
};

LEDColorSetting greenColor = { 0.0, 1.0, 0.0 };

/*
 * Trigger the light sensor reading periodically
 */


void LightSensorSampling(uint32_t time)
{
    // Trigger ADC sampling for the light sensor
    LsTrigRead();

    // Schedule the next callback in 4000 milliseconds
    ScheduleCallback(LightSensorSampling, time + 4000);
}



void CheckNewLightReading()
{


    if (!LsData())
        return;
    uint32_t raw_data = LsRaw(); //light reading

    uprintf("Light Sensor Reading: %u \r\n", (uint32_t)raw_data);

    seg7.digit[0] = raw_data % 10;             //Displays null for first index
    seg7.digit[1] = (raw_data / 10) % 10;      //Displays temperature value for second index
    seg7.digit[2] = (raw_data / 100) % 10;     //Displays temperature value for third index
    seg7.digit[3] = (raw_data / 1000) % 10;    //Displays temperature value for fourth index
    seg7.colon_on = false;

    if (LsRaw() >= 500){
        uprintf("DOOR UNLOCKED\n");

        if(!sys.buzzer_on){
            Seg7Update(&seg7);
            LedPwmSet(00000, 00000, 00000, 20000);
            BuzzerOn();
            sys.buzzer_on = true;
            SysCtlDelay(SysCtlClockGet()/3); // Delay for 1 second
            BuzzerOff();
            sys.buzzer_on = false;
            return;
        }
        else{
            Seg7Update(&seg7);
            BuzzerOff();
            sys.buzzer_on = false;
        }
    }
    else{
        Seg7Update(&seg7);
        LedPwmSet(00000, 20000, 00000, 00000);
        uprintf("DOOR LOCKED\n");

    }

    Seg7Update(&seg7);

}

/*
 * The main function: Initialize and the execute the callback scheduler loop
 */
int main(void)
{
    LpInit();
    BuzzerInit();
    LedPwmInit();

    Seg7Init();
    LsInitialize();



    uprintf("%s\n\r", "Lab Project: Door Lock Mechanism");

    // Schedule the first callback
    ScheduleCallback(LightSensorSampling, 3000);

    // Loop forever
    while (true)
    {
        // Wait for interrupt to happen
        __asm("    wfi");

        // Check new temperature reading and light reading
        CheckNewLightReading();

        // Call the callback scheduler
        ScheduleExecute();
    }
}
