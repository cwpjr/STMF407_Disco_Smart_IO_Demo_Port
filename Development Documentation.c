// How I developed this application port documentation file.
This was my 1rst git so this is a stub... The link to real (ie complete) repo is:
https://github.com/cwpjr/STMF407_Disco_Spi1FDM1NSS
# IAR-Smart-IO-Port-STM407-Disco
This is a port of the Smart-IO Command Demo-ST32F411-JumpStartC and Host_Interface_Layer, to IAR EWARM, on the STM32F407vg Discovery board, For Nathans Bedbug Intelligence Group.
By Clyde W. Phillips Jr cwpjr02@gmail.com

// How I developed this application port documentation file.

/* This is a port of the Smart-IO board software from a custom compiler.
The custom compiler and IDE is called Jumpstart-C.
It's a one man shop at this point. He's smart and working...
His Jumpstart-C includes an api called jsapi.
As of late 2017 his product had a Code Blocks IDE
running his compiler, and a hi level jsapi
that had runtimes for an AVR and a STM M0 and F401/411.
*/

/*
This is a port of the Smart-IO Command Demo-ST32F411-JumpStartC
and Host_Interface_Layer, to IAR EWARM, on the STM32F407vg Discovery board,
For Nathans BB group.
Bedbug Intelligence Group
*/

/*
STM32Cubemx is used to generate initialization code and supplies HAL routines.
http://www.st.com/en/development-tools/stm32cubemx.html
http://www.st.com/content/ccc/resource/technical/document/user_manual/10/c5/1a/43/3a/70/43/7d/DM00104712.pdf/files/DM00104712.pdf/jcr:content/translations/en.DM00104712.pdf
NOTE: Pins with labels append _PIN to the label ie label=Label_Name becomes Label_Name_Pin
*/

// Here are the peripherals and protocols used for this project.
// The Smart-IO board will be attached to a BIG potentiostat pcb
// Designed by Micheal Simon msimon6808@yahoo.com
// Bedbug Intelligence Group @ 2018
/*
RCC (CLOCK) Section:
Get ahold of systick~!
stm32f4xx_hal.c has it.
__weak void HAL_Delay(__IO uint32_t Delay) delay (in milliseconds)
*/

/*
UART Section:
huart2 for Smart-IO debug dump
huart3 for pStat app and Fish compatability
*/

/*
SPI section:
Per Simons Potentiostat 90100-Rev28Jan2018 - Schematic.pdf:
--
hspi1 is the Master SPI bus to all the potentiostat components.
The components are an external bus to the Smart-IO board.
and internal (on the board parts) of an ADC and a DAC.
And 
Chip selections are GPIO Output pins; on pins PD12-PD14.
--
The HAL and jsapi/Smart-IO do things different enough to warrant a discussion.
--
HAL is what I have to work with for this port. It's cheap, i.e. free
Smart-IO's ble smartphone App sends data to the Smart-IO board.
It's OEM'd to signal an interrupt to the controlling board.
So HAL says I can do polling | IRQ/DMA.
The short is if I poll for the interrupt I need to be sure
there are no program flow considerations that would require using interrupts.
Interrupts should be chosen to enable data thruput in code flow.
Otherwise polling (blocking) is clearer.
If using interruots then use the HAL RX/TX interrupt routines.
*/

/*
Smart-IO Section:
Command Demo-ST32F411-JumpStartC is a startup stand alone demo (ithink)
The command line portion uses uart2 as a command line interface.

I am implementing this on the STM32F407vg Disco Board to
understand better whether It can handle muxing with FISH on uart3.
*/


/*  MX_SPI1_Init(); // Creates its own hspi handle: */
/*    SPI_HandleTypeDef  hspi; // can't be changed - overwritten by CubeMX.
      I have to verify it's settings and make my own and call after if so.
      And verify clock is giving me the required Smart-IO Clock and
      the other spi devices needs.
Smart-IO Maximum bus frequency is 1 MHz
ADC
DAc
    STM NSS is not used. SPI chip select is software managed.
    NSS on SPI1 forces SWD vs JTAG.
*/

// The Smart-IO required SPI init:
/*
 SPI in 8-bit mode
? Maximum bus frequency is 1 MHz
 CPOL is 0 and CPHA is 1
 MSBit transmitted first
X ncs is active low
NSS (ncs) is handled in software  as 123 on PD13,14, and 15.
Host IRQ - interrupt signal (Smart.IO to MCU), active low. Also used for bootloader
firmware update on PA9.
RESET - resetting the Smart.IO module, active low om PC7.
*/
// The generated code for hspi:

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;

