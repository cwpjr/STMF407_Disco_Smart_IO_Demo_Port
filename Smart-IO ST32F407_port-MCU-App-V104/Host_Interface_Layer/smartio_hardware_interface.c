/* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * https://imagecraft.com/smartio/
 */
#include <stdio.h>
#include <stdarg.h>
//#include <jsapi.h>
//FIGUREd_OUT and FIXed #include "LibSmartIO-AVR-M328P-JumpStartC/smartio_interface.h"
#include "smartio_interface.h"
#include "smartio_hardware_interface.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "spi.h"

/* STMF407vg Discovery Board Smart.IO pin assignments
 * SPI has hspi1 handler from the STM32F4 Hal Layer.
 * To roughly manipulate thes SPI's,
 * use HAL calls that have hspix as one function call argument:
 *  PORT  PIN AF
 *  pa5,   5, 5,       // SCK
 *  pa6,   6, 5,       // MISO
 *  pa7,   7, 5,       // MOSI
*/
/*
The BBFind project is running a potentiostat on STMF205 /F405
and on the STM407vg Discovery Boards.
So we use PD12, PD13 nd PD14 to SPI chip select
the Smart-IO board, the adc and the dac, in turn.

STM32CubeMX leaves it to me to call hal routines.
To encapsulate my chip select functionality.
stm32f2xx_hal_gpio.c
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
*/
 /*
 *  pd12,  6,          // CS
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
/* The original code:
    portc.Clear(7);
    DelayMilliSecs(10);
    portc.Set(7);
*/
    //portc.Clear(7);
    HAL_GPIO_WritePin(GPIOD, Smart_IO_Reset_LOW_Pin, GPIO_PIN_RESET);
    //DelayMilliSecs(10);
    HAL_Delay(10);
    //portc.Set(7);
    HAL_GPIO_WritePin(GPIOD, Smart_IO_Reset_LOW_Pin, GPIO_PIN_SET);
    }

/* send bytes out using the SPI port
 *  sendbuf: buffer containing content to send
 *  sendlen: number of bytes to send
 */
void SmartIO_SPI_SendBytes(unsigned char *sendbuf, int sendlen)
    {
/* The original code:
    spi1.ChipSelect();
    spi1.Write(sendlen);
    spi1.Write(sendlen >> 8);
    spi1.WriteByteArray(0, sendbuf, sendlen);
    spi1.ChipDeselect();
*/
    //spi1.ChipSelect();
    HAL_GPIO_WritePin(GPIOD, SPI_CS_Smart_IO_Pin, GPIO_PIN_SET);
    
/* This block seems to be the 'SendBytes'
    spi1.Write(sendlen);
    spi1.Write(sendlen >> 8);
    spi1.WriteByteArray(0, sendbuf, sendlen);
*/
/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
//HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
    HAL_SPI_Transmit(&hspi1, sendbuf, sendlen, 1000);

    //spi1.ChipDeselect();
    HAL_GPIO_WritePin(GPIOD, SPI_CS_Smart_IO_Pin, GPIO_PIN_RESET);
    
    }

/* read bytes from the SPI port
 *  replybuf: buffer to fill the content with
 *  buflen: length of "replybuf"
 */
int SmartIO_SPI_ReadBytes(unsigned char *replybuf, int buflen)
    {
/*
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
*/
    int replylen = 0;
    //spi1.ChipSelect();
    HAL_GPIO_WritePin(GPIOD, SPI_CS_Smart_IO_Pin, GPIO_PIN_SET);

/* Block of the read logic
    int low = spi1.Read();
    int replylen = (spi1.Read() << 8) | low;

    // printf("reading %d bytes from Smart.IO\n", replylen);
    if (replylen > buflen)
        replylen = -1;
    else
        spi1.ReadBytes(replybuf, replylen);
*/
//HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    HAL_SPI_Receive(&hspi1, replybuf, buflen, 1000);
    

/* The end block 
    spi1.ChipDeselect();
    return replylen;
*/
    if (replylen > buflen)
        replylen = -1;
    else
    HAL_GPIO_WritePin(GPIOD, SPI_CS_Smart_IO_Pin, GPIO_PIN_RESET);
    return replylen;
    }

/* Initialize the hardware pins connected to Smart.IO  */
void SmartIO_HardwareInit(void (*IRQ_ISR)(void))
    {
      
    /* Done by STM32CubeMX init code. Sortof.
        This code enables the spi but also
        inits the irq pin and patches (*IRQ_ISR)
        into the spi(x) irq vector.
      So patch (*IRQ_ISR) into my spi irq vector
      and use the _It hal function for the reads
      from the Smart-IO board.
    */
/* SmartIO_HardwareInit() last call after HW init. */
    SmartIO_HardReset();
    
    /*
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
*/
    }

/* Wait until the Host INTR pin to go low as part of the read protocol
 */
void SmartIO_SPI_FinishRead(void)
    {
    // while ((porta.Read() & BIT(HOST_INTR_PIN)) != 0)
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
