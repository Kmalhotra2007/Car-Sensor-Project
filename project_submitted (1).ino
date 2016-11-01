/*
   FILE          : Project
   PROJECT       : PROG8125 -Using an ultrasonic sensor and designing a reverse car sensor
   PROGRAMMER    : Kunal Malhotra
   FIRST VERSION : 2016-08-05
   DESCRIPTION   : This sketch reads a PING))) ultrasonic rangefinder and returns the distance to the closest object in range. To do this, it sends a pulse to the sensor to
                   initiate a reading, then listens for a pulse to return.  The length of the returning pulse is proportional  to the distance of the object from the sensor.
                   Additional to that, it prints the distance in inches and centimeters on the LCD.Depending on the distance we get further get outputs from LCD, RGB LED and
                   the LCD screen.
                   The function void InitialSound() uses the tone function and gives the out using the speaker when the obtacle is at a particular distance fron the ultrasonic sensor.
                   The function long microsecondsToInches(long microseconds) takes the input value and converts it to Inches .
                   The function long microsecondsToCentimeters(long microseconds)takes the input value and converts it toCentimeters.
                   The function GettingTheDistance()sends the pulse using the ultrasonic sensor and measures the time taken fot the pulse to reflect and return to the sensor and prints
                   on the LCD.
                   The function ConvertingAndPrinting(long DigitalValue) uses the functions that convers the input time into distance and prints them on the LCD.
                   The function IntermediateSound() uses the tone function and gives the out using the speaker when the obtacle is at a particular distance fron the ultrasonic sensor.
                   The function FinalSound() uses the tone function and gives the out using the speaker when the obtacle is at a particular distance fron the ultrasonic sensor.
                   The function GraphOnLCD(long VariableForGraph) prints a graph on the second row of LCD which varies depending on the input value

                   The circuit:
                   +V connection of the PING))) attached to +5V
                   GND connection of the PING))) attached to ground
                   SIG connection of the PING))) attached to digital pin 9
                   The Pin Number connection from LCD to Teensy are as follows-
                   LCD RS pin to digital pin 12
                   LCD Enable pin to digital pin 11
                   LCD D4 pin to digital pin 5
                   LCD D5 pin to digital pin 4
                   LCD D6 pin to digital pin 3
                   LCD D7 pin to digital pin 2
                   LCD R/W pin to ground
                   LCD VSS pin to ground
                   LCD VCC pin to 5V
                   10K resistor:
                   ends to +5V and ground
                   wiper to LCD VO pin (pin 3)
*/
#include <LiquidCrystal.h>

const int pingPin = 9;                                                                                      // The Pin number for the ultrasonic sensor
const int8_t RedPin = 23;                                                                                   //
const int8_t GreenPin = 22;                                                                                 //Pin Numbers for different Pins of RGB LED as constant as they are not
const int8_t BluePin = 20;                                                                                  //going to change
const int8_t SpeakerPin = 6;                                                                                //Speaker Pin Number
const int16_t FullSaturationFreequency = 255;                                                               //Maximum Freequency for the RGB LED
const int16_t MinimumLEDFreequency = 0;                                                                     //Minimum Freequency for RGB LED
const int32_t GreenForYellow = 50;                                                                          //Setting the freequency of Green LED as  50 to mix and blink yellow color
int16_t FreequencyForSpeaker1 = 250;                                                                        //Different Freequecies for te Speaker, these are use in the program depending
int16_t FreequencyForSpeaker2 = 255;                                                                        //on the type of sound we want on the ouput
uint16_t GreenLightDistance1 = 200;                                                                          //Distance between which Green Light will GLow
uint16_t GreenLightDistance2=300;                                                                            //
uint16_t YellowLightDistance1=101;                                                                           //Range of Distance between which Yellow colored LED will glow
uint16_t YellowLightDistance2=199;                                                                           //
uint16_t RedLightDistance=100;                                                                               //If the distance is less than 100 Red Led will glow                                                   
uint16_t BlueLightDistance=300;                                                                              //If the distance is more than 300 Blue LED will glow
uint16_t MinimumDistanceValue = 0;                                                                           //Minimum and Maximum Distance that can be read by our sensor
uint16_t MaximumDistanceValue = 334;                                                                         //
uint16_t MinimumNumberOfLCDBoxes=1;                                                                          //Minimum and maximum number of boxes in out LED, These variable are used with
uint16_t MaximumNumberOfLCDBoxes=16;                                                                         //the mapping function
int32_t LCDPixelBlock=1023;                                                                                 //Pxel values that show a block or a blanck on the LCD
int32_t LCDPixelBlank = 1022;                                                                               //
int8_t DelayTwoMicroSeconds = 2;                                                                            //                                
int8_t DelayFiveMcroseconds =5;                                                                             //
int8_t DelayFifteenMicroseconds=15;                                                                         //Different Delays used across the Programs in different Functions 
int16_t DelayHundredMcroseconds=100;                                                                        //
int16_t DelayThreeHundredMcroseconds=300;                                                                   //
int16_t InitialToneDelay=1000;                                                                              //
int16_t IntermediateToneDelay = 100;                                                                        //
int16_t FinalToneDelay = 150;                                                                               //


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                                                                       //Pin Numbers Assigned to LCD

