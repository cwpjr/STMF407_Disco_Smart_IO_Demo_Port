 /* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * https://imagecraft.com/smartio/
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <jsapi.h>
#include "smartio_interface.h"
#include "smartio_api.h"

#define PRODUCT_ID  100
#define BUILD_ID    3

int connected;

static void Setup(void);

void Button1(uint16_t val)
    {
    printf("Button1 gets %d\n", val);
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
    }

void text_entry1(char *s)
    {
    printf("text1 entered:[%s]\n", s);
    }

void menu_entry(uint16_t i)
    {
    printf("menu entry %d called\n", i);
    }

void checkbox1(uint16_t i)
    {
    printf("checkboxes (bitmask) 0x%x selected\n", i);
    }

void radio_buttons1(uint16_t i)
    {
    printf("radio button %d selected\n", i);
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

void _3pos_buttons1(uint16_t i)
    {
    printf("3 pos button %d selected\n", i);
    }

void expandable_list1(uint16_t i)
    {
    printf("expandable list entry %d selected\n", i);
    }

void CreateUI(void)
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
    SmartIO_EnableIf(u0+1, u1+1, 0);

    SmartIO_MakeSpacerSlice(2);
    u2 = SmartIO_MakeLabel(0, 1, "Auto On/Off Schedule");
    u3 = SmartIO_MakeLabel(0, 0, " Weekdays");
    u4 = SmartIO_MakeTimeSelector(0, 0, "17:00", time_selector1);
    SmartIO_AddText(u4, "ON at");
    SmartIO_SetSliceIcon(u4, SMARTIO_ICON_QUERY);
    u5 = SmartIO_MakeTimeSelector(0, 0, "0:00", time_selector2);
    SmartIO_AddText(u5, "OFF at");
    SmartIO_SetSliceIcon(u5, SMARTIO_ICON_QUERY);
    SmartIO_MakeSpacerSlice(1);

    u6 = SmartIO_MakeCheckboxes(1, 1, 0);
    SmartIO_AddListItem(u6+1, "Same as 'Weekdays'");

    u7 = SmartIO_MakeLabel(0, 0, " Weekend");
    u8 = SmartIO_MakeTimeSelector(0, 0, "18:00", time_selector3);
    SmartIO_AddText(u8, "ON at");
    SmartIO_SetSliceIcon(u8, SMARTIO_ICON_QUERY);
    u9 = SmartIO_MakeTimeSelector(0, 0, "1:00", time_selector4);
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
            DelayTenth(3);
            CreateUI();
            }
        last_state = connected;
        }
    return 0;
    }

static void Setup(void)
    {
    jsapi_clock.SetSystemClock(16, 0, false, 84, 5);
    jsapi_cortex_core.SysTick_Timer(SYSTICK_MILLISECOND);

    usart2.SetPins(&porta, 2, 7, &porta, 3, 7);
    usart2.MakeUSART(9600, 8, 1, 0);

    printf("\r\nImageCraft STM32F411 ST-Nucleo... System running at %dMhz\n", jsapi_clock.GetSysClkFreq() / 1000000);
    }

int putchar(unsigned char ch)
    {
    if (ch == '\n')
        usart2.putchar('\r');
    usart2.putchar(ch);
    return ch;
    }

int getchar(void)
    {
    return usart2.getchar();
    }
