/**************************************************
Contributors:       Fawaaz Kamali Siddiqui
---------------------------------------------------
Last Update:        9-August-2023
===================================================
Description:        The Arduino Mega is receiving a tag/card
                    UID from the Arduino Nano. If a correct 
                    tag/card is scanned, a correct passcode
                    is required for entry. If the correct 
                    passcode is not entered, entry is denied.
                    This sketch is controlling the Arduino 
                    Mega.
===================================================
Connections:        rs:         44
                    en:         45
                    d4:         46
                    d5:         47
                    d6:         48
                    d7:         49
                    servo:      50      
                    buzzer:     A0        
                    keypad row: 5, 6, 7, 8
                    keypad col: 2, 3, 4                          
**************************************************/

#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>


// Variable Declarations 
int pos = 0;
int servoPin = 50;
int buzzerPin = A0;
const byte authorizedUID[4] = {0xC3, 0xDB, 0xFD, 0xF6};  // only one tag is given access
byte receivedUID[4];

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[rows] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad 
byte colPins[cols] = {2, 3, 4}; //connect to the column pinouts of the keypad 

const char* passcode = "55555";
String enteredPasscode;
int passcodeLength = strlen(passcode);

int rs = 44;
int en = 45;
int d4 = 46;
int d5 = 47;
int d6 = 48;
int d7 = 49;

// Instantiations - for devices with seperate libraries
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
Servo servo;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {

    // Initialize connections 
    Serial.begin(9600);
    Serial.println("RFID Receiver Initialized");

    servo.attach(servoPin);
    pinMode(buzzerPin, OUTPUT);

    lcd.begin(16,2);


}

void loop() {

    // Main program 

    servo.write(pos);  // make sure door is locked initially
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Scan your card:");
    
    // Begin to scan RFID tag
    if (Serial.available() >= 4) { // Assuming UID size is 4 bytes
        Serial.readBytes(receivedUID, 4); // reading data from Arduino Nano
        
        Serial.print("Received UID Tag: ");
        printUID(receivedUID); // A custom function to print UID
        if (compareUID(receivedUID, authorizedUID)) {
            
            // Perfpasscode from userorm your authorized action here
            Serial.println("Enter passcode:");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Enter passcode:");
            enteredPasscode = getPasscode();
            if (enteredPasscode == passcode) {
                Serial.println("Access Authorized");

                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Access: ");
                lcd.setCursor(0,1);
                lcd.print("Authorized");

                // unlock door and then close it automatically
                unlockDoor();
                delay(5000);
                lockDoor();
            }
            else {  // if wrong passcode is entered
                Serial.println("Access Denied");
                
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Access:");
                lcd.setCursor(0,1);
                lcd.print("Denied");

                // turn on buzzer for 2 seconds
                analogWrite(buzzerPin, 255);
                delay(2000);
                analogWrite(buzzerPin, 0);                
            }
        } else {    // if wrong tag/card is scanned
            Serial.println("Unauthorized Access");

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Access:");
            lcd.setCursor(0,1);
            lcd.print("Denied");
            // turn on buzzer for 2 seconds
            analogWrite(buzzerPin, 255);
            delay(2000);
            analogWrite(buzzerPin, 0);
        }
        Serial.println();
    }
    

}

// Servo methods

void unlockDoor() {

    // rotate clockwise
    for (pos = 0; pos <= 180; pos++) {
        servo.write(pos);
        delay(15);  
    }
}

void lockDoor() {

    // rotate anti-clockwise
    for (pos = 180; pos >= 0; pos--) {
        servo.write(pos);
        delay(15);
    }
}

// RFID Scanner methods

void printUID(byte uid[4]) {
  for (int i = 0; i < 4; i++) {
    Serial.print(uid[i] < 0x10 ? "0" : "");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
}

bool compareUID(byte uid1[4], const byte uid2[4]) {
  for (int i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false; // UID mismatch
    }
  }
  return true; // UID match
}

// Matrix Keypad methods

char getKey() {
    char key = keypad.waitForKey();
    if (key) {
        Serial.print(key);
        return key;
    }
}

String getPasscode() {
    String userInput;
    for (int i = 0; i < passcodeLength; i++) {
        userInput += getKey();
        // output numbers to lcd screen
        lcd.setCursor(0,1);
        lcd.print(userInput);
    }
    delay(500);     // so user can see final number on lcd 
    return userInput;
}


