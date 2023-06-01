#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"

/**
 * main.c
 */

uint32_t valor1 = 0;
uint32_t valor2 = 0;
uint32_t valor3 = 0;
uint32_t valor4 = 0;
uint32_t oscuroMin1 = 3200; //valor minimo tomado como cubierto para el ADC
uint32_t oscuroMin2 = 3600;
uint32_t oscuroMin3 = 2900;
uint32_t oscuroMin4 = 3300;

#define oscurominModifier 75

char parqueosOcupados = 0;

int Timer0IntHandler(void) {

    return 0;
}

int UARTIntHandler(void) {

return 0;
}

int ADC0IntHandler(void) {

return 0;
}

void readADC() {
    ADCIntClear(ADC0_BASE, 6);
    ADCIntClear(ADC0_BASE, 5);
    ADCIntClear(ADC0_BASE, 4);

    /*for (uint32_t i = 7; i > 3; i--) {
        ADCIntClear(ADC0_BASE, i);
        ADCProcessorTrigger(ADC0_BASE, i);
        while(!ADCIntStatus(ADC0_BASE, i, false));
        ADCSequenceDataGet(ADC0_BASE, i, adcvalor[7 - i]);

    }

    valor1 = adcvalor[0];
    valor2 = adcvalor[1];
    valor3 = adcvalor[2];
    valor4 = adcvalor[1];

    */

    ADCIntClear(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC0_BASE, 0);
    while(!ADCIntStatus(ADC0_BASE, 0, false)) {};
    ADCSequenceDataGet(ADC0_BASE, 0, &valor1);


    ADCProcessorTrigger(ADC0_BASE, 1);
    while(!ADCIntStatus(ADC0_BASE, 1, false));
    ADCSequenceDataGet(ADC0_BASE, 1, &valor2);

    ADCProcessorTrigger(ADC0_BASE, 2);
    while(!ADCIntStatus(ADC0_BASE, 2, false));
    ADCSequenceDataGet(ADC0_BASE, 2, &valor3);

    ADCProcessorTrigger(ADC0_BASE, 3);
    while(!ADCIntStatus(ADC0_BASE, 3, false));
    ADCSequenceDataGet(ADC0_BASE, 3, &valor4);

    return;

}



int main(void)   {

    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5); //configurar clock a 16  MHz
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //clock para puerto F, A, UART y timer
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        //while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        //ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_HALF, 1); //configuracion de adc

        IntMasterDisable();

        ADCSequenceDisable(ADC0_BASE, 0);
        ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 1); //Creando la secuencia 1 con prioridad 0, se activa con adc_trigger_processor
        ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH7); //Paso 0 de la secuencia 1 lee del canal an 7 (PD_0)
        ADCSequenceEnable(ADC0_BASE, 0);
        //ADCIntEnable(ADC0_BASE, 7);

        //Se repite lo mismo que antes para canal 6 (pd1)
        ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 2);
        ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);
        ADCSequenceEnable(ADC0_BASE, 1);

        //Se repite lo mismo que antes para canal 5 (pd2)
        ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 3);
        ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH5);
        ADCSequenceEnable(ADC0_BASE, 2);

        //Se repite lo mismo que antes para canal 4 (pd3)
        ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 4);
        ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH4);
        ADCSequenceEnable(ADC0_BASE, 3);

        //configurar pines d como adc
        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
        GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
        GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //configurar luces en la tiva

        //configure al timer 0 como una periodica
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_A, 16000);
        TimerEnable(TIMER0_BASE, TIMER_A); //activa el timer 0
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        //configurar pins para UART
        GPIOPinConfigure(GPIO_PB0_U1RX);//GPIO_PB0_U0RX
        GPIOPinConfigure(GPIO_PB1_U1TX);//GPIO_PB1_U0TX
        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM);
        UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //Configurar UART
        UARTEnable(UART1_BASE);

        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); //conectar botones en tiva
        GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //configurar pulldown



        while(1) {
            readADC();

            if (valor1 > (oscuroMin1)) {parqueosOcupados = parqueosOcupados | 1;
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, 0x20);
            }
            else {parqueosOcupados = parqueosOcupados & 0b1110;
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, 0x10);}

            if (valor2 > (oscuroMin2)) {parqueosOcupados = parqueosOcupados | 2;
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2| GPIO_PIN_3, 0x08);}
                        else {                        parqueosOcupados = parqueosOcupados & 0b1101;
                        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2| GPIO_PIN_3, 0x04); }

            if (valor3 > (oscuroMin3)) {parqueosOcupados = parqueosOcupados | 4;
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x08);
            }
                                    else { parqueosOcupados = parqueosOcupados & 0b1011;
                                    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2| GPIO_PIN_3, 0x04);}

            if (valor4 > (oscuroMin4)) {
                parqueosOcupados = parqueosOcupados | 8;
                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6| GPIO_PIN_7, 0x80);            }
             else {parqueosOcupados = parqueosOcupados & 0b0111;
             GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6| GPIO_PIN_7, 0x40);
             }


            //char luzOcupado = (parqueosOcupados) << 4;
            //char luzDesocupado = (!parqueosOcupados) << 4;

            //GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, luzDesocupado); //Luces verdes
            //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, luzOcupado); //Luces rojas

            if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) { //recalibrar modo luz
                    while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)); //debounce
                    oscuroMin1 = valor1 + oscurominModifier;
                    oscuroMin2= valor2 + oscurominModifier;
                    oscuroMin3 = valor3 + oscurominModifier;
                    oscuroMin4 = valor4 + oscurominModifier;}

            if (TimerIntStatus(TIMER0_BASE, false)) {
                TimerIntClear(TIMER0_BASE, TIMER_A);
                TimerLoadSet(TIMER0_BASE, TIMER_A, 1600000);
UARTCharPut(UART1_BASE, parqueosOcupados);
            };

        }



    return 0;
}


