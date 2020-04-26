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


// Define menu texts (ïğåîáğàçîâàííûå ñ óäàëåííûì ïåğâûì áàéòîì èç ğóññêèõ ñèìâîëîâ
//Main menu
/*//#define txtFan "’•¢˜›¯¢ "		//ÂÅÍÒÈËßÒÎĞ
//#define txtWindow "š"				//ÎÊÍÀ
#define txtFan "Ventilator"
#define txtWindow "Okna"
//#define txtWatering "Ÿ›˜’"				//ÏÎËÈÂ
#include "gh_menu_1251.h"
#define txtTank "‘§š"					//ÁÎ×ÊÀ
#define txtConfigurations "¡¢ ™š"	//ÍÀÑÒĞÎÉÊÀ
#define txtTAir "¢’¾·´ = "				//ÒÂîçä = 
#define txtTEarth "¢—µ¼» = "			//ÒÇåìë = 

//FAN submenu
#define txtAutoMode "’¢  •-˜¼"					//ÀÂÒÎ ĞÅÆÈÌ *
#define txtOn "’š›"									//ÂÊË *
#define txtOff "’-š›"								//ÂÛÊË *
#define txtTAirStartCooling "¢’ ¡¢ ¢ ¥›"			//ÒÂ ÑÒÀĞÒ ÎÕË
#define txtTAirStopCooling "¢’ ¡¢Ÿ ¥›"			//ÒÂ ÑÒÎÏ ÎÕË
#define txtTAirStartHeating "¢’ ¡¢ ¢ “ "			//ÒÂ ÑÒÀĞÒ ÍÀÃĞ
#define txtTAirStopHeating "¢’ ¡¢Ÿ “ "			//ÒÂ ÑÒÎÏ ÍÀÃĞ
#define txtDTFanOn "¢’-¢— ¡¢ ¢"					//ÒÂ-ÒÇ ÑÒÀĞÒ
#define txtDTFanOff "¢’-¢— ¡¢Ÿ"					//ĞÀÇÍÈÖÀ Ò-Ğ ÑÒÎÏ
*/
#define txtFan "Ventilator"
#define txtWindow "Okna"
#define txtWatering "Poliv"				//ÏÎËÈÂ
#define txtTank "Bochka"					//ÁÎ×ÊÀ
#define txtConfigurations "Configuration"	//ÍÀÑÒĞÎÉÊÀ
#define txtTAir "TAir = "				//ÒÂîçä = 
#define txtTEarth "TEarth = "			//ÒÇåìë = 

//FAN submenu
#define txtAutoMode "AUTO MODE"					//ÀÂÒÎ ĞÅÆÈÌ *
#define txtOn "On"									//ÂÊË *
#define txtOff "Off"								//ÂÛÊË *
#define txtTAirStartCooling "TA Start Cool "			//ÒÂ ÑÒÀĞÒ ÎÕË
#define txtTAirStopCooling "TA Stop Cool "			//ÒÂ ÑÒÎÏ ÎÕË
#define txtTAirStartHeating "TA Start Heat"			//ÒÂ ÑÒÀĞÒ ÍÀÃĞ
#define txtTAirStopHeating "TA Stop Heat "			//ÒÂ ÑÒÎÏ ÍÀÃĞ
#define txtDTFanOn "TA-TE Start"					//ÒÂ-ÒÇ ÑÒÀĞÒ
#define txtDTFanOff "T diff Stop"					//ĞÀÇÍÈÖÀ Ò-Ğ ÑÒÎÏ

//WINDOWS submenu
#define txtOpen "Open"									//Îòêğûòü *
#define txtClose "Close"								//Çàêğûòü *
#define txtMotorMaxWorkMillis "MotorLim"		//ÌàêñÂğ ğàáîòû ìîòîğà ìñ

void setMenuSleepTime();
result ScreenSaver(menuOut&, idleEvent);


#endif