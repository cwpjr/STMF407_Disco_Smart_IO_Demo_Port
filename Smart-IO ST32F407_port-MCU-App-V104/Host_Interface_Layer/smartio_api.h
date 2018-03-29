/* Copyright 2017 ImageCraft Creations Inc., All rights reserved.
 * Smart.IO Host Interface Layer
 * https://imagecraft.com/smartio/
 */
#pragma once
#include <stdint.h>

/* Please refer to the Host Interface Layer document on how to set the values of these define
 *  https://imagecraft.com/documentation/smart-io-documentation
 */
#if defined(_AVR)
#define HOST_SRAM_POOL_SIZE         128
#define CALLBACK_TABLE_SIZE         50
#else
#define HOST_SRAM_POOL_SIZE         256
#define CALLBACK_TABLE_SIZE         100
#endif

/* DO NOT MODIFY ANYTHING BELOW
 */
#define FETCH_WORD(s, i)            (((s)[i+1] << 8) | (s)[i])
#define SMARTIO_MAX_EEPROM_SIZE     (2048-32)   // 32 bytes reserved for Smart.IO

extern unsigned char *host_sram_block;

typedef uint16_t tHandle;
typedef int16_t tStatus;

enum SMARTIO_ICON {
    SMARTIO_ICON_NONE, SMARTIO_ICON_ALERT, SMARTIO_ICON_ERROR, SMARTIO_ICON_H_ONOFF, SMARTIO_ICON_INFO, SMARTIO_ICON_NEXT,
    SMARTIO_ICON_POWER, SMARTIO_ICON_PREVIOUS, SMARTIO_ICON_QUERY, SMARTIO_ICON_SETTINGS, SMARTIO_ICON_V_ONOFF,
};

enum {
    SMARTIO_SYS_EEPROM_WRITE = 0xFF00,
    SMARTIO_SYS_LED_SET, SMARTIO_SYS_LED_CLEAR, SMARTIO_SYS_LED_TOGGLE,
    SMARTIO_SYS_RAND, SMARTIO_SYS_DEBUG_UART,
    SMARTIO_SYS_PHONE_BEEP,
    SMARTIO_SYS_UART_WRITE,

    // These return multiple bytes
    __SMARTIO_SYS_MULTIBYTE_RETURN = 0xFF80,
    SMARTIO_SYS_EEPROM_READ = __SMARTIO_SYS_MULTIBYTE_RETURN, SMARTIO_SYS_UNIQUE_ID,
    SMARTIO_SYS_PHONE_TIME, SMARTIO_SYS_PHONE_GPS,
};

enum {
    SMARTIO_HOST_DISCONNECT = 0x100,
    SMARTIO_HOST_CONNECT,
};

char *  SmartIO_GetVersion(void);
void    SmartIO_Init(void (*connect_callback)(void), void (*disconnect_callback)(void));
void    SmartIO_HardReset(void);

tStatus SmartIO_LoadCache(uint32_t product_id, uint16_t build_id);
tStatus SmartIO_SaveCache(void);

tStatus SmartIO_AppTitle(char *title);
tHandle SmartIO_AddMenu(char *label, void (*callback)(uint16_t));

tStatus SmartIO_Resumeable(uint16_t);
tStatus SmartIO_Reset(void);

tHandle SmartIO_MakePage(void);
tStatus SmartIO_SetCurrentPage(tHandle);
tStatus SmartIO_LockPage(tHandle);
tStatus SmartIO_UnlockPage(void);
tStatus SmartIO_PageTitle(tHandle, char *);

tStatus SmartIO_PopupAlert(uint16_t variation);

// up to 8 handles can be specified. End with a 0 null handle(not included as part of the 8 value)
// option is currently ignored and should be set to zero
tStatus SmartIO_GroupObjects(uint16_t option, tHandle, ...);
// up to 8 dependent handles. End with a 0 null handle(not included as part of the 8 value)
tStatus SmartIO_EnableIf(tHandle control, tHandle dependent1, ...);

