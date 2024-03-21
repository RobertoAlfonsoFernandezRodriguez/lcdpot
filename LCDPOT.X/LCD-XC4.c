#include <xc.h>
#include "LCD-XC4.h"

#if  __XC8
    void LCDInit()
    {
        #if _16F1937 | _18F46K22
            ANSELD = 0;
        #endif

        #if _16F1937
            OSCCONbits.IRCF = 0xD;      // for 1937 to run at 4 MHz
        #endif

        LCD_POR_DIR=0x0F;
        LCD_EN_DIR=0;
        LCD_RW_DIR=0;
        LCD_RS_DIR=0;
        
        
        __delay_ms(LCD_Startup);        // required by display controller to allow power to stabilize
        LCDPutCmd(0x32);                // required by display initialization
        LCDPutCmd(FUNCTION_SET);        // set interface size, # of lines and font
        LCDPutCmd(DISPLAY_SETUP);       // turn on display and sets up cursor
        mLCD_CLEAR;
        LCDPutCmd(ENTRY_MODE);          // set cursor movement direction
    }

    /*******************************************************************
    * FUNCTION:     LCDWriteNibble()
    * INPUTS:       data byte to be written to display
    *               bit for writing to commnd or data register
    * OUTPUTS:      None
    * DESCRIPTION:  Fuction to write a nibble
    * AUTHOR:       Mark Reynolds
    * DATE:         Jan 2013
    * REVISIONS:    Jan 2013 - Initial Version
    *******************************************************************/
    void LCDWriteNibble(unsigned char ch,unsigned char b)
    {
        //ch = ch >> 4;                   // always send the upper nibble
        ch = ch & 0xF0;                 // mask off the nibble to be transmitted

        LCD_PORT = LCD_PORT & 0x0F;     // clear the lower half of LCD_PORT
        LCD_PORT = LCD_PORT | ch;       // move the nibble onto LCD_PORT

        LCD_RS = b;                     // set data/instr bit to 0 = insructions; 1 = data
        LCD_RW = 0;                     // RW - set write mode
        LCD_EN = 1;                     // set up enable before writing nibble
        LCD_EN = 0;                     // turn off enable after write of nibble
    }

    /*******************************************************************
    * FUNCTION:     LCDPutChar()
    * INPUTS:       ch = character to send
    * OUTPUTS:      None
    * DESCRIPTION:  Writes character to LCD at current cursor position
    * AUTHOR:       Mark Reynolds
    * DATE:         Jan 2013
    * REVISIONS:    Jan 2013 - Initial Version
    *******************************************************************/
    void LCDPutChar(unsigned char ch)
    {
        __delay_ms(LCD_delay);
        LCDWriteNibble(ch,data);
        ch = ch << 4;
        LCDWriteNibble(ch,data);
    }

    /*******************************************************************
    * FUNCTION:     LCDPutCmd()
    * INPUTS:       ch = character to send
    * OUTPUTS:      None
    * DESCRIPTION:  Writes character to LCD command register
    * AUTHOR:       Mark Reynolds
    * DATE:         Jan 2013
    * REVISIONS:    Jan 2013 - Initial Version
    *******************************************************************/
    void LCDPutCmd(unsigned char ch)
    {
        __delay_ms(LCD_delay);
        LCDWriteNibble(ch,instr);
        ch = ch << 4;
        __delay_ms(1);
        LCDWriteNibble(ch,instr);
    }

    /*******************************************************************
    * FUNCTION:     LCDPutStr()
    * INPUTS:       *ptr = Pointer to string to send
    * OUTPUTS:      None
    * DESCRIPTION:  Writes string to LCD at current cursor position
    * AUTHOR:       Mark Reynolds
    * DATE:         Jan 2013
    * REVISIONS:    Jan 2013 - Initial Version
    *******************************************************************/
    void LCDPutStr(unsigned char *ptr)
    {
        while(*ptr) LCDPutChar(*(ptr++));
    }
#endif                                  // __XC8


