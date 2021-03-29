#include <PN532_SPI.h>
#include <PN532.h>
#include <Ndef.h>
#include <NfcAdapter.h>

#include <ThingSpeak.h>
#include <WiFiNINA.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <SPI.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);

char SSID[] = "cs";
char pass[] = "gkmrxuW6";

// ThingSpeak information
char writeAPIKey[] = "9MYQGS5ORSSMV8XQ";
unsigned long channelID = 921031; 
int countTag = 0;

WiFiClient client;

void setup(void)
{  
    Serial.begin(4800); // start the arduino serial portß

    // wait for initilization of wifi
    Serial.println("Connecting");
    
    WiFi.begin(SSID, pass);
    ThingSpeak.begin(client);

    if (WiFi.status() != WL_CONNECTED) 
    {
        Serial.println("Failed");
    } 
    else 
    {
        Serial.println("Connected");
    } 
}

void loop()
{
    //As soon as the data  received from the Internet ,output the data through the UART Port .
    while (WiFi.status() == WL_CONNECTED)
    {
      nfc.begin();
      NfcTag tag = nfc.read();
      if (nfc.tagPresent()) // Do an NFC scan to see if an NFC tag is present
      {
        tag.print();
        String tagS = "1"; //connected
        countTag += 1;
        Serial.println(countTag);
        ThingSpeak.setField(3, tagS);
        ThingSpeak.writeFields(channelID, writeAPIKey);
      }
      if (!nfc.tagPresent())
      {
        String tagS = "2"; //not connected
        ThingSpeak.setField(3, tagS);
        ThingSpeak.writeFields(channelID, writeAPIKey);
      }
      delay(1000);
      exit(0);
     }
}