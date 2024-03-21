#ifndef __LCD_XC_H__
#define __LCD_XC_H__

                                                             // set up the timing for the LCD delays
#define _XTAL_FREQ 2000000L                                  // device clock frequency (_XTAL_FREQ) used by XC8 for __delay_ms()

#ifndef FCY
    #define FCY _XTAL_FREQ/2                                 // device instruction frequency (FCY) used by XC16 for __delay_ms()
#endif

#define LCD_delay           5                                // ~5mS
#define LCD_Startup         15                               // ~15mS

// Command set for Hitachi 44780U LCD display controller
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_CURSOR_BACK     0x10
#define LCD_CURSOR_FWD      0x14
#define LCD_PAN_LEFT        0x18
#define LCD_PAN_RIGHT       0x1C
#define LCD_CURSOR_OFF      0x0C
#define LCD_CURSOR_ON       0x0E
#define LCD_CURSOR_BLINK    0x0F
#define LCD_CURSOR_LINE2    0xC0

// Macros recommended for new labs
#define mOPEN_LCD           LCDInit()
#define mLCD_PAN_RIGHT      LCDPutCmd(LCD_PAN_RIGHT)
#define mLCD_PAN_LEFT       LCDPutCmd(LCD_PAN_LEFT)
#define mLCD_CURSOR_BLINK   LCDPutCmd(LCD_CURSOR_BLINK)
#define mLCD_HOME           LCDPutCmd(LCD_HOME)
#define mLCD_CLEAR          LCDPutCmd(LCD_CLEAR)

#define mCURSOR_ON          LCDPutCmd(LCD_CURSOR_ON)
#define mCURSOR_OFF         LCDPutCmd(LCD_CURSOR_OFF)
#define mCURSOR_FWD         LCDPutCmd(LCD_CURSOR_FWD)
#define mCURSOR_BACK        LCDPutCmd(LCD_CURSOR_BACK)
#define mCURSOR_LINE2       LCDPutCmd(LCD_CURSOR_LINE2)

// Common function prototypes for XC8 & XC16
void LCDInit(void);
void LCDPutChar(unsigned char ch);
void LCDPutCmd(unsigned char cmd);

// Common legacy macros
#define LCDHome()           LCDPutCmd(LCD_HOME)              // legacy support
#define LCDClr()            LCDPutCmd(LCD_CLEAR)             // legacy support
#define LCDPutChar          LCDPutChar                       // legacy support

#if __XC8
    //----------------------------------------------------------------------
    // Definitions specific to the XC8 compiler
    //----------------------------------------------------------------------

    // display controller setup commands from page 46 of Hitachi datasheet
    #define FUNCTION_SET        0x28                         // 4 bit interface, 2 lines, 5x8 font
    #define ENTRY_MODE          0x06                         // increment mode
    #define DISPLAY_SETUP       0x0C                         // display on, cursor off, blink offd

    // Macro definitions specific to XC8 (not recommended for new code)
    #define text_display        LCDPutStr                    // legacy support
    #define LCDLine1()          LCDPutCmd(LCD_HOME)          // legacy support
    #define LCDLine2()          LCDPutCmd(LCD_CURSOR_LINE2)  // legacy support
    #define shift_cursor()      LCDPutCmd(LCD_CURSOR_FWD)    // legacy support
    #define cursor_on()         LCDPutCmd(LCD_CURSOR_ON)     // legacy support
    #define DisplayClr()        LCDPutCmd(LCD_CLEAR)         // Legacy support

    // Function prototypes
    void LCDInit(void );                                     // initialize the LCD module
    void LCDPutChar(unsigned char);                          // display an ASCii character in data mode
    void LCDPutCmd(unsigned char);                           // send an ASCii command to the LCD in instruction mode
    void LCDPutStr(unsigned char *);                         // display a string
    void LCDWriteNibble(unsigned char,unsigned char);

    //----------------------------------------------------------------------
    // Definitions specific to the PICDEM 2 Plus
    // These apply to the Black (2011), Green (2007) and Red (2001) versions
    //----------------------------------------------------------------------

    // single bit for selecting command register or data register
    #define instr        0
    #define data         1

    // Configuracion de pines y bits
    // Puerto de salida el sistema toma los ultimos pines es decir 4,5,6 y 7 para escribir
    #define LCD_PORT     LATB //Puerto de envio
    #define LCD_POR_DIR  TRISB //Direccion, se configura como salida
    
    //Pines de configuración de LCD pueden ser otros cambiar a preferencia del usuario
    #define LCD_EN       LATBbits.LB2                    // LCD enable
    #define LCD_RW       LATBbits.LB1                      // LCD read/write line
    #define LCD_RS       LATBbits.LB0                   // LCD register select line

    //Direcciones preestablecidas para configurar como salida
    #define LCD_EN_DIR   TRISBbits.RB2
    #define LCD_RW_DIR   TRISBbits.RB1
    #define LCD_RS_DIR   TRISBbits.RB0
#endif                                                      // __XC8


#if __XC16
    //----------------------------------------------------------------------
    // Definitions specific to XC16 compiler
    //----------------------------------------------------------------------

    // display controller setup commands from page 46 of Hitachi datasheet
    #define FUNCTION_SET        0x3C                        // 8 bit interface, 2 lines, 5x8 font
    #define ENTRY_MODE          0x06                        // increment mode
    #define DISPLAY_SETUP       0x0C                        // display on, cursor off, blink off

    // Macro definitions (not recommended for new code)
    #define LCDCurFwd()         LCDPutCmd(LCD_CURSOR_FWD)   // legacy support
    #define LCDCurBack()        LCDPutCmd(LCD_CURSOR_BACK)  // legacy support
    #define LCDPanLeft()        LCDPutCmd(LCD_PAN_LEFT)     // legacy support
    #define LCDPanRight()       LCDPutCmd(LCD_PAN_RIGHT)    // legacy support
    #define LCDClear()          LCDPutCmd(LCD_CLEAR)        // legacy support
    #define LCDPut              LCDPutChar                  // legacy support
    #define LCDL1Home()         LCDPutCur(0,0)              // legacy support
    #define LCDL2Home()         LCDPutCur(1,0)              // legacy support

    // Specific function prototypes for XC16
    void LCDPutStr(char *str);
    void LCDPutCur(char row, char col);
    void LCDPutInt(long num, char radix);
    void LCDPutUInt(unsigned long num, char radix);
    void LCDPutFloat(float fnum, char precision);
 
    //----------------------------------------------------------------------
    // Definitions specific to the Explorer 16
    //----------------------------------------------------------------------

    // NOTE: To use the macros below, YOU must have previously defined _XTAL_FREQ
    #define __delay_ms(x) Wait((unsigned long)((x)*(_XTAL_FREQ/22000.0)))
    inline static void Wait(unsigned long count) {while(count) count--;}   // 11 cycles per iteration + 17 overhead

    #define MAX_DIGITS          6
    #define BIN                 2
    #define OCT                 8
    #define DEC                 10
    #define HEX                 16
#endif                                                      // __XC16

#endif                                                      // __LCD_XC_H__
