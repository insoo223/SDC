
//----------------------------------------------
//--- MACRO
//----------------------------------------------

// Get the first byte from any size of data type
// A global variable is always takes 4 bytes in its size.
#define _BYTE(X) (X & 0xFF)

// Converts a bit number into a byte value.
#define _BV(X) (1 << X)

//----------------------------------------------
//--- Pin assignments for tripple 74HC595s
//----------------------------------------------

// Bit location of 74HC595 sector 
// A "sector" tells which of three 74HC595 chips holds a corresponding signal: top, middle, or bottom 
#define SECTOR_BIT_LOCATION_74HC595   4  

// bit mask for a corresponding signal ID
#define DATA_BITS_MASK_74HC595   0x07

//--- bottom 74HC595 pin assignments
// signal controlled by "trippleXwrite"function has a sector & pin number
#define MR_FWDREV   0x01
#define ML_FWDREV   0x02 
#define MR_ONOFF    0x03
#define ML_ONOFF    0x04
#define PGM_START   0x05
#define GREEN_LED   PGM_START
#define PGM_CMD     0x06
#define YELLOW_LED  PGM_CMD
#define PGM_END     0x07
#define RED_LED     PGM_END

//--- middle 74HC595 pin assignments
#define IR_FRONT   0x10 //power pin to the front IR
#define IR_LEFT    0x11 //power pin to the left IR
#define IR_RIGHT   0x12 //power pin to the right IR
#define Ux_TRIG    0x13 //trigger pin to the Ultrasound transducer

// signal controlled by "control74HC595"function only has pin number
/* ver01 - 3 pcs boards
#define DIGIT_K_PIN  4  // Kilo(1000s) digit control pin
#define DIGIT_H_PIN  5  // Hundres(100s) digit control pin
#define DIGIT_T_PIN  6  // Tens(10s) digit control pin
#define DIGIT_U_PIN  7  // Units(1s) digit control pin
*/
#define DIGIT_K_PIN  3  // Kilo(1000s) digit control pin
#define DIGIT_H_PIN  37 //2  // Hundres(100s) digit control pin
#define DIGIT_T_PIN  33 //1  // Tens(10s) digit control pin
#define DIGIT_U_PIN  10 //0  // Units(1s) digit control pin

//--- top 74HC595 pin assignments

/*
#define LATCH4digit7seg   0x20
#define CLOCK4digit7seg   0x21
#define DATA4digit7seg    0x22
*/

#define pi 3.14

#define DP 17

#define IR_Rcv_FRONTpin A0
#define IR_Rcv_LEFTpin A1
#define IR_Rcv_RIGHTpin A2
//#define MR_SPEED A3 //arduino analog pin A3

//pin assignment at CD4021BE
//#define PGM_MODEpinCD4021         2 //ver01  
#define PGM_MODEpinCD4021           0   
//#define TactileButtonPinCD4021    1 //ver01
#define TactileButtonPinCD4021      1 
#define SlideSwitch2ndPinCD4021     2 
//#define UxECHOpinCD4021     0  // Echo Pin
//#define DHT11_DATApinCD4021 0 //used as both input and output, so that connect directly to Arduino digital pin 
//#define MR_SPEEDpinCD4021 0 // too fast, so that connect directly to Arduino digital pin

#define EN1pin 10   // Arduino D10 pin
#define EN2pin 9  // Arduino D9 pin

// bottom 74HC595 pin
#define MLPpin 0
#define MLNpin 1
#define MRPpin 2
#define MRNpin 3

//#define MR_SPEEDpin   7 //arduino digital pin 7
#define MR_SPEEDpin   11 //arduino digital pin 11

//#define dht_dpin      8  //ver01 arduino digital pin 7
#define dht_dpin      12  //arduino digital pin 12
//#define UxECHOpin     13 
#define UxECHOpin     13 

// singlular operation: only the currently stated command will be exerted
#define SOLE_MODE 0
// add operational mode on top of other working state
// the currently stated command will be collaborated with the previouly set command state
#define FRIEND_MODE curState74HC595 

// duration of each operation(move fwd, reverse, left, right etc.)
// for demo purpose
#define OP_DURATION 4000 

#define RECT_ITERATION 100 
#define RECT_TL_ITERATION 80 
#define SHORT_TURN_ITERATION 50 

#define FWD_DURATION 200 // [ms]
#define BWD_DURATION 300 // [ms]
#define STOP_DURATION 500 // [ms]
#define STOP_DURATION_AFTER_TURN 500 // [ms]

#define speedCheckInterval 200 //ms
//num of machine clock counts to get speed
#define quickCheckSpeedIteration 20000 //times
#define NUMSLOTS 20

#define FWD_MODE 0
#define TURN_LEFT_MODE 1000
#define TURN_RIGHT_MODE 2000
#define BWD_MODE 3000

#define MAX_TURN_RIGHT_IN_A_ROW 3
#define MAX_TURN_LEFT_IN_A_ROW 3

#define MAX_ADC 1023

#define LIR_ABS_TURN (1023*0.6)
#define RIR_ABS_TURN (1023*0.6)

#define ADCdiff_30_15cm  50
#define REF_DIST  30 // no 15, only  30

#define IR_SETmode 0
#define IR_RUNmode 1

#define COMPENSATION_REF_FIR15 0.7

#define LED_ON_DURATION 0 //[ms]
#define LED_OFF_DURATION 0 //[ms]

#define EEPROM_READ_SAFEMARGIN 10 //[ms]

// location of the starting address of the current IR sensitivity reference value 
// 0 means the 1st address of EEPROM contains the starting address of IR sensitivity measured beforehand
#define EEPROM_IR_SENSITIVITY_ADDR_PTR 0 

#define EEPROM_IR_SENSITIVITY_FRONT_OFFSET   0x00
#define EEPROM_IR_SENSITIVITY_LEFT_OFFSET    0x01
#define EEPROM_IR_SENSITIVITY_RIGHT_OFFSET   0x02

#define EEPROM_IR_SENSITIVITY_MAX_ADDR  250

#define IR_ONOFF_SAFEMARGIN 1 //[ms]

#define DISP_4digit7seg_ITERATION_NORMAL     50 // Comfortable viewing time for 4digit 7segment LED
#define DISP_4digit7seg_ITERATION_DATE       50 
#define DISP_4digit7seg_ITERATION_DAY        50 
#define DISP_4digit7seg_ITERATION_TIME       1000
#define DISP_4digit7seg_ITERATION_TEMPHUMID  50


#define SET_IR_SENSITIVITY_READY_ON_DURATION 1000 // [ms]
#define SET_IR_SENSITIVITY_READY_OFF_DURATION 2000 // [ms]
#define SET_IR_SENSITIVITY_COMPLETE_DURATION 300 // [ms]
#define SET_IR_SENSITIVITY_NEXT_DURATION 3000 // [ms]

#define PREFIX_DISP_IR_FRONT 1000
#define PREFIX_DISP_IR_LEFT  2000
#define PREFIX_DISP_IR_RIGHT 3000



