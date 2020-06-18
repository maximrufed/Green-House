// -----------------------------------------------------------------------
// Класс gh_Config - Конфигурация теплицы - используется всеми устройствами
//				Хранит аппаратную конфигурацию всех устройств теплицы (в проекте) - а зачем?? она же в хардкодится в программе
//				Хранит текущее состояние настроек теплицы
//				Хранит текущее состояние настроек в памяти EEPROM (в проекте)
//				Умеет сохранять текущее состояние настроек и конфигурацию на SD Карту (в проекте)
//
// Устройства:	SD Карта (в проекте)
//
// -----------------------------------------------------------------------


#ifndef GH_CONFIG_H
#define GH_CONFIG_H

#include <avr/eeprom.h>
#include "gh_EarthFan.h"
#include "gh_windows.h"
#include "gh_Watering.h"
#include "gh_logger.h"
extern logger lg;

struct gh_SettingsRefs
{
  //byte T;     // 
  GHWindowSettings *Window;
  TerraAccumulatorSettings *TerraAccumulator;
  BarrelSettings *WaterTank;
  WaterLineSettings *WaterLine1;
  WaterLineSettings *WaterLine2;
};

class gh_Config
{
  public:
  void Begin(GHWindowSettings *WinSettings, TerraAccumulatorSettings *TASettings, BarrelSettings *WaterTankSettings, WaterLineSettings *WL1, WaterLineSettings *WL2);
  void Poll(byte Minute);      
  void EEPROM_UpdateSettings();
  void EEPROM_GetSettings();

  gh_SettingsRefs SettingsRefs;
  private:
  uint16_t EEPROMAdresses[5];
};

#endif
