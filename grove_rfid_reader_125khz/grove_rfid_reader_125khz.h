/*
 * grove_rfid_reader_125khz.h
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


#ifndef __GROVE_RFID_READER_125KHZ_H__
#define __GROVE_RFID_READER_125KHZ_H__

#include "suli2.h"

//GROVE_NAME        "Grove 125KHz RFID Reader"
//SKU               113020002
//IF_TYPE           UART
//IMAGE_URL         http://www.seeedstudio.com/wiki/images/c/c9/Grove_-_125KHz_RFID_Reader.jpg
//DESCRIPTION       "This Grove-125KHz RFID Reader is a module used to read uem4100 RFID card information with two output formats: Uart and Wiegand. It has a sensitivity with maximum 7cm sensing distance."
//WIKI_URL          http://www.seeedstudio.com/wiki/Grove_-_125KHz_RFID_Reader
//ADDED_AT          "2016-07-01"
//AUTHOR            "Travis Brown"

class GroveRfidReader125Khz
{
public:
    GroveRfidReader125Khz(int pintx, int pinrx);

    /**
    * Read the last RFID data received. The data will be cleared after this read.
    *
    * @param data - a string in hex format, e.g. FFBBCC0011
    *
    * @return bool
    */
    bool read_last_data_recved(char **data);

    /**
    * An event which reports the data sequence the RFID Reader has received.
    */
    DEFINE_EVENT(rfid_recv_data, SULI_EDT_STRING);

    void _read_data();

    UART_T *uart;
    TIMER_T *timer;

    bool new_data_available;
    uint8_t *data;
};

#endif