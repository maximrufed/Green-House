#include "gh_menu.h" 
#include "globals.h"
#include "gh_tsensors.h"
#include "gh_earthfan.h"
#include "gh_windows.h"
#include "gh_Watering.h"
#include "gh_Config.h"
#include "gh_Logger.h"

extern logger lg;
extern T_Sensors TSensors;
extern Earth_Fan EarthFan;
extern GHWindow Window;
extern gh_Barrel WaterTank;
extern gh_Config ControllerConfiguration;
extern gh_WaterLine WateringLine1;
extern gh_WaterLine WateringLine2;

//LCD_1602_RUS lcd(LCD_ADR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  
//LCD_1602_RUS <LiquidCrystal_I2C> lcd(0x27, 16, 2);
//extern LCD_1602_RUS lcd;
LCD_1602_RUS lcd(LCD_ADR,20,4);  
//LCD_1602_RUS <LiquidCrystal_I2C> lcd(0x27, 20, 4);

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

result FanAutoMode() {
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
	,OP(txtAutoMode,FanAutoMode,enterEvent)
	,OP(txtOn,FanOn,enterEvent)
    ,OP(txtOff,FanOff,enterEvent)
	, FIELD(EarthFan.Settings.TAirStartCooling, txtTAirStartCooling, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.Settings.TAirStopCooling, txtTAirStopCooling, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.Settings.TAirStartHeating, txtTAirStartHeating, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.Settings.TAirStopHeating, txtTAirStopHeating, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.Settings.DTFanOn, txtDTFanOn, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, FIELD(EarthFan.Settings.DTFanOff, txtDTFanOff, "C", 0, 60, 10, 1, doNothing, anyEvent, wrapStyle)
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// ------------------------------------------------------------
// Меню ОКНА


result WindowAutoMode() {
  Window.SetManualMode(false);
  return proceed;
}
result WindowOpen() {
  if(!Window.IsManualMode()) Window.SetManualMode(true);
  Window.Open();
  return proceed;
}
result WindowClose() {
  if(!Window.IsManualMode()) Window.SetManualMode(true);
  Window.Close();
  return proceed;
}

MENU(submWindows, txtWindow, doNothing, anyEvent, wrapStyle
	, OP(txtAutoMode,WindowAutoMode,enterEvent)
	, OP(txtOpen, WindowOpen, enterEvent) 
	, OP(txtClose, WindowClose, enterEvent) 
	, FIELD(Window.Settings.MotorMaxWorkMillis, txtMotorMaxWorkMillis, "ms", 400, 10000, 1000, 50, doNothing, anyEvent, wrapStyle)
  , FIELD(Window.Settings.TAirOpen, txtTAirOpen, "C", 0, 40, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(Window.Settings.TAirClose, txtTAirClose, "C", 0, 40, 5, 1, doNothing, anyEvent, wrapStyle)
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// ------------------------------------------------------------
// МЕНЮ БОЧКИ

result WaterTankAutoMode() {
  WaterTank.SetManualMode(false);
  return proceed;
}

result WaterTankStartFilling() {
  if(!WaterTank.IsManualMode()) WaterTank.SetManualMode(true);
  WaterTank.StartFilling();
  return proceed;
}

result WaterTankStopFilling() {
  if(!WaterTank.IsManualMode()) WaterTank.SetManualMode(true);
  WaterTank.StopFilling();
  return proceed;
}

MENU(submWaterTank, txtBarrel, doNothing, anyEvent, wrapStyle
  , OP(txtAutoMode,WaterTankAutoMode,enterEvent)
  , OP(txtStartFilling, WaterTankStartFilling, enterEvent) 
  , OP(txtStopFilling, WaterTankStopFilling, enterEvent) 
  , FIELD(WaterTank.Settings.StartFillingHour, txtStartFillingHour, "", 0, 23, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WaterTank.Settings.StartFillingMinute, txtStartFillingMinute, "", 0, 59, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WaterTank.Settings.MaxFillingMinute, txtMaxFillingMinute, "", 0, 59, 5, 1, doNothing, anyEvent, wrapStyle)
  , EXIT("<Back")
); 

// ------------------------------------------------------------
// ------------------------------------------------------------
// меню Линии полива №1

result WaterLine1AutoMode() {
  WateringLine1.SetManualMode(false);
  return proceed;
}

result WaterLine1StartWatering() {
  if(!WateringLine1.IsManualMode()) WateringLine1.SetManualMode(true);
  WateringLine1.StartWatering();
  return proceed;
}

result WaterLine1StopFilling() {
  if(!WateringLine1.IsManualMode()) WateringLine1.SetManualMode(true);
  WateringLine1.StopWatering();
  return proceed;
}

result ResetWatering1Date() {
  WateringLine1.ResetWateringDate();
  return proceed;
}

MENU(submWateringLine1, txtWaterLine1, doNothing, anyEvent, wrapStyle
  , OP(txtAutoMode,WaterLine1AutoMode,enterEvent)
  , OP(txtStartWatering, WaterLine1StartWatering, enterEvent) 
  , OP(txtStopWatering, WaterLine1StopFilling, enterEvent) 
  , OP(txtResetWateringDate, ResetWatering1Date, enterEvent) 
  , FIELD(WateringLine1.Settings.StartWateringHour, txtWateringStartHour, "", 0, 23, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WateringLine1.Settings.StartWateringMin, txtWateringStartMin, "", 0, 59, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WateringLine1.Settings.IntervalDays, txtWateringIntervalDays, "", 0, 30, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WateringLine1.Settings.DurationMins, txtWateringDurationMins, "", 0, 360, 20, 5, doNothing, anyEvent, wrapStyle)
  , EXIT("<Back")
); 

// ------------------------------------------------------------
// ------------------------------------------------------------
// меню Линии полива №2

result WaterLine2AutoMode() {
  WateringLine2.SetManualMode(false);
  return proceed;
}

result WaterLine2StartWatering() {
  if(!WateringLine2.IsManualMode()) WateringLine2.SetManualMode(true);
  WateringLine2.StartWatering();
  return proceed;
}

result WaterLine2StopFilling() {
  if(!WateringLine2.IsManualMode()) WateringLine2.SetManualMode(true);
  WateringLine2.StopWatering();
  return proceed;
}

result ResetWatering2Date() {
  WateringLine2.ResetWateringDate();
  return proceed;
}

MENU(submWateringLine2, txtWaterLine2, doNothing, anyEvent, wrapStyle
  , OP(txtAutoMode,WaterLine2AutoMode,enterEvent)
  , OP(txtStartWatering, WaterLine2StartWatering, enterEvent) 
  , OP(txtStopWatering, WaterLine2StopFilling, enterEvent) 
  , OP(txtResetWateringDate, ResetWatering2Date, enterEvent) 
  , FIELD(WateringLine2.Settings.StartWateringHour, txtWateringStartHour, "", 0, 23, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WateringLine2.Settings.StartWateringMin, txtWateringStartMin, "", 0, 59, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WateringLine2.Settings.IntervalDays, txtWateringIntervalDays, "", 0, 30, 5, 1, doNothing, anyEvent, wrapStyle)
  , FIELD(WateringLine2.Settings.DurationMins, txtWateringDurationMins, "", 0, 360, 20, 5, doNothing, anyEvent, wrapStyle)
  , EXIT("<Back")
); 


// ------------------------------------------------------------
// Меню Конфигурации


result SaveConfigToEEPROM() {
  lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_SAVESETTINGSBYMENU, 0, 0); // Делаем запись в журнале активности
  ControllerConfiguration.EEPROM_UpdateSettings();
  return proceed;
}


MENU(submConfigurations, txtConfigurations, doNothing, anyEvent, wrapStyle
	, FIELD(nav.timeOut, "Screensaver", "sec", 10, 255, 100, 10, doNothing, anyEvent, wrapStyle)
	, FIELD(screenOffTimeOut, "Screen Off", "sec", 60, 600, 100, 10, doNothing, anyEvent, wrapStyle)
  , OP(txtConfigToEEPROM, SaveConfigToEEPROM, enterEvent) 
	, EXIT("<Back")
); 

// ------------------------------------------------------------
// ------------------------------------------------------------
// ГЛАВНОЕ МЕНЮ

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle
	, SUBMENU(submFan)
	, SUBMENU(submWindows)
	, SUBMENU(submWaterTank)
  , SUBMENU(submWateringLine1)
  , SUBMENU(submWateringLine2)
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
			/*o.clear();
			o.setCursor(0, 0);
			o.print(txtTAir);
			o.print(TSensors.GetTAir());
			o.setCursor(0, 1);
			o.print(txtTEarth);
			o.print(TSensors.GetTEarth());
      */
      o.clear();
      o.setCursor(0,0);
      o.print("AirIn: ");
      o.print((int)TSensors.GetTAir());
      o.setCursor(10,0);
      o.print("AirOut:");
      o.print((int)TSensors.GetTOut());
      o.setCursor(0,1);
      o.print("Earth1:");
      o.print((int)TSensors.GetTEarth());
      o.setCursor(10,1);
      o.print("Earth2:");
      o.print((int)TSensors.GetTEarth2());
      o.setCursor(0,2);
      o.print("FanIn: ");
      o.print((int)TSensors.GetT_TAFanIn());
      o.setCursor(10,2);
      o.print("FanOut:");
      o.print((int)TSensors.GetT_TAFanOut());
      o.setCursor(0,3);
      o.print("Board: ");
      o.print((int)TSensors.GetTBoard());
      
			if(millis() - millisInIdle > (unsigned long)screenOffTimeOut*1000){
				o.setCursor(0, 2);
				o.print("Screen OFF");
				lcd.setBacklight(0);
				LOG ("ВЫКЛЮЧАЕМ ЭКРАН!!!");
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
