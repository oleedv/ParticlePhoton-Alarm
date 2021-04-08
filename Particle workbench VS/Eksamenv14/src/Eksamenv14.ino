/*
 * Project Eksamen_v1.4
 * Description: Alarm system that is supposed to be mounted above a doorway, so it registers whenever the door passes by it and triggers the alarm. 
 * Author:
 * Date:
 */
#include <IRremote.h>
#include <HC-SR04.h>

const int triggerPinHC_SR04 = A0;
const int echoPinHC_SR04 = D0;
const int buttonPin = D2;
const int buzzerPin = D3;
const int redLedPin = D4;
const int greenLedPin = D5;

const float distanceToTrigger = 20; //How close a object needs to be for the alarm to trigger
const int loopDelay = 200; // How often void loop() should run in ms

int buttonState = 0;
int alarmState = 0;
int emailAlreadySent = 0;
int emailCounter = 1;

IRsend irsend;
HC_SR04 rangefinder = HC_SR04(triggerPinHC_SR04, echoPinHC_SR04);
// setup() runs once, when the device is first turned on.
void setup()
{
  Serial.begin(9600);
  rangefinder.init();
  alarmState = 0;
  emailAlreadySent = 0;
  buttonState = 0;
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(redLedPin, HIGH); //Set the red led to on by default to show that the alarm is off
  // RGB_color(1, 0, 0); // Sets LED to red, to show that the alarm is off
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  float senrorInputTo = sensorInput();
  Serial.printf("Distance in centimeters: %.1f\n", senrorInputTo);
  //validate if must trigger on distance event, also the sensor sometimes malfunctions and outputs -1.0, this is accounted for.
  if (senrorInputTo <= distanceToTrigger && senrorInputTo >= 0)
  {
    if (alarmState == 1) //Check if the alarm is on
    {
      sendIR();
      if (emailAlreadySent == 0){
      String emailMsg = String("Someone has opened your door! Alarm has been sounded.");
      //sendEmail(email, subject, body)
      sendEmail("posium420@gmail.com", "Alarm has been triggered!", String(emailMsg)); //Please dont spam my email Mr.Sensor or Mrs. Sensor 
      emailAlreadySent = 1;
      }
    }
    else
    {
      Serial.println("Somebody passed the door, but alarm is off");
      //Flashes the red led once to indikate that the alarm is off but that it triggered
      digitalWrite(redLedPin, LOW);
      delay(100);
      digitalWrite(redLedPin, HIGH);
    }
  }

  buttonState = digitalRead(buttonPin);
  //String strTextButton = (String) buttonState;
  //Serial.println("Button state: " + strTextButton);
  //When btn is pressed, turn alarm on or off by using states
  if (buttonState == 1) // && senrorInputToInt < 60
  {
    Serial.println("Button pressed");
    if (alarmState == 0)
    {
      //alarm should be fired
      alarmState = 1; //alarm activated
      emailAlreadySent = 0;
    }
    else
    {
      //alarm set to off
      emailAlreadySent = 0;
      alarmState = 0;
    }
  }

  //trigger the alarm
  if (alarmState == 1)
  {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    //RGB_color(0, 255, 0); // Sets LED to GREEN, to show that the alarm is ON
    Serial.println("Alarm is on");
  }
  else
  {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    //RGB_color(255, 0, 0); // Sets LED to red, to show that the alarm is OFF
    Serial.println("Alarm is off");
  }

//Each loop, add +1 to emailCounter, when it reaches the limit it will reset. To avoid email spam and not need to reset it after each alarm
 if (emailCounter){
   emailCounter ++;
 }
  if (emailCounter == 150){
    emailAlreadySent = 0;
    emailCounter = 1;
  }
  String strEmailCounter = (String) emailCounter;
  String strEmailAlreadySent = (String) emailAlreadySent;
  Serial.println("Email Counter: " + strEmailCounter);
  Serial.println("Email state: " + strEmailAlreadySent);
    Serial.println("---------------------------------");
  delay(loopDelay);
}

void sendIR()
{
  irsend.sendNEC(0xF7C03F, 32); //Turn ON the LED Strips
  for (int i = 1; i < 11; i++)
  {
    Serial.print("Flashing LED Strips: ");
    Serial.println(i);

    flashLEDs();
    tone(buzzerPin, 5000, 3000);
    irsend.sendNEC(0xF720DF, 32); //Red
    delay(300);
    flashLEDs();
    tone(buzzerPin, 1000, 1000);
    irsend.sendNEC(0xF7E01F, 32); //White
    delay(200);
    flashLEDs();
    tone(buzzerPin, 5000, 3000);
    irsend.sendNEC(0xF7609F, 32); //Blue
    delay(300);
  }
  irsend.sendNEC(0xF740BF, 32); //Turn OFF the LED Strips
}

int sendEmail(String email, String subject, String body)
{
  String publishData = String::format(
      "{ \"my_email\": \"%s\", \"subject\": \"%s\", \"body\": \"%s\" }", email.c_str(), subject.c_str(), body.c_str());
  Particle.publish("mailgunEvent", publishData, PRIVATE);
  return 0;
}

float sensorInput()
{
  float cm = rangefinder.distCM();
  //Serial.printf("Range finding duration: %lu | Distance in centimeters: %.2f\n", calcTime, cm);
  //delay(1000);
  return cm;
}

void flashLEDs()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(redLedPin, HIGH);
    delay(10);
    digitalWrite(redLedPin, LOW);
    delay(10);
    digitalWrite(greenLedPin, HIGH);
    delay(10);
    digitalWrite(greenLedPin, LOW);
    delay(10);
  }
}

// Works but my RBG LED is broken, switched to two normal leds :(
void RGBcolor(int redValue, int greenValue, int blueValue)
{
  //digitalWrite(redPin, redValue);
  //digitalWrite(greenPin, greenValue);
  //digitalWrite(bluePin, blueValue);
}