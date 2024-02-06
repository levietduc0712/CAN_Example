/*******************************************************************************
* Filename: CAN_RX_TX.c
*                                                                    
* Description: This example code illustrates how eCAN module works as transmitter 
* and receiver.
*
* CAN-B is other module CAN and CAN-A is eCAN module on LAUNCHXL-F28069M. They 
* need to be connected together through CAN transceivers. This code can only 
* be run with 28xx devices that have eCAN modules on-chip.
*
*******************************************************************************/

//
// Included Files
//
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

void    ATX2B(void);        // This function transmits from CAN-A to CAN-B
void    BTX2A(void);        // This function transmits from CAN-B to CAN-A
void    error(void);        // Program will terminate here upon error

struct ECAN_REGS ECanaShadow;

// Globals for this example

int     rxMsgData[8];

void main()
{
// Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
   InitSysCtrl();

// Initalize eCAN GPIO:
   InitECanGpio();

   InitECan();
    EALLOW;

/* Write to the MSGID field of both CAN modules */

    ECanaMboxes.MBOX0.MSGID.all = 0x9555AAA0;
    ECanaMboxes.MBOX4.MSGID.all = 0xC5555553;

/* Initialize the DLC field of CAN-A */

     ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
     ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;


/* Enable CAN-A Mailboxes */

   ECanaRegs.CANME.all = 0x00000011;


/* Begin transmitting */

   while(1)                 // Uncomment this for infinite transmission loops
   {
        ATX2B();
        BTX2A();
    }
}

/*****************/
/* End of main() */
/*****************/

/*---------------------------------------------*/
/* This function transmits from CAN-A to CAN-B */
/*---------------------------------------------*/

void    ATX2B(void)
{
// Configure Mailbox 0 as Transmit mailbox

    EALLOW;

    /* Configure Mailbox under test as a Transmit mailbox */

        ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
        ECanaShadow.CANMD.bit.MD0 = 0;
        ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    /* Enable Mailbox under test */

        ECanaShadow.CANME.all = ECanaRegs.CANME.all;
        ECanaShadow.CANME.bit.ME0 = 1;
        ECanaRegs.CANME.all = ECanaShadow.CANME.all;

        ECanaMboxes.MBOX0.MDL.byte.BYTE0 = 1;
        ECanaMboxes.MBOX0.MDL.byte.BYTE1 = 2;
        ECanaMboxes.MBOX0.MDL.byte.BYTE2 = 3;
        ECanaMboxes.MBOX0.MDL.byte.BYTE3 = 4;
        ECanaMboxes.MBOX0.MDH.byte.BYTE4 = 5;
        ECanaMboxes.MBOX0.MDH.byte.BYTE5 = 6;
        ECanaMboxes.MBOX0.MDH.byte.BYTE6 = 7;
        ECanaMboxes.MBOX0.MDH.byte.BYTE7 = 8;

        ECanaShadow.CANTRS.all = 0;
        ECanaShadow.CANTRS.bit.TRS0 = 1;     // Set TRS for mailbox under test
        ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
           do {ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;}   // Wait for TA0 bit to be set..
            while(ECanaShadow.CANTA.bit.TA0 == 0 );

        ECanaShadow.CANTA.all = 0;
        ECanaShadow.CANTA.bit.TA0 = 1;         // Clear TA0
        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

}

/*---------------------------------------------*/
/* This function transmits from CAN-B to CAN-A */
/*---------------------------------------------*/

void    BTX2A(void)
{

// Configure Mailbox 4 as Receive mailbox

   EALLOW;

   ECanaShadow.CANMD.all = 0;
   ECanaShadow.CANMD.bit.MD4 = 1;                  /* Configure Mailbox 4 as Receive */
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

   ECanaLAMRegs.LAM4.all  = 0x1FFFFFFF;

   ECanaShadow.CANME.all = 0;
   ECanaShadow.CANME.bit.ME4 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;

//   while(ECanaRegs.CANRMP.all != 0x00000010 ) {}  // Wait for RMP4 to be set..

   ECanaRegs.CANRMP.all = 0x00000010;            // Clear RMP4 bit and start


   ECanaRegs.CANMD.all = 0x00000010;
   ECanaRegs.CANRMP.all = 0x00000010;            // Clear RMP4 bit and start

   rxMsgData[0] = ECanaMboxes.MBOX4.MDL.byte.BYTE0;
   rxMsgData[1] = ECanaMboxes.MBOX4.MDL.byte.BYTE1;
   rxMsgData[2] = ECanaMboxes.MBOX4.MDL.byte.BYTE2;
   rxMsgData[3] = ECanaMboxes.MBOX4.MDL.byte.BYTE3;
   rxMsgData[4] = ECanaMboxes.MBOX4.MDH.byte.BYTE4;
   rxMsgData[5] = ECanaMboxes.MBOX4.MDH.byte.BYTE5;
   rxMsgData[6] = ECanaMboxes.MBOX4.MDH.byte.BYTE6;
   rxMsgData[7] = ECanaMboxes.MBOX4.MDH.byte.BYTE7;

}

/*----------------------------------------*/
/* Program will terminate here upon error */
/*----------------------------------------*/
void error(void)
{
    asm("     ESTOP0");  // test failed!! Stop!
    for (;;);
}
