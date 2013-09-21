#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 2
SoftwareSerial rfserial = SoftwareSerial(rxPin, txPin);

byte SeekCard[] = {0xFF,0x00,0x01,0x82,0x83};
byte Prim_Card[] = {0xFF,0x00,0x06,0x82,0x02,0xCD,0xD0,0xFC,0x64,0x87};

void setup(){
    Serial.begin(9600);
    rfserial.begin(19200);
  }

void loop(){
  SeekTag();
  delay(2000);
}

void printbad()
{
Serial.println("Error Reading Error");
}

void SeekTag()
{

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

    //seek card command
    for (int i =0; i< 5; i++){
    rfserial.write(SeekCard[i]);
    }

    if(rfserial.available()>0)
    {
    //check for response from module

      Serial.print("n Preparing to read...n Checking Checksum n");
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
            rfserial.flush();
            delay(5000);

              //read card data, capture data and print
             if(rfserial.available())
                {
                    for (int i =0; i< 11; i++){
                    CardResponse[i] = rfserial.read();
                    Serial.print(CardResponse[i], HEX);
                    Serial.print(" ");  
                    }
                    Serial.println("");
                    //check the checksum of the card
                    for (int i =1; i<9; i++){
                     cardchecksum += CardResponse[i];
                    }

                    //check the checksum of the card
                    if (cardchecksum == Prim_Card[9]){

                      //if the card matches then this is the primary card
                      Serial.println("This is Primary Card");

                    }
                    else{
                      //else this is the secondary card
                      Serial.println("Not Primary");
                    }
                }

        }
        else{
        printbad();

        }
    }
}
