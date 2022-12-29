#include <LiquidCrystal.h>
#include <Servo.h>


// variables for defult mode
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int botton1 = 7;
int botton2 = 6;
int mode = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// variables for first mode
int countedTime = 0;
bool calibrated = false;
Servo servo;

// variables for second mode
int motionSensor = 8;
int buzzer = 10;
bool detection = false;


void setup() 
{
  lcd.begin(16, 2);
  
  servo.attach(9);
  
  pinMode(botton1,INPUT);
  pinMode(botton2,INPUT);
  
  pinMode(motionSensor,INPUT);
  pinMode(buzzer,OUTPUT);

  servo.write(90);
}

void loop() 
{

  switch(mode)
  {
    case 0:
      DefultMode();
      break;
    case 1:
      FirstMode();
      break;
    case 2:
      SecondMode();
      break;
  }
  delay(100);
}


//**************************************************************************************************************//
// Deafult mode is for selecting function of our project                                                        //
//**************************************************************************************************************//
void DefultMode(){
  lcd.setCursor(0, 0);
  lcd.print("Press Button    ");
  lcd.setCursor(0, 1);
  lcd.print("1-servo 2-motion");

  if(BottonClicked(botton1)==true)
    mode = 1;
  else if(BottonClicked(botton2)==true)
    mode = 2;
}


//**************************************************************************************************************//
// First mode is for servo to wich we calibrate time of it's work                                               //
//**************************************************************************************************************//
void FirstMode()
{
  
  if(calibrated == false)
  {
      lcd.setCursor(0, 0);
      lcd.print("Click to calib. ");
      lcd.setCursor(0, 1);
      lcd.print("1-calib. 2-exit ");
    if(BottonClicked(botton1)==true)
    {
          countedTime = 0;
          FMCalibration();
    }else if(BottonClicked(botton2)==true){
      mode = 0;
    }
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Calibreted      ");
    lcd.setCursor(0, 1);
    lcd.print("1-activ  2-exit ");

    if(BottonClicked(botton1)==true){
      servo.write(0);
      delay(countedTime);
      servo.write(90);
      delay(500);
      servo.write(180);
      delay(countedTime);
      servo.write(90);
      delay(500); 
  }
       
  }
  if(BottonClicked(botton2)==true){
      mode = 0;
      calibrated = false;
  }
  
}
//**************************************************************************************************************//
// First mode calibration acctivate sequence of time measurement                                                //
//**************************************************************************************************************//
void FMCalibration()
{
  bool stop = false;
  lcd.setCursor(0, 0);
  lcd.print("Calibration...  ");
  lcd.setCursor(0, 1);
  lcd.print("     1-stop     ");
  while(stop != true){
    countedTime = countedTime + 1;
    if(BottonClicked(botton1)==true)
    {
        stop = true;
        calibrated = true;
    }
    delay(1);
  }
}

//**************************************************************************************************************//
// Second mode is for moion detector signaled by buzzer                                                         //
//**************************************************************************************************************//
void SecondMode()
{
  if(detection == false)
  {
    lcd.setCursor(0, 0);
    lcd.print("Motion detector ");
    lcd.setCursor(0, 1);
    lcd.print("1-start 2-exit  ");

    if(BottonClicked(botton1)==true) 
      detection = true;
    else if(BottonClicked(botton2)==true) 
      mode = 0;
  }
  else    
    MotionDetection();

}

//**************************************************************************************************************//
// Second mode is for moion detector signaled by buzzer                                                         //
//**************************************************************************************************************//
void MotionDetection(){
  int motion = digitalRead(motionSensor);
  
  if(motion == LOW){
    lcd.setCursor(0, 0);
    lcd.print("   No Motion    ");
    lcd.setCursor(0, 1);
    lcd.print("     2-exit     ");
    if(BottonClicked(botton2)==true)
    {
        noTone(buzzer);
        detection = false;
        mode = 0;
    }    
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("!Motion detected");
    lcd.setCursor(0, 1);
    lcd.print("     2-exit     ");
    
    tone(buzzer, 3000, 1000);
    if(BottonClicked(botton2)==true)
    {
        noTone(buzzer);
        detection = false;
        mode = 0;
    }      
  }
  
}

//**************************************************************************************************************//
// Botton clicked is for preventing the detection of voltage oscillations                                       //
//**************************************************************************************************************//
bool BottonClicked(int botton)
{
  if(digitalRead(botton)==HIGH)
  {
      delay(20);
      while(digitalRead(botton) == HIGH);      
      delay(20);
      return true;
  }else return false;
}