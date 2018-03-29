/* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * https://imagecraft.com/smartio/
 */
#include <stdio.h>
#include <stdarg.h>
#include <jsapi.h>
#include "smartio_interface.h"
#include "smartio_hardware_interface.h"

/* ST-Nucleo F411 Smart.IO pin assignments
 * SPI
 *  PORT  PIN AF
 *  porta, 5, 5,       // SCK
 *  porta, 7, 5,       // MOSI
 *  porta, 6, 5,       // MISO
 *  portb, 6,          // CS
 *
 * HOST interrupt
 *  PA9
 *
 * Smart.IO RESET
 *  PC7
 */

#define HOST_INTR_PIN       9
#define EXTI_PORT           exti9

/* Hardware Smart.IO
 */
void SmartIO_HardReset(void)
    {
    portc.Clear(7);
    DelayMilliSecs(10);
    portc.Set(7);
    }

/* send bytes out using the SPI port
 *  sendbuf: buffer containing content to send
 *  sendlen: number of bytes to send
 */
void SmartIO_SPI_SendBytes(unsigned char *sendbuf, int sendlen)
    {
    spi1.ChipSelect();
    spi1.Write(sendlen);
    spi1.Write(sendlen >> 8);
    spi1.WriteByteArray(0, sendbuf, sendlen);
    spi1.ChipDeselect();
    }

/* read bytes from the SPI port
 *  replybuf: buffer to fill the content with
 *  buflen: length of "replybuf"
 */
int SmartIO_SPI_ReadBytes(unsigned char *replybuf, int buflen)
    {
    spi1.ChipSelect();
    int low = spi1.Read();
    int replylen = (spi1.Read() << 8) | low;

    // printf("reading %d bytes from Smart.IO\n", replylen);
    if (replylen > buflen)
        replylen = -1;
    else
        spi1.ReadBytes(replybuf, replylen);

    spi1.ChipDeselect();
    return replylen;
    }

/* Initialize the hardware pins connected to Smart.IO
 */
void SmartIO_HardwareInit(void (*IRQ_ISR)(void))
    {
    // Initialize the SPI
    spi1.SetPins(
                &porta, 5, 5,       // SCK
                &porta, 7, 5,       // MOSI
                &porta, 6, 5,       // MISO
                &portb, 6,          // CS
                true                // active low
                );

    // CPOL - 0, CPHA - 1, 1Mhz clock (fastest for Smart.IO SPI)
    spi1.MakeSPI(8, 0b01, SPI_MHZ);
    spi1.Enable();

    // Smart.IO "SPI IRQ" input, ACTIVE HIGH
    porta.MakeInput(HOST_INTR_PIN);

    // Smart.IO will pull the line HIGH when it transmits.
    EXTI_PORT.MakeEXTI(&porta, PUPDR_NONE, EXTI_RISING_EDGE, 0, IRQ_ISR);

    // RESET pin
    portc.MakeOutput(7, OSPEED_LOW);
    SmartIO_HardReset();

    }

/* Wait until the Host INTR pin to go low as part of the read protocol
 */
void SmartIO_SPI_FinishRead(void)
    {
    while ((porta.Read() & BIT(HOST_INTR_PIN)) != 0)
        ;
    }

/* Handle error conditions
 */
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
