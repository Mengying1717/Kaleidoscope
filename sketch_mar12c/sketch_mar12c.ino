#include <Mouse.h>
int horzPin = A0;  // Analog output of horizontal joystick pin
int vertPin = A1;  // Analog output of vertical joystick pin
int analogInPin = A2; // Analog output of potentiometer pin
int savePin = 8; // digital output of save button
int cleanPin = 9; // digital output of clean button
int shapePin = 6; // digital output of shape mode button
int linePin = 4; // digital output of line mode button
int lineLedPin = 5;
int shapeLedPin = 7;

int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
const int sensitivity = 180;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouseClickFlag = 0;

int invertMouse = 1;        //Invert joystick based on orientation

void setup(){
  Serial.begin(9600);
  while(!Serial){
    ;;
  }

  pinMode(horzPin, INPUT);  // Set all pins as inputs
  pinMode(vertPin, INPUT);
  pinMode(savePin, INPUT);
  pinMode(cleanPin, INPUT);
  pinMode(shapePin, INPUT);
  pinMode(linePin, INPUT); 
  pinMode(lineLedPin, OUTPUT);
  pinMode(shapeLedPin, OUTPUT);
  
  digitalWrite(lineLedPin, HIGH); 
  digitalWrite(shapeLedPin, HIGH); 
  digitalWrite(linePin, LOW);  // Pull button select pin LOW
  
  delay(1000);  // short delay to let outputs settle
  vertZero = analogRead(vertPin);  // get the initial values
  horzZero = analogRead(horzPin);  // Joystick should be in neutral position when reading these

  Mouse.begin(); //Init mouse emulation
}

void loop(){
  
  int ardRead = analogRead(analogInPin);
  int mappedVal = map(ardRead, 0, 1023, 1,30);
  int saveRead = digitalRead(savePin);
  int cleanRead = digitalRead(cleanPin);
  int shapeRead = digitalRead(shapePin);
  int lineRead = digitalRead(linePin);

  String mappedValS = String(mappedVal);
  String saveReadS = String(saveRead);
  String cleanReadS = String(cleanRead);
  String shapeReadS = String(shapeRead);
  String lineReadS = String(lineRead);
  
  
  Serial.println(mappedValS + "," + saveReadS + "," + cleanReadS + "," + shapeReadS + "," + lineReadS);
  delay(1);

  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

  if (vertValue != 0){
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
  }
  if (horzValue != 0){
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis
  }

  if(digitalRead(shapePin) == HIGH){
    digitalWrite(shapeLedPin, LOW);
  } else{
    digitalWrite(shapeLedPin, HIGH);
  } 
  if(digitalRead(linePin) == HIGH){
    if (!Mouse.isPressed(MOUSE_LEFT)) {
         Mouse.press(MOUSE_LEFT);
         digitalWrite(lineLedPin, LOW);
      }
   } else {                           // else the mouse button is not pressed:
      // if the mouse is pressed, release it:
      if (Mouse.isPressed(MOUSE_LEFT)) {
         Mouse.release(MOUSE_LEFT);
         digitalWrite(lineLedPin, HIGH);
      }
  } 
}
