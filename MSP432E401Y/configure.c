/*
 *
 * configure.c
 * Author: Jack Danielski
 * June 2018
 *
 */

#include "ti/devices/msp432e4/driverlib/driverlib.h"

//#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "configure.h"

extern tCANMsgObject CANTx[2];
extern tCANMsgObject CANRx;

/* System clock rate in Hz */
extern uint32_t sysClock;

/* CAN variables */
extern bool rxMsg;
extern bool errFlag;
extern uint32_t msgCount;

/* Demo CAN messages Buttons and LEDs */
extern uint8_t msg1TxData[2];
extern uint8_t msg2TxData[2];

extern uint8_t msgRxData[8];

/* Scheduler variables */
extern uint8_t schedulerTimer;
extern uint8_t counter10ms;
extern uint8_t counter100ms;
extern uint8_t counter1s;

/*
 * configure CAN0 interface
 *  -uses pins 33 (RX) and 34 (TX)
 */
//void configure_CAN() {
//    // for help look at
//    // ~/ti/simplelink_...2_20_00_20/examples/nortos/MSP.../driverlib/
//
//    // Need to set RCGCCAN register
//
//    CAN0->CTL = ( CAN_CTL_INIT | CAN_CTL_IE |
//            CAN_CTL_EIE | CAN_CTL_DAR | CAN_CTL_CCE );
//
//    //    CAN0->BIT = 0x00;
//
//    // Set Baud Rate Prescaler Extension
//    //CAN0->BRPE = 0x00;
//
//
//    CAN0->IF1MSK1 = 0x1612;
//
//    CAN0->CTL &= ~CAN_CTL_INIT;
//}

void configureCAN() {
    /* Configure the CAN and its pins PA0 and PA1 @ 500Kbps */

    /* Enable the clock to the GPIO Port A and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)))
    {
    }

    /* Enable CAN0 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    /* Configure GPIO Pins for CAN mode */
    MAP_GPIOPinConfigure(GPIO_PA0_CAN0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_CAN0TX);
    MAP_GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Initialize the CAN controller */
    MAP_CANInit(CAN0_BASE);

    /* Set up the bit rate for the CAN bus.  CAN bus is set to 500 Kbps */
    MAP_CANBitRateSet(CAN0_BASE, sysClock, 500000);

    /* Enable interrupts on the CAN peripheral */
    MAP_CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    /* Enable auto-retry on CAN transmit */
    MAP_CANRetrySet(CAN0_BASE, true);

    /* Enable the CAN interrupt */
    MAP_IntEnable(INT_CAN0);

    /* Enable the CAN for operation */
    MAP_CANEnable(CAN0_BASE);
}


void CAN0_IRQHandler(void)
{
    uint32_t canStatus;

    /* Read the CAN interrupt status to find the cause of the interrupt */
    canStatus = MAP_CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    /* If the cause is a controller status interrupt, then get the status */
    if(canStatus == CAN_INT_INTID_STATUS)
    {
        /* Read the controller status.  This will return a field of status
         * error bits that can indicate various errors */
        canStatus = MAP_CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        /* Set a flag to indicate some errors may have occurred */
        errFlag = true;
    }

    /* Check if the cause is message object 1, which what we are using for
     * sending messages */
    else if(canStatus == 1)
    {
        /* Getting to this point means that the TX interrupt occurred on
         * message object 1, and the message TX is complete.  Clear the
         * message object interrupt */
        MAP_CANIntClear(CAN0_BASE, 1);

        /* Since the message was sent, clear any error flags */
        errFlag = false;
    }

    /* Check if the cause is message object 2, which what we are using for
     * sending messages */
    else if(canStatus == 2)
    {
        /* Getting to this point means that the TX interrupt occurred on
         * message object 2, and the message TX is complete.  Clear the
         * message object interrupt */
        MAP_CANIntClear(CAN0_BASE, 2);

        /* Since the message was sent, clear any error flags */
        errFlag = false;
    }

    /* Check if the cause is message object 3, which what we are using for
     * receiving messages */
    else if(canStatus == 3)
    {
        /* Getting to this point means that the RX interrupt occurred on
         * message object 3, and the message RX is complete.  Clear the
         * message object interrupt */
        MAP_CANIntClear(CAN0_BASE, 3);

        /* Set flag to indicate received message is pending */
        rxMsg = true;

        /* Since the message was sent, clear any error flags */
        errFlag = false;
    }
    else
    {
    }
}
