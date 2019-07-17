#ifndef ui_h
#define ui_h

//methods
void show_7seg(int a);
void show_7seg_oni(int i, int a);
void lcd_inital(void);
void refresh_lcd(void);
void test_ui(void);
void refresh_7seg(void);
void refresh_ui(void);
void log(char str[]);
void log_adc();
void ui_move_cursor_up_down(uint8_t upOrDown);
void ui_enterName_putchar(char c);
void ui_move_cursor_left_right(uint8_t leftOrRight);
void cursorUpdate(void);
void changeState(int toState, int nextState);
void lightOnBoardLED(int num, uint8_t onOrOff);
void lightDipLED(int num, uint8_t onOrOff);
void lightHandlerOnBoardLEDs();
char checkLCD(int x, int y);
void showBiunes(int x, int y);
void gotoNextState();

#endif

