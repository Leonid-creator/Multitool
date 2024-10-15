#include <OLED_I2C.h>

class Button{
  public:
  bool lastStateButton = false;
  byte buttonPin;
  Button(byte pin){
    buttonPin = pin;
  }
  bool buttonWasPressed(){
    if(!digitalRead(buttonPin) == true && lastStateButton == false){
      lastStateButton = true;
      return true;
    }
    if(!digitalRead(buttonPin) == false && lastStateButton == true){
      lastStateButton = false;
    }
    return false;
  }
  private:
};

OLED  myOLED(SDA, SCL);
extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
Button button_UP(4);
Button button_DOWN(2);
Button button_ENTER(8);
Button button_BACK(7);

volatile int cursorPosition;
int averageBrightness;
unsigned long brightness;
byte brightnessCounter;


void getBrightness(){
  
  while(true){
    brightness += analogRead(A6);
    brightnessCounter++;
    if(brightnessCounter >= 10){
      averageBrightness = brightness/10;
      brightnessCounter = 0;
      brightness = 0;
    }
    myOLED.clrScr();
    myOLED.print("Brightness", CENTER, 0);
    myOLED.setFont(BigNumbers);
    myOLED.printNumI(averageBrightness, CENTER, 20);
    myOLED.setFont(SmallFont);
    myOLED.update();
    if(button_BACK.buttonWasPressed()){
      break;
    }
  }
}

void setPWM(){
  int pwmValue = 0;
  bool pwmIsReady = false;

  while(!pwmIsReady){
    pwmValue = (analogRead(3) / 4);
    if(pwmValue == 0){
      pwmIsReady = true;
      break;
    }
    myOLED.clrScr();
    myOLED.print("PWM is not ready", CENTER, 0);
    myOLED.print("set the potentiometer", CENTER, 10);
    myOLED.print(" to zero position", CENTER, 20);
    myOLED.printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 30);
    myOLED.update();
  }

  while(true){
    pwmValue = (analogRead(3) / 4);
    analogWrite(9, pwmValue);
    myOLED.clrScr();
    myOLED.print("PWM", CENTER, 0);
    myOLED.setFont(BigNumbers);
    myOLED.printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 20);
    myOLED.setFont(SmallFont);
    myOLED.update();
    if(button_BACK.buttonWasPressed()){
      analogWrite(9, 0);
      break;
    }
  }
}

void getVoltage(){
  int pwmValue = 0;
  float voltage = 0;
  while(true){
    pwmValue = (analogRead(3) / 4);
    analogWrite(9, pwmValue);
    voltage = analogRead(2);

    myOLED.clrScr();
    myOLED.print("PWM", CENTER, 0);
    //myOLED.setFont(BigNumbers);
    myOLED.printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 20);
    myOLED.printNumI(voltage, CENTER, 30);
    //myOLED.setFont(SmallFont);
    myOLED.update();
    if(button_BACK.buttonWasPressed()){
      analogWrite(9, 0);
      break;
    }
  }
}

void setup() {
  if(!myOLED.begin(SSD1306_128X64)){
    while(1);
  }
  myOLED.setFont(SmallFont);
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  pinMode(A6, INPUT); //first light sensor
  pinMode(A7, INPUT); //second light sensor
  pinMode(A3, INPUT); //potentiometer
  pinMode(A2, INPUT); //voltage

  analogReference(DEFAULT);
  Serial.println("Start");
}

// the loop function runs over and over again forever
void loop() {
  if(button_UP.buttonWasPressed()){
    cursorPosition--;
  }
  if(button_DOWN.buttonWasPressed()){
    cursorPosition++;
  }
  cursorPosition = constrain(cursorPosition, 1, 4);

  myOLED.clrScr();
  myOLED.print("MAIN MENU", CENTER, 0);
  myOLED.print("Brightness", 10, 10);
  myOLED.print("Get voltage", 10, 20);
  myOLED.print("Set voltage (soon)", 10, 30);
  myOLED.print("Set PWM", 10, 40);
  myOLED.print(">", 0, cursorPosition*10);
  myOLED.update();
  
  if(button_ENTER.buttonWasPressed()){
    if(cursorPosition == 1){
      getBrightness();
    }else if(cursorPosition == 2){
      getVoltage();
    }else if(cursorPosition == 4){
      setPWM();
    }
  }
  delay(2);
}