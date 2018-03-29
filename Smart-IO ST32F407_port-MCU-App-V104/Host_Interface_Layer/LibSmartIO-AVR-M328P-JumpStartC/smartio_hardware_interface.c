/* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * https://imagecraft.com/smartio/
 */
#include <stdio.h>
#include <stdarg.h>
#include <iccioavr.h>
#include <macros.h>

#include "smartio_interface.h"
#include "smartio_hardware_interface.h"

// #define printf  cprintf

/* ATMega328 Smart.IO pin assignments
 * SPI
 *  PB5     // SCK      D13
 *  PB3     // MOSI     D11
 *  PB4     // MISO     D12
 *  PB2     // SS       D10
 *
 * HOST interrupt
 *  PB0     // Pin change interrupt PCINT0      D8
 *
 * Smart.IO RESET
 *  PB1     // D9
 */

 //
 // Smart.IO hardware reset definitions
 //
 #define    RESET_PORT  PORTB
 #define    RESET_DDR   DDRB
 #define    RESET_PIN   1
 #define    RESET_BIT   (1 << RESET_PIN)
 //
 //     Issue a hard reset to the Smart.io module
 //
void SmartIO_HardReset(void)
{
    unsigned int    myDelay = 1400 ;    // Gives approx 1.25mS with 8MHz clock
    RESET_PORT &= ~(RESET_BIT) ;
    while(myDelay--)
    {
        asm("NOP") ;
    }
    RESET_PORT |= RESET_BIT ;
}
/* Initialize the Smart.IO interface
 */

 #define    SPI_PORT    PORTB
 #define    DDR_SPI     DDRB
 //
 // SPI SCK
 //
 #define    SPI_SCK_PIN 5
 #define    SPI_SCK     (1 << SPI_SCK_PIN)
//
// SPI MOSI
//
#define     SPI_MOSI_PIN    3
#define     SPI_MOSI        (1 << SPI_MOSI_PIN)
//
// SPI MISO
//
#define     SPI_MISO_PIN    4
#define     SPI_MISO        (1 << SPI_MISO_PIN)
//
// SPI Slave select
//
#define     SPI_SS_PIN      2
#define     SPI_SS          (1 << SPI_SS_PIN)

#define     SPI_CHIP_SELECT     SPI_PORT &= ~(SPI_SS)
#define     SPI_CHIP_DESELECT   SPI_PORT |= SPI_SS
//
// Host_IRQ
//
#define     HOST_IRQ_PORT   PORTB
#define     HOST_IRQ_INPUT  PINB
#define     HOST_IRQ_DDR    DDRB
#define     HOST_IRQ_BIT    0
#define     HOST_IRQ        (1 << HOST_IRQ_BIT)

#define     HOST_IRQ_INT        ( 1 << PCINT0)
#define     HOST_IRQ_MASK_REG   PCMSK0
#define     HOST_IRQ_GROUP      (1 << PCIE0)

//
// Smart.IO interrupt handler
//
//      Just needs to call the host handler when the
//      Host_IRQ goes HI.
//
//      Since the pin change interrupt will occur on both edges
//      do NOT call the host callback unless the input is HI
//

void (*hostCallback)(void) ;

#pragma interrupt_handler host_irq_handler:4

