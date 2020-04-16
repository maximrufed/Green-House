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
#define txtFan "’•¢˜›¯¢ "		//ÂÅÍÒÈËßÒÎĞ
#define txtWindow "š"				//ÎÊÍÀ
#define txtWatering "Ÿ›˜’"				//ÏÎËÈÂ
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


void setMenuSleepTime();
result ScreenSaver(menuOut&, idleEvent);


#endif