void setup()
{
  Serial.begin(9600);                                                                                        //Initialize serial communication to 9600 baud rate
  lcd.begin(16, 2);                                                                                          //Initialising the LCD which has 16 collumns and 2 rows
}

// FUNCTION      : microsecondsToInches(long Microseconds)
// DESCRIPTION   : Converts the input value (i.e in microseconds) to Inches by doing the calculations while returning the value. According to Parallax's datasheet for the PING)))
//                 ,there are73.746 microseconds per inch (i.e. sound travels at 1130 feet per second).  This gives the distance travelled by the ping, outbound and return,
//                 so we divide by 2 to get the distance of the obstacle.               
// PARAMETERS    : long micrseconds- It consists of the input value in microseconds
// RETURNS       : Microseconds/74/2

long microsecondsToInches(long Microseconds)
{
  return Microseconds / 74 / 2;                                                                              //Calculating the Value in Inches and returning to the Function
}

// FUNCTION      : microsecondsToCenrimeters(long Microseconds)
// DESCRIPTION   : Converts the input value (i.e in microseconds) to centimeters by doing the calculations while returning the value.he speed of sound is 340 m/s or 29 microseconds
//                 per centimeter.The ping travels out and back, so to find the distance of the object we take half of the distance travelled. According to Parallax's datasheet for
//                 the PING)))object we take half of the distance travelled.      
// PARAMETERS    : long micrseconds - It consists of the input value in microseconds
// RETURNS       : Microseconds/74/2

long microsecondsToCentimeters(long Microseconds)
{
  return Microseconds / 29 / 2;                                                                              //Calculating the Value in Inches and returning to the Function
}

// FUNCTION      : GettingTheDistance()
// DESCRIPTION   : This function transmits the pulse using ultrasonic sensor and receives after the reflection from an obstacle.Appropriate delays are given during this process.
//                 A function called pulseIn() calculates the time from transmiting to receiving of the pulse and stores it in a variable.     
// PARAMETERS    : Nothing
// RETURNS       : long duration - consits the time elapsed between transmiting and receiving of the pulse

unsigned long GettingTheDistance()
{
  long duration;                                                                                            //Declaring a variable of long type and using it to store the time
  pinMode(pingPin, OUTPUT);                                                                                 // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  digitalWrite(pingPin, LOW);                                                                               // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  delayMicroseconds(DelayTwoMicroSeconds);                                                                  //A short duration as we dont want much place for sending the pulse
  digitalWrite(pingPin, HIGH);                                                                              //Sending the Pulse
  delayMicroseconds(DelayFiveMcroseconds);                                                                  //A small delay between transmitting the pulse and turning it off
  digitalWrite(pingPin, LOW);                                                                               //We stop the transmitting of the pulse here
  pinMode(pingPin, INPUT);                                                                                  // The same pin is used to read the signal from the PING))): a HIGH
                                                                                                            // pulse whose duration is the time (in microseconds) from the sending
                                                                                                            // of the ping to the reception of its echo off of an object.
  duration = pulseIn(pingPin, HIGH);                                                                        //Starts timing when the sensor went high and stops timing when it went low
                                                                                                            //pulsePin is function that calculates the time for the pulse
  return duration;                                                                                          //The calculated time is returned to the function and I use that values in
                                                                                                            //different funcions
}

