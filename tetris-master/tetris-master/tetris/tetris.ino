//  based on; http://coderecipe.jp/recipe/iHjJBJx9Si/

#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);		// HW SPI Com: CS = 10, A0 = 9, RST = 8 (Hardware Pins are  SCK = 13 and MOSI = 11)

// settings for I/O pins
#define KEY_PIN_PREV 4            // pin for previous button
#define KEY_PIN_NEXT 3            // pin for next button
#define KEY_PIN_SELECT 2          // pin for select button
#define BUZZER_PIN 5              // pin for buzzer (need pwm)
#define DISPLAY_BACKLIGHT_PIN 7   // pin for lcd backlight (need pwm)

// game settings
#define BOARD_COLS 12    // 10 columns
#define BOARD_ROWS 20   // 20 BOARD_ROWS

#define BLOCK_W 4            //  block width(pixels)
#define BLOCK_H 6             //  block height(pixels)

#define DROP_INTERVAL_INIT 250  // millisec


// key settings
#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
uint8_t last_key_code = KEY_NONE;

int board[BOARD_ROWS][BOARD_COLS];
int block_shapes[7][16] = {
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

void setup(void) {
  //Serial.begin(9600);
  
  u8g.setRot90();
  u8g.setContrast(0);
  analogWrite(DISPLAY_BACKLIGHT_PIN,1024);
  
  randomSeed(analogRead(0));
  
  keySetup();
  newGame();
}

void loop(void) {
  getKeyPress();
  keyPress();
  
  // picture loop
  if (50 < (millis() - draw_lastmills)) {
    draw_lastmills = millis();
    u8g.firstPage();  
    do {
      render();
    } while( u8g.nextPage() );
    
    if (drop_interval < (millis() - timer_lastmills)) {
      timer_lastmills = millis();
      tick();
    }
  }

  delay(5); 
}

void newGame() {
  //  board init
  for (int y=0;y<BOARD_ROWS;y++) {
    for (int x=0;x<BOARD_COLS;x++) {
      board[y][x] =0;
    }
  }
  
  drop_interval = DROP_INTERVAL_INIT;
  timer_lastmills = 0;
  draw_lastmills = 0;
  gameover = false; 
  
  nextBlock();
}


void nextBlock() {
  int id = random(0,7);
  int i = 0;
  for (int y=0;y<4;y++) {
    for (int x=0;x<4;x++) {
      block_current[y][x] = block_shapes[id][i++];
    }
  }
  
  block_x = 4;
  block_y = 0;
}

void tick() {
  if (valid( 0, 1, block_current )) {
    block_y++;
  }
  else {
    freeze();
    clearLines();
    if (gameover) {
      newGame();
      return;
    }
    
    nextBlock();
  }
}

boolean valid(int offsetX, int offsetY, int newCurrent[4][4]) {
  offsetX = block_x + offsetX;
  offsetY = block_y + offsetY;
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      if ( newCurrent[ y ][ x ] ) {
        if (
          x + offsetX < 0 || y+offsetY >= BOARD_ROWS || x+offsetX >= BOARD_COLS ||
          board[y+offsetY][x+offsetX] == 1
        ) {
          if (offsetY == 1 &&  offsetX-block_x == 0 && offsetY-block_y == 1) {
            gameover = true;
          }
          return false;
        }
      }
    }
  }
  return true;
}

void freeze() {
  for (int y=0;y<4;y++) {
    for (int x=0;x<4;x++) {
      if (block_current[y][x] == 1) {
        board[y+block_y][x+block_x] = block_current[y][x];
      }
    }
  }
}

void clearLines() {
  for (int y = BOARD_ROWS-1; y>=0; y--) {
    boolean rowFilled = true;
    for (int x = 0;x<BOARD_COLS;x++) {
      if (board[y][x] == 0) {
        rowFilled = false;
        break;
      }
    }
    
    if (rowFilled) {
      tone(BUZZER_PIN, 1024, 20);

      for (int yy=y; yy>0; yy--) {
        for (int x=0; x<BOARD_COLS;x++) {
          board[yy][x] = board[yy-1][x];
        }
      }
      y++;
    }
  }
}


void render() { 
  //  draw frame
  u8g.drawLine(6, 0, 6, BLOCK_H * BOARD_ROWS + 2);
  u8g.drawLine(BLOCK_W * BOARD_COLS + 9, 0, BLOCK_W * BOARD_COLS + 9, BLOCK_H * BOARD_ROWS + 2);
  u8g.drawLine(6, BLOCK_H * BOARD_ROWS + 2, BLOCK_W * BOARD_COLS + 9, BLOCK_H * BOARD_ROWS + 2);
  
  //  draw board
  for (int x=0;x<BOARD_COLS;x++) {
    for (int y=0;y<BOARD_ROWS;y++) {
      if (board[y][x] == 1) {
            drawBlock(x, y);
      }
    }
  }
  
  //  draw block shape
  for (int y=0;y<4;y++) {
    for (int x=0;x<4;x++) {
      if (block_current[y][x] == 1) {
        drawBlock(block_x+x, block_y+y);
      }
    }
  }
}

void drawBlock(int x, int y) {
  x = BLOCK_W * x;
  y = BLOCK_H * y;
  u8g.drawBox(x + 8, y, BLOCK_W, BLOCK_H);
}


void keyPress(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }

  last_key_code = uiKeyCode;
  draw_lastmills = 0;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if (valid(1,0,block_current)) {
        block_x++;
      }
      break;
    case KEY_PREV:
      if (valid(-1,0,block_current)) {
        block_x--;
      }
      break;
    case KEY_SELECT:
      // TODO:bug
      int rotated[4][4];
      
      for (int y=0;y<4;y++) {
        for (int x=0;x<4;x++) {
          rotated[y][x] = block_current[3-x][y];
        }
      }
      
      if (valid(0, 0, rotated)) {
        for (int y=0;y<4;y++) {
          for (int x=0;x<4;x++) {
            block_current[y][x] = rotated[y][x];
          }
        }
      }
      break;
  }
}

