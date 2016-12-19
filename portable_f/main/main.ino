#include "U8glib.h"

U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);    // HW SPI Com: CS = 10, A0 = 9, RST = 8 (Hardware Pins are  SCK = 13 and MOSI = 11)

// settings for I/O pins
#define KEY_PIN_X_AXIS 14          //x축 움직이기, A14
#define KEY_PIN_Y_AXIS 15          //y축 움직이기(회전하기), A15

#define DISPLAY_BACKLIGHT_PIN 7   // pin for lcd backlight (need pwm)

// game settings
#define BOARD_COLS 12    // 10 columns
#define BOARD_ROWS 20   // 20 BOARD_ROWS

#define BLOCK_W 4            //  block width(pixels)
#define BLOCK_H 6             //  block height(pixels)

#define DROP_INTERVAL_INIT 550  // millisec, 블록이 떨어지는 속도

// key settings
#define KEY_NONE          0
#define KEY_PREV          1
#define KEY_NEXT          2

#define KEY_ROTATE_DOWN   3
#define KEY_ROTATE_UP     4
#define KEY_TET_LEFT      5
#define KEY_TET_RIGHT     6

#define KEY_GAME_START    7 //menu 에서 쓸 예정
#define KEY_BACK          8

#define KEY_SPEED         9

// GAME STATE MACROS
#define GAME_STATE_MAIN   1
#define GAME_STATE_TETRIS 2
#define GAME_STATE_PAUSE  3
#define GAME_STATE_END    4

//키 할당, 아두이노 Digital 번호
uint8_t uiKeyPrev = 22; 
uint8_t uiKeyNext = 24;
uint8_t uiKeySelect = 26;
uint8_t uiKeyBack = 28;
uint8_t uiKeyLeft = 30;
uint8_t uiKeyRight = 32;
uint8_t uiKeySpeed = 34;

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
uint8_t last_key_code = KEY_NONE;

//화면 상태
uint8_t screen_current = 0;
//속도
uint8_t speed_state = 0;
//점수
int point_max = 0;
int point = 0;
char points_max[10] = "0";
char points[10] = "0";

int board[BOARD_ROWS][BOARD_COLS];
int block_shapes[7][16] = 
{
    { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int block_current[4][4];  // shape of current block
int block_x, block_y;      // position of current block

int drop_interval;           // interval(millisec)
long timer_lastmills;
long draw_lastmills;

boolean gameover;

void setup(void) 
{
  //Serial.begin(9600);
  
  //u8g.setRot180();
  u8g.setRot90();
  u8g.setContrast(0);
  analogWrite(DISPLAY_BACKLIGHT_PIN,1024);
  
  randomSeed(analogRead(0));
  
  keySetup();
  screen_current = 1;
}

void loop(void) 
{
  getKeyPress();
  keyPress();

  switch(screen_current)
  {
    case GAME_STATE_MAIN:  //  1
      //처음에 화면 보이게 하기
      u8g.firstPage();  
      do {
        draw();
      } while( u8g.nextPage() );
      // rebuild the picture after some delay
      delay(5); 
      break;
    case GAME_STATE_TETRIS: //  2
      // picture loop
      if (50 < (millis() - draw_lastmills)) 
      {
        draw_lastmills = millis();
        u8g.firstPage();  
        do {
            render();
        } while( u8g.nextPage() );
        
        if (drop_interval < (millis() - timer_lastmills)) 
        {
          timer_lastmills = millis();
          tick();
        }
      }
      delay(5);
      break;
     case GAME_STATE_PAUSE:  //  3
      //중간에 화면 보이게 하기
      u8g.firstPage();  
      do {
        draw_2();
      } while( u8g.nextPage() );
      // rebuild the picture after some delay
      delay(5); 
      break;
    default://정의되지 않은 값일 경우
      break;
  }
}


