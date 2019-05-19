int FcombinedRcv, FambientRcv, FpureRcv, prevFpureRcv;
int LcombinedRcv, LambientRcv, LpureRcv, prevLpureRcv;
int RcombinedRcv, RambientRcv, RpureRcv, prevRpureRcv;

//Pin connected to ST_CP of 74HC595 & CD4021BE
int latchPin, latch4021;
//Pin connected to SH_CP of 74HC595 & CD4021BE
int clockPin, clock4021;  
////Pin connected to DS of 74HC595 & CD4021BE
int dataPin, data4021;

char LEDtest[9] = {
		0x01, //1st LED
		0x02, //2nd
		0x04, //3rd
		0x08, //4th
		0x10, //5th LED
		0x20, //6th
		0x40, //7th		
                0x80 //8th
};

char OnesDigit[18] = {
		0b11101110, //0
		0b01000010, //1
		0b10110110, //2
		0b11010110, //3
		0b01011010, //4
		0b11011100, //5
		0b11111000, //6
		0b01000110, //7
		0b11111110, //8
		0b01011110, //9  
		0b01111110, //A  
		0b11111000, //b  
		0b10110000, //c  
		0b11110010, //d  
		0b10111100, //E  
		0b00111100, //F  
		0b00000000, //Blank (16)
		0b00000001 //DP (17)
	};//OnesDigit[10]


bool UNO = false; //false true

char curState74HC595=0x00; //0x00 0x08
char curBottomX=0x00, curMiddleX=0x00, curTopX=0x00;

bool doneTL=false;
bool doneTR=false;

const float tireD=6.60;
//const float  pi=3.14;
unsigned long cntOpenSlot=0, cntTirePerimeter=0;
int prevState=0, curState=0;
float tirePerimeter, travelDist, travelSpeed, prevSpeed;
unsigned long lapse, prevTime, curTime; 
unsigned long startTime,loopLapse, loopPrevTime=0;

// ADC value of Front IR sensor at 30cm away from an object using 3.7V LIPO battery
int FIR30; 
// ADC value of Front IR sensor at 15cm away from an object using 3.7V LIPO battery
int FIR15;

// ADC value of Left IR sensor at 30cm away from an object using 3.7V LIPO battery
int LIR30;
// ADC value of Left IR sensor at 15cm away from an object using 3.7V LIPO battery
int LIR15;

// ADC value of Right IR sensor at 30cm away from an object using 3.7V LIPO battery
int RIR30;
// ADC value of Right IR sensor at 15cm away from an object using 3.7V LIPO battery
int RIR15;

// backward distance
int FIR_BW = 500; 
int Ux_BW = 10;

int prevMode;
int cntTURN_LEFT_MODE=0;
int cntTURN_RIGHT_MODE=0;

int  Ldist, Rdist, Lunit, Runit;
int ref_front;
int ref_frontUx = 20;

int IRmode;

bool calibratedSpeed=false;

int statusTop74HC595;
int statusMiddle74HC595;
int statusBottom74HC595;

bool nightMode=false;
int singleDigitDelay=4;
