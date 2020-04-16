#ifndef GH_LCD_H
#define GH_LCD_H

//#include "wire.h"
#include <LCD_1602_RUS.h>
//#include <LiquidCrystal_I2C.h>

#define SCREEN_WIDTH 20
//extern LiquidCrystal_I2C lcd;
extern LCD_1602_RUS lcd;


void print_message(const char *buf, bool clean = true);
void print_screen(const char *message, const char *button1, const char *button2);

#endif