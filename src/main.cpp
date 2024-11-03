#include <OLED_I2C.h>

class Button{
  private:
  bool lastStateButton = false;
  byte buttonPin;
  public:
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
};

OLED  myOLED(SDA, SCL);
extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
Button button_UP(4);
Button button_DOWN(2);
Button button_ENTER(7);
Button button_BACK(8);

uint32_t oledUpdateTimer;
int oledUpdateDelay = 100;
int oledUpdateCount;

int fps;
uint32_t fpsTimer;
unsigned int fpsDelay = 1000;

volatile int cursorPosition;
int averageBrightness_1;
int averageBrightness_2;
unsigned long brightness_1;
unsigned long brightness_2;
byte brightnessCounter;


void getBrightness(){
  
  while(true){
    brightness_1 += analogRead(A6);
    brightness_2 += analogRead(A7);

    brightnessCounter++;
    if(brightnessCounter >= 10){
      averageBrightness_1 = brightness_1/10;
      averageBrightness_2 = brightness_2/10;
      brightnessCounter = 0;
      brightness_1 = 0;
      brightness_2 = 0;
    }

    myOLED.clrScr();
    myOLED.print("Brightness", CENTER, 0);
    myOLED.setFont(BigNumbers);
    myOLED.printNumI(averageBrightness_1 + averageBrightness_2, CENTER, 20);
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
    myOLED.printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 20);
    myOLED.setFont(BigNumbers);
    myOLED.printNumI(voltage, CENTER, 30);
    myOLED.setFont(SmallFont);
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

  pinMode(2, INPUT_PULLUP); //button down
  pinMode(4, INPUT_PULLUP); //button up
  pinMode(7, INPUT_PULLUP); //button enter
  pinMode(8, INPUT_PULLUP); //button back

  pinMode(A6, INPUT); //first light sensor
  pinMode(A7, INPUT); //second light sensor
  pinMode(A3, INPUT); //potentiometer
  pinMode(A2, INPUT); //voltage

  analogReference(DEFAULT);
  Serial.println("Start");
}

void loop() {
  if(button_UP.buttonWasPressed()){
    cursorPosition--;
  }
  if(button_DOWN.buttonWasPressed()){
    cursorPosition++;
  }
  cursorPosition = constrain(cursorPosition, 1, 4);

  if(int(millis()-oledUpdateTimer) >= oledUpdateDelay){
    oledUpdateTimer = millis();
    myOLED.clrScr();
    myOLED.print("MAIN MENU", CENTER, 0);
    myOLED.print("Brightness", 10, 10);
    myOLED.print("Get voltage", 10, 20);
    myOLED.print("Set voltage (soon)", 10, 30);
    myOLED.print("Set PWM", 10, 40);
    myOLED.print(">", 0, cursorPosition*10);
    myOLED.update();
    oledUpdateCount++;
  }

  if(int(millis()-fpsTimer) >= fpsDelay){
    fps = oledUpdateCount;
    oledUpdateCount = 0;
    fpsTimer = millis();
  }
  
  if(button_ENTER.buttonWasPressed()){
    if(cursorPosition == 1){
      getBrightness();
    }else if(cursorPosition == 2){
      getVoltage();
    }else if(cursorPosition == 4){
      setPWM();
    }
  }
}