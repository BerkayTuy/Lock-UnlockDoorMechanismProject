; temp_sensor_asm.asm
; Lab 6, ECE 266, spring 2023
;
; Assembly code for the temperature sensor
;
; Created by Zhao Zhang

        ; To include names declared in C
        .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_ints.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/interrupt.h","driverlib/adc.h","temp_sensor.h"

        .data

;******************************************************************************
; Static/global variables
;******************************************************************************

; Temperature sensor reading states
; typedef struct {
;    uint32_t raw_data;
;    bool new_data_ready;
; } TempSensorState;
; static TempSensorState temp_sensor = {0, false};
temp_sensor
        .word   0
        .byte   false

        .text

;******************************************************************************
; Declaration of fields
;******************************************************************************
_ADC0_PERIPH        .field      SYSCTL_PERIPH_ADC0
_ADC0_BASE          .field      ADC0_BASE
_temp_sensor        .field      temp_sensor
_TempSensorISR      .field      TempSensorISR

;******************************************************************************
; voidTsInit()
;
; Initialize ADC to use Tiva C's internal temperature sensor
;
; Resources: ADC0, sequence #0, special channel TS (temperature sensor)
; Configurations: processor trigger, interrupt enabled, use step 0 only
;******************************************************************************
TsInit  .asmfunc
        push {lr}

        ; Enable the ADC0 peripheral
        ; call SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        LDR    r0, _ADC0_PERIPH
        BL      SysCtlPeripheralEnable

        ; Configure ADC0's sequencer #0
        ; call ADCSequenceConfigure(ADC0_BASE, 0 /* sequencer */, ADC_TRIGGER_PROCESSOR, 0 /* priority */)
        LDR     r0, _ADC0_BASE
        MOV	    r1, #1
        MOV	    r2, #0
        BL      ADCSequenceConfigure

       ; Configure step 0 of sequencer 0 to use the temperature sensor, with
       ; interrupt enable, as the end of the sequence
        ; call ADCSequenceStepConfigure(ADC0_BASE, 0 /* sequencer */, 0 /* step */,
        ;                         ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END)
        LDR		r0, _ADC0_BASE
        MOV		r1, #1
        MOV		r2, #0
        MOV		r3, #(ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END)
        BL 		ADCSequenceStepConfigure

        ; Enable ADC0, sequencer 0
        ; call ADCSequenceEnable(ADC0_BASE, 0 /* sequencer */);
        LDR     r0, _ADC0_BASE
        MOV     r1, #1
        BL      ADCSequenceEnable

        ; Configure the interrupt system for temperature sensor
        ; call ADCIntRegister(ADC0_BASE, 0 /* sequencer */, TempSensorISR)
        ; call ADCIntEnable(ADC0_BASE, 0 /* sequencer */)
        LDR     r0, _ADC0_BASE
        MOV     r1, #1
        LDR     r2, _TempSensorISR
        BL      ADCIntRegister

		LDR		r0,_INT_ADC)SS1
		MOV		r1, #0
		BL		IntPrioritySet

        LDR     r0, _ADC0_BASE
        MOV     r1, #1
        BL      ADCIntEnable

        pop     {pc}
        .endasmfunc

;******************************************************************************
; void TsTriggerSampling()
;
; Trigger temperature reading
;******************************************************************************
TsTriggerSampling
        .asmfunc
        push    {lr}

        ; CODE DELETED

        pop     {pc}
        .endasmfunc

;******************************************************************************
; TsNewDataReady
;
; Check if any new data is ready
;******************************************************************************
TsNewDataReady
        .asmfunc

        ; CODE DELETED

        BX      lr
        .endasmfunc

;******************************************************************************
; Read the current input from the temperature sensor
;******************************************************************************
TsReadRaw
        .asmfunc

        ; CODE DELETED

        BX      lr
        .endasmfunc

;******************************************************************************
; ISR for reading temperature data. It is called when ADC conversion is done.
;******************************************************************************
TempSensorISR
        .asmfunc
        push    {lr}

        ; CODE DELETED

        pop     {pc}
        .endasmfunc
