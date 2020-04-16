#if ARDUINO >= 100  //so this will work with Arduino 1.0 or the older 022 version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DS18B20.h"

DS18B20::DS18B20(DeviceAddress SensorAddress, DallasTemperature* TSensors)
//:_pin(pin), _wire(pin), _sensor(&_wire)
  {
	//_SensorAddress = SensorAddress;
	_TSensors = TSensors;

   //_sensor.begin();
}
// Снимаем температуру с датчика
float DS18B20::getT()
    {

	
    _TSensors->requestTemperatures();
    float temp = _TSensors->getTempC((uint8_t*) _SensorAddress);
    return temp;
    } 
void DS18B20::begin()
    {
    //_sensor.begin();
    }
    
