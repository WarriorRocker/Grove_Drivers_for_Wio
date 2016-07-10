/*
 * grove_rfid_reader_125khz.cpp
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
#include "grove_rfid_reader_125khz.h"

GroveRfidReader125Khz::GroveRfidReader125Khz(int pintx, int pinrx)
{
    this->uart = (UART_T *)malloc(sizeof(UART_T));
    //suli_timer_install(this->timer, 100000, grove_rfid_reader_125khz_timer_handler, this, true);
    suli_uart_init(uart, pintx, pinrx, 9600);
}

void GroveRfidReader125Khz::_read_data()
{
    int length = 0;
    uint8_t *stream;
    
    while ((suli_uart_readable(uart)) && (length < 64))
    {
        length += suli_uart_read_bytes(uart, stream, 1);
    }
    
    if (length > 0)
    {
        this->data = stream;
        this->new_data_available = true;
        POST_EVENT(rfid_recv_data, this->data);
    }
}

bool GroveRfidReader125Khz::read_last_data_recved(char **p_data)
{
    this->_read_data();

    if (this->new_data_available)
    {
        *p_data = this->data;
        this->new_data_available = false;
    }
    else
    {
        *p_data = "";
    }

    return true;
}

static void grove_rfid_reader_125khz_timer_handler(void *para)
{
    GroveRfidReader125Khz *g = (GroveRfidReader125Khz *)para;
    g->_read_data();
}
