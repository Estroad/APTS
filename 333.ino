#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SSID        "hhkkhhkk" 

#define PASSWORD    "111111asd"  

#define HOST_NAME   "192.168.0.20"  

#define HOST_PORT   (23)

#define SS_PIN 10
#define RST_PIN 9

bool isConnected = false;
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

SoftwareSerial mySerial(2,3); /* RX:D10, TX:D9 */
ESP8266 wifi(mySerial);

// Init array that will store new NUID 
byte nuidPICC[4];
int getnum[12];
uint8_t getnum2[12];

uint8_t *asd;
int tmp;

void setup() { 
  Serial.begin(9600);

  Serial.print("setup begin\r\n");
    
  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP:");
        Serial.println( wifi.getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }
    // wifi.createTCP(HOST_NAME, HOST_PORT);
    //uint8_t buf[]="Usage\n1 : Turn On LED\n2 : Turn Off LED\nS : LED status\n\n";
    //wifi.send(buf, strlen(buf));
    Serial.print("setup end\r\n");
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

    if(wifi.createTCP(HOST_NAME, HOST_PORT)){
       uint8_t sbuf[] = "ready";
         wifi.send(sbuf, strlen(sbuf));
         char *test ="test!!";
         uint32_t s = strlen(test);
         wifi.send(test,s);
      
  }
  else{
    Serial.print("err");
  }
}
 
void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    sendme();
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  int lot=0;
  //uint8_t* asd;
  asd ="0";
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    //wifi.send(buffer[i], strlen(buffer[i]));
    tmp = buffer[i];

   
    if((tmp/100) != 0){
    getnum[lot]= tmp/100;
    getnum[lot+1] = (tmp/10)-((tmp/100)*10);
    getnum[lot+2] = tmp%10;
    }
    else{
      getnum[lot] = 0;
       getnum[lot+1] = (tmp/10);
       getnum[lot+2] = tmp%10;
    }

    
   // getnum[lot2+3] = ' ';
    lot +=3; 
   
  }

   
       /*( while(1){
            if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
                Serial.print("create tcp ok\r\n");
                isConnected = true;
                Serial.print(strlen(getnum));
                 wifi.send(getnum, 12);
                 isConnected =false;
                break;
            } else {
                Serial.print("create tcp err\r\n");
            }
        }*/
     
     
     // sendme();
   //  wifi.createTCP(HOST_NAME, HOST_PORT);


//   wifi.createTCP(HOST_NAME, HOST_PORT); 
  // wifi.send(getnum, strlen(getnum));
 
}







void sendme(){
 

     for(int i = 0; i<=11;i++){
       uint8_t a1[]="1";
       uint8_t a2[]="2";
       uint8_t a3[]="3";
       uint8_t a4[]="4";
       uint8_t a5[]="5";
       uint8_t a6[]="6";
       uint8_t a7[]="7";
       uint8_t a8[]="8";
       uint8_t a9[]="9";
       uint8_t a0[]="0";
       
       
       
        Serial.print(getnum[i]);
       // Serial.print("  ");
        //strcat(aaa, getnum[i]);
         //aaa += (uint8_t) getnum[i]; //<---------------???
        switch(getnum[i]){
          case 1:
          wifi.send(a1,1);
          break;
          case 2:
          wifi.send(a2,1);
          break;
          case 3:
          wifi.send(a3,1);
          break;
          case 4:
          wifi.send(a4,1);
          break;
          case 5:
          wifi.send(a5,1);
          break;
          case 6:
          wifi.send(a6,1);
          break;
          case 7:
          wifi.send(a7,1);
          break;
          case 8:
          wifi.send(a8,1);
          break;
          case 9:
          wifi.send(a9,1);
          break;
          default:
          wifi.send(a0,1);
          break;
          
          
        }
      }
       //Serial.print(*aaa);
      //wifi.send(aaa, 10);
      //Serial.print(aaa[0]);
     // wifi.send(aaa,1);

     /*if( wifi.send( asd,strlen(asd))){
      Serial.print("sendok");
     }
     else{
      Serial.print("WHYYYYYYYYYYYY");
}*/
} 
