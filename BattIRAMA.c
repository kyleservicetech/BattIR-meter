/*
BATTERY IR METER TEST UNIT RELAY SYSTEM VERSION III
AMA Magazine
This version includes a precision 0.2% 5 Volt DC reference

Version 10-05-2018   (Cleaned up the files)
FINAL COPY!
*******************************************************************************
The "C" program in the mikroC format follows:
*******************************************************************************
*/

// Set up PicChip for LCD function per instruction in mikroC manual
     sbit LCD_RS at RB3_bit;
     sbit LCD_EN at RB2_bit;
     sbit LCD_d4 at RC4_bit;
     sbit LCD_d5 at RC5_bit;
     sbit LCD_d6 at RC6_bit;
     sbit LCD_d7 at RC7_bit;

     sbit LCD_RS_Direction at TRISB3_bit;
     sbit LCD_EN_Direction at TRISB2_bit;
     sbit LCD_D4_Direction at TRISC4_bit;
     sbit LCD_D5_Direction at TRISC5_bit;
     sbit LCD_D6_Direction at TRISC6_bit;
     sbit LCD_D7_Direction at TRISC7_bit;
 //set up definitions
     unsigned char lcd_loc ;
     unsigned long mi_volt,milli_read,counter,milli_readavg,Max_IR,Tot_IR;
     float Test_Amps,Batt_IR,Test_LoAmp,Test_HiAmp;
     unsigned long CellCnt,BattNL,BattV5ohm,BattV2ohm,BattV14ohm,Batt_Mah,calc;
     unsigned long ch,Cell_1,Cell_2;
     unsigned char Mah_1000, Mah_100;                        //Batt Mah numbers
     unsigned long tlong   ;
     unsigned char counts,LCD1,LCD2 ;
     unsigned int ArrayV [8] ;
     unsigned int Array14ohm [8];
     unsigned int Array2ohm [8];
     unsigned int Array5ohm [8];
     unsigned long int ArrayIR [8];
//   Look Up tables
     unsigned char LCDRow[] = {1,2,3,4,1,2,3,4}       ;              //LCD Rows
     unsigned char LCDCol[] = {4,4,4,4,15,15,15,15}   ;           //LCD Columns
     unsigned char LCDIRCol[] = {6,6,6,6,17,17,17,17}   ;         //LCD Columns
     unsigned char LCDIRColIR[] = {4,4,4,4,15,15,15,15} ;       //IRLCD Columns
// these are the channel assignments to turn on proper relay for each cell
//changed port A from RA3 to RA2
     unsigned char porta_bat[] ={0x04,0x20,0x40,0x00,0x00,0x00,0x00};
     unsigned char portc_bat[] ={0x00,0x00,0x00,0x01,0x02,0x04,0x08};
     #define switch portb.rb1
     #define pressed 1
//watch which resistor goes to which cpu port!
     #define R2ohm PORTb.rb5
     #define R5ohm PORTb.rb4
     #define on 1
     #define off 0
