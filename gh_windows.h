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

#include "gh_Logger.h"
extern Logger lg;


// Возможные состояния окна
#define OPENING			1		// Сейчас открывается
#define CLOSING			2		// Сейчас закрывается
#define OPEN			3		// Открыто
#define	CLOSED			4		// Закрыто
#define	OPENING_FAILED	5		// Открывание не завершилось корректно, ошибка
#define CLOSING_FAILED	6		// Закрывание не завершилось корректно, ошибка


struct GHWindowSettings					// Структура для хранения внутренних настроек окна
{
	uint16_t MotorMaxWorkMillis; 	// Время работы мотора в секундах для полного открытия/закрытия окна
  int8_t TAirOpen;             // Температура воздуха, при достижении которой 
  int8_t TAirClose;            // Температура воздуха, при достижении которой 
};

struct GHWindowHardwareConfig				// Конфигурация пинов оборудования окна
{
  uint8_t PinRelayPow = 0xFF;              // Реле подачи питания на мотор окна
	uint8_t PinRelay1 = 0xFF;						    // Реле №1 управления реверсом мотора
	uint8_t PinRelay2 = 0xFF;						    // Реле №2 управления реверсом мотора
	uint8_t PinWindowMotorLed = 0xFF;				// Индикатор работы мотора
	uint8_t PinWindowModeLed = 0xFF;					// Индикатор индикатор ручного режима управления окном
	uint8_t PinWindowAlarmLed = 0xFF;				// Индикатор тревоги при застревании окна
	uint8_t PinLimitSwitchOpen = 0xFF;				// Концевой выключатель открытого окна
	uint8_t PinLimitSwitchClosed = 0xFF;				// Концевой выключатель закрытого окна
	GHWindowHardwareConfig () {}
};



class GHWindow
{
  public:
	bool Begin(GHWindowHardwareConfig HWConfig); // Возврат false если не определены два реле управления мотором
	void Open();							// Открыть окно
	void Close();							// Закрыть окно
	void HaltMotor();						// Экстренная остановка мотора, включение режима тревоги
	void WindowPoll(int8_t TEarth, int8_t TAir, bool IsNight);			// Обработка логики работы умного окна
	void SetManualMode(bool bMode);			// Установить/снять ручной режим работы окна
	bool IsMotorOn();						// Возврат True если мотор включен
	bool IsManualMode();					// Проверить режим работы окна
	bool IsAlarmOn();						// Возврат True если окно находится в режиме тревоги
	GHWindowSettings WinSettings;			// Настройки окна
  
  private:
  const char ObjectName[7] = "Window";      // используется для логгера
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