void host_irq_handler(void)
{
    //
    // If the Host_IRQ input is HI call host callback
    //
    if((HOST_IRQ_INPUT & HOST_IRQ) != 0 && hostCallback)
    {
        hostCallback();
    }
}
//
// Initialize the Smart.IO interface
//
void SmartIO_HardwareInit(void (*IRQ_ISR)(void))
{
    //
    // Save the Host_IRQ callback
    //
    hostCallback = IRQ_ISR ;
    //
    // Set SS HI
    //
    SPI_PORT |= SPI_SS ;
    //
    // Set MOSI, SCK, and SS as output ports
    //
    DDR_SPI |= (SPI_MOSI + SPI_SCK + SPI_SS) ;
    SPI_PORT |= SPI_SS ;
    //
    // Ensure that MISO is an input port
    //
    DDR_SPI &= ~(SPI_MISO) ;
    //
    // Configure SPI:
    //      SPIE        == 0    Interrupts disabled
    //      SPE         == 1    enable SPI
    //      DORD        == 0    MS bit first
    //      MSTR        == 1    master mode
    //      CPOL        == 0    leading ede is risong, training edge is falling
    //      CPHA        == 1    Setup on leading edge, sample on training edge
    //      SPR0 & SPR1 == 1    Clk/16
    //
    SPCR = ((1 << SPE) + (1 << MSTR) + (1 << SPR0) + (1 << CPHA));
    //
    // Host_IRQ is input
    //
    HOST_IRQ_DDR &= ~(HOST_IRQ_BIT) ;
    //
    // Enable the Host_IRQ interrupt input
    //
    PCMSK0 |= HOST_IRQ_INT ; // Enable pin input interrupt on change
    PCICR |= HOST_IRQ_GROUP ;           // Enable interupt group for input IRQ
    //
    // Setup the reset pin as active low output
    //
    RESET_PORT |= RESET_BIT;    // Start it HI
    RESET_DDR |= RESET_BIT ;    // Set as output
    SEI();
    SmartIO_HardReset();
}
//
//  SPI Transfer byte
//
//      Used by both read and write functions. When reading a byte
//  a dymmy byte (0xFF) is written. When writing a byte the received byte
//  is returned to the caller
//
unsigned char SPI_TransferByte(unsigned char aByte)
{
    //
    // Send byte
    //
    SPDR = aByte ;
    while(!(SPSR & (1 << SPIF)))
        ;
    return(SPDR) ;
}

/* send bytes out using the SPI port
 *  sendbuf: buffer containing content to send
 *  sendlen: number of bytes to send
 */
void SmartIO_SPI_SendBytes(unsigned char *sendbuf, int sendlen)
{
    unsigned char inputChar ;
    SPI_CHIP_SELECT ;
    SPI_TransferByte(sendlen);
    SPI_TransferByte(sendlen >> 8);
    while(sendlen--)
    {
        SPI_TransferByte(*sendbuf++) ;
    }
    SPI_CHIP_DESELECT ;
}

/* read bytes from the SPI port
 *  replybuf: buffer to fill the content with
 *  buflen: length of "replybuf"
 */
int SmartIO_SPI_ReadBytes(unsigned char *replybuf, int buflen)
    {
    SPI_CHIP_SELECT ;
    int low = SPI_TransferByte(0x00);
    int replylen = (SPI_TransferByte(0x00) << 8) | low;
    if (replylen > buflen)
    {
        replylen = -1;
    }
    else
    {
        int byteCount = replylen ;
        while(byteCount--)
        {
            *replybuf++ = SPI_TransferByte(0x00) ;
        }
    }
    SPI_CHIP_DESELECT ;
    return replylen;
    }


/* Wait until the Host INTR pin to go low as part of the read protocol
 */
void SmartIO_SPI_FinishRead(void)
{
    while ((HOST_IRQ_INPUT & HOST_IRQ) != 0)
        ;
}

/* Handle error conditions
 */
 #define printf(x)
void SmartIO_Error(int n, ...)
    {
    va_list args;
    unsigned h;

    va_start(args, n);

    switch (n)
        {
    case SMARTIO_ERROR_BADRETURN:
        printf("SMARTIO_ERROR_BADRETURN\n");
        break;
    case SMARTIO_ERROR_BAD_RETURN_HANDLE:
        printf("SMARTIO_ERROR_BAD_RETURN_HANDLE\n");
        break;
    case SMARTIO_ERROR_BAD_ASYNC_HANDLE:
        printf("SMARTIO_ERROR_BAD_ASYNC_HANDLE\n");
        break;
    case SMARTIO_ERROR_CALLBACK_TABLE_TOO_SMALL:
        printf("SMARTIO_ERROR_CALLBACK_TABLE_TOO_SMALL\n");
        break;
    case SMARTIO_ERROR_CALLBACK_HANDLE_NOT_FOUND:
        h = va_arg(args, unsigned);
        printf("SMARTIO_ERROR_CALLBACK_HANDLE_NOT_FOUND handle %d\n", h);
        break;
    case SMARTIO_ERROR_BUFFER_TOOSMALL:
        printf("SMARTIO_ERROR_BUFFER_TOOSMALL\n");
        break;
    case SMARTIO_ERROR_INVALID_HANDLE2:
        printf("SMARTIO_ERROR_INVALID_HANDLE2\n");
        break;
    case SMARTIO_ERROR_COMMAND_BUFFER_OVERFLOW:
        printf("SMARTIO_ERROR_COMMAND_BUFFER_OVERFLOW\n");
        }
    }