// When called the first time, the App stops responding to end user interaction and displays a spinning circle.
// The app "freezes" until ANY command is made by the host firmware, including another STOP_RESUME
// NOTE: "System commands" that are serviced by the Smart.IO and not the app, e.g.
// Read/Write EEPROM, will NOT un-freeze the app
tStatus SmartIO_StopResume(void);

// Inout Object
// Return TWO handle object
// the return value is the handle for the SLICE, return_value+1 is the handle for the object itself
//
// The slice is the container for the object. Thus, disable/hid either the object or the
// slice has the same effect, ditto with enable/show
tHandle SmartIO_MakeOnOffButton(uint16_t alignment, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_Make3PosButton(uint16_t alignment, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_MakeIncrementer(uint16_t alignment, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_MakeSlider(uint16_t alignment, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_MakeExpandableList(uint16_t alignment, uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_MakePicker(uint16_t alignment, uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_MakeMultiSelector(uint16_t alignment, uint16_t nentries, uint16_t isSingleSelectOnly, void (*callback)(uint16_t));
tHandle SmartIO_MakeNumberSelector(uint16_t alignment, uint16_t default_val, uint16_t low, uint16_t high, void (*callback)(uint32_t));

//Time format in "HH:MM" and must be in 24-hour format
// display option: 0: display AM/PM 12 hour format, 1: no AM/PM 24 hour format
tHandle SmartIO_MakeTimeSelector(uint16_t alignment, uint16_t display_24h, char *initial_value, void (*callback)(uint16_t, uint16_t));
tHandle SmartIO_MakeAnalogTimeSelector(uint16_t alignment, uint16_t display_24h, char *initial_value, void (*callback)(uint16_t, uint16_t));
tHandle SmartIO_MakeCalendarSelector(uint16_t alignment, char *initial_value, void (*callback)(char *));
tHandle SmartIO_MakeWeekdaySelector(uint16_t alignment, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_MakeOK(uint16_t alignment, char *text, void (*callback)(uint16_t));
tHandle SmartIO_MakeCancelOK(uint16_t alignment, char *text, void (*callback)(uint16_t));
tHandle SmartIO_MakeOKLinkTo(uint16_t alignment, tHandle popup_handle, char *text, void (*callback)(uint16_t));

tHandle SmartIO_MakeCheckboxes(uint16_t alignment, uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_MakeRadioButtons(uint16_t alignment, uint16_t nentries, void (*callback)(uint16_t));

tHandle SmartIO_MakeTextEntry(uint16_t alignment, uint16_t isRoundCorners, uint16_t nlines, void (*callback)(char *));
tHandle SmartIO_MakeNumberEntry(uint16_t alignment, uint16_t isRoundCorners, void (*callback)(uint32_t));

// Use SmartIO_AddText(handle, "password hint") to add hint
tHandle SmartIO_MakePasswordEntry(uint16_t alignment, void (*callback)(char *));

// Text Box is meant to be narrower(up to up to 60% of the screen width across ~200 virtual pixels) and can have a slice icon
// whereas multiline box takes the full width
tHandle SmartIO_MakeTextBox(uint16_t alignment, uint16_t width, uint16_t nlines, char *text);
tHandle SmartIO_MakeMultilineBox(uint16_t nlines, char *text);

// DIsplay in a number within a white-background box. ndigits specifies the size
// if ndigits is zero, then the size of the box changes with the value
tHandle SmartIO_MakeCounter(uint16_t alignment, uint16_t ndigits, uint32_t initial_value);

// width is in virtual pixels with a max of 320 virtual pixels
tHandle SmartIO_MakeProgressBar(uint16_t alignment, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_MakeProgressCircle(uint16_t alignment, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_MakeHGauge(uint16_t alignment, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_MakeVGauge(uint16_t alignment, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_MakeSemiCircularGauge
                                 (uint16_t alignment, uint16_t max_mark, uint16_t initial_percentage);
tHandle SmartIO_MakeCircularGauge(uint16_t alignment, uint16_t max_mark, uint16_t initial_percentage);
tHandle SmartIO_MakeBatteryLevel(uint16_t alignment, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_MakeRGBLed(uint16_t alignment, uint16_t color, uint16_t initial_state);

tHandle SmartIO_MakeCustomHGauge(uint16_t alignment, uint32_t start_color, uint32_t end_color, uint16_t initial_percentage);
tHandle SmartIO_MakeCustomVGauge(uint16_t alignment, uint32_t start_color, uint32_t end_color, uint16_t initial_percentage);

// Slice icon valid except for
// Calendar and multiline text
tStatus SmartIO_SetSliceIcon(tHandle, uint16_t icon_enum);

tHandle SmartIO_MakeFreeformSlice(uint16_t y_size);

tHandle SmartIO_FFS_OnOffButton(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_FFS_3PosButton(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_FFS_Incrementer(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_FFS_Slider(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));

tHandle SmartIO_FFS_Picker(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_FFS_MultiSelector(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t nentries, uint16_t isSingleSelectOnly, void (*callback)(uint16_t));
tHandle SmartIO_FFS_NumberSelector(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t default_val, uint16_t low, uint16_t high, void (*callback)(uint16_t));
tHandle SmartIO_FFS_TimeSelector(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t display_24h, char *initial_value, void (*callback)(uint16_t, uint16_t));
tHandle SmartIO_FFS_AnalogTimeSelector(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t display_24h, char *initial_value, void (*callback)(uint16_t, uint16_t));
tHandle SmartIO_FFS_CalendarSelector(tHandle, uint16_t loc_x, uint16_t loc_y, char *initial_value, void (*callback)(char *));
tHandle SmartIO_FFS_WeekdaySelector(tHandle, uint16_t loc_x, uint16_t loc_y, char *initial_value, void (*callback)(char *));

tHandle SmartIO_FFS_Checkboxes(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_FFS_RadioButtons(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t nentries, void (*callback)(uint16_t));

tHandle SmartIO_FFS_TextEntry(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t isRoundCorners, void (*callback)(char *));
tHandle SmartIO_FFS_NumberEntry(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t isRoundCorners, void (*callback)(char *));

// Use SmartIO_AddText(tHandle, uint16_t loc_x, uint16_t loc_y, handle, "password hint" value) to add hint
tHandle SmartIO_FFS_PasswordEntry(tHandle, uint16_t loc_x, uint16_t loc_y, void (*callback)(char *));

// Text Box is meant to be narrower and can have a slice icon
// whereas multiline box takes the full width
tHandle SmartIO_FFS_TextBox(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t width, uint16_t nlines, char *text);
tHandle SmartIO_FFS_MultilineBox(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t nlines, char *text);
tHandle SmartIO_FFS_Counter(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t ndigits, uint32_t initial_value);

// width is in virtual pixels with a max of 320 virtual pixels
tHandle SmartIO_FFS_ProgressBar(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_FFS_ProgressCircle(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_FFS_HGauge(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_FFS_VGauge(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_FFS_SemiCircularGauge(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t max_mark, uint16_t initial_percentage);
tHandle SmartIO_FFS_CircularGauge(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t max_mark, uint16_t initial_percentage);
tHandle SmartIO_FFS_BatteryLevel(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_FFS_RGBLed(tHandle, uint16_t loc_x, uint16_t loc_y, uint16_t color, uint16_t initial_state);

tHandle SmartIO_FFS_CustomHGauge(tHandle, uint16_t loc_x, uint16_t loc_y, uint32_t start_color, uint32_t end_color, uint16_t initial_percentage);
tHandle SmartIO_FFS_CustomVGauge(tHandle, uint16_t loc_x, uint16_t loc_y, uint32_t start_color, uint32_t end_color, uint16_t initial_percentage);

tHandle SmartIO_FFS_Label(tHandle, uint16_t loc_x, uint16_t loc_y, char *label);

tHandle SmartIO_PopupOnOffButton(uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_Popup3PosButton(uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_PopupIncrementer(uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));
tHandle SmartIO_PopupSlider(uint16_t variation, uint16_t initial_value, void (*callback)(uint16_t));

tHandle SmartIO_PopupPicker(uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_PopupMultiSelector(uint16_t nentries, uint16_t isSingleSelectOnly, void (*callback)(uint16_t));
tHandle SmartIO_PopupNumberSelector(uint16_t default_val, uint16_t low, uint16_t high, void (*callback)(uint16_t));
tHandle SmartIO_PopupTimeSelector(uint16_t display_24h, char *initial_value, void (*callback)(uint16_t, uint16_t));
tHandle SmartIO_PopupAnalogTimeSelector(uint16_t display_24h, char *initial_value, void (*callback)(uint16_t, uint16_t));
tHandle SmartIO_PopupCalendarSelector
                                (tHandle, uint16_t loc_x, uint16_t loc_y, char *initial_value, void (*callback)(char *));
tHandle SmartIO_PopupWeekdaySelector(char *initial_value, void (*callback)(char *));

tHandle SmartIO_PopupCheckboxes(uint16_t nentries, void (*callback)(uint16_t));
tHandle SmartIO_PopupRadioButtons(uint16_t nentries, void (*callback)(uint16_t));

tHandle SmartIO_PopupTextEntry(uint16_t isRoundCorners, void (*callback)(char *));
tHandle SmartIO_PopupNumberEntry(uint16_t isRoundCorners, void (*callback)(char *));

// Use SmartIO_AddText(tHandle, (uint16_t loc_x, uint16_t loc_y, handle, "password hint") to add hint
tHandle SmartIO_PopupPasswordEntry(void (*callback)(char *));

// Text Box is meant to be narrower and can have a slice icon
// whereas multiline box takes the full width
tHandle SmartIO_PopupTextBox(uint16_t width, uint16_t nlines, char *text);
tHandle SmartIO_PopupMultilineBox(uint16_t nlines, char *text);
tHandle SmartIO_PopupCounter(uint16_t ndigits, uint32_t initial_value);

// width is in virtual pixels with a max of 320 virtual pixels
tHandle SmartIO_PopupProgressBar(uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_PopupProgressCircle(uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_PopupHGauge(uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_PopupVGauge(uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_PopupSemiCircularGauge(uint16_t max_mark, uint16_t initial_percentage);
tHandle SmartIO_PopupCircularGauge(uint16_t max_mark, uint16_t initial_percentage);
tHandle SmartIO_PopupBatteryLevel(uint16_t variation, uint16_t initial_percentage);
tHandle SmartIO_PopupRGBLed(uint16_t color, uint16_t initial_state);

tHandle SmartIO_PopupCustomHGauge(uint32_t start_color, uint32_t end_color, uint16_t initial_percentage);
tHandle SmartIO_PopupCustomVGauge(uint32_t start_color, uint32_t end_color, uint16_t initial_percentage);

tHandle SmartIO_PopupLabel(char *label);

tStatus SmartIO_AppendPopup(tHandle source, tHandle next);

// Slice text valid ONLY for on/off, 2-pos, incrementer, picker, number_selector, time_selector, OK_LinkTo, Progress Circle, Gauges, LED, Battery_level
tStatus SmartIO_AddText(tHandle, char * value);

// Works with any text field, but mostly useful for TEXT_BOX and MULTILINE_TEXT
tStatus SmartIO_ClearText(tHandle);
tHandle SmartIO_AddListItem(tHandle, char *);
tStatus SmartIO_FillColor(tHandle, uint32_t color);
tStatus SmartIO_SetFont(tHandle, uint16_t isSansSerifFont, uint16_t fontsize, uint32_t font_color);

tStatus SmartIO_UpdateIntValue      (tHandle, uint16_t);
tStatus SmartIO_UpdateIntValue2     (tHandle, uint16_t, uint16_t);
tStatus SmartIO_UpdateIntValue3     (tHandle, uint16_t, uint16_t, uint16_t);
tStatus SmartIO_UpdateStringValue   (tHandle, char *);

tStatus SmartIO_UpdateOnOffButton	(tHandle, uint16_t);
tStatus SmartIO_Update3PosButton	(tHandle, uint16_t);
tStatus SmartIO_UpdateIncrementer	(tHandle, uint16_t);
tStatus SmartIO_UpdateSlider	    (tHandle, uint16_t);
tStatus SmartIO_UpdateExpandableList(tHandle, uint16_t);
tStatus SmartIO_UpdatePicker	    (tHandle, uint16_t);
tStatus SmartIO_UpdateMultiSelector	(tHandle, uint16_t);
tStatus SmartIO_UpdateNumberSelector(tHandle, uint16_t);
tStatus SmartIO_UpdateCheckboxes	(tHandle, uint16_t);
tStatus SmartIO_UpdateRadioButtons	(tHandle, uint16_t);
tStatus SmartIO_UpdateCounter   	(tHandle, uint16_t);
tStatus SmartIO_UpdateProgressBar	(tHandle, uint16_t);
tStatus SmartIO_UpdateProgressCircle(tHandle, uint16_t);
tStatus SmartIO_UpdateHGauge	    (tHandle, uint16_t);
tStatus SmartIO_UpdateVGauge	    (tHandle, uint16_t);
tStatus SmartIO_UpdateSemiCircularGauge(tHandle, uint16_t);
tStatus SmartIO_UpdateCircularGauge	(tHandle, uint16_t);
tStatus SmartIO_UpdateCustomHGauge	(tHandle, uint16_t);
tStatus SmartIO_UpdateCustomVGauge	(tHandle, uint16_t);
tStatus SmartIO_UpdateBatteryLevel	(tHandle, uint16_t);

tStatus SmartIO_UpdateRGBLed        (tHandle, uint16_t color, uint16_t on_off);
tStatus SmartIO_UpdateTextBox       (tHandle, char *);

tHandle SmartIO_MakeSpacerSlice     (int);
tStatus SmartIO_AutoBalance         (tHandle);

// Page and object management
// These apply to any object, including page or other UI elements
//
// Enable implies show
// Show does not imply enable
//
// Disable does not imply hide
// Hide implies disable
tStatus SmartIO_EnableObject(tHandle);
tStatus SmartIO_DisableObject(tHandle);
tStatus SmartIO_ShowObject(tHandle);
tStatus SmartIO_HideObject(tHandle);
tStatus SmartIO_DeleteObject(tHandle);

// Static Objects
tHandle SmartIO_MakeLabel(uint16_t alignment, uint16_t height, char *label);
tHandle SmartIO_MakeLine(uint16_t loc_x, uint16_t loc_y, uint16_t end_x, uint16_t end_y, uint16_t color);
tHandle SmartIO_MakeCircle(uint16_t loc_x, uint16_t loc_y, uint16_t diameter_x, uint16_t diameter_y, uint16_t outline_color, uint16_t fill_color);
tHandle SmartIO_MakeRectangle(uint16_t loc_x, uint16_t loc_y, uint16_t end_x, uint16_t end_y, uint16_t outline_color, uint16_t fill_color);
tHandle SmartIO_MakeTriangle(uint16_t loc_x, uint16_t loc_y, uint16_t loc_z, uint16_t outline_color, uint16_t fill_color);

// To Be Defined
tHandle SmartIO_MakeIcon(uint16_t);
tHandle SmartIO_MakeGraphics(uint16_t);

// System(SMART.IO) Commands
unsigned char *SmartIO_ReadEEPROM(uint16_t address, uint16_t length);
tStatus SmartIO_WriteEEPROM(uint16_t address, uint16_t length, unsigned char *buffer);
tStatus SmartIO_SetLED(uint16_t led);
tStatus SmartIO_ClearLED(uint16_t led);
tStatus SmartIO_ToggleLED(uint16_t led);
uint32_t SmartIO_GenRandomNumber(void);
tStatus SmartIO_DebugUART(uint16_t);
// return utin32_t [2], always return the same ID from the same Smart.IO chip
uint32_t *SmartIO_GetUniqueID(void);

// Phone Commands
char *SmartIO_GetPhoneTime(void);
char *SmartIO_GetPhoneGPS(void);
tStatus SmartIO_Beep(void);
