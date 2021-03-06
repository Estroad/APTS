#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SSID        "hhhkkkhhhkkk"  

#define PASSWORD    "111111asd"  

#define HOST_NAME   "192.168.0.20"  

#define HOST_PORT   (2080)

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
//uint8_t *getnum2[12];

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
      uint8_t *aaa="";
          if(wifi.createTCP("192.168.0.20", 2080)){
       uint8_t sbuf[] = "ready";
         wifi.send(sbuf, strlen(sbuf));
         char *test ="test!!";
         uint32_t s = strlen(test);
         wifi.send(test,s);
      
  }
  else{
    Serial.print("err");
  }
     for(int i = 0; i<=11;i++){
        Serial.print(getnum[i]);
        //strcat(aaa,getnum[i]);
     // aaa += getnum[i];
      //getnum2[i] = getnum[i];
      }
      
//wifi.send( aaa,12);
//wifi.send( getnum2,12);

     /*if( wifi.send( asd,strlen(asd))){
      Serial.print("sendok");
     }
     else{
      Serial.print("WHYYYYYYYYYYYY");
}*/
}
