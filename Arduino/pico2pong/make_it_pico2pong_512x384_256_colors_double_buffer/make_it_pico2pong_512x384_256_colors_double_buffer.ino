/* make_it_pico2pong_512x384_256_colors_double_buffer.ino
   weigu.lu
   Relies on the fantastic adafruit library
*/

#include <Adafruit_dvhstx.h>
#include <Fonts/FreeSansBold18pt7b.h> // A custom font

DVHSTXPinout pinConfig = ADAFRUIT_HSTXDVIBELL_CFG;
//DVHSTXPinout pinConfig = DVHSTX_PINOUT_DEFAULT;
DVHSTX8 display(pinConfig, DVHSTX_RESOLUTION_512x384,true);  // 256 colors from palette
//DVHSTX8 display(pinConfig, DVHSTX_RESOLUTION_640x480);  // does not work with buffer!

//#define DEBUG
#define P2P_BAUDRATE   3000000
#define DEBUG_BAUDRATE 1000000

#define LED_BUILTIN 26
#define HEIGHT  384
#define WIDTH   512
#define BLACK   0
#define RED     1
#define GREEN   2
#define BLUE    3
#define MAGENTA 4
#define YELLOW  5
#define CYAN    6 
#define WHITE   7

const unsigned int MAX_SERIAL_STREAM_LENGTH = 30;
byte telegram[MAX_SERIAL_STREAM_LENGTH];
unsigned int serial_count = 0;  
byte incoming_byte;
int paddle0_x = 0, paddle0_y = 150;
int paddle1_x = 502, paddle1_y = 150;
int ball_x = WIDTH/2, ball_y = HEIGHT/2;
byte score_0 = 0, score_1 = 0;
unsigned int paddle0_width = 10, paddle0_lenght = 84, paddle0_color = YELLOW;
unsigned int paddle1_width = 10, paddle1_lenght = 84, paddle1_color = CYAN;
unsigned int ball_radius = 5, ball_color = GREEN;

void clear_serial2_buffer() {
  while (Serial2.available() > 0) {  //clear the buffer!
    Serial2.read();
  } 
}

unsigned int read_telegram() {
  unsigned int serial_cnt = 0;
  if (Serial2.available()) {   // Test the header// wait for the whole stream
    delayMicroseconds(100);  // 27 char = 270 bit, with 1000000bit/s use 300µs
  }
  telegram[0] = Serial2.read();  
  if (telegram[0] == 0xEE) {    
    while ((Serial2.available()) && (serial_cnt < MAX_SERIAL_STREAM_LENGTH)) {
      serial_cnt++;
      telegram[serial_cnt] = Serial2.read();
      if (telegram[serial_cnt] == '\n') {      
        clear_serial2_buffer;    
        return serial_cnt;            
      }
    }
    clear_serial2_buffer;
    return 0;            
  }  
  clear_serial2_buffer;
  return 0;  
}  

void parse_telegram(unsigned int serial_cnt) {
  unsigned int number;
  if (telegram[0] == 0xEE) {      
    for (byte i = 0; i < serial_cnt; i++) {
      if (telegram[i] == 'p') {      
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= HEIGHT)) {
          paddle0_y = number;
        }  
        #ifdef DEBUG
          Serial.print(" p0: ");
          Serial.print(paddle0_y);
        #endif // DEBUG  
      }
      if (telegram[i] == 'q') {      
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= HEIGHT)) {
          paddle1_y = number;
        }  
        #ifdef DEBUG
          Serial.print(" p1: ");
          Serial.print(paddle1_y);
        #endif // DEBUG  
      }
      if (telegram[i] == 'x') {      
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= WIDTH)) {
          ball_x = number;
        }  
        #ifdef DEBUG
          Serial.print(" x: ");
          Serial.print(ball_x);
        #endif // DEBUG  
      }
      if (telegram[i] == 'y') {      
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= HEIGHT)) {
          ball_y = number;
        }  
        #ifdef DEBUG
          Serial.print(" y: ");
          Serial.print(ball_y);
        #endif // DEBUG    
      }
      /*if (telegram[i] == 'r') {
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= 100)) {
          ball_radius = number;
        }
        #ifdef DEBUG
        Serial.print(" r: ");
        Serial.print(ball_radius);
        #endif // DEBUG
      }*/
      if (telegram[i] == 's') {      
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= 9)) {
          score_0 = number;
        }  
        #ifdef DEBUG
          Serial.print(" s0: ");
          Serial.print(score_0);
        #endif // DEBUG    
      }
      if (telegram[i] == 't') {      
        number = (telegram[i+1]-48)*100 + (telegram[i+2]-48)*10 + (telegram[i+3]-48);
        if ((number >= 0) && (number <= 9)) {
          score_1 = number;
        }  
        #ifdef DEBUG
          Serial.print(" s1: ");
          Serial.print(score_1);
        #endif // DEBUG    
      }
    }  
  }
  #ifdef DEBUG
    Serial.println();
  #endif // DEBUG    
}

