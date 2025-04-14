#ifndef LCD_H
#define LCD_H
#define LCD_CLR 0x01  // Command to clear the display


void lcd_init(void);
void lcd_moveto(unsigned char, unsigned char);
void lcd_stringout(char *);
void lcd_writecommand(unsigned char);
void lcd_writedata(unsigned char);

#endif
