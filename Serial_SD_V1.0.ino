/*********************
ESP8266 as a TCP Servicer
*********************/

#define DEBUG true
// include the SD library:
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;

void setup()
{
    SerialUSB.begin(115200);
    //while (!SerialUSB) {
    ; // wait for serial port to connect
    //}
  Serial1.begin(115200); // your esp's baud rate might be different
  SerialUSB.print("\nMaduino Zero ESP8266 v1.6 Test Begin!\r\n");
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point 
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
 
    SerialUSB.print("\nInitializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega, 14 on Leonardo) must be left as an output
  // or the SD library functions will not work.


  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    SerialUSB.println("initialization failed. Things to check:");
    SerialUSB.println("* is a card is inserted?");
    SerialUSB.println("* Is your wiring correct?");
    SerialUSB.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    SerialUSB.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  SerialUSB.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      SerialUSB.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      SerialUSB.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      SerialUSB.println("SDHC");
      break;
    default:
      SerialUSB.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    SerialUSB.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  SerialUSB.print("\nVolume type is FAT");
  SerialUSB.println(volume.fatType(), DEC);
  SerialUSB.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  SerialUSB.print("Volume size (bytes): ");
  SerialUSB.println(volumesize);
  SerialUSB.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  SerialUSB.println(volumesize);
  SerialUSB.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  SerialUSB.println(volumesize);

}
 
void loop()
{
  
}
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    Serial1.print(command); // send the read character to the Serial
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial1.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial1.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      SerialUSB.print(response);
    }
    
    return response;
}

//boolean connectWiFi()
