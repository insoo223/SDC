//
//  Four7segX.cpp
//  SDC
//
//  Created by In soo Kim on 10/25/15.
//  Release to Public domain
//

#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"


Four7segX :: Four7segX()
{
    _7segABC_Num[0] = 0b00111111; // DP_IDXGFEDCBA
    _7segABC_Num[1] = 0b00000110;
    _7segABC_Num[2] = 0b01011011;
    _7segABC_Num[3] = 0b01001111;
    _7segABC_Num[4] = 0b01100110;
    _7segABC_Num[5] = 0b01101101;
    _7segABC_Num[6] = 0b01111100;
    _7segABC_Num[7] = 0b00000111;
    _7segABC_Num[8] = 0b01111111;
    _7segABC_Num[9] = 0b01100111;
    _7segABC_Num[10] = 0b00000000; //BLANK_IDX (10)
    _7segABC_Num[11] = 0b10000000; //DP_IDX (11)
    
    _7segABC_Num_UpsideDown[0] = 0b00111111; //0
    _7segABC_Num_UpsideDown[1] = 0b00110000; //1
    _7segABC_Num_UpsideDown[2] = 0b01011011; //2
    _7segABC_Num_UpsideDown[3] = 0b01111001; //3
    _7segABC_Num_UpsideDown[4] = 0b01110100; //4
    _7segABC_Num_UpsideDown[5] = 0b01101101; //5
    _7segABC_Num_UpsideDown[6] = 0b01100111; //6
    _7segABC_Num_UpsideDown[7] = 0b00111000; //7
    _7segABC_Num_UpsideDown[8] = 0b01111111; //8
    _7segABC_Num_UpsideDown[9] = 0b01111100; //9
    _7segABC_Num_UpsideDown[10] = 0b00000000; //BLANK_IDX (10)
    _7segABC_Num_UpsideDown[11] = 0b10000000; //DP_IDX (11)
    
    _7segABC_Alpha[0] = 0b01110111; //A
    _7segABC_Alpha[1] = 0b01111100; //b //_7segABC_Alpha[1] = 0b01111111; //B
    _7segABC_Alpha[2] = 0b01011000; //c //_7segABC_Alpha[2] = 0b00111001; //C
    _7segABC_Alpha[3] = 0b01011110; //d
    _7segABC_Alpha[4] = 0b01111011; //e
    _7segABC_Alpha[5] = 0b01110001; //F
    _7segABC_Alpha[6] = 0b01101111; //g
    _7segABC_Alpha[7] = 0b01110100; //h
    _7segABC_Alpha[8] = 0b00000100; //i //_7segABC_Alpha[8] = 0b00000110; //I
    _7segABC_Alpha[9] = 0b00001110; //J
    _7segABC_Alpha[10] = 0b01110000; //k
    
    _7segABC_Alpha[11] = 0b00111000; //L
    _7segABC_Alpha[12] = 0b00100011; //m
    _7segABC_Alpha[13] = 0b01010100; //n
    _7segABC_Alpha[14] = 0b01011100; //o //_7segABC_Alpha[14] = 0b00111111; //O
    _7segABC_Alpha[15] = 0b01110011; //p
    _7segABC_Alpha[16] = 0b01100111; //q
    _7segABC_Alpha[17] = 0b01010000; //r //_7segABC_Alpha[17] = 0b01110111; //R
    _7segABC_Alpha[18] = 0b01101101; //S
    _7segABC_Alpha[19] = 0b01111000; //t //_7segABC_Alpha[19] = 0b00000111; //T
    _7segABC_Alpha[20] = 0b00011100; //u
    _7segABC_Alpha[21] = 0b01110010; //v
    _7segABC_Alpha[22] = 0b01001111; //W
    _7segABC_Alpha[23] = 0b01110110; //X
    _7segABC_Alpha[24] = 0b01100110; //y
    _7segABC_Alpha[25] = 0b01001001; //Z
    _7segABC_Alpha[26] = 0b00000000; //blank
    
    _A=0; _B=2; _C=6; _D=4; _E=3; _F=1; _G=7; _DP=5;
    
    _74HC595pin[0] = _A;
    _74HC595pin[1] = _B;
    _74HC595pin[2] = _C;
    _74HC595pin[3] = _D;
    _74HC595pin[4] = _E;
    _74HC595pin[5] = _F;
    _74HC595pin[6] = _G;
    _74HC595pin[7] = _DP;
    
}//Four7segX


