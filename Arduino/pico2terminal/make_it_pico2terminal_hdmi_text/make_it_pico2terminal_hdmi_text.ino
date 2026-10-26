// make_it_pico2terminal_hdmi_text
// weigu.lu
// This sketch uses the Adafruit-DVI-HSTX library, licensed under MIT.
// Original library: https://github.com/adafruit/Adafruit-DVI-HSTX.

#include <Adafruit_dvhstx.h>

DVHSTXPinout pinConfig = ADAFRUIT_HSTXDVIBELL_CFG;
//DVHSTXPinout pinConfig = DVHSTX_PINOUT_DEFAULT;
DVHSTXText display(pinConfig);

//# define LED_BUILTIN 26

void setup() {
  Serial.begin(115200);  
  Serial2.setRX(5);
  Serial2.setTX(4);  
  Serial2.setFIFOSize(3000); // 91 char/line * 30 lines = 2730
  Serial2.begin(1000000); 
  delay(1000);  
    if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
      delay(1000);//digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  display.setColor(TextColor::BG_BLACK, TextColor::TEXT_WHITE);
  display.clear();
  display.showCursor();
  display.print("Hi, to the Make It Text Terminal!\n");
  display.write('\n');
  Serial.println("Hello Serial\n");
}

byte incoming_byte;

void loop() {
   while (Serial2.available() > 0) {
    incoming_byte = Serial2.read();
    //Serial.write(incoming_byte);
    display.write(incoming_byte);

  }
  sleep_us(1000);
}