// FUNCTION      : ConvertingAndPrinting(long DigitalValue)
// DESCRIPTION   : This function uses the above created functions which converts the received time value to inches and centimeters and prints them on the serial port as well as LCD.
//                 In addition to that it also shows a graph on the second row of LCD.The graph is displayed by calling a different function.
// PARAMETERS    : long DigitalValue -Consits the time elapsed between transmiting and receiving of the pulse
// RETURNS       : Cm - The calculated value in centimeters is returned to the function 

unsigned long ConvertingAndPrinting(long DigitalValue)
{
  long Inches;                                                                                              //Declaring variables local to the function
  long Cm;                                                                                                  //
                                                                                                   
  Inches = microsecondsToInches(DigitalValue);                                                              //Storing the function value in these two variables
  Cm = microsecondsToCentimeters(DigitalValue);                                                             //and using them further in the function
  Serial.print(Inches);                                                                                     //Printing Inches on the virtual Serial Port
  Serial.print("In, ");                                                                                     //
  lcd.setCursor(0, 0);                                                                                      //Setting the cursor at first collumn and first row
  lcd.printf("%3din ", Inches );                                                                            //Printing the distance in Inches on the LCD
  Serial.print(Cm);                                                                                         //Printing calue in centimeters on the serial port
  Serial.print("Cm");                                                                                       //
  Serial.println();                                                                                         //We have it here so that we print the value in next line everytime
  lcd.setCursor(7, 0);                                                                                      //Setting the cursor at 7th collumn and 1st row
  lcd.printf("%3d cm", Cm );                                                                                //Printing the value in centimers on the 7th collumn of the LCD
  delay(DelayHundredMcroseconds);                                                                           //A delay of 100 ms is given on the lcd screen to print the data on LCD
  return Cm ;                                                                                               //A value in Centimeters is returned to the function
  
}

//
//FUNCTION    : InitialSound
//DESCRIPTION : 
//              This function is used to sound the warning alarm wdepending on the measures distance. It uses a function called tone() which has Pin number, 
//              freequency and delay as its parameters..It also provides delay between the tone() functions so that there is atleast a gap of 1 secs between the change in sound freequency.              
//PARAMETERS  : This function does not has any parameters.
//             
//RETURNS     : This Function is not returning any value.

void InitialSound()
{
  tone(SpeakerPin,FreequencyForSpeaker1,InitialToneDelay);                                                  //The speaker is attached to pin number 6,and freequency of 250 is given with
                                                                                                            //a 1000 millisecond delay
  delay(InitialToneDelay);                                                                                  //A delay if 1 sec is given between a sound of a different freequency is given
                                                                                                            //to the speaker
  tone(SpeakerPin, FreequencyForSpeaker2,InitialToneDelay);                                                 //The speaker is attached to pin number 6,and freequency of 255 is given with
                                                                                                            //a 1000 millisecond delay
  delay(InitialToneDelay);                                                                                  //A delay if 1 sec is given before the noTone signal given to the speaker
  noTone(SpeakerPin);                                                                                       //The speaker will stop giving an, output "(6)" shows that the speaker pin is 6
}

//FUNCTION    : IntermediateSound()
//DESCRIPTION : 
//              This function is used to sound the warning alarm depending on the measures distance. It uses a function called tone() which has Pin number, 
//              freequency and delay as its parameters..It also provides delay between the tone() functions so that there is atleast a gap of 300 microsecs between the change in sound
//              freequency.              
//PARAMETERS  : This function does not has any parameters.
//             
//RETURNS     : This Function is not returning any value.

void IntermediateSound()
{
  tone(SpeakerPin,FreequencyForSpeaker1,IntermediateToneDelay);                                             //The speaker is attached to pin number 6,and freequency of 250 is given with
                                                                                                            //a 100 millisecond delay
  delay(DelayThreeHundredMcroseconds);                                                                      //A delay if 300 msec is given between a sound of a different freequency is given
                                                                                                            //to the speaker
  tone(SpeakerPin, FreequencyForSpeaker2,IntermediateToneDelay);                                            //The speaker is attached to pin number 6,and freequency of 255 is given with
                                                                                                            //a 100 millisecond delay
  delay(DelayThreeHundredMcroseconds);                                                                      //A delay if 300 msec is given before the noTone signal given to the speaker
  noTone(SpeakerPin);                                                                                       //The speaker will stop giving an, output "(6)" shows that the speaker pin is 6
}

