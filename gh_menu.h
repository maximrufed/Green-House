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


// Define menu texts (��������������� � ��������� ������ ������ �� ������� ��������
//Main menu
/*//#define txtFan "����������"		//����������
//#define txtWindow "����"				//����
#define txtFan "Ventilator"
#define txtWindow "Okna"
//#define txtWatering "�����"				//�����
#include "gh_menu_1251.h"
#define txtTank "�����"					//�����
#define txtConfigurations "���������"	//���������
#define txtTAir "����� = "				//����� = 
#define txtTEarth "����� = "			//����� = 

//FAN submenu
#define txtAutoMode "���� ��-��"					//���� ����� *
#define txtOn "���"									//��� *
#define txtOff "�-��"								//���� *
#define txtTAirStartCooling "�� ����� ���"			//�� ����� ���
#define txtTAirStopCooling "�� ���� ���"			//�� ���� ���
#define txtTAirStartHeating "�� ����� ����"			//�� ����� ����
#define txtTAirStopHeating "�� ���� ����"			//�� ���� ����
#define txtDTFanOn "��-�� �����"					//��-�� �����
#define txtDTFanOff "��-�� ����"					//������� �-� ����
*/
#define txtFan "Ventilator"
#define txtWindow "Okna"
#define txtWatering "Poliv"				//�����
#define txtTank "Bochka"					//�����
#define txtConfigurations "Configuration"	//���������
#define txtTAir "TAir = "				//����� = 
#define txtTEarth "TEarth = "			//����� = 

//FAN submenu
#define txtAutoMode "AUTO MODE"					//���� ����� *
#define txtOn "On"									//��� *
#define txtOff "Off"								//���� *
#define txtTAirStartCooling "TA Start Cool "			//�� ����� ���
#define txtTAirStopCooling "TA Stop Cool "			//�� ���� ���
#define txtTAirStartHeating "TA Start Heat"			//�� ����� ����
#define txtTAirStopHeating "TA Stop Heat�"			//�� ���� ����
#define txtDTFanOn "TA-TE Start"					//��-�� �����
#define txtDTFanOff "T diff Stop"					//������� �-� ����

//WINDOWS submenu
#define txtOpen "Open"									//������� *
#define txtClose "Close"								//������� *
#define txtMotorMaxWorkMillis "MotorLim"		//������ ������ ������ ��

void setMenuSleepTime();
result ScreenSaver(menuOut&, idleEvent);


#endif