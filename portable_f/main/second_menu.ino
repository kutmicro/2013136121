//중간화면, screen == 3
void drawLogo_2(uint8_t d)
{
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(0+d, 30+d, "T");
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(23+d,30+d,"E");
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(43+d,30+d,"T");
  
  u8g.drawHLine(2+d, 35+d, 47);
  u8g.drawVLine(45+d, 35+d, 12);
}

void drawURL_2(void)
{
  u8g.setFont(u8g_font_4x6);
  u8g.drawStr(1,54,"press start = ");
  u8g.drawStr(32,60,"go title");
  u8g.drawStr(1,72,"press select = ");
  u8g.drawStr(20,78,"back tetris");
  u8g.drawStr(1,102," 1st points=");
  u8g.drawStr(50,102, points_max);
  u8g.drawStr(1,108,"your points=");
  u8g.drawStr(50,108, points);
}


void draw_2(void) 
{
  drawLogo_2(0);
  drawURL_2();
}
