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
};

struct GHWindowHardwareConfig				// Конфигурация пинов оборудования окна
{
	byte PinRelay1 = 0;						// Реле №1 управления мотором окна
	byte PinRelay2 = 0;						// Реле №2 управления мотором окна
	byte PinWindowMotorLed = 0;				// Индикатор работы мотора
	byte PinWindowModeLed = 0;				// Индикатор индикатор ручного режима управления окном
	byte PinWindowAlarmLed = 0;				// Индикатор тревоги при застревании окна
	byte PinLimitSwitchOpen = 0;			// Концевой выключатель открытого окна
	byte PinLimitSwitchClosed = 0;			// Концевой выключатель закрытого окна
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


	void StartMotorToOpen();				// Включить мотор на открывание
	void StartMotorToClose();				// Включить мотор на закрывание
	void StopMotor();						// Выключить мотор
	void SetAlarm(bool bAlarm);				// Установить/сбросить режим тревоги
	void CompleteOperationByTimerOrLS();	// Завершить операцию закрытия/открытия по таймеру или концевому выключателю

};

#endif