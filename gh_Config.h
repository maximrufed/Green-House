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
#include "gh_Logger.h"
extern Logger lg;

struct gh_SettingsRefs
{
  //byte T;     // 
  GHWindowSettings *Window;
  TerraAccumulatorSettings *TerraAccumulator;
};

class gh_Config
{
  public:
  void Begin(GHWindowSettings *WinSettings, TerraAccumulatorSettings *TASettings);
  void Poll(byte Minute);      
  void EEPROM_UpdateSettings();
  void EEPROM_GetSettings();

  gh_SettingsRefs SettingsRefs;
  private:
  uint16_t EEPROMAdresses[2];
};

#endif
