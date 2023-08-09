/**************************************************
Contributors:       Fawaaz Kamali Siddiqui
---------------------------------------------------
Last Update:        9-August-2023
===================================================
Description:        The Arduino Nano is connected to an RFID
                    Scanner. The UID of the tag/card is read 
                    from the scanner and written to the Arduino
                    Mega that is controlling all the other 
                    components. This sketch is controlling the 
                    Arduino Nano.
===================================================
Connections:        SDA:        10
                    SCK:        13
                    MOSI:       11
                    MISO:       12
                    RST:        9                                
**************************************************/

#include <SPI.h>
#include <MFRC522.h>

// Variable Declarations 
#define SS_PIN 10
#define RST_PIN 9


// Instantiations - for devices with separate libraries
MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {

    // Initialize connections
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("RFID Sender Initialized"); 


}

void loop() {

    // Main program 
    
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.write(mfrc522.uid.uidByte, mfrc522.uid.size);
        mfrc522.PICC_HaltA();
    }
    

}