//FUNCTION    : FinalSound()
//DESCRIPTION : 
//              This function is used to sound the warning alarm depending on the measures distance. It uses a function called tone() which has Pin number, 
//              freequency and delay as its parameters..It also provides delay between the tone() functions so that there is atleast a gap of 15 micro secs between the change in sound 
//              freequency.              
//PARAMETERS  : This function does not has any parameters.
//             
//RETURNS     : This Function is not returning any value.

void FinalSound()
{
  tone(SpeakerPin,FreequencyForSpeaker1,FinalToneDelay);                                                     //The speaker is attached to pin number 6,and freequency of 250 is given with
                                                                                                             //a 150 millisecond dela
  delay(DelayFifteenMicroseconds);                                                                           //A delay if 15 msec is given between a sound of a different freequency is given
                                                                                                             //to the speaker
  tone(SpeakerPin, FreequencyForSpeaker2,FinalToneDelay);                                                    //The speaker is attached to pin number 6,and freequency of 255 is given with
                                                                                                             //a 150 millisecond delay
  delay(DelayFifteenMicroseconds);                                                                           //A delay if 15 msec is given before the noTone signal given to the speaker
  noTone(SpeakerPin);                                                                                        //The speaker will stop giving an, output "(6)" shows that the speaker pin is 6
}

//FUNCTION    : BlinkRedColor
//DESCRIPTION :This function consists of the commands required to illuminate the Blue color by mixing of different colors in a RGB LED.
//              
//PARAMETERS  : This function does not has any parameters.
//
//RETURNS     : This Function is not returning any value.

void BlinkRedColor()
{
  analogWrite(RedPin, FullSaturationFreequency);                                                              //Assigning Red LED to 255 freequency
  analogWrite(BluePin, MinimumLEDFreequency);                                                                 //The blue LED is turned off for this color
  analogWrite(GreenPin, MinimumLEDFreequency);                                                                //The green LED is turned off for this color
}

//FUNCTION    : BlinkYellowColor
//DESCRIPTION :This function consists of the commands required to illuminate the Yellow color by mixing of different colors in a RGB LED.
//              
//PARAMETERS  : This function does not has any parameters.
//
//RETURNS     : This Function is not returning any value.

void BlinkYellowColor()
{
  analogWrite(RedPin, FullSaturationFreequency);                                                              //Assigning Red LED to 255 freequency
  analogWrite(GreenPin, GreenForYellow);                                                                      //Assigning Green LED to 50 freequency
  analogWrite(BluePin, MinimumLEDFreequency);                                                                 //The blue LED is turned off for this color
}

//FUNCTION    : BlinkGreenColor
//DESCRIPTION :This function consists of the commands required to illuminate the Green color by mixing of different colors in a RGB LED.
//              
//PARAMETERS  : This function does not has any parameters.
//
//RETURNS     : This Function is not returning any value.

void BlinkGreenColor()
{
  analogWrite(RedPin, MinimumLEDFreequency );                                                                 //The blue LED is turned off for this color
  analogWrite(BluePin, MinimumLEDFreequency);                                                                 //The blue LED is turned off for this color
  analogWrite(GreenPin, FullSaturationFreequency);                                                            //Assigning Blue LED to 255 freequency
}

//FUNCTION    : BlinkBluColor()
//DESCRIPTION :This function consists of the commands required to illuminate the Blue color by mixing of different colors in a RGB LED.
//              
//PARAMETERS  : This function does not has any parameters.
//
//RETURNS     : This Function is not returning any value.

void BlinkBlueColor()
{
  analogWrite(RedPin, MinimumLEDFreequency );                                                                 //Assigning Red LED to 0 freequency
  analogWrite(BluePin, FullSaturationFreequency);                                                             //The blue LED is turned on to 255 freequency
  analogWrite(GreenPin,  MinimumLEDFreequency );                                                              //The green LED is turned off for this color
}
//FUNCTION    :SensorAndLED(long SensorValue)
//DESCRIPTION :Depending on the value of distance in Centimeter, this function blinks LED and gives an output from the Speaker.It checks whether the distance is between a particular range
//             and performs the respective functions.In addition to tha this function is resposible for printing a graph on the second row of the LCD by calling a separate function in 
//             this function            
//PARAMETERS  :long SensorValue - It consists of the time duration between transmiting and receiving the pulses
//
//RETURNS     : This Function is not returning any value.

