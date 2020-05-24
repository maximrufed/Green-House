// -----------------------------------------------------------------------
// Класс GHWindow - Окно
//				Ручной режим - открывает и закрывает окно теплицы
//				Умное окно - управляет окном по т-ре и времени суток (в проекте)
// 
// Устройства:	мотор (реле)
//				2 концевых выключателя (опционально)
//				индикаторы: работа мотора, авторежим, ошибка (опционально)
//
// -----------------------------------------------------------------------

#ifndef GH_WINDOWS_H
#define GH_WINDOWS_H

#include <Arduino.h>
#include "globals.h"

// Возможные состояния окна
#define OPENING			1		// Сейчас открывается
#define CLOSING			2		// Сейчас закрывается
#define OPEN			3		// Открыто
#define	CLOSED			4		// Закрыто
#define	OPENING_FAILED	5		// Открывание не завершилось корректно, ошибка
#define CLOSING_FAILED	6		// Закрывание не завершилось корректно, ошибка


struct GHWindowSettings						// Структура для хранения внутренних настроек окна
{
	int MotorMaxWorkMillis; 				// Время работы мотора в секундах для полного открытия/закрытия окна
  byte TAirOpen;      // Температура воздуха, при достижении которой 
  byte TAirClose;     // Температура воздуха, при достижении которой 
};

struct GHWindowHardwareConfig				// Конфигурация пинов оборудования окна
{
  byte PinRelayPow = -1;              // Реле подачи питания на мотор окна
	byte PinRelay1 = -1;						    // Реле №1 управления реверсом мотора
	byte PinRelay2 = -1;						    // Реле №2 управления реверсом мотора
	byte PinWindowMotorLed = -1;				// Индикатор работы мотора
	byte PinWindowModeLed = -1;					// Индикатор индикатор ручного режима управления окном
	byte PinWindowAlarmLed = -1;				// Индикатор тревоги при застревании окна
	byte PinLimitSwitchOpen = -1;				// Концевой выключатель открытого окна
	byte PinLimitSwitchClosed = -1;				// Концевой выключатель закрытого окна
	GHWindowHardwareConfig () {}
};



class GHWindow
{
  public:
	bool Begin(GHWindowHardwareConfig HWConfig); // Возврат false если не определены два реле управления мотором
	void Open();							// Открыть окно
	void Close();							// Закрыть окно
	void HaltMotor();						// Экстренная остановка мотора, включение режима тревоги
	void WindowPoll(float TEarth, float TAir, bool IsNight);			// Обработка логики работы умного окна
	void SetManualMode(bool bMode);			// Установить/снять ручной режим работы окна
	bool IsMotorOn();						// Возврат True если мотор включен
	bool IsManualMode();					// Проверить режим работы окна
	bool IsAlarmOn();						// Возврат True если окно находится в режиме тревоги
	GHWindowSettings WinSettings;			// Настройки окна
  
  private:
	GHWindowHardwareConfig WinCfg;			// Конфигурация пинов
	byte WindowStatus;						// Состояние окна. Принимает значения, определенные через define выше
	bool bIsMotorOn;
	bool bIsAlarm;
	bool bIsManualMode;
	unsigned long millisInOperation;		// Количество миллисекунд, которые уже длится операция открытия/закрытия


	void SetMotorToOpen();				// Установить режим открывания
	void SetMotorToClose();				// Установить режим закрывания
  void StartMotor();            // Включить мотор
	void StopMotor();						  // Выключить мотор
	void SetAlarm(bool bAlarm);				// Установить/сбросить режим тревоги
	void CompleteOperationByTimerOrLS();	// Завершить операцию закрытия/открытия по таймеру или концевому выключателю

};

#endif
