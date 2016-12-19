//로고, screen == 1
void drawLogo(uint8_t d)
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

void drawURL(void)
{
  u8g.setFont(u8g_font_4x6);
  u8g.drawStr(1,72,"press start = ");
  u8g.drawStr(16,78,"start tetris");
  u8g.drawStr(1,102," 1st points=");
  u8g.drawStr(50,102, points_max);
  u8g.drawStr(1,108,"your points=");
  u8g.drawStr(50,108, points);
}


void draw(void) 
{
  drawLogo(0);
  drawURL();
}
