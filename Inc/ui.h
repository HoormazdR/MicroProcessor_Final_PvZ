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

// Internal Macros
#define HEX__(n) 0x##n##LU
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)

// User-visible Macros
#define B8(d) ((unsigned char)B8__(HEX__(d)))
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) + B8(dlsb))
#define B32(dmsb,db2,db3,dlsb) \
(((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))

#define EQ_STATE(state,candidStare) !((state)^(candidStare))

#endif

