 /* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * Version 1.03 2017/11/9
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

#define BUILD_ID    101
#define PRODUCT_ID  2

#define BUFLEN      1024

int connected;

// Hardware setup and initialization
static void Setup(void);
int GetCommandLine(char *buf, int len);

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

void doTest(int i)
    {
    tHandle p0, p1, p2, p3;
    tHandle u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;

    if (i != 8)
        SmartIO_LoadCache(PRODUCT_ID, (BUILD_ID << 4) | i);

    p0 = SmartIO_MakePage();
    switch (i)
        {
    case 0:
        u0 = SmartIO_AddMenu(   "Terms of Service", menu_entry);
        SmartIO_AddListItem(u0, "Enable Flux Capacitor");
        SmartIO_AddListItem(u0, "Enable Chameleon Circuit");
        break;
    case 1:
        u0 = SmartIO_MakeTextBox(0, 200, 6, "%BFour score and seven years ago%b our fathers brought forth, upon this continent, a new nation, conceived in liberty...");
        SmartIO_AddText(u0, "Lincoln's address");
        SmartIO_SetSliceIcon(u0, SMARTIO_ICON_INFO);
        break;
    case 2:
        SmartIO_AppTitle("Smart Wall Plug");
        u0 = SmartIO_MakeOnOffButton(0, 0, 1, Button1);
        SmartIO_AddText(u0, "Power");
        SmartIO_SetSliceIcon(u0, SMARTIO_ICON_POWER);
        u1 = SmartIO_MakeSlider(1, 0, 30, Slider1);

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
// no chekbox
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
        break;
    case 3:
        u0 = SmartIO_MakeCheckboxes(0, 5, checkbox1);
        SmartIO_AddListItem(u0+1, "One");
        SmartIO_AddListItem(u0+1, "Two");
        SmartIO_AddListItem(u0+1, "Three");
        SmartIO_AddListItem(u0+1, "Four");
        SmartIO_AddListItem(u0+1, "Five");

        SmartIO_MakeMultilineBox(7, "Night will fall and drown the sun when a good man goes to war. Friendship dies and true love lies. Night will fall and the dark will rise when a good man goes to war. Demons run but count the cost; the battle's won but the child is lost. Demons run when a good man goes to war.");

        u1 = SmartIO_MakeRadioButtons(0, 5, radio_buttons1);
        SmartIO_AddListItem(u1+1, "One");
        SmartIO_AddListItem(u1+1, "Two");
        SmartIO_AddListItem(u1+1, "Three");
        SmartIO_AddListItem(u1+1, "Four");
        SmartIO_AddListItem(u1+1, "Five012345678901234567");
        break;
    case 4:
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
        break;
    case 5:
        SmartIO_PageTitle(p0, "Charging Status");

        SmartIO_MakeLabel(0, 0, "Input");
        u0 = SmartIO_Make3PosButton(0, 4, 1, _3pos_buttons1);
        SmartIO_AddText(u0, "Slow/Normal/Fast");
        SmartIO_MakeSpacerSlice(3);
        u1 = SmartIO_MakeHGauge(0, 0, 40);
        SmartIO_AddText(u1, "Current input level");
        SmartIO_MakeSpacerSlice(1);
        SmartIO_MakeLabel(1, 0, "Battery level  ");
        u2 = SmartIO_MakeBatteryLevel(0, 1, 80);
        SmartIO_ClearText(u2);
        SmartIO_MakeSpacerSlice(4);
        SmartIO_AutoBalance(p0);
        break;
    case 6:
        SmartIO_AppTitle("Pokemon Control");
        u0 = SmartIO_MakeExpandableList(0, 5, expandable_list1);
        SmartIO_AddListItem(u0+1, "Select a Pokemon");
        SmartIO_AddListItem(u0+1, "Pikachu");
        SmartIO_AddListItem(u0+1, "Magikarp");
        SmartIO_AddListItem(u0+1, "Charmander");
        SmartIO_AddListItem(u0+1, "Bulbasaur");
        SmartIO_AddListItem(u0+1, "Snorlax");
        SmartIO_MakeSpacerSlice(4);
        u2 = SmartIO_MakeHGauge(0, 1, 30);
        SmartIO_AddText(u2, "current CP");
        SmartIO_MakeSpacerSlice(1);
        u1 = SmartIO_MakeSlider(0, 0, 70, Slider1);
        SmartIO_AddText(u1, "Power Up!");
        SmartIO_EnableIf(u0+1, u2+1, u1+1, 0);
        SmartIO_MakeSpacerSlice(1);
        SmartIO_MakeOK(2, "Apply", OK1);
        SmartIO_MakeSpacerSlice(1);
        SmartIO_AutoBalance(p0);
        break;
    case 7:
        u0 = SmartIO_MakeTextEntry(0, 1, 0, text_entry1);
        u1 = SmartIO_MakeTextEntry(0, 1, 3, text_entry2);
        break;
    case 8:
        printf("Unique ID is");
        uint32_t *p = SmartIO_GetUniqueID();
        printf(" %X %X\n", p[0], p[1]);
#define NELEMENTS   10
        uint32_t val1[NELEMENTS], *val2;
        uint16_t addr = SmartIO_GenRandomNumber() % (SMARTIO_MAX_EEPROM_SIZE - sizeof(uint32_t)*NELEMENTS);

        for (int i = 0; i < NELEMENTS; i++)
            val1[i] = SmartIO_GenRandomNumber();

        printf("writing eeprom...");
        SmartIO_WriteEEPROM(addr, sizeof(uint32_t)*NELEMENTS, (unsigned char*)&val1[0]);
        printf("reading eeprom...");
        val2 = (uint32_t *)SmartIO_ReadEEPROM(addr, sizeof(uint32_t)*NELEMENTS);

        printf("\ndone write/read %d bytes EEPROM @%d...\n", NELEMENTS*4, addr);
        for (int i = 0; i < NELEMENTS; i++)
            printf("%d w:%X r:%X\n", i, val1[i], val2[i]);

        printf("5 random numbers...");
        for (int i = 0; i < 5; i++)
            printf("%X ", SmartIO_GenRandomNumber());
        printf("\n");
        break;
        }

    if (i != 8)
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
    SmartIO_Init(Connect_CB, Disconnect_CB);

    printf("Command driver for Smart.IO %s\n", SmartIO_GetVersion());
    SPI_State = SPI_IDLE;
    int something_happened = 1;

    while (1)
        {
        unsigned char databuf[BUFLEN];

        if (something_happened)
            {
            putchar('>'); putchar(' ');
            something_happened = 0;
            }
        if (SPI_State == SPI_IDLE && usart2.kbhit())
            {
            something_happened = 1;
            int n = GetCommandLine(databuf, BUFLEN);
            if (n == 0)
                continue;

            if (databuf[0] == 255)
                SmartIO_HardReset();
            else if (!connected)
                printf("Not connected to Smart.IO/BLE. Command not sent\n");
            else if (databuf[0] == 128)
                doTest(databuf[2]);
            else
                {
                printf("sending %d bytes to SPI slave...", n);
                tHandle h = SmartIO__SendBytes(databuf, n);
                printf("return %d\n", h);
                }
            }
        else if (SPI_State == SPI_SMARTIO_ASYNC_REQUEST)
            SmartIO_ProcessUserInput();
        }
    return 0;
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

int GetCommandLine(char *databuf, int len)
    {
    unsigned char cmdline[BUFLEN];
    int databuf_index = 0;
    int cmdline_index = 0;

    while (cmdline_index < BUFLEN)
        {
        int c = getchar();
        putchar(c);

        if (c == '\b' || c == '\x7f')
            {
            if (cmdline_index)
                --cmdline_index;
            }
        else if (c == '\r' || c == '\n')
            {
            putchar('\n');
            cmdline[cmdline_index] = 0;
            for (char *s = cmdline; *s && databuf_index < len; )
                {
                while (isspace(*s))
                    s++;
                if (*s == 0)
                    break;

                if (isdigit(*s))
                    {
                    int n = strtol(s, &s, 0);
                    databuf[databuf_index++] = n & 0xFF;
                    databuf[databuf_index++] = n >> 8;
                    }
                else
                    {
                    if (*s == '"')
                        {
                        s++;
                        while (*s && *s != '"')
                            databuf[databuf_index++] = *s++;

                        if (*s != '"')
                            {
                            printf("Unterminated \"\n");
                            databuf_index = 0;
                            break;
                            }

                        if ((databuf_index & 1) != 0)
                            databuf[databuf_index++] = 0;
                        }
                    s++;
                    }
                }
            return databuf_index;
            }
        else
            cmdline[cmdline_index++] = c;
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
