/*
 * This Arduino Nano Sketch reads the information provided by the BattIR meter via the BattIR meters five MicroChip programming pins.
 * The original connections were D11 and D12.  They have been moved to D2 and D3 to allow insertion of the Arduino Nano into the 3D printed holder.
 * Note: The Arduino Nano clones require the installation of a driver in a Windows PC.  
   One source is https://www.instructables.com/id/Arduino-Nano-CH340/
 * Also, set up Arduino "Tools" Processor ATMega328P(Old Bootloader) for Arduino Nano Clones
 * Date 12/28/2018

pin 12 is data, 11 is clock
now pin 3 is data, pin 2 is clock.
*/
  int long counter = 0;
  unsigned int Cntr = 0;
  unsigned int  cycle = 0;
    int rotate = 0;
  unsigned int data = 0;
  unsigned int test_report = 0;
    #include <EEPROM.h>
void setup() {
//  Serial.begin(9600);
  Serial.begin(9600);
  pinMode(3,INPUT) ;
  pinMode (2,INPUT);
  pinMode (10,OUTPUT);
  pinMode (9, OUTPUT);

  }
 
//this routine centers the program at the bit point
  void getbit()
{     
  do
   Cntr = digitalRead(2);
   while (Cntr == HIGH);

  do
   Cntr = digitalRead(2);
   while (Cntr == LOW);
 //     delay(0.5);
      }

   
    //this routine gets one byte from BattIR meter and save in "data"
  void getbyte()
  {
    digitalWrite(10,1);
    //get start of serial data string
  do
    counter = pulseIn(2,1);
  while (counter<800);
    digitalWrite (10,0);
   data = 0;            //clear data variable

 //get data one bit at a time

   getbit();
delayMicroseconds(40);
   digitalWrite(9,1);
 Cntr = digitalRead(3);
  digitalWrite(9,0); 
    if (Cntr == HIGH)
   bitWrite (data,7,1);

     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
  if (Cntr == HIGH)
   bitWrite (data,6,1);

     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
    if (Cntr == HIGH)
   bitWrite (data,5,1);

     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
    if (Cntr == HIGH)
   bitWrite (data,4,1);

     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
    if (Cntr == HIGH)
   bitWrite (data,3,1);

     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
    if (Cntr == HIGH)
   bitWrite (data,2,1);

     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
    if (Cntr == HIGH)
   bitWrite (data,1,1);
   
     getbit();
     delayMicroseconds(40);
     Cntr = digitalRead(3);
    if (Cntr == HIGH)
   bitWrite (data,0,1);
  } 
void loop() 
{

  getbyte();
  if (data == 200)
  {
     
  Serial.println ("Cell#1  Cell#2  Cell#3  Cell#4  Cell#5  Cell#6  Cell#7");
  }

  else 
  if (data == 201) //carriage return
  {
  Serial.println(" ");
  }
  else
  if (data == 204)
  {
    Serial.print ("No Load Battery Volts");
  }

  else 
  if (data == 205) //Low Amp voltage readings
  {
  Serial.print("Low Amp Voltage");
  }

  else 
  if (data == 206) //Medium amp voltage readings
  {
  Serial.print("Medium Amp Voltage");
  }

  else 
  if (data == 207) //low amp voltage readings
  {
  Serial.print("High Amp Voltage");
  }

  else 
  if (data == 208) //cell IR values
  {
  Serial.print("Cell IR Values");
  }

  else 
  if (data == 203) //carriage return
  {
  Serial.print(".");
  }
  else 
  if (data == 209) //carriage return
  {
  Serial.print("Test Report #  ");
   test_report = EEPROM.read(0);
    Serial.print (test_report);
    test_report = test_report +1;
    EEPROM.write (0,test_report);
  }
 
  else
  if (data == 202) //print space
  {
  Serial.print ("    ");  //adjust spacing here to line up on USB reader
  }

  else
  {
   Serial.print(data);
   }

}
 
