//테트리스, screen == 2
void newGame() 
{
  //  board init
  for (int y=0;y<BOARD_ROWS;y++) 
  {
    for (int x=0;x<BOARD_COLS;x++) 
    {
      board[y][x] =0;
    }
  }
  
  drop_interval = DROP_INTERVAL_INIT;
  timer_lastmills = 0;
  draw_lastmills = 0;
  gameover = false; 
  
  nextBlock();
}


void nextBlock() 
{
  int id = random(0,7);
  int i = 0;
  for (int y=0;y<4;y++) 
  {
    for (int x=0;x<4;x++) 
    {
      block_current[y][x] = block_shapes[id][i++];
    }
  }
  
  block_x = 4;
  block_y = 0;
}

void tick() 
{
  if (valid( 0, 1, block_current )) 
  {
    //속도가 빨라진다.
    if ( speed_state != 0 )
    {
      drop_interval = 0;
    }
    block_y++;
  }
  else 
  {
    //속도를 다시 원래대로 돌린다.
    if ( speed_state != 0 )
    {
      drop_interval = DROP_INTERVAL_INIT;
      speed_state = 0;
    }
    
    freeze();
    clearLines();
    if (gameover) 
    {
      //newGame();
      return;
    }
    nextBlock();
  }
}

boolean valid(int offsetX, int offsetY, int newCurrent[4][4]) 
{
  offsetX = block_x + offsetX;
  offsetY = block_y + offsetY;
  for (int y=0; y<4; y++) 
  {
    for (int x=0; x<4; x++) 
    {
      if ( newCurrent[ y ][ x ] ) 
      {
        if ( x + offsetX < 0 || y+offsetY >= BOARD_ROWS || x+offsetX >= BOARD_COLS || board[y+offsetY][x+offsetX] == 1 ) 
        {
          if (offsetY == 1 &&  offsetX-block_x == 0 && offsetY-block_y == 1) 
          {
            gameover = true;
          }
          return false;
        }
      }
    }
  }  
  return true;
}

void freeze() 
{
  for (int y=0;y<4;y++) 
  {
    for (int x=0;x<4;x++) 
    {
      if (block_current[y][x] == 1) 
      {
        board[y+block_y][x+block_x] = block_current[y][x];
      }
    }
  }
}

void clearLines() 
{
  for (int y = BOARD_ROWS-1; y>=0; y--) 
  {
    boolean rowFilled = true;
    for (int x = 0;x<BOARD_COLS;x++) 
    {
      if (board[y][x] == 0) 
      {
        rowFilled = false;
        break;
      }
    }
    
    if (rowFilled) 
    {
      for (int yy=y; yy>0; yy--) 
      {
        for (int x=0; x<BOARD_COLS;x++) 
        {
          board[yy][x] = board[yy-1][x];
        }
      }
      y++;
      point++;
      itoa(point, points, 10); //숫자를 문자열로
      if( point >= point_max )
      {
        point_max = point;
        itoa(point_max, points_max, 10);
      }
    }
  }
}


void render() 
{ 
  //  draw frame
  u8g.drawLine(6, 0, 6, BLOCK_H * BOARD_ROWS + 2);
  u8g.drawLine(BLOCK_W * BOARD_COLS + 9, 0, BLOCK_W * BOARD_COLS + 9, BLOCK_H * BOARD_ROWS + 2);
  u8g.drawLine(6, BLOCK_H * BOARD_ROWS + 2, BLOCK_W * BOARD_COLS + 9, BLOCK_H * BOARD_ROWS + 2);
  
  //  draw board
  for (int x=0;x<BOARD_COLS;x++) 
  {
    for (int y=0;y<BOARD_ROWS;y++) 
    {
      if (board[y][x] == 1) 
      {
            drawBlock(x, y);
      }
    }
  }
  
  //  draw block shape
  for (int y=0;y<4;y++) 
  {
    for (int x=0;x<4;x++) 
    {
      if (block_current[y][x] == 1) 
      {
        drawBlock(block_x+x, block_y+y);
      }
    }
  }
}

void drawBlock(int x, int y) 
{
  x = BLOCK_W * x;
  y = BLOCK_H * y;
  u8g.drawBox(x + 8, y, BLOCK_W, BLOCK_H);
}
