#ifndef DS18B20_h
#define DS18B20_h

#if ARDUINO >= 100  //so this will work with Arduino 1.0 or the older 022 version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <inttypes.h>
#include <OneWire.h>
#include <DallasTemperature.h>
class DS18B20 //: public OneWire , public DallasTemperature
{   
  public:
    //DS18B20(uint8_t);// takes a byte pin number where the temperature sensor is attached
	DS18B20(DeviceAddress, DallasTemperature*);// takes a byte index number of sensor and a DallasTemperature sensors reference
  	float getT();// returns temperature in degrees C
  	void  begin();
  private:
    //uint8_t _pin;        //The pin the sensor is attached to.
	DeviceAddress _SensorAddress;        //The Index on the wire where the sensor is attached to.
    //OneWire _wire;       //An instance of the oneWire bus for this sensor.
    DallasTemperature* _TSensors; //An instance of DallasTemperature for this sensor.
};
#endif