void SensorAndLED(long SensorValue)
{
  unsigned long ComparingFactor = ConvertingAndPrinting(SensorValue);                                        //Storing the value of the variable in an unsigned long variable
  if ((ComparingFactor >=GreenLightDistance1) && (ComparingFactor <=GreenLightDistance2))                    //Checking if the distance value in centimeters is beween a certain limit
  {
    InitialSound();                                                                                          //Calling the function which will result in an ouput through the speaker
    BlinkGreenColor();                                                                                       //Blinks Green Color, which also tells us there is enough distance between
                                                                                                             //the sensor and the obstacle
  }
  else if ((ComparingFactor >= YellowLightDistance1) && (ComparingFactor <= YellowLightDistance2))           //Checking if the distance value in centimeters is beween a certain limit
  {
    IntermediateSound();                                                                                     //Calling the function which will result in an ouput through the speaker
    BlinkYellowColor();                                                                                      //Blinks Yellow Color, which also tells us there is little distance between
                                                                                                             //the sensor and the obstacle
  }
  else if ((ComparingFactor <= RedLightDistance))                                                            //Checking if the value in centimeters is Less that 100 cm
  {
    FinalSound();                                                                                            //Gives an output of fast freequency sound from the speaker
    BlinkRedColor();                                                                                         //Calling the function which illuminates the red light
  }
  else if (ComparingFactor > BlueLightDistance)                                                              //Checing if the value is centimeters is close to the maximum value what out
                                                                                                             //current sensor can sense.
  {
    BlinkBlueColor();                                                                                        //Blinks blue light which tells hat there isnt anyobject nearby
  }
  GraphOnLCD(SensorValue);                                                                                   //This Funtion prints a Bar graph on the second row of the LCD
}

//FUNCTION    :GraphOnLCD(long VariableForGraph)
//DESCRIPTION :We know that the maximum distance that our sensor can detect is 3.3 metres, this function maps the output from sensor with the 16 bits of lcd.This way the blocks in 
//             in the LCD are filled according to the change in output value from the sensor.           
//PARAMETERS  :long VariableForGraph - It consists of the time duration between transmiting and receiving the pulses
//
//RETURNS     : This Function is not returning any value.

void GraphOnLCD(long VariableForGraph)
{
  unsigned long ComparingFactor = ConvertingAndPrinting(VariableForGraph);                                   //Storing the value of function(distance in cm) in a variable
  uint16_t i = 0;                                                                                            //Declaring a varaible i and initialising it to 0
  static uint16_t PreviousMappingValue = 0;                                                                  //Declaring a varaible MappingFactor and initialising it to 0
  uint16_t MappingFactor = map(ComparingFactor,MinimumDistanceValue,MaximumDistanceValue,
                           MinimumNumberOfLCDBoxes,MaximumNumberOfLCDBoxes);                                 //Taking our input and break it down to 16
  if (MappingFactor > PreviousMappingValue)                                                                  //Check if the number changes to a higher number
  {
    for (i = 0; MappingFactor >= i; i++)                                                                     //Counting up from 0 to 15
    {
      lcd.setCursor(i, 1);                                                                                   //Start at the bottom left and work forward
      Serial.printf("hi \n");
      lcd.write(LCDPixelBlock);                                                                              //Shows a block
    }
  }
  if (MappingFactor <= PreviousMappingValue)                                                                 //Checking if the number changed to a smaller number
  {
    for (i = 15; MappingFactor <= i; i--)                                                                    //Count down from 15 to 0
    {
      lcd.setCursor(i, 1);                                                                                   //Start at the bottom right and work back
      lcd.write(LCDPixelBlank);                                                                              //Show the blank on the lcd
    }
  }
  PreviousMappingValue = MappingFactor;                                                                      //Updating the Mapping Value
}

void loop()
{
  unsigned long Time = GettingTheDistance();                                                                 //Storing the value of the function in an unsignedlong varible and using it                                                                                                            //as an argument
  ConvertingAndPrinting(Time);                                                                               //
  SensorAndLED(Time);                                                                                        //Calling these two functions in the loop so that they keep on running'
}


