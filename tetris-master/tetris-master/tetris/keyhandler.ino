
void keySetup(void) {
  // configure input keys 
  pinMode(KEY_PIN_PREV, INPUT_PULLUP);      // set pin to input
  pinMode(KEY_PIN_NEXT, INPUT_PULLUP);           // set pin to input
  pinMode(KEY_PIN_SELECT, INPUT_PULLUP);         // set pin to input
}

void getKeyPress(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(KEY_PIN_PREV) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(KEY_PIN_NEXT) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(KEY_PIN_SELECT) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else 
    uiKeyCodeFirst = KEY_NONE;

  if ( uiKeyCodeSecond == uiKeyCodeFirst ) {
    uiKeyCode = uiKeyCodeFirst;
  }
  else {
    uiKeyCode = KEY_NONE;
  }
}