//*****************************************************************************
//***********Analog Digital Function.  Input, CPU, Output ArrayV **************
//*****************************************************************************
     void ADFunction(void)
{
          ADC_Init_Advanced(_ADC_INTERNAL_VREFL | _ADC_EXTERNAL_VREFH);
          ArrayV[0] = 0x00;
          ArrayV[1] = 0x00;
          ArrayV[2] = 0x00;
          ArrayV[3] = 0x00;
          ArrayV[4] = 0x00;
          ArrayV[5] = 0x00;
          ArrayV[6] = 0x00;
          ArrayV[7] = 0x00;
          CellCnt = 0;
      for(counts=0;counts<7;counts++)
  {
          porta = (porta_bat[CellCnt]);
          delay_us(20);
          portc = (portc_bat[Cellcnt]);
          delay_ms(40)      ;                                //wait for warm up
          milli_readavg = 0 ;                              //clear milli reader
     for (counter=0; counter<10; counter++)            //Read voltage 10x times
  {
         milli_read= ADC_Get_Sample(0) ;        //this call uses programmed ref
         milli_readavg = (milli_readavg+Milli_read) ;  //add up all 10 readings
  }
         mi_volt = (milli_readavg*0.1254);       //calibrate voltage range here
    if (mi_volt <100)                                     //If zero volts, skip
    {
         ArrayV[CellCnt] = 0x00;                             //enter zero count
    }
      else
    {
         ArrayV[CellCnt] = mi_volt;                 //Store cell volts to array
         CellCnt=CellCnt+1 ;
    }
  }
 //if first cell is zero volts, clear display to all zeros
     if (ArrayV[0]<200)                             //Test first Array for zero
   {
         ArrayV[1] = 0x00;                          //If zero, clear full Array
         ArrayV[2] = 0x00;
         ArrayV[3] = 0x00;
         ArrayV[4] = 0x00;
         ArrayV[5] = 0x00;
         ArrayV[6] = 0x00;
    }
     else
   {}
         portb = 0x00;                               //turn off power resistors
         delay_ms(2);                                         //2 ms mikroC bug
         porta = 0x00;                                    //turn everything off
         delay_ms(2);
         portc = 0x00;
}
//****************************************************************************
//***LCD Function Input ArrayV  Output to LCD Display. ***********************
//****************************************************************************
   void LCDFunction(void)
   {
    {
      for (counts=0; counts<(CellCnt);counts++)
     {
         tlong = ArrayV [counts] ;                                 //show volts
//         LCD_CHR(LCDRow[counts],LCDCol[counts],0x30+ch);            //4th digit
         ch = tlong /1000;
         LCD_CHR(LCDRow[counts],LCDCol[counts],0x30+ch);            //4th digit
         LCD_CHR_CP ('.');                                           // decimal
         ch = (tlong/100)%10;
         LCD_CHR_CP(0x30+ch);                                       //3rd digit
         ch=(tlong/10)%10;                                          //2nd digit
         LCD_CHR_CP(0x30+ch);
         ch = (tlong /1)%10 ;                                       //1st digit
         LCD_CHR_CP (0x30+ch);
     }
        delay_ms(50);
   }
  }
   void Switch_push (void)  //This routine flashes push, and waits til sw is on
   {
     do
    {
       Lcd_out(1,10,"P");                             //display vertical "Push"
       Lcd_out(2,10,"u");
       Lcd_out(3,10,"s");
       Lcd_out(4,10,"h");
       delay_ms(100);
       Lcd_out(1,10," ");                                    //flash on and off
       Lcd_out(2,10," ");
       Lcd_out(3,10," ");
       Lcd_out(4,10," ");
       delay_ms(100);
     }
   while(switch==0)       ;
       delay_ms(100);
     }
   void Switch_release (void) //This routine flashes rls, and waits til sw is 0
     {
      do
    {
       Lcd_out(1,10,"R");                             //display vertical "Push"
       Lcd_out(2,10,"l");
       Lcd_out(3,10,"s");
       Lcd_out(4,10," ");
       delay_ms(100);
       Lcd_out(1,10," ");                                    //flash on and off
       Lcd_out(2,10," ");
       Lcd_out(3,10," ");
       Lcd_out(4,10," ");
       delay_ms(100);
     }
   while(switch==1)     ;
   }
//*****************************************************************************
//*****Battery Voltage Function with tlong input, LCD1,2 screen location*******
//*****************************************************************************
  void Display_Data(void)
     {
        ch = tlong /10000;
        LCD_CHR(LCD1,LCD2,0x30+ch);//4th digit
        ch = (tlong/1000)%10;
        LCD_CHR_CP(0x30+ch);                                        //3rd digit
        LCD_CHR_CP ('.');                                            // decimal
        ch=(tlong/100)%10;                                          //2nd digit
        LCD_CHR_CP(0x30+ch);

        ch = (tlong /10)%10 ;                                       //1st digit
        LCD_CHR_CP (0x30+ch);                       //don't show to many digits
     }
     
     void Display_reset (void)
     {
       Lcd_Out(1,1,"#1");
       Lcd_Out(2,1,"#2");
       Lcd_Out(3,1,"#3");
       Lcd_Out(4,1,"#4");
       Lcd_Out(1,9,"V  #5");
       Lcd_Out(2,9,"V  #6");
       Lcd_Out(3,9,"V  #7");
       Lcd_Out(4,9,"V  #7");
       Lcd_Out(1,20,"V");
       Lcd_Out(2,20,"V");
       Lcd_Out(3,20,"V");
       Lcd_Out(4,9,"V Bat");
       Lcd_Out(4,20,"V");
      }
