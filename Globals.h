// ����
#define BTN_SEL 6				// ������ ������
#define BTN_UP 8				// ������ �����
#define BTN_DOWN 7				// ������ ����
#define ONE_WIRE_BUS 2			// ���� ������������� ��������
#define LED_FAN 38				// ��������� ������ �����������
#define LED_FAN_MANUAL_MODE 40	// ��������� ������� ������ �����������
#define L3 42
#define L4 44
#define L5 46
#define L6 48
#define L7 50
#define L8 52
#define RELAY_EARTH_FAN 39		// ���� ����������� ��� ���������� ������������
#define RELAY_WINDOW_OPEN 41	// ���� ������ ���������� ��������� ����
#define RELAY_WINDOW_CLOSE 43	// ���� ������ ���������� ��������� ����
#define R4 45
#define R5 47
#define R6 49
#define R7 51
#define R8 53


// Адреса устройств на шине ONE_WIRE
#define TS_EARTH_ADR { 0x28, 0x9B, 0x12, 0x43, 0x98, 0x01, 0x02, 0xB5 }	// адрес датчика Т-ры Земли
#define TS_AIR_ADR { 0x28, 0xFF, 0xBE, 0xDF, 0x6D, 0x18, 0x01, 0x4F }	// адрес датчика Т-ры Воздуха

// Адреса устройств на шине I2C
#define LCD_ADR 0x27 // 'PCF8574 8-Bit I/O Expander' OR 'LCM1602 LCD Adapter '
#define EEPROM_ADR 0x50 // 'AT24C32 EEPROM'
#define RTC_ADR 0x68 // 'DS3231 real-time clock' OR 'MPU-9250 Nine axis sensor module' �� ����� ����������, �.�. ����� � ���������� RTCLib.h




// ���������� �����������
#define LOGGING  // uncomment to turn on Serial monitor

#ifdef LOGGING
#define LOG(str) Serial.println(str);
#else
#define LOG(str) (str)
#endif
