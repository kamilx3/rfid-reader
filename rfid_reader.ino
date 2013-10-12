#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11
SoftwareSerial rfserial = SoftwareSerial(rxPin, txPin);

byte SeekCard[] = {0xFF,0x00,0x01,0x82,0x83};
byte Prim_Card[] = {0xFF,0x00,0x06,0x82,0x02,0xCD,0xD0,0xFC,0x64,0x87};

void setup(){
    Serial.begin(9600);
    rfserial.begin(19200);
}

void loop(){
    SeekTag();
    delay(1000);
}

void printbad(){
    Serial.println("Error Reading Error");
}

void SeekTag(){
    //an array for the RFID response acknowledging command
    byte ModResponse[6];
    //checksum reference
    byte modrespchecksum = 0xD0;
    //checkum checker
    byte response_checksum_math;
    //an array for the RFID card data that has been found
    byte CardResponse[10];
    //checksum variable for card
    byte cardchecksum;

    Serial.println("Sending read request.");
    for (int i =0; i< 5; i++){
        rfserial.write(SeekCard[i]);
    }
    rfserial.flush();
    delay(1000);
    //seek card command
    if(rfserial.available()>0)
    {
        //check for response from module
        Serial.print("\nPreparing to read...\nChecking Checksum \n");
        for (int i =0; i< 7; i++){
            ModResponse[i] = rfserial.read();      
            Serial.print(ModResponse[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
        //adds all the RFID command acknowledgement bytes except header and checksum
        for (int i= 1; i<5; i++){
            response_checksum_math += ModResponse[i];
        }
        //if the response is of checksum is equal to 0xD0
        if (modrespchecksum == response_checksum_math)
        {    //then seek for card
            Serial.println("Checksum is good");
            Serial.println("Waiting for card response.....");    

 //wait for signal from rfid    

            //flushng of buffer and delay is needed to read card
            delay(4000);

              //read card data, capture data and print
             if(rfserial.available())
                {
                    int i=0;
                    while (rfserial.available()){
                    CardResponse[i] = rfserial.read();
                    Serial.print(CardResponse[i], HEX);
                    Serial.print(" ");
                    i++; 
                    }
                    Serial.println("");
                    //check the checksum of the card
                    for (int i =1; i<9; i++){
                     cardchecksum += CardResponse[i];
                    }
                    
                    if (cardchecksum == Prim_Card[9]){
                     Serial.println("This is Primary Card");
                    }
                    else{
                      Serial.println("Not Primary");
                    }
                }
            rfserial.flush();
        }
        else{
          rfserial.flush();
          printbad();
        }
    }
}
