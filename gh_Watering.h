// -----------------------------------------------------------------------
// Набор объектов для полива:
// Класс GH_Barrel - бочка с нормально закрытым клапаном наполнения
//				Ручной режим - вкл/выкл клапан
//				Авто режим - управляет наполнением бочки автоматически - по таймеру, по последнему поливу дня и по нижнему датчику
//      Меню: время начала наполнения бочки, максимальное время вкючения клапана (аварийное отключение)
//      Устройства: клапан водопровода (реле), датчик верхнего уровня (геркон), датчик нижнего уровня (геркон)
//      Индикаторы: ручной режим (красный), клапан открыт (зеленый), тревога (красный мигает)
//
// Класс WaterLine - клапан полива одной грядки 
//        Ручной режим - вкл/выкл клапан
//        Авто режим - полив по расписанию
//      Меню: дни полива (каждый день - каждый 20й день), время начала полива, количество минут полива
//      Устройства: вентилятор (реле)
//		  Индикаторы: ручной режим (красный), клапан открыт (зеленый)
//
// -----------------------------------------------------------------------

#ifndef GH_WATERING_H
#define GH_WATERING_H

//#include <Arduino.h>
#include "globals.h"
#include "gh_Logger.h"
extern Logger lg;

struct GHBarrelHardwareConfig        // Конфигурация пинов поливающего оборудования 
{
  byte RelayPin = 0xFF;                 // Реле клапана полива
  byte ValveOpenLedPin = 0xFF;          // Индикатор открытого клапана
  byte ModeLedPin = 0xFF;               // Индикатор ручного режима работы
  byte FullDetectorPin = 0xFF;          // Геркон-датчик наполнения бочки
  byte EmptyDetectorPin = 0xFF;         // Геркон-датчик опустения бочки
  GHBarrelHardwareConfig () {}
};

struct BarrelSettings
{
  byte StartFillingHour;      // время начала наполнения бочки
  byte StartFillingMinute;     // время начала наполнения бочки
  byte MaxFillingMinute;      // максимальное время вкючения клапана (аварийное отключение)
};

class gh_Barrel
{
  public:
  BarrelSettings Settings;
  GHBarrelHardwareConfig Cfg;
  
  bool Begin(GHBarrelHardwareConfig HwCfg);
  void Poll(byte NowHour, byte NowMinute);

  void StartFilling();
  void StopFilling();
  void SetManualMode(bool bMode);     // Установить/снять ручной режим работы
  
  bool IsEmpty();
  bool IsFull();
  bool IsFilling();
  bool IsManualMode();          // Проверить режим работы

  
  private:
  bool bIsValveOpen;
  bool bIsEmpty;
  bool bIsFull;
  bool bIsManualMode;
  unsigned long millisStartFilling;    // Количество миллисекунд, которые уже длится наполнение
};


#endif