#if __XC16
    void LCDInit()
    {
        PMCON = 0x83BF;                 // configure the PMP
        PMMODE = 0x3FF;
        PMAEN = 0x0001;                 // display controller defines 4 character to setup LCD

        __delay_ms(LCD_Startup);        // required by display controller to allow power to stabilize
        LCDPutCmd(0x32);                // required by display initialization
        LCDPutCmd(FUNCTION_SET);        // set interface size, # of lines and font
        LCDPutCmd(DISPLAY_SETUP);       // turn on display and sets up cursor
        mLCD_CLEAR;
        LCDPutCmd(ENTRY_MODE);          // set cursor movement direction
    }

    /*******************************************************************
    * FUNCTION:     LCDPutChar()
    * INPUTS:       ch = character to send
    * OUTPUTS:      None
    * DESCRIPTION:  Writes character to LCD at current cursor position
    * AUTHOR:       Mark Reynolds
    * DATE:         Feb 2013
    * REVISIONS:    Feb 2013 - Initial Version
    *******************************************************************/
    void LCDPutChar(unsigned char ch)
    {
        PMADDR = 0x0001;
        PMDIN1 = ch;
        __delay_ms(LCD_delay);
    }

    /*******************************************************************
    * FUNCTION:     LCDPutCmd()
    * INPUTS:       cmd = command to send
    * OUTPUTS:      None
    * DESCRIPTION:  Writes a command with a fast delay
    * AUTHOR:       Mark Reynolds
    * DATE:         Feb 2013
    * REVISIONS:    Feb 2013 - Initial Version
    *******************************************************************/
    void LCDPutCmd(unsigned char cmd)
    {
        PMADDR = 0x0000;
        PMDIN1 = cmd;
        __delay_ms(LCD_delay);
    }

    /*******************************************************************
    * FUNCTION:     LCDPutCur()
    * INPUTS:       row = LCD row, col = LCD column (both zero based)
    * OUTPUTS:      None
    * DESCRIPTION:  Moves cursor to specific position on display
    *               Valid values for row: 0 - 1
    *               Valid values for col: 0 - 15
    * AUTHOR:       Rob Ostapiuk
    * DATE:         24 JAN 2008
    * REVISIONS:    24 JAN 2008 - Initial Version
    *******************************************************************/
    void LCDPutCur(char row, char col)
    {
        row <<= 6;
        LCDPutCmd(0x80|row|col);
    }

    /*******************************************************************
    * FUNCTION:     LCDPutStr()
    * INPUTS:       *ptr = Pointer to string to send
    * OUTPUTS:      None
    * DESCRIPTION:  Writes string to LCD at current cursor position
    * AUTHOR:       Mark Reynolds
    * DATE:         Jan 2013
    * REVISIONS:    Jan 2013 - Initial Version
    *******************************************************************/
    void LCDPutStr(char *ptr)
    {
        while(*ptr) LCDPutChar(*(ptr++));
    }

    /*******************************************************************
    * FUNCTION:     LCDPutFloat()
    * INPUTS:       num = integer number to display
    *               precision = desired number of decimal places
    * OUTPUTS:      None
    * DESCRIPTION:  Writes a number to LCD at current cursor position
    * AUTHOR:       Rob Ostapiuk
    * DATE:         24 JAN 2008
    * REVISIONS:    24 JAN 2008 - Initial Version
    *******************************************************************/
    void LCDPutFloat(float fnum, char precision)
    {
        int i;                              // Loop index variable
        unsigned char digits[MAX_DIGITS];   // Array to hold individual digits
                                            // NOTE: digits stored in reverse order
                                            // LSD is stored in digits[0]
        long num;                           // Holds long integer version of fnum

        // If number is zero, simply display a zero and return
        if (fnum == 0)
        {
            LCDPutChar('0');
            return;
        }

        // Multiply by 10 until all desired decimal places are on left of
        // the decimal point
        i = precision;
        while (i > 0)
        {
            fnum *= 10;
            i--;
        }
        num = (long)fnum;

        // If number is negative, display minus sign and invert the
        // sign of num for conversion
        if (num < 0)
        {
            num = -num;
            LCDPutChar('-');
        }

        // Break up number into its individual digits
        // and clear unused elements of digits array
        for (i=0; i<MAX_DIGITS; i++)
        {
            digits[i] = 0;
            if (num == 0) continue;
            digits[i] = (char)(num % 10);
            num /= 10;
        }

        // Display the digits...
        // Setup loop index to end of array and work back to front
        i = MAX_DIGITS-1;

        while (digits[i] == 0) i--;  // Skip leading zeros

        while (i >= 0)
        {
            // Insert decimal point for desired precision
            if (i == (precision-1))
                LCDPutChar('.');

            // Send ASCII value of digit to display
            LCDPutChar(digits[i]+48);

            // Move to next digit
            i--;
        }
    }

    /*******************************************************************
    * FUNCTION:     LCDPutInt()
    * INPUTS:       num = integer number to display
    *               radix = base to use when displaying number
    * OUTPUTS:      None
    * DESCRIPTION:  Writes a number to LCD at current cursor position
    * AUTHOR:       Rob Ostapiuk
    * DATE:         24 JAN 2008
    * REVISIONS:    24 JAN 2008 - Initial Version
    *******************************************************************/
    void LCDPutInt(long num, char radix)
    {
        if (num < 0)
        {
            num = -num;
            LCDPutChar('-');
        }
        LCDPutUInt(num, radix);
    }

    /*******************************************************************
    * FUNCTION:     LCDPutUInt()
    * INPUTS:       num = unsigned integer number to display
    *               radix = base to use when displaying number
    * OUTPUTS:      None
    * DESCRIPTION:  Writes a number to LCD at current cursor position
    * AUTHOR:       Rob Ostapiuk
    * DATE:         24 JAN 2008
    * REVISIONS:    24 JAN 2008 - Initial Version
    *******************************************************************/
    void LCDPutUInt(unsigned long num, char radix)
    {
        int i;                      // Loop index variable
        /*--------------------------------------------------------------
        digits[] - Array to hold individual digits.
        NOTE: Digits stored in reverse order: LSD is in digits[0]
        --------------------------------------------------------------*/
        unsigned char digits[MAX_DIGITS];
        /*--------------------------------------------------------------
        oldDigits[] - Array to hold previous value of digits[].
        Used to know which digits need to be updated on the display.
        --------------------------------------------------------------*/
        static unsigned char oldDigits[MAX_DIGITS];

        /*--------------------------------------------------------------
        (0) If the number is zero, just display a zero and exit.
        --------------------------------------------------------------*/
        if (num == 0L)
        {
            LCDPutChar('0');
            return;
        }

        /*--------------------------------------------------------------
        (1) Initialize oldDigits[] so that a digit
        change may be detected on the first pass.
        --------------------------------------------------------------*/
        for (i=0; i<MAX_DIGITS; i++)
            oldDigits[i] = 0;

        /*--------------------------------------------------------------
        (2) If radix is hex, display a 0x prefix.
        --------------------------------------------------------------*/
        if (radix == HEX) LCDPutStr("0x");

        /*--------------------------------------------------------------
        (3) Break up number into its individual digits and clear any
        unused elements of the digits array
        --------------------------------------------------------------*/
        for (i=0; i<MAX_DIGITS; i++)
        {
            digits[i] = 0;
            if (num == 0) continue;
            digits[i] = (char)(num % radix);
            num /= radix;
        }

        /*--------------------------------------------------------------
        (4) Write the digits to the LCD...
            Setup loop index to end of array and work back to front.
        --------------------------------------------------------------*/
        i = MAX_DIGITS-1;

        /*--------------------------------------------------------------
        (5) Skip over leading zeros.
        --------------------------------------------------------------*/
        while (digits[i] == 0) i--;

        /*--------------------------------------------------------------
        (6) For each remaining digit...
        --------------------------------------------------------------*/
        while (i >= 0)
        {
            /*----------------------------------------------------------
            (7) If the digit has changed since last written...
            NOTE: CANNOT PROPERLY HANDLE DOWN COUNT (trailing
            zeros not cleared from display)
            ----------------------------------------------------------*/
            if ((digits[i] != oldDigits[i]) || (digits[i] == 0))
            {
                /*------------------------------------------------------
                (8) If the digit has a value of 0 through 9, write
                its ASCII value to display ('0' = ASCII 48 decimal).
                This works for radix = BIN, OCT or DEC.
                ------------------------------------------------------*/
                if (digits[i] <= 9)
                    LCDPutChar(digits[i]+48);

                /*------------------------------------------------------
                (9) If the digit has a value of 10 through 15, write
                the ASCII value for the letters A through F to the
                display ('A' = ASCII 55 decimal).
                This is only used for radix = HEX.
                ------------------------------------------------------*/
                else
                    LCDPutChar(digits[i]+55);
            }
            /*----------------------------------------------------------
            (10) If the digit hasn't changed since last written,
             move the cursor one position to the right.
            ----------------------------------------------------------*/
            else
            {
                LCDCurFwd();
            }
            /*----------------------------------------------------------
            (11) Move to the next digit in digits[].
            ----------------------------------------------------------*/
            i--;
        }

        /*--------------------------------------------------------------
        (12) Copy current digits[] to oldDigits[]
        --------------------------------------------------------------*/
        for(i=0; i<MAX_DIGITS; i++)
        {
            oldDigits[i] = digits[i];
        }
    }
#endif                                   // __XC16



/*********************************************************************
* FileName:        LCD-XC.c
* Dependencies:    See INCLUDES section below
* Processor:       PIC16 or PIC18 on PICDEM 2 plus
* Processor:       PIC24 or PIC33 on Explorer 16
* Compiler:        xc8 & xc16
* Company:         Microchip Technology, Inc.
* Version:         1.2
* Software License Agreement
*
* The software supplied herewith by Microchip Technology Incorporated
* (the \93Company\94) for its PICmicro\AE Microcontroller is intended and
* supplied to you, the Company\92s customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN \93AS IS\94 CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
*
* Author            Date        Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Stu Chandler      Mar 2013    v1.3 replaced __XCn__ with __XCn due to parser bug
* Mark Reynolds     Mar 2013    v1.2 added missing macros for backward compatibility
* Mark Reynolds     Feb 2013    v1.1 First Release for XC8 & XC16; Supports PICDEM2 & Explorer 16
* Mark Reynolds     Jan 2013    v1.0 First release for xc8; PICDEM2 standard driver
*********************************************************************************************************************
*       PICDEM2 Plus setup details
*
*   LCD Module on RoHS (green soldermask) PICDEM2 Plus is Ocular OM16214
*   LCD Module on pre-RoHS (red soldermask) PICDEM2 Plus is NT7605
*   Display controller is based on the Hitachi 44780U
*   Display is 16 characters x 2 lines; 5x7 dot resolution w/ cursor or 5x8 w/o cursor
*   PICDEM2 Plus is wired for a 4 bit interface
*
**********************************************************************************************************************
*   LCD interface on PICDEM 2 PLUS board: (28 PIN REFERENCE)
*   PORTD RD7:RD4 are control/power lines to LCD module
*   RD7 = LCD VCC   : on=1; off=0
*   RD6 = LCD EN    : LCD enable signal
*   RD5 = RD/WR     : LCD Read/Write operation, (Read = 1; Write = 0)
*   RD4 = RS        : LCD Register Select, (Instruction Reg = 0; Data Reg = 1)
*
*   PORTD RD3:RD0 are data lines for LCD Module -- LCD Module used in 4-bit mode
*   RD3 = LCD D7
*   RD2 = LCD D6
*   RD1 = LCD D5
*   RD0 = LCD D4
*
*********************************************************************************************************************/
