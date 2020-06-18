#include "gh_Config.h"

//---------------------------------------------------------------------
// Begin
void gh_Config::Begin(GHWindowSettings *WinSettings, TerraAccumulatorSettings *TASettings, BarrelSettings *WaterTankSettings, WaterLineSettings *WL1, WaterLineSettings *WL2) {
  SettingsRefs.Window = WinSettings;
  SettingsRefs.TerraAccumulator = TASettings;
  SettingsRefs.WaterTank = WaterTankSettings;
  SettingsRefs.WaterLine1 = WL1;
  SettingsRefs.WaterLine2 = WL2;
  EEPROMAdresses[0] = 0;
  EEPROMAdresses[1] = EEPROMAdresses[0] + sizeof(*WinSettings);
  EEPROMAdresses[2] = EEPROMAdresses[1] + sizeof(*TASettings);
  EEPROMAdresses[3] = EEPROMAdresses[2] + sizeof(*WaterTankSettings);
  EEPROMAdresses[4] = EEPROMAdresses[3] + sizeof(*WL1);
  LOG_CFG("Reading settings from EEPROM");

  //EEPROM_UpdateSettings();
  EEPROM_GetSettings();
}


//---------------------------------------------------------------------
// Poll
void gh_Config::Poll(byte Minute) {
  static bool bNeedToSave=true;
  // Сохраняем конфигурацию каждые полчаса
  if(Minute%30 == 0 and bNeedToSave) {
    lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_SAVESETTINGSBYTIMER, 0, 0); // Делаем запись в журнале активности
    EEPROM_UpdateSettings();
    bNeedToSave = false;
  } 
  if(Minute%30 == 1) {
    bNeedToSave = true;
  }
}

//---------------------------------------------------------------------
// EEPROM_UpdateSettings
void gh_Config::EEPROM_UpdateSettings() {
  LOG_CFG((String)"Writing to EEPROM Window Settings. To address: " + (String)EEPROMAdresses[0] + (String)"; Block of " + (String)sizeof(*SettingsRefs.Window) + " bytes");
  eeprom_update_block((void*)SettingsRefs.Window, (const void*)EEPROMAdresses[0], sizeof(*SettingsRefs.Window));
  
  byte *b;
  b = (void *)SettingsRefs.Window;
  for(int i=0; i<sizeof(*SettingsRefs.Window); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();
  
  LOG_CFG((String)"Writing to EEPROM TerraAccumulator Settings. To address: " + (String)EEPROMAdresses[1] + (String)"; Block of " + (String)sizeof(*SettingsRefs.TerraAccumulator) + " bytes");
  eeprom_update_block((void*)SettingsRefs.TerraAccumulator, (const void*)EEPROMAdresses[1], sizeof(*SettingsRefs.TerraAccumulator));
  b = (void *)SettingsRefs.TerraAccumulator;
  for(int i=0; i<sizeof(*SettingsRefs.TerraAccumulator); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  LOG_CFG((String)"Writing to EEPROM WaterTank Settings. To address: " + (String)EEPROMAdresses[2] + (String)"; Block of " + (String)sizeof(*SettingsRefs.WaterTank) + " bytes");
  eeprom_update_block((void*)SettingsRefs.WaterTank, (const void*)EEPROMAdresses[2], sizeof(*SettingsRefs.WaterTank));
  b = (void *)SettingsRefs.WaterTank;
  for(int i=0; i<sizeof(*SettingsRefs.WaterTank); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  LOG_CFG((String)"Writing to EEPROM WaterLine1 Settings. To address: " + (String)EEPROMAdresses[3] + (String)"; Block of " + (String)sizeof(*SettingsRefs.WaterLine1) + " bytes");
  eeprom_update_block((void*)SettingsRefs.WaterLine1, (const void*)EEPROMAdresses[3], sizeof(*SettingsRefs.WaterLine1));
  b = (void *)SettingsRefs.WaterLine1;
  for(int i=0; i<sizeof(*SettingsRefs.WaterLine1); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  LOG_CFG((String)"Writing to EEPROM WaterLine2 Settings. To address: " + (String)EEPROMAdresses[4] + (String)"; Block of " + (String)sizeof(*SettingsRefs.WaterLine2) + " bytes");
  eeprom_update_block((void*)SettingsRefs.WaterLine2, (const void*)EEPROMAdresses[4], sizeof(*SettingsRefs.WaterLine2));
  b = (void *)SettingsRefs.WaterLine2;
  for(int i=0; i<sizeof(*SettingsRefs.WaterLine2); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_SAVESETTINGS, 0, 0); // Делаем запись в журнале активности

}

//---------------------------------------------------------------------
// EEPROM_GetSettings
void gh_Config::EEPROM_GetSettings() {
  LOG_CFG("gh_Config::EEPROM_GetSettings");
  if( eeprom_read_byte(0) == 0xFF ) {
    // Таким образом определяем, что прошивка нулевая и еще ничего в ней нет. Тогда и не портим свои переменные, которые уже инициализированы
    lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_GETSETTINGSEMPTYEEPROM, 0, 0); // Делаем запись в журнале активности
    return;
  }
  
  eeprom_read_block((void*)SettingsRefs.Window, (const void*)EEPROMAdresses[0], sizeof(*SettingsRefs.Window));
  LOG_CFG("Window Settings:");
  LOG_CFG("Starting address: " + (String)EEPROMAdresses[0]);
  byte *b = (void *)SettingsRefs.Window;
  for(int i=0; i<sizeof(*SettingsRefs.Window); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  eeprom_read_block((void*)SettingsRefs.TerraAccumulator, (const void*)EEPROMAdresses[1], sizeof(*SettingsRefs.TerraAccumulator));
  LOG_CFG("TerraAccumulator Settings:");
  LOG_CFG("Starting address: " + (String)EEPROMAdresses[1]);
  b = (void *)SettingsRefs.TerraAccumulator;
  for(int i=0; i<sizeof(*SettingsRefs.TerraAccumulator); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  eeprom_read_block((void*)SettingsRefs.WaterTank, (const void*)EEPROMAdresses[2], sizeof(*SettingsRefs.WaterTank));
  LOG_CFG("WaterTank Settings:");
  LOG_CFG("Starting address: " + (String)EEPROMAdresses[2]);
  b = (void *)SettingsRefs.WaterTank;
  for(int i=0; i<sizeof(*SettingsRefs.WaterTank); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  eeprom_read_block((void*)SettingsRefs.WaterLine1, (const void*)EEPROMAdresses[3], sizeof(*SettingsRefs.WaterLine1));
  LOG_CFG("WaterLine1 Settings:");
  LOG_CFG("Starting address: " + (String)EEPROMAdresses[3]);
  b = (void *)SettingsRefs.WaterLine1;
  for(int i=0; i<sizeof(*SettingsRefs.WaterLine1); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();
  
  eeprom_read_block((void*)SettingsRefs.WaterLine2, (const void*)EEPROMAdresses[4], sizeof(*SettingsRefs.WaterLine2));
  LOG_CFG("WaterLine1 Settings:");
  LOG_CFG("Starting address: " + (String)EEPROMAdresses[4]);
  b = (void *)SettingsRefs.WaterLine2;
  for(int i=0; i<sizeof(*SettingsRefs.WaterLine2); i++) {
    Serial.print(b[i] + (String)" ");
  }
  Serial.println();

  lg.RecordActivityInt(DEV_BOARD, EVT_BOARD_EEPROM, S_EVT_BOARD_EEPROM_GETSETTINGSOK, 0, 0); // Делаем запись в журнале активности

}