//*****************************************************************************
//***********This is the main program ****************************t************
//*****************************************************************************
 void main()
 {
//Configure A/D converter
        ANCON0 = 0X01 ;                           //configures channel 0 as A/D
        ANCON1 = 0X00;                                //configure port B as all
        ADCON1 = 0b00010000;                            // digital default A/D!
        lcd_loc = 0x00 ;
// Configure I/O ports
        trisa = 0b00001011;                 //port a, bit 0,1 are analog inputs
        trisb = 0b00000011  ;                   //make port b, bits 0, 1 inputs
        trisc = 0b00000000;
        porta = 0;
        portb = 0;                                             //turn off loads
        portc = 0;
        Lcd_init();
        Lcd_cmd(_LCD_CLEAR);
        Lcd_cmd(_LCD_CURSOR_OFF);

//get 9 Volt battery value
        tlong=Adc_Read(1);
        tlong = tlong*10000;
        tlong = tlong/2640;
        LCD1 = 3;
        LCD2 = 14;
        Lcd_Out(1,1,"Battery IR Meter    ");             //show info on display
        Lcd_Out(2,1,"9 Volt Batt < 6.7VDC");     //check for low 9 volt battery
        Lcd_Out(3,1,"9 Volt Batt       V ");
        Lcd_Out(4,1,"SW On Pwr Up set Mah");
        Display_Data();
        delay_ms(2000);
//set off low battery warning if below 6.70 Volts DC
    if (tlong<6700)
     {
low_batt:
        Lcd_cmd(_LCD_CLEAR);                          //lock in low batt, don't
        delay_ms(200);                                    //allow use of BattIR
        Lcd_Out (4,1,"Change 9 VDC Battery");
        delay_ms(200);
        goto low_batt ;
     }
       else
     {
     }
//*********Program End Return Point *******************************************
 reinit:
        Lcd_init();
        Lcd_cmd(_LCD_CLEAR);
        Lcd_cmd(_LCD_CURSOR_OFF);
        porta = 0;                //shut everything off
        portb = 0;               //turn off loads
        portc = 0;
  //set up battery mah value for maximum current calculation
        if (switch == 0)                               //do the switch routines
        goto   Get_EEprom;
        Mah_1000 = 0;                                             //Clear files
        Mah_100  = 0;
        Lcd_Out(1,1,"Battery IR Meter    ");             //show info on display
        Lcd_Out(2,1,"Push Switch         ");
        Lcd_Out(3,1,"Do 1000 Mah Batt    ");
        Lcd_Out(4,1,"Batt Mah Cap        ");
        LCD_CHR(4,15,0x30+Mah_1000);
        LCD_CHR(4,16,0x30+Mah_100);
        LCD_CHR(4,17,0x30);
        LCD_CHR(4,18,0x30);
//This routine sets up the test batteries milliampere hour rating
        delay_ms(400);
        Lcd_Out(2,1,"Hold/Release Switch ");
Mah1000_adj:                                                //adjust 1000's Mah
        if (Mah_1000>0x09)                            //if above 9, set to zero
        Mah_1000 = 0;
        LCD_CHR(4,15,0x30+Mah_1000);
        delay_ms(400);
        if(switch == 1)
     {
        Mah_1000 = Mah_1000+1;
        goto Mah1000_adj ;
     }
 //do 100 mah                                                //adjust 100's Mah
    {
        delay_ms(500);
        Lcd_Out(3,1,"Do 100 Mah Batt        ");
 Switch_pusha:
        Lcd_Out(2,1,"Push Switch         ");
    if (switch == 0)
       goto   Switch_pusha;
       Lcd_Out(2,1,"Hold/Release Switch ");
       
Mah100_adj:
 //      Lcd_Out(2,1,"Hold to 100's mah   ");                                                //if above 9, set to zero
    if (Mah_100>0x09)
       Mah_100 = 0;
       LCD_CHR(4,16,0x30+Mah_100);
       delay_ms(400);
    if(switch == 1)
     {
       Mah_100 = Mah_100+1;
       goto Mah100_adj       ;
     }
    }
  {
//Turn off test currents,
       R2ohm = off;
       delay_ms (10);
       R5ohm = off;                                  //turn off power resistors
       porta=0;
       portb=0;
       portc=0;
   //Save mah values to eeprom
       eeprom_write(0,mah_1000);
       eeprom_write(01,mah_100);
       delay_ms(100);
//Retrieve mah values from EEProm on normal start up here
Get_EEPRom:
       Mah_1000 = EEProm_read(0x00);
       Mah_100 =  EEPROM_Read(0x01);
//and verify existing mah settings
       Lcd_Out(1,1,"Battery IR Meter    ");             //show info on display
       Lcd_Out(2,1,"Check Test Settings ");
       Lcd_Out(3,1,"Batt Cap      00 Mah");
       Lcd_Out(4,1,"       Wait         ");
//***********************************************************************

       Batt_Mah = (Mah_1000*10)+Mah_100;
       LCD_CHR(3,13,0x30+Mah_1000);
       LCD_CHR(3,14,0x30+Mah_100);
       delay_ms (2500);                               //wait for battery reading
  }
 //this routine reads the individual cell voltages and displays them in a loop
 {
restart:
       Lcd_cmd(_LCD_CLEAR);
                                                   //individual LCD data inputs
    do
     { 
     Display_reset();                       //set up display for volts readouts
       portb = 0b00000000;                        //no load battery measurement
       delay_ms(30);                           //  wait for relays to stabilize
RestartA:
       ADFunction();                                      //get 7 channel volts
   if (ArrayV[0]<500)                                  //Test first Array Volts
    {                                               //test no battery connected
       Lcd_cmd(_LCD_CLEAR);
       Lcd_Out(3,1,"No Battery Connected");           //Clear Display to blanks
       delay_ms(400);
       Lcd_Out(3,1,"                    ");           //Clear Display to blanks
       delay_ms(200);                                               //stay here
       goto RestartA;
    }
    {          
      Display_reset();
                                                        //do program
//*********** Setup display for constants, do not change readouts
       ADFunction();                                    //get all cell voltages
       BattNL = ArrayV[0];                     //save battery no load for later
       LCDFunction();                                 //display 7 channel volts
       tlong = ArrayV[0]+ArrayV[1]+ArrayV[2]
       +ArrayV[3]+ArrayV[4]+ArrayV[5]+ArrayV[6];        //add up all cell volts
       LCD1 = 4;
       LCD2 = 15;
       Display_data();                                  //print LiPo batt volts
       delay_ms(1);
    }
 //Test for switch pushed for doing load tests   //hold here till button pushed
       Lcd_out(1,10,"P");                             //display vertical "Push"
       Lcd_out(2,10,"u");
       Lcd_out(3,10,"s");
       Lcd_out(4,10,"h");
       delay_ms(400);
       Lcd_out(1,10,"S");                                    //flash on and off
       Lcd_out(2,10,"W");
       Lcd_out(3,10," ");
       Lcd_out(4,10," ");
       delay_ms(200);
       Lcd_out(1,10," ");                             //display vertical "Push"
       Lcd_out(2,10," ");
       Lcd_out(3,10," ");
       Lcd_out(4,10," ");
       delay_ms(200);

//   test if switch is pushed, if not, loop back to continuous voltage reads
     }
    while (switch == 0);
 }
       Lcd_out(4,1,"For Fast Hold Switch");          //show how to fast forward
       delay_ms(1000);
 //Routine applies 5 Ohm load, reads all cell voltages and stores to Array5ohm
       Lcd_cmd(_LCD_CLEAR);
       Lcd_Out(1,1,"Test in Progress    ");
       delay_ms(5);
       Lcd_Out(2,1,"5 Ohm Load is ON    ");
       R5ohm = 1 ;
       delay_ms(700);                          //on time for 5 ohms - 2 seconds
       ADFunction();                                       //read A/D  voltages
       portb = 0x00;                                          //turn off 5 ohms
       Lcd_Out(2,1,"5 Ohm Load is OFF   ");
 //*********** Record Volts for Low Current Test Amps *************************
       Array5ohm[0] = ArrayV[0];                               //transfer array
       Array5ohm[1] = ArrayV[1];
       Array5ohm[2] = ArrayV[2];
       Array5ohm[3] = ArrayV[3];
       Array5ohm[4] = ArrayV[4];
       Array5ohm[5] = ArrayV[5];
       Array5ohm[6] = ArrayV[6];
       Array5ohm[7] = ArrayV[7];
       BattV5ohm = Array5ohm[0]+Array5ohm[1]+Array5ohm[2] //add up all voltages
       +Array5ohm[3]+Array5ohm[4]+Array5ohm[5]+Array5ohm[6];
 //do 2 ohm load test
       Lcd_Out(3,1,"2 Ohm Load is ON    ");
       delay_ms(10);
       R2ohm = 1;
       delay_ms(200);                          //on time for 5 ohms - 2 seconds
       ADFunction();                                       //read A/D  voltages
       portb = 0x00;                                          //turn off 5 ohms
       Lcd_Out(3,1,"2 Ohm Load is OFF   ");
 //*********** Record Volts for Low Current Test Amps *************************
       Array2ohm[0] = ArrayV[0];                               //transfer array
       Array2ohm[1] = ArrayV[1];
       Array2ohm[2] = ArrayV[2];
       Array2ohm[3] = ArrayV[3];
       Array2ohm[4] = ArrayV[4];
       Array2ohm[5] = ArrayV[5];
       Array2ohm[6] = ArrayV[6];
       Array2ohm[7] = ArrayV[7];
       BattV2ohm = Array2ohm[0]+Array2ohm[1]+Array2ohm[2] //add up all voltages
       +Array2ohm[3]+Array2ohm[4]+Array2ohm[5]+Array2ohm[6];
//Routine applies 1.428 Ohm Load, reads all cell voltages stores to Array2ohm
       Lcd_Out(4,1,"1.4  Ohm Load is ON ");
       delay_ms(5);
       portb = 0b00110000;                      //turn on 5 and 2 ohm resistors
       delay_ms(200);                        //on time for 5 ohms - 1.0 seconds
       ADFunction();                                       //read A/D  voltages
       portb = 0x00;                                          //turn off 5 ohms
       Lcd_Out(4,1,"1.4 Ohm Load is  OFF");
 //*****************Record Volts For High Current Test Amps *******************
       Lcd_Out(4,1,"1.4  Ohm Load is OFF");
       Array14ohm[0] = ArrayV[0];              //copy to Array14ohm for IR calc
       Array14ohm[1] = ArrayV[1];
       Array14ohm[2] = ArrayV[2];
       Array14ohm[3] = ArrayV[3];
       Array14ohm[4] = ArrayV[4];
       Array14ohm[5] = ArrayV[5];
       Array14ohm[6] = ArrayV[6];
       Array14ohm[7] = ArrayV[7];
       BattV14ohm = Array14ohm[0]+Array14ohm[1]+Array14ohm[2]
       +Array14ohm[3]+Array14ohm[4]+Array14ohm[5]+Array14ohm[6];
       delay_ms(100);
//*****************************************************************************
//***********this routine checks for voltage sag due to loading battery *******
//*****if no voltage sag, give warning about connecting red/black wires********
//*****************************************************************************
   if(Array5ohm[0]-Array14ohm[0]<5)             //check for less than 5 mv sag
   {
       Lcd_cmd(_LCD_CLEAR);
       Lcd_out(1,1,"Please Check Battery");
       Lcd_out(3,1,"BattIR meter        ");
       Lcd_out(2,1,"Red and Black Wire  ");
       Lcd_out(4,1,"                    ");
       delay_ms(5000);
       goto restart;
   }
    {
       goto show_result;
    }
   if(Array5ohm[0]-Array14ohm[0]>5000)             //check for negative reading
    {
       Lcd_cmd(_LCD_CLEAR);
       Lcd_out(1,1,"Please Check Battery");
       Lcd_out(3,1,"BattIR meter        ");
       Lcd_out(2,1,"Red and Black Wire  ");
       Lcd_out(4,1,"                    ");
       delay_ms(5000);
       goto restart;
    }
    {
     goto show_result;
    }
//*****************************************************************************
//if switch is closed at this time, skip all readings and go to IR readouts
//******************************************************************************
show_result:
   if(switch==1)
       goto do_IR;
//else, show results volts and amps at 5, 2 and 1.4 Ohms
       Lcd_cmd(_LCD_CLEAR);
       Lcd_out(1,1,"Results             ");
       Lcd_out(2,1,"LoAmps      V      A");
       Lcd_out(3,1,"MdAmps      V      A");
       Lcd_out(4,1,"HiAmps      V      A");
//do pack voltage at low Amps and Hi Amps
       tlong = BattV5ohm;                                  //show low amp Volts
       ch = tlong/10000;
       LCD1 = 2;                                  // LCD1&2 are print locations
       LCD2 = 8;
       Display_data();
  
       tlong = BattV2ohm;                                 //show high amp volts
       ch = tlong/10000;
       LCD1=3;
       LCD2=8;
       Display_data();
       tlong = BattV14ohm;                                //show high amp volts
       ch = tlong/10000;
       LCD1=4;
       LCD2=8;
       Display_data();

       tlong=battV5ohm/5;                           //show low amp test current
       ch = tlong/10000;
       LCD1 = 2;
       LCD2 = 15;
       Display_data();

       tlong=battV2ohm/2;                           //show low amp test current
       ch = tlong/10000;
       LCD1 = 3;
       LCD2 = 15;
       Display_data();
  
       tlong=battV14ohm/1.428   ;                  //show high amp test current
       ch = tlong/10000;
       LCD1 = 4;
       LCD2 = 15;
       Display_data();                                 //hold for key push here
       delay_ms(100);
   do
   {
       LCD_Out(1,9,"Push Switch");
       delay_ms(100);
       LCD_Out (1,9,"           ");
       delay_ms(100);
   }
   while (switch == 0);
//show battery voltages under load
       Lcd_Out(1,1,"#1      V  #5      V");
       Lcd_Out(2,1,"#2      V  #6      V");
       Lcd_Out(3,1,"#3      V  #7      V");
       Lcd_Out(4,1,"#4      V VoltLowAmp");

       ArrayV[0] = Array5ohm[0];               //copy to Array14ohm for IR calc
       ArrayV[1] = Array5ohm[1];
       ArrayV[2] = Array5ohm[2];
       ArrayV[3] = Array5ohm[3];
       ArrayV[4] = Array5ohm[4];
       ArrayV[5] = Array5ohm[5];
       ArrayV[6] = Array5ohm[6];
       ArrayV[7] = Array5ohm[7];
       LCDFunction();
   Switch_release();
   Switch_push ();
 //show results at 2 ohms
       Lcd_Out(1,1,"#1      V  #5      V");
       Lcd_Out(2,1,"#2      V  #6      V");
       Lcd_Out(3,1,"#3      V  #7      V");
       Lcd_Out(4,1,"#4      V VoltMidAmp");

       ArrayV[0] = Array2ohm[0];               //copy to Array14ohm for IR calc
       ArrayV[1] = Array2ohm[1];
       ArrayV[2] = Array2ohm[2];
       ArrayV[3] = Array2ohm[3];
       ArrayV[4] = Array2ohm[4];
       ArrayV[5] = Array2ohm[5];
       ArrayV[6] = Array2ohm[6];
       ArrayV[7] = Array2ohm[7];
       LCDFunction();
   Switch_release();
   Switch_push();
       delay_ms(200);
       Lcd_Out(1,1,"#1      V  #5      V");
       Lcd_Out(2,1,"#2      V  #6      V");
       Lcd_Out(3,1,"#3      V  #7      V");
       Lcd_Out(4,1,"#4      V VoltsHiAmp");
       ArrayV[0]=  Array14ohm[0]  ;            //copy to Array14ohm for IR calc
       ArrayV[1]=  Array14ohm[1]  ;
       ArrayV[2]=  Array14ohm[2]  ;
       ArrayV[3]=  Array14ohm[3]  ;
       ArrayV[4]=  Array14ohm[4]  ;
       ArrayV[5]=  Array14ohm[5]   ;
       ArrayV[6]=  Array14ohm[6]  ;
       ArrayV[7]=  Array14ohm[7]  ;
       LCDFunction();
   Switch_release();
   Switch_push();

Do_ir:
 //do IR tests
       Lcd_Out(1,1,"1ir        5ir      ");
       Lcd_Out(2,1,"2ir        6ir      ");
       Lcd_Out(3,1,"3ir        7ir      ");
       Lcd_Out(4,1,"4ir       Milli-Ohms");
//calculate IR where IR = (Vlow-Vhi)/(AmpsHi-AmpsLo)
       ArrayIR [0] = 0;                     //clear array for less than 7 cells
       ArrayIR [1] = 0;
       ArrayIR [2] = 0;
       ArrayIR [3] = 0;
       ArrayIR [4] = 0;
       ArrayIR [5] = 0;
       ArrayIR [6] = 0;
       ArrayIR [7] = 0;
       Tot_IR      = 0;                                  //clear total IR value
  //do calculations
  for (counts=0; counts<(CellCnt);counts++)
   {
       Test_Amps = (BattV5ohm/5)-(BattV14ohm/1.428);
       tlong  = (Array5ohm[counts]-Array14ohm[counts]) ;
       tlong = tlong*100000;                     //adjust ranges on calculation
       tlong = tlong/Test_Amps;
       ArrayIR [counts] = tlong;
       Tot_IR = Tot_IR + tlong;                             //add up total IR's
       ch = tlong/10000 ;                                              //9xx.xx
       LCD_CHR(LCDRow[counts],LCDIRColIR[counts],0x30+ch);          //4th digit
       ch =  (tlong/1000)%10;
       LCD_CHR_CP(0X30+ch);
       ch = (tlong/100)%10;
       LCD_CHR_CP(0x30+ch);                                         //3rd digit
       LCD_CHR_CP ('.');                                             // decimal
       ch=(tlong/10)%10;
       LCD_CHR_CP(0x30+ch);                                         //2nd digit
       ch = (tlong /1)%10 ;
       LCD_CHR_CP (0x30+ch);                                        //1st digit
   }
       delay_ms(100);
  Switch_release();
       Lcd_Out(4,11,"T-IR ");
       tlong = Tot_IR;

       ch = tlong /10000;                                   //adjust full range
       LCD_CHR(4,16,0x30+ch);                                       //4th digit
       ch = (tlong/1000)%10;
       LCD_CHR_CP(0x30+ch);                                         //3rd digit

       ch=(tlong/100)%10;                                           //2nd digit
       LCD_CHR_CP(0x30+ch);
       LCD_CHR_CP ('.');                                            // decimal
       ch = (tlong /10)%10 ;
       LCD_CHR_CP (0x30+ch);                                        //1st digit
   Switch_push();
       delay_ms(100);
       Lcd_Out(1,1,"Max Current is     A");
       Lcd_Out(2,1,"Reference RCGroups  ");
       Lcd_Out(3,1,"Forsyth,Julian,Giles");
       Lcd_Out(4,1,"                    ");
//this routine finds the maximum IR value in the IR Array
   for (counts = 0; counts < 7; counts ++)
     {
   if (ArrayIR[0]<ArrayIR[counts])
       ArrayIR[0] = ArrayIR[counts]    ;
     }
       Max_IR = ArrayIR[0];                             //calculate IR for cell
       Calc = 60000*Batt_mah;
       tlong = sqrt (calc/Max_IR)    ;              //do square root on results
       ch = tlong /100;
       LCD_CHR(1,16,0x30+ch); //4th digit
       ch = (tlong/10)%10;
       LCD_CHR_CP(0x30+ch);                                         //3rd digit
       ch=(tlong/1)%10;                                             //2nd digit
       LCD_CHR_CP(0x30+ch);
                                                                    //1st digit
       delay_ms(100);
   do
    {    Lcd_out(4,1,"    Push Switch     ");
       delay_ms(100);
       Lcd_out(4,1,"                    ");
       delay_ms(100);
    }
   while(switch==0)   ;
   do
    {
       Lcd_out(4,1,"   Release Switch   ");
       delay_ms(100);
       Lcd_out(4,1,"                    ");
       delay_ms(100);
    }
       while(switch==1)         ;
       goto restart;
}

//end of program