
void keySetup(void) 
{
  // configure input keys 
  pinMode(KEY_PIN_X_AXIS, INPUT_PULLUP);      // set pin to input
  pinMode(KEY_PIN_Y_AXIS, INPUT_PULLUP);           // set pin to input

  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyLeft, HIGH);       // turn on pullup resistors
  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyRight, HIGH);       // turn on pullup resistors
  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyPrev, HIGH);       // turn on pullup resistors
  pinMode(uiKeyNext, INPUT);           // set pin to input
  digitalWrite(uiKeyNext, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelect, INPUT);           // set pin to input
  digitalWrite(uiKeySelect, HIGH);       // turn on pullup resistors
  pinMode(uiKeyBack, INPUT);           // set pin to input
  digitalWrite(uiKeyBack, HIGH);       // turn on pullup resistors
  pinMode(uiKeySpeed, INPUT);           // set pin to input
  digitalWrite(uiKeySpeed, HIGH);       // turn on pullup resistors
}

void getKeyPress(void) 
{
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( analogRead(KEY_PIN_X_AXIS) >= 0 && analogRead(KEY_PIN_X_AXIS) <= 400 )
    uiKeyCodeFirst = KEY_TET_LEFT; //왼쪽으로 움직이기
  else if ( analogRead(KEY_PIN_X_AXIS) >=500 && analogRead(KEY_PIN_X_AXIS) <= 1000 )
    uiKeyCodeFirst = KEY_TET_RIGHT; //오른쪽으로 움직이기
  else if ( digitalRead(uiKeyLeft) == LOW )
    uiKeyCodeFirst = KEY_TET_LEFT; //왼쪽으로 움직이기, 버튼
  else if ( digitalRead(uiKeyRight) == LOW )
    uiKeyCodeFirst = KEY_TET_RIGHT; //오른쪽으로 움직이기, 버튼
  else if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = KEY_ROTATE_UP;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = KEY_ROTATE_DOWN;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = KEY_GAME_START;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = KEY_BACK;
  else if ( digitalRead(uiKeySpeed) == LOW )
    uiKeyCodeFirst = KEY_SPEED;
  else 
    uiKeyCodeFirst = KEY_NONE;

  if ( uiKeyCodeSecond == uiKeyCodeFirst ) 
  {
    uiKeyCode = uiKeyCodeFirst;
  }
  else 
  {
    uiKeyCode = KEY_NONE;
  }
}

void keyPress(void) 
{
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) 
  {
    return;
  }

  last_key_code = uiKeyCode;
  draw_lastmills = 0;
  
  switch ( uiKeyCode ) 
  {
    case KEY_TET_RIGHT: //블록을 오른쪽으로
      if (valid(1,0,block_current)) 
      {
        block_x++;
      }
      break;
    case KEY_TET_LEFT: //블록을 왼쪽으로
      if (valid(-1,0,block_current)) 
      {
        block_x--;
      }
      break;
    case KEY_ROTATE_DOWN: //블록을 시계 방향 회전
      // TODO:bug
      int rotated[4][4];
      
      for (int y=0;y<4;y++) 
      {
        for (int x=0;x<4;x++) 
        {
          rotated[y][x] = block_current[3-x][y];
        }
      }
      
      if (valid(0, 0, rotated)) 
      {
        for (int y=0;y<4;y++) 
        {
          for (int x=0;x<4;x++) 
          {
            block_current[y][x] = rotated[y][x];
          }
        }
      }
      break;
    case KEY_ROTATE_UP: //블록을 반시계 방향 회전
      // TODO:bug
      //int rotated[4][4];
      for (int y=0;y<4;y++) 
      {
        for (int x=0;x<4;x++) 
        {
          rotated[y][x] = block_current[x][3-y];
        }
      }
      
      if (valid(0, 0, rotated)) 
      {
        for (int y=0;y<4;y++) 
        {
          for (int x=0;x<4;x++) 
          {
            block_current[y][x] = rotated[y][x];
          }
        }
      }
      break;
    case KEY_NEXT: //메뉴에서 밑으로
      break;
    case KEY_PREV: //메뉴에서 위로
      break;
    case KEY_GAME_START:
      if(screen_current == GAME_STATE_MAIN)
      {
        point = 0;
        itoa(point, points, 10); //숫자를 문자열로
        newGame();
        screen_current = GAME_STATE_TETRIS;
      }
      else if(screen_current == GAME_STATE_TETRIS)
      {
        screen_current = GAME_STATE_PAUSE;
      }
      else if(screen_current == GAME_STATE_PAUSE)
      {
        screen_current = GAME_STATE_MAIN;
        point = 0;
        itoa(point, points, 10); //숫자를 문자열로
      }
      break;
    case KEY_BACK:
      if(screen_current == GAME_STATE_PAUSE)
      {
        screen_current = GAME_STATE_TETRIS;
      }
      break;
    case KEY_SPEED:
      if(speed_state == 0)
      {
        speed_state = 1;
      }
      break;
  } 
}
