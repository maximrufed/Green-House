#include "gh_Config.h"

//---------------------------------------------------------------------
// Begin
void gh_Config::Begin(GHWindowSettings *WinSettings, TerraAccumulatorSettings *TASettings) {
  SettingsRefs.Window = WinSettings;
  SettingsRefs.TerraAccumulator = TASettings;
  EEPROMAdresses[0] = 0;
  EEPROMAdresses[1] = EEPROMAdresses[0] + sizeof(*WinSettings);
  LOG("Reading settings from EEPROM");
  //EEPROM_UpdateSettings();
  EEPROM_GetSettings();
}


//---------------------------------------------------------------------
// Poll
void gh_Config::Poll(byte Minute) {
  // Сохраняем конфигурацию каждый полчаса
  if(Minute%30 == 0) {
    lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_SAVESETTINGSBYTIMER, 0, 0); // Делаем запись в журнале активности
    EEPROM_UpdateSettings();
  }
}

//---------------------------------------------------------------------
// EEPROM_UpdateSettings
void gh_Config::EEPROM_UpdateSettings() {
  LOG((String)"Writing to EEPROM Window Settings. To address: " + (String)EEPROMAdresses[0] + (String)"; Block of " + (String)sizeof(*SettingsRefs.Window) + " bytes");
  eeprom_update_block((void*)SettingsRefs.Window, (const void*)EEPROMAdresses[0], sizeof(*SettingsRefs.Window));
  
  byte *b;
  b = (void *)SettingsRefs.Window;
  for(int i=0; i<sizeof(*SettingsRefs.Window); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();
  
  LOG((String)"Writing to EEPROM TerraAccumulator Settings. To address: " + (String)EEPROMAdresses[1] + (String)"; Block of " + (String)sizeof(*SettingsRefs.TerraAccumulator) + " bytes");
  eeprom_update_block((void*)SettingsRefs.TerraAccumulator, (const void*)EEPROMAdresses[1], sizeof(*SettingsRefs.TerraAccumulator));
  b = (void *)SettingsRefs.TerraAccumulator;
  for(int i=0; i<sizeof(*SettingsRefs.TerraAccumulator); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_SAVESETTINGS, 0, 0); // Делаем запись в журнале активности

}

//---------------------------------------------------------------------
// EEPROM_GetSettings
void gh_Config::EEPROM_GetSettings() {
  if( eeprom_read_byte(0) == 0xFF ) {
    // Таким образом определяем, что прошивка нулевая и еще ничего в ней нет. Тогда и не портим свои переменные, которые уже инициализированы
    lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_GETSETTINGSEMPTYEEPROM, 0, 0); // Делаем запись в журнале активности
    return;
  }
  
  eeprom_read_block((void*)SettingsRefs.Window, (const void*)EEPROMAdresses[0], sizeof(*SettingsRefs.Window));
  LOG("Window Settings:");
  LOG("Starting address: " + (String)EEPROMAdresses[0]);
  byte *b = (void *)SettingsRefs.Window;
  for(int i=0; i<sizeof(*SettingsRefs.Window); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  
  eeprom_read_block((void*)SettingsRefs.TerraAccumulator, (const void*)EEPROMAdresses[1], sizeof(*SettingsRefs.TerraAccumulator));

  LOG("Window Settings:");
  LOG("Starting address: " + (String)EEPROMAdresses[1]);
  b = (void *)SettingsRefs.TerraAccumulator;
  for(int i=0; i<sizeof(*SettingsRefs.TerraAccumulator); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_GETSETTINGSOK, 0, 0); // Делаем запись в журнале активности

}
