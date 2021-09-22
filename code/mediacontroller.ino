#include <CapPin.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

/*=====================================================*/
/* Capacitive Touch */

CapPin playpause = CapPin(6);
CapPin next = CapPin(9);
CapPin voldown = CapPin(10);
CapPin volup = CapPin(12);
CapPin pins[] = {playpause, next, voldown, volup};
char* pinNames[] = {"PLAYPAUSE", "MEDIANEXT", "VOLUME-", "VOLUME+"};
boolean currentPressed[] = {false, false, false, false};
#define THRESH 100
float smoothed[8] = {0,0,0,0};
/*=====================================================*/

/*=====================================================*/
/* Bluetooth */

Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);
/*=====================================================*/

void setup()                    
{

  Serial.begin(115200);
  
  // start bluetooth
  if ( !ble.begin(VERBOSE_MODE) )
  {
    Serial.println("Couldn't find Bluefruit");
  }
  Serial.println( F("Bluetooth OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      Serial.println(F("Factory reset failed!"));
    }
  }

  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // This demo only works with firmware 0.6.6 and higher!
  // Request the Bluefruit firmware rev and check if it is valid
  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    Serial.println(F("This sketch requires firmware version " MINIMUM_FIRMWARE_VERSION " or higher!"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Services (including Control Key): "));
  if (! ble.sendCommandCheckOK(F( "AT+BLEHIDEN=On"  ))) {
    Serial.println(F("Failed to enable HID (firmware >=0.6.6?)"));
  }

  /* Adding or removing services requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    Serial.println(F("Couldn't reset??"));
  }
}

void loop()                    
{
  for (int i=0; i<4; i++) {
    delay(1);
    long touch = pins[i].readPin(2000);
    smoothed[i] = smooth(touch, .8, smoothed[i]);

    if ((smoothed[i] > THRESH) && (! currentPressed[i])) {
      currentPressed[i] = true;
      ble.print("AT+BleHidControlKey=");
      ble.println(pinNames[i]);
      if( ! ble.waitForOK() ) {
        Serial.println( F("FAILED!") );
        // Failed, probably pairing is not complete yet
        Serial.println( F("Please make sure Bluefruit is paired and try again") );
      }
    } else if ((smoothed[i] <= THRESH) && (currentPressed[i])) {
      currentPressed[i] = false;
    }
    delay(5);
  }                       
}

// simple lowpass filter
// requires recycling the output in the "smoothedVal" param
int smooth(int data, float filterVal, float smoothedVal){

  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .999999;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}
