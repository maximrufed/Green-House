// -----------------------------------------------------------------------
// Мощнейшее экранное меню от https://github.com/neu-rah/ArduinoMenu
// 
// Описание https://github.com/neu-rah/ArduinoMenu/wiki
//
// Устройства:	LCD дисплей
//
// -----------------------------------------------------------------------


#ifndef GH_MENU_H
#define GH_MENU_H

#include <menu.h>//menu macros and objects

//#include <menuIO/lcdOut.h>//malpartidas lcd menu output
#include <menuIO/lcdOut_AK_LCDRUS.h>//malpartidas lcd menu output
#include <menuIO/serialIn.h>//Serial input
#include <menuIO/serialOut.h>
#include <menuIO/altKeyIn.h>
#include <menuIO/chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)

extern keyIn<3> joystickBtns;
extern navRoot nav;

extern config myMenuOptions;

//LCD Definitions
#include <LCD_1602_RUS.h>
extern LCD_1602_RUS lcd;


// Define menu texts (преобразованные с удаленным первым байтом из русских символов
//Main menu
/*//#define txtFan "’•ќў›Їўћ "		//ВЕНТИЛЯТОР
//#define txtWindow "ћљќђ"				//ОКНА
#define txtFan "Ventilator"
#define txtWindow "Okna"
//#define txtWatering "џћ›’"				//ПОЛИВ
#include "gh_menu_1251.h"
#define txtTank "‘ћ§љђ"					//БОЧКА
#define txtConfigurations "ќђЎў ћ™љђ"	//НАСТРОЙКА
#define txtTAir "ў’ѕ·ґ = "				//ТВозд = 
#define txtTEarth "ў—µј» = "			//ТЗемл = 

//FAN submenu
#define txtAutoMode "ђ’ўћ  •-ј"					//АВТО РЕЖИМ *
#define txtOn "’љ›"									//ВКЛ *
#define txtOff "’-љ›"								//ВЫКЛ *
#define txtTAirStartCooling "ў’ Ўўђ ў ћҐ›"			//ТВ СТАРТ ОХЛ
#define txtTAirStopCooling "ў’ Ўўћџ ћҐ›"			//ТВ СТОП ОХЛ
#define txtTAirStartHeating "ў’ Ўўђ ў ќђ“ "			//ТВ СТАРТ НАГР
#define txtTAirStopHeating "ў’ Ўўћџ ќђ“ "			//ТВ СТОП НАГР
#define txtDTFanOn "ў’-ў— Ўўђ ў"					//ТВ-ТЗ СТАРТ
#define txtDTFanOff "ў’-ў— Ўўћџ"					//РАЗНИЦА Т-Р СТОП
*/
#define txtFan "Ventilator"
#define txtWindow "Okna"
#define txtWatering "Poliv"				//ПОЛИВ
#define txtTank "Bochka"					//БОЧКА
#define txtConfigurations "Configuration"	//НАСТРОЙКА
#define txtTAir "TAir = "				//ТВозд = 
#define txtTEarth "TEarth = "			//ТЗемл = 

//FAN submenu
#define txtAutoMode "AUTO MODE"					//АВТО РЕЖИМ *
#define txtOn "On"									//ВКЛ *
#define txtOff "Off"								//ВЫКЛ *
#define txtTAirStartCooling "TA Start Cool "			//ТВ СТАРТ ОХЛ
#define txtTAirStopCooling "TA Stop Cool "			//ТВ СТОП ОХЛ
#define txtTAirStartHeating "TA Start Heat"			//ТВ СТАРТ НАГР
#define txtTAirStopHeating "TA Stop Heat "			//ТВ СТОП НАГР
#define txtDTFanOn "TA-TE Start"					//ТВ-ТЗ СТАРТ
#define txtDTFanOff "T diff Stop"					//РАЗНИЦА Т-Р СТОП

//WINDOWS submenu
#define txtOpen "Open"									//Открыть *
#define txtClose "Close"								//Закрыть *
#define txtMotorMaxWorkMillis "MotorLim"		//МаксВр работы мотора мс

void setMenuSleepTime();
result ScreenSaver(menuOut&, idleEvent);


#endif