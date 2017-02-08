/*
* grove_temp_hum_am2315.h
*
* Copyright (c) 2016 seeed technology inc.
* Website    : www.seeed.cc
* Author     : Travis Brown (warriorrocker@gmail.com)
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/


#ifndef __GROVE_TEMP_HUM_AM2315_H__
#define __GROVE_TEMP_HUM_AM2315_H__

#include "suli2.h"

//GROVE_NAME        "Grove - Encased I2C Temperature/Humidity Sensor"
//SKU               f06c581e-4d3d-11e6-beb8-9e71128cae77
//IF_TYPE           I2C
//IMAGE_URL         http://www.seeedstudio.com/wiki/images/c/c5/UV_Sensor_01.jpg
//DESCRIPTION       "The Grove – UV Sensor is used for detecting the intensity of incident ultraviolet(UV) radiation. This form of electromagnetic radiation has shorter wavelengths than visible radiation."
//WIKI_URL          http://www.seeedstudio.com/wiki/Grove_-_UV_Sensor
//ADDED_AT          "2016-07-18"
//AUTHOR            "Travis Brown"

// Device i2c address
#define GROVEHM2315_ADDRESS        (0x5c << 1)
#define GROVEHM2315_READREG        0x03

class GroveHm2315
{
public:
	GroveHm2315(int pinsda, int pinscl);

	/**
	* The temperature and humidity sensor will update the reading every 2 seconds.
	*
	* @param celsius_degree - unit: Celsius degree
	*
	* @return bool
	*/
	bool read_temperature(float *celsius_degree);

	/**
	* The temperature and humidity sensor will update the reading every 2 seconds.
	*
	* @param fahrenheit_degree - Fahrenheit degree
	*
	* @return bool
	*/
	bool read_temperature_f(float *fahrenheit_degree);

	/**
	* The temperature and humidity sensor will update the reading every 2 seconds.
	*
	* @param humidity - 0~100(%)
	*
	* @return bool
	*/
	bool read_humidity(float *humidity);


private:
	bool _hm2315_read_temp_humidity();

	I2C_T *i2c;

	float _temperature;
	float _humidity;
};

#endif