void set_palette_colors() {
  display.setColor(0,0x00000000); // BLACK  
  display.setColor(1,0x00FF0000); // RED
  display.setColor(2,0x0000FF00); // GREEN
  display.setColor(3,0x000000FF); // BLUE
  display.setColor(4,0x00FF00FF); // MAGENTA
  display.setColor(5,0x00FFFF00); // YELLOW
  display.setColor(6,0x0000FFFF); // CYAN
  display.setColor(7,0x00FFFFFF); // WHITE
}

void show_colors() {
  display.fillRect(0,10,18,20,0);
  display.fillRect(20,10,18,20,1);
  display.fillRect(40,10,18,20,2);
  display.fillRect(60,10,18,20,3);
  display.fillRect(80,10,18,20,4);
  display.fillRect(100,10,18,20,5);
  display.fillRect(120,10,18,20,6);
  display.fillRect(140,10,18,20,7);   
}

void setup() { // Runs once on startup
  Serial.begin(DEBUG_BAUDRATE);
  delay(1000);
  Serial2.setRX(5);
  Serial2.setTX(4);
  delay(1000);
  Serial2.begin(P2P_BAUDRATE); 
  delay(1000);
  Serial.println("Hello Serial\n");
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }  
  set_palette_colors(); 
  display.fillScreen(BLACK); // Backround      
  show_colors();    
  delay(1000);
  display.fillScreen(0);  
}

void loop() {  
  serial_count = 0;
  serial_count = read_telegram();  
  #ifdef DEBUG  
    Serial.println(serial_count);
    if (serial_count > 5) {
      Serial.print(telegram[0],HEX);  
      for (byte i = 1; i < serial_count; i++) {
        Serial.write(telegram[i]);  
      }  
    }  
    Serial.println();
  #endif // DEBUG    
  if ((serial_count > 5) && (serial_count < MAX_SERIAL_STREAM_LENGTH)) {
    parse_telegram(serial_count);
  }      
  unsigned int num = 5;
  display.fillScreen(0);
  display.setCursor(175, 5);
  display.setTextSize(3);  
  display.setTextColor(WHITE);
  display.print("PICO2PONG");
  display.setTextSize(1);  
  display.setTextColor(YELLOW);
  display.setCursor(15, 5); 
  display.print("Player 0");
  display.setTextSize(3);  
  display.setCursor(100, 20); 
  display.print(score_0);
  display.setTextSize(1);  
  display.setTextColor(CYAN);  
  display.setCursor(450, 5); 
  display.print("Player 1");
  display.setTextSize(3);  
  display.setCursor(400, 20); 
  display.print(score_1);

  if (score_0 == 5) {    
    display.setTextColor(RED);
    display.setTextSize(4);  
    display.setCursor(130, 250);
    display.print("!GAME OVER!");
    display.setCursor(110, 310);
    display.print("PLAYER 0 WINS");
  }
  else if (score_1 == 5) {    
    display.setTextColor(RED);
    display.setTextSize(4);  
    display.setCursor(130, 250);
    display.print("!GAME OVER!");
    display.setCursor(110, 310);
    display.print("PLAYER 1 WINS");
  }
  
  display.fillRect(paddle0_x, paddle0_y, paddle0_width, paddle0_lenght, paddle0_color);
  display.fillRect(paddle1_x, paddle1_y, paddle1_width, paddle1_lenght, paddle1_color);
  display.fillCircle(ball_x, ball_y, ball_radius, ball_color);
  display.swap();
}
