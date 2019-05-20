/*
 ButtonMenu.h
 SDC
 
 Created by In soo Kim on 10/29/15.
 Release to Public domain
 
 Updated on 10/29/15
 */

#ifndef ButtonMenu_h
#define ButtonMenu_h

#include "Arduino.h"
#include "Four7segX.h"
#include "CD4021X.h"
#include "RTX.h"
#include "DHT11X.h"
#include "IRX.h"

//----------------------------------------------
//--- operation system ID
//----------------------------------------------
#define MENU_SYSTEM      0
#define SELF_DRIVING_IR  1
#define SELF_DRIVING_UX  2
#define UX_PRG           3

//----------------------------------------------
//--- menu system
//----------------------------------------------
#define menuSelCompleteINTERVAL  2000

class BUTTONMENU
{
    
public:
    BUTTONMENU();
    void selectOpMode();
    
private:
    void initOpMode();
    void ShowInterimSelect();
    void CountButton();
    void menuSystem();
    void doMainMenu();
    void doAltMenu();
    
    unsigned long _prevTime, _curTime, _lapse;
    byte _playMode;
    bool _done_initOpMode;
    byte _menuCnt, _prevMenu, _altCnt;
    
    Four7segX _7X;
    CD4021X _cd4021;
    RTX _rt;
    DHT11X _dht11;
    IRX _ir;
    
    
};
#endif /* ButtonMenu_h */
