﻿#include "gh_menu.h" 
#include "globals.h"
#include "gh_tsensors.h"
#include "gh_earthfan.h"

extern T_Sensors TSensors;
extern Earth_Fan EarthFan;


LCD_1602_RUS lcd(LCD_ADR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  

static unsigned int screenOffTimeOut = 100;

// используем глобальные Options для установки invertFieldKeys. Иначе навигация ломается: меню - вверх, изменение -начений - вниз и наоборот 
config myMenuOptions('>','-',defaultNavCodes,true);

// ------------------------------------------------------------
//   Определяем меню

using namespace Menu;

// Определяем глубину вложенности меню
#define MAX_DEPTH 2

// Определяем устройства вывода
MENU_OUTPUTS(out, MAX_DEPTH
	,LCD_OUT(lcd,{0, 0, 20, 4})
    ,NONE
);

// Определяем устройста ввода - кнопки
keyMap joystickBtn_map[] = {
	{BTN_SEL, defaultNavCodes[enterCmd].ch, INPUT_PULLUP},
	{BTN_UP, defaultNavCodes[upCmd].ch, INPUT_PULLUP},
	{BTN_DOWN, defaultNavCodes[downCmd].ch, INPUT_PULLUP},
};

keyIn<3> joystickBtns(joystickBtn_map);

// Определяем устройство ввода - серийный порт
serialIn serial(Serial);

// Определяем после
menuIn* inputsList[] = {&joystickBtns, &serial};

chainStream<2> in(inputsList);//3 is the number of inputs

// ------------------------------------------------------------
// ------------------------------------------------------------
// Меню ВЕНТИЛЯТОР

result AutoMode() {
  EarthFan.SetManualMode(false);
  return proceed;
}
result FanOn() {
  EarthFan.SetManualMode(true);
  EarthFan.On();
  return proceed;
}
result FanOff() {
  EarthFan.SetManualMode(true);
  EarthFan.Off();
  return proceed;
}
MENU(submFan, txtFan, doNothing, anyEvent, wrapStyle
	,OP(txtAutoMode,AutoMode,enterEvent)
	,OP(txtOn,FanOn,enterEvent)
    ,OP(txtOff,FanOff,enterEvent)
	, FIELD(EarthFan.TASettings.TAirStartCooling, txtTAirStartCooling, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.TASettings.TAirStopCooling, txtTAirStopCooling, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.TASettings.TAirStartHeating, txtTAirStartHeating, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.TASettings.TAirStopHeating, txtTAirStopHeating, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.TASettings.DTFanOn, txtDTFanOn, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.TASettings.DTFanOff, txtDTFanOff, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// ------------------------------------------------------------
// Меню ОКНА
MENU(submWindows, txtWindow, doNothing, anyEvent, wrapStyle
	, OP("Otkrytt", doNothing, noEvent) 
	, OP("Zakrytt", doNothing, noEvent) 
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// 
MENU(submWatering, txtWatering, doNothing, anyEvent, wrapStyle
	, OP("Vkluchitt", doNothing, noEvent) 
	, OP("Vyklychit", doNothing, noEvent) 
	, EXIT("<Back")
); 
 
// ------------------------------------------------------------
// 
MENU(submWaterTank, txtTank, doNothing, anyEvent, wrapStyle
	, OP("Napolnitt", doNothing, noEvent) 
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// 

MENU(submConfigurations, txtConfigurations, doNothing, anyEvent, wrapStyle
	, FIELD(nav.timeOut, "Screensaver", "sec", 10, 255, 100, 10, doNothing, anyEvent, wrapStyle)
	, FIELD(screenOffTimeOut, "Screen Off", "sec", 60, 600, 100, 10, doNothing, anyEvent, wrapStyle)
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// 
MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle
	, SUBMENU(submFan)
	, SUBMENU(submWindows)
	, SUBMENU(submWatering)
	, SUBMENU(submWaterTank)
	, SUBMENU(submConfigurations)
	, EXIT("<Back")
); 
 
// ------------------------------------------------------------
// 
NAVROOT(nav, mainMenu, MAX_DEPTH, in, out); //the navigation root object

// ------------------------------------------------------------
// 
result ScreenSaver(menuOut& o, idleEvent e) {
	static unsigned long millisInIdle;
	switch (e) { 
		case idleStart: 
			o.clear();
			o.print("suspending menu!");
			millisInIdle = millis();
			delay(100);
			break;
		case idling:
			o.clear();
			o.setCursor(0, 0);
			o.print(txtTAir);
			o.print(TSensors.GetTAir());
			o.setCursor(0, 1);
			o.print(txtTEarth);
			o.print(TSensors.GetTEarth());
			if(millis() - millisInIdle > (unsigned long)screenOffTimeOut*1000){
				o.setCursor(0, 2);
				o.print("Screen OFF");
				lcd.setBacklight(0);
			}

			break;
		case idleEnd:
			o.clear();
			o.print("Resuming menu...");
			millisInIdle = 0;
			lcd.setBacklight(255);
			delay(100);
			break;
	}
	return proceed;
}