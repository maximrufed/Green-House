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
#define txtFan "����������"		//����������
#define txtWindow "����"				//����
#define txtWatering "�����"				//�����
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


void setMenuSleepTime();
result ScreenSaver(menuOut&, idleEvent);


#endif