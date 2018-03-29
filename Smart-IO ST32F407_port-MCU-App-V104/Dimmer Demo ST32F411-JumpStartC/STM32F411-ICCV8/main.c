 /* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * https://imagecraft.com/smartio/
 */
/*
 *
 * Test driver for Smart.IO. Set up to use ST-Nucleo (411) and ImageCraft JumpStart C for Cortex
 * compiler. Using a terminal emulator, you can type the commands
 *  128 <x>
 * where <x> is 0 to 8, to create sample UI pages.
 *
 * While you may also send individual commands to Smart.IO, but that process is not documented. You
 * should use the API functions (smartio_api.h) instead.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <jsapi.h>
#include "smartio_interface.h"
#include "smartio_api.h"

#define PA2_CHANNO  3
#define LIGHT_INITIAL_VALUE     50

#define PRODUCT_ID  1
#define BUILD_ID    3

#define BUFLEN      1024

#define printf      my_printf

int connected = 0;
static int cached = 0;
static int current_light_value = LIGHT_INITIAL_VALUE;
// Hardware setup and initialization
static void Setup(void);

int my_printf(char *fmt, ...) {}

void Button1(uint16_t val)
    {
    printf("Button1 gets %d\n", val);
    if (val == 0)
        {
        timer2.Disable();
        porta.MakeOutput(2, OSPEED_HIGH);
        porta.Clear(2);
        }
    else
        {
        porta.MakeAltFunction(2, 1, OSPEED_HIGH);
        timer2.Enable();
        timer2.ChangePWMDutyCycle(PA2_CHANNO, current_light_value);
        }
    }

void Button2(uint16_t val)
    {
    printf("Button2 gets %d\n", val);
    }

void OK1(uint16_t val)
    {
    printf("OK1 gets %d\n", val);
    }

void Slider1(uint16_t val)
    {
    printf("Slider1 gets %d\n", val);
    current_light_value = val;
    timer2.ChangePWMDutyCycle(PA2_CHANNO, current_light_value);
    SmartIO_ToggleLED(3);
    }

void text_entry1(char *s)
    {
    printf("text1 entered:[%s]\n", s);
    }

void text_entry2(char *s)
    {
    printf("text2 entered:[%s]\n", s);
    }

void number_entry1(char *s)
    {
    printf("number entered:[%s]\n", s);
    }
void password_entry1(char *s)
    {
    printf("password entered:[%s]\n", s);
    }

void calendar_selector1(char *s)
    {
    printf("calendar selected: %s\n", s);
    }
void number_selector1(uint32_t n)
    {
    printf("number selected: %d\n", n);
    }

void time_selector1(uint16_t hh, uint16_t mm)
    {
    printf("time1 selected %d:%d\n", hh, mm);
    }

void time_selector2(uint16_t hh, uint16_t mm)
    {
    printf("time2 selected %d:%d\n", hh, mm);
    }

void time_selector3(uint16_t hh, uint16_t mm)
    {
    printf("time3 selected %d:%d\n", hh, mm);
    }

void time_selector4(uint16_t hh, uint16_t mm)
    {
    printf("time4 selected %d:%d\n", hh, mm);
    }

void CreateUI()
    {
    tHandle p0, p1, p2, p3;
    tHandle u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;

    SmartIO_LoadCache(PRODUCT_ID, BUILD_ID);
    p0 = SmartIO_MakePage();
    SmartIO_AppTitle("Smart Wall Plug");
    u0 = SmartIO_MakeOnOffButton(0, 0, 1, Button1);
    SmartIO_AddText(u0, "Power");
    SmartIO_SetSliceIcon(u0, SMARTIO_ICON_POWER);
    u1 = SmartIO_MakeSlider(1, 0, 30, Slider1);
    SmartIO_UpdateSlider(u1+1, current_light_value);

    SmartIO_EnableIf(u0+1, u1+1, 0);

    SmartIO_MakeSpacerSlice(2);
    u2 = SmartIO_MakeLabel(0, 1, "Auto On/Off Schedule");
    u3 = SmartIO_MakeLabel(0, 0, " Weekdays");
    u4 = SmartIO_MakeTimeSelector(0, 0, "17:00", 0);
        SmartIO_AddText(u4, "ON at");
        SmartIO_SetSliceIcon(u4, SMARTIO_ICON_QUERY);
    u5 = SmartIO_MakeTimeSelector(0, 0, "0:00", 0);
    SmartIO_AddText(u5, "OFF at");
    SmartIO_SetSliceIcon(u5, SMARTIO_ICON_QUERY);
    SmartIO_MakeSpacerSlice(1);
// no chekbox
    u7 = SmartIO_MakeLabel(0, 0, " Weekend");
    u8 = SmartIO_MakeTimeSelector(0, 0, "18:00", 0);
        SmartIO_AddText(u8, "ON at");
        SmartIO_SetSliceIcon(u8, SMARTIO_ICON_QUERY);
    u9 = SmartIO_MakeTimeSelector(0, 0, "1:00", 0);
        SmartIO_AddText(u9, "OFF at");
        SmartIO_SetSliceIcon(u9, SMARTIO_ICON_QUERY);
    SmartIO_GroupObjects(0, u3, u4, u5, u7, u8, u9, 0);
    SmartIO_MakeSpacerSlice(3);
    SmartIO_AutoBalance(p0);
    SmartIO_SaveCache();
    }

void Connect_CB(void)
    {
    printf("BLE connected\n");
    connected = 1;
    }

void Disconnect_CB(void)
    {
    printf("BLE disconnected\n");
    SPI_State = SPI_IDLE;
    connected = 0;
    cached = 0;
    }

int main(void)
    {
    Setup();
    printf("Smart.IO Version %s\n", SmartIO_GetVersion());

    SmartIO_Init(Connect_CB, Disconnect_CB);
    int last_state = 0;
    while (1)
        {
        while (last_state == connected)
            if (SPI_State == SPI_SMARTIO_ASYNC_REQUEST)
                SmartIO_ProcessUserInput();

        if (connected)
            {
            DelayTenth(5);
            CreateUI();
            }
        last_state = connected;
        }

    return 0;
    }

int putchar(unsigned char ch)
    {
    return ch;
    }

int getchar(void)
    {
    }

static void Setup(void)
    {
    jsapi_clock.SetSystemClock(16, 0, false, 84, 5);
    jsapi_cortex_core.SysTick_Timer(SYSTICK_MILLISECOND);
/*
    usart2.SetPins(&porta, 2, 7, &porta, 3, 7);
    usart2.MakeUSART(9600, 8, 1, 0);
 */
    printf("\r\nImageCraft STM32F411 ST-Nucleo... System running at %dMhz\n", jsapi_clock.GetSysClkFreq() / 1000000);
    timer2.MakePWM(PA2_CHANNO, 10000, LIGHT_INITIAL_VALUE);
    timer2.SetPinsForPWM(&porta, 2, 1);
    }
