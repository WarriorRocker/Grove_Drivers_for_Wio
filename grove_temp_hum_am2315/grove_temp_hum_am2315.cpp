/*
* grove_temp_hum_am2315.cpp
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


#include "suli2.h"
#include "grove_temp_hum_am2315.h"

GroveHm2315::GroveHm2315(int pinsda, int pinscl)
{
	this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
	suli_i2c_init(this->i2c, pinsda, pinscl);
}

bool GroveHm2315::read_temperature(float *temperature)
{
	if (!_hm2315_read_temp_humidity())
		return false;
	*temperature = _temperature;
	return true;
}

bool GroveHm2315::read_temperature_f(float *temperature)
{
	if (!_hm2315_read_temp_humidity())
		return false;
	*temperature = _temperature * 9 / 5 + 32;
	return true;
}

bool GroveHm2315::read_humidity(float *humidity)
{
	if (!_hm2315_read_temp_humidity())
		return false;
	*humidity = _humidity;
	return true;
}

bool GroveHm2315::_hm2315_read_temp_humidity()
{
	uint8_t write_data[3] = { GROVEHM2315_READREG, 0x00, 0x04 };
	uint8_t read_data[8];

	suli_i2c_write(i2c, GROVEHM2315_ADDRESS, 0x00, 0);
	suli_i2c_write(i2c, GROVEHM2315_ADDRESS, write_data, 3);
	suli_delay_ms(10);
	suli_i2c_read(i2c, GROVEHM2315_ADDRESS, read_data, 8);

	if ((read_data[0] != GROVEHM2315_READREG) || (read_data[1] != 4))
		return false;

	_temperature = read_data[4] & 0x7F;
	_temperature *= 256;
	_temperature += read_data[5];
	_temperature /= 10;
	if (read_data[4] >> 7) _temperature = -_temperature;

	_humidity = read_data[2];
	_humidity *= 256;
	_humidity += read_data[3];
	_humidity /= 10;

	return true;
}