//----------------------------------------------------------
void Four7segX :: disp4digits(int num, int pos, int duration)
{
    int ones, tens, hundreds, thousands;
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    thousands=num/1000;
    hundreds=(num - thousands*1000)/100;
    tens=(num - thousands*1000 - hundreds*100)/10;
    ones=(num - thousands*1000 - hundreds*100 - tens*10);
    
    X.getCurrentX(&topX, &midX, &botX);
    
    //eliminate a remnant display at the 1000s digit
    X.ctrlAll (0x00, _BV(DIGIT_K_PIN), botX);
    
    //delay(gSingleDigitDelay);
    
    for(int k=0; k<duration; k++)
    {
        //mid74HC595: 0x01 - control 1s common cathode transistor
        topX = getTopX_Num(ones);
        if (pos & 0x01 == 1)
            topX |= getTopX_Num(DP_IDX);
        X.ctrlAll (topX, _BV(DIGIT_U_PIN) | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll (topX, ~_BV(DIGIT_U_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(gSingleDigitDelay);
        
        //mid74HC595: 0x02 - control 10s common cathode transistor
        // remove leading zeros
        if ( (thousands == 0) && (hundreds == 0) && (tens == 0))
            topX = getTopX_Num(BLANK_IDX);
        else
            topX = getTopX_Num(tens);
        
        if ((pos & 0x02)>>1 == 1)
            topX |= getTopX_Num(DP_IDX);
        
        X.ctrlAll (topX, _BV(DIGIT_T_PIN)  | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll (topX, ~_BV(DIGIT_T_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(gSingleDigitDelay);
        
        //mid74HC595: 0x20 - control 100s common cathode transistor
        //remove leading zeros
        if ( (thousands == 0) && (hundreds == 0) )
            topX = getTopX_Num(BLANK_IDX);
        else
            topX = getTopX_Num(hundreds);
        
        if ((pos & 0x04)>>2 == 1)
            topX |= getTopX_Num(DP_IDX);
        
        X.ctrlAll (topX, _BV(DIGIT_H_PIN)  | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll (topX, ~_BV(DIGIT_H_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(gSingleDigitDelay);
        
        //mid74HC595: 0x10 - control 1000s common cathode transistor
        //remove leading zeros
        
        
        if (thousands == 0)
            topX = getTopX_Num(BLANK_IDX);
        else
            topX = getTopX_Num(thousands);
        
        if ( (pos & 0x08)>>3 == 1)
            topX |= getTopX_Num(DP_IDX);
        
        X.ctrlAll (topX, _BV(DIGIT_K_PIN)  | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll (topX, ~_BV(DIGIT_K_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(gSingleDigitDelay);
        
    }//for(int k=0; k<duration; k++)
    
    //eliminate a remnant display in K-unit
    X.ctrlAll(0,_BV(DIGIT_K_PIN)  | midX, botX);
}//disp4digits

//-------------------------------------------------
void Four7segX :: disp4chars(char* str, int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    for (int k=0; k<duration; k++)
    {
        topX = getTopX_ABC(str[3]);
        midX = _BV(DIGIT_U_PIN);
        X.ctrlAll(topX, midX, botX);
        if (gNightMode)
        {
            midX &= ~_BV(DIGIT_U_PIN);
            X.ctrlAll(topX, midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        else
        {
            delay(gSingleDigitDelay);
        }
        
        topX = getTopX_ABC(str[2]);
        midX = _BV(DIGIT_T_PIN);
        X.ctrlAll(topX, midX, botX);
        if (gNightMode)
        {
            midX &= ~_BV(DIGIT_T_PIN);
            X.ctrlAll(topX, midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        else
        {
            delay(gSingleDigitDelay);
        }
        
        topX = getTopX_ABC(str[1]);
        midX = _BV(DIGIT_H_PIN);
        X.ctrlAll(topX, midX, botX);
        if (gNightMode)
        {
            midX &= ~_BV(DIGIT_H_PIN);
            X.ctrlAll(topX, midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        else
        {
            delay(gSingleDigitDelay);
        }
        
        topX = getTopX_ABC(str[0]);
        midX = _BV(DIGIT_K_PIN);
        X.ctrlAll(topX, midX, botX);
        if (gNightMode)
        {
            midX &= ~_BV(DIGIT_K_PIN);
            X.ctrlAll(topX, midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        else
        {
            delay(gSingleDigitDelay);
        }
    }//for(int k=0; k<duration; k++)
    //eliminate a remnant display in K-unit
    X.ctrlAll(0,_BV(DIGIT_K_PIN)  | midX, botX);
}//disp4chars

//----------------------------------------------------------
void Four7segX :: disp4digits_UpsideDown(int num, int pos, int duration)
{
    uint8_t topX, midX, botX;
    int ones, tens, hundreds, thousands;
    byte DP_IDXAdded, dispBLANK_IDX;
    
    thousands=num/1000;
    hundreds=(num - thousands*1000)/100;
    tens=(num - thousands*1000 - hundreds*100)/10;
    ones=(num - thousands*1000 - hundreds*100 - tens*10);
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    for(int k=0; k<duration; k++)
    {
        //mid74HC595: 0x01 - control 1s common cathode transistor
        topX = getTopX_Num_UpsideDown(ones);
        if (pos & 0x01 == 1)
            topX |= getTopX_Num_UpsideDown(DP_IDX);
        X.ctrlAll(topX, _BV(DIGIT_K_PIN) | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll(topX, ~_BV(DIGIT_K_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(NIGHT_BRIGHTNESS_DELAY);
        
        //mid74HC595: 0x02 - control 10s common cathode transistor
        // remove leading zeros
        if ( (thousands == 0) && (hundreds == 0) && (tens == 0))
            topX = getTopX_Num_UpsideDown(BLANK_IDX);
        else
            topX = getTopX_Num_UpsideDown(tens);
        
        if ((pos & 0x02)>>1 == 1)
            topX |= getTopX_Num_UpsideDown(DP_IDX);
        
        X.ctrlAll(topX, _BV(DIGIT_H_PIN)  | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll(topX, ~_BV(DIGIT_H_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(NIGHT_BRIGHTNESS_DELAY);
        
        //mid74HC595: 0x20 - control 100s common cathode transistor
        //remove leading zeros
        if ( (thousands == 0) && (hundreds == 0) )
            topX = getTopX_Num_UpsideDown(BLANK_IDX);
        else
            topX = getTopX_Num_UpsideDown(hundreds);
        
        if ((pos & 0x04)>>2 == 1)
            topX |= getTopX_Num_UpsideDown(DP_IDX);
        
        X.ctrlAll(topX, _BV(DIGIT_T_PIN)  | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll(topX, ~_BV(DIGIT_T_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(NIGHT_BRIGHTNESS_DELAY);
        
        //mid74HC595: 0x10 - control 1000s common cathode transistor
        //remove leading zeros
        
        if (thousands == 0)
            topX = getTopX_Num_UpsideDown(BLANK_IDX);
        else
            topX = getTopX_Num_UpsideDown(thousands);
        
        if ( (pos & 0x08)>>3 == 1)
            topX |= getTopX_Num_UpsideDown(DP_IDX);
        
        X.ctrlAll(topX, _BV(DIGIT_U_PIN)  | midX, botX);
        if (gNightMode)
        {
            X.ctrlAll(topX, ~_BV(DIGIT_U_PIN) & midX, botX);
            delay(NIGHT_BRIGHTNESS_DELAY);
        }
        
        delay(NIGHT_BRIGHTNESS_DELAY);
        
    }//for(int k=0; k<duration; k++)
    
}//disp4digits_UpsideDown

//----------------------------------------------------------

//-------------------------------------------------
uint8_t Four7segX :: getTopX_Num(uint8_t num)
{
    uint8_t targetByte, topByte=0;
    uint8_t i;
    
    // get A to G & DP_IDX bit pattern for "num"
    targetByte = _7segABC_Num[num];
    
    // convert 7seg ABC uint8_t into the uint8_t for 74HC595 Q-pin
    for (i=0; i<8; i++)
        if ( (targetByte >> i) & 0x01 )
            topByte |= 1 << _74HC595pin[i];
    
    return (topByte);
} //getTopX_Num

//-------------------------------------------------
uint8_t Four7segX :: getTopX_Num_UpsideDown(uint8_t num)
{
    uint8_t targetByte, topByte=0;
    uint8_t i;
    
    // get A to G & DP_IDX bit pattern for "num"
    targetByte = _7segABC_Num_UpsideDown[num];
    
    // convert 7seg ABC uint8_t into the uint8_t for 74HC595 Q-pin
    for (i=0; i<8; i++)
        if ( (targetByte >> i) & 0x01 )
            topByte |= 1 << _74HC595pin[i];
    
    return (topByte);
} //getTopX_Num_UpsideDown

//-------------------------------------------------
uint8_t Four7segX :: getTopX_ABC(char C)
{
    uint8_t targetByte, topByte=0;
    uint8_t i, arrIndex;
    
    //check space or blank char
    if ((int)C == 32)
        arrIndex = 26;
    else
        arrIndex = C - 'A';
    // get A to G & DP_IDX bit pattern for "num"
    targetByte = _7segABC_Alpha[arrIndex];
    
    // convert 7seg ABC uint8_t into the uint8_t for 74HC595 Q-pin
    for (i=0; i<8; i++)
        if ( (targetByte >> i) & 0x01 )
            topByte |= 1 << _74HC595pin[i];
    
    return (topByte);
} //getTopX_ABC



