//
//  ButtonMenu.cpp
//  SDC
//
//  Created by In soo Kim on 10/29/15.
//  Release to Public domain
//

#include "Arduino.h"
#include "ButtonMenu.h"

//--------------------------------------------------
BUTTONMENU :: BUTTONMENU()
{
    _curTime = millis();
    _prevTime = _curTime;
    
    _done_initOpMode = false;
    
}//ButtonMenu

//----------------------------------------------------
void BUTTONMENU :: selectOpMode()
{
    initOpMode(); //do only once.
    
    switch (_playMode)
    {
        case MENU_SYSTEM:
            //_7X.disp4chars("MENU", 50);
            menuSystem();
            break;
            
        case UX_PRG:
            break;
            
        case SELF_DRIVING_IR:
            //_7X.disp4chars("DRVG", 50);
            //test_IRX_runSDC();
            _ir.runSDC(false, false);
            break;
        case SELF_DRIVING_UX:
            break;
    }
    
    //X.ctrlAll(0,0,0);
}//selectOpMode

//----------------------------------------------------
void BUTTONMENU :: menuSystem()
{
    
    //check alternative menu switch
    if ( _cd4021.read2ndSlideSW() == HIGH)
        _altCnt = 1;
    else
        _altCnt = 0;
    
    CountButton();
    
    _curTime = millis();
    _lapse = _curTime - _prevTime;
    
    if (_lapse > menuSelCompleteINTERVAL)
    {
        if (_menuCnt != 0)
            _prevMenu=_menuCnt;

        if (_altCnt == 0)
            doMainMenu();
        else if (_altCnt == 1)
            doAltMenu();
        else
        {
            _menuCnt = 0;
            _altCnt = 0;
        }
        
        _prevTime = millis();
        _lapse = 0;
    }//if (_lapse > menuSelCompleteINTERVAL)
    
}//menuSystem

//------------------------------------
void BUTTONMENU :: doMainMenu()
{
    switch (_menuCnt)
    {
        case 1: //main menu 1
            //_7X.disp4digits(1001, 0, 50);
            //display pure IR rx value of each side
            _ir.getPure_AllIRs(true, true);
            _menuCnt = 0;
            _altCnt = 0;
            break;
            
        case 2: //main menu 2
            //_7X.disp4digits(1002, 0, 50);
            _ir.saveRefPure_AllIR(true, true);
            _menuCnt = 0;
            _altCnt = 0;
            break;
            
        case 3: //main menu 3
            //_7X.disp4digits(1003, 0, 50);
            //display time, temperature, and humidity
            _rt.displayTime_4digit7seg();
            _dht11.displayTempHumidity(100);
            _menuCnt = 0;
            _altCnt = 0;
            break;
            
        case 4: //main menu 4
            _7X.disp4digits(1004, 0, 50);
            _menuCnt = 0;
            _altCnt = 0;
            break;
            
        case 5: //main menu 5
            _7X.disp4digits(1005, 0, 50);
            _menuCnt = 0;
            _altCnt = 0;
            break;
            
        case 6: //main menu 6
            _7X.disp4digits(1006, 0, 50);
            _menuCnt = 0;
            _altCnt = 0;
            break;
            //main default menu
        default:
            if (_prevMenu == 0)
                _7X.disp4digits(0,0,30);
            _menuCnt = _prevMenu;
            _altCnt = 0;
    }//switch
    
}//doMainMenu

//------------------------------------
void BUTTONMENU :: doAltMenu()
{
    switch (_menuCnt)
    {
            //alternative menu 1
        case 1: //and (_altCnt == 1)
            _7X.disp4digits(2001, 0, 50);
            _menuCnt = 0;
            _altCnt = 1;
            break;
            
            //alternative menu 2
        case 2: //and (_altCnt == 1)
            _7X.disp4digits(2002, 0, 50);
            _menuCnt = 0;
            _altCnt = 1;
            break;
            
            //alternative menu 3
        case 3: //and (_altCnt == 1)
            _7X.disp4digits(2003, 0, 50);
            _menuCnt = 0;
            _altCnt = 1;
            break;
            
            //alternative menu 4
        case 4:
            _7X.disp4digits(2004, 0, 50);
            _menuCnt = 0;
            _altCnt = 1;
            break;
            
            //alternative menu 5
        case 5:
            _7X.disp4digits(2005, 0, 50);
            _menuCnt = 0;
            _altCnt = 1;
            break;
            
            //alternative menu 6
        case 6:
            //initialize EEPROM of Arduino
            _ir.initEEPROM();
            _menuCnt = 0;
            _altCnt = 1;
            break;
            
        default:
            if (_prevMenu == 0)
                _7X.disp4digits(0,0,30);
            _menuCnt = _prevMenu;
            _altCnt = 1;
    }//switch (_menuCnt)
}//doAltMenu

//------------------------------------
void BUTTONMENU :: CountButton()
{
    //if pressed, LOW
    if (_cd4021.readTactileButton() == LOW)
    {
        delay(200); // for debounce
        _menuCnt++;
        ShowInterimSelect();
        _prevTime = millis();
    }//if (digitalRead(button) == HIGH)
}//CountButton

//------------------------------------
void BUTTONMENU :: ShowInterimSelect()
{
    _7X.disp4digits(_menuCnt, 0, 10);
    
}//ShowInterimSelect

//----------------------------------------------------
void BUTTONMENU :: initOpMode()
{
    while (!_done_initOpMode)
    {
        if ( _cd4021.readTactileButton() == LOW)
        {
            _playMode = SELF_DRIVING_IR;
        }
        else //default
        {
            _playMode = MENU_SYSTEM;
        }
        _done_initOpMode = true;
    }//while (!_done_initOpMode)
}//initOpMode

