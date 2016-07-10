/*
* grove_chainable_rgb_led.cpp
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
#include "grove_chainable_rgb_led.h"

GroveChainableRgbLed::GroveChainableRgbLed(int pintx, int pinrx)
{
    this->io_clk = (IO_T *)malloc(sizeof(IO_T));
    this->io_data = (IO_T *)malloc(sizeof(IO_T));

    suli_pin_init(io_clk, pinrx, SULI_OUTPUT);
    suli_pin_init(io_data, pintx, SULI_OUTPUT);

    led_cnt = GCRGBLED_MAX_LED_CNT;

    _clear_buffer();
}

bool GroveChainableRgbLed::write_clear(uint8_t total_led_cnt, char *rgb_hex_string)
{
    led_cnt = total_led_cnt > GCRGBLED_MAX_LED_CNT ? GCRGBLED_MAX_LED_CNT : total_led_cnt;

    if (_extract_rgb_from_string(0, rgb_hex_string))
    {
        for (int led_num = 1; led_num < led_cnt; led_num++)
        {
            rgb_buffer.pixels[led_num].r = rgb_buffer.pixels[0].r;
            rgb_buffer.pixels[led_num].g = rgb_buffer.pixels[0].g;
            rgb_buffer.pixels[led_num].b = rgb_buffer.pixels[0].b;
        }
        _rainbow_flow_brightness = 100;
        _show_from_buffer();
        return true;
    }
    else return false;
}

bool GroveChainableRgbLed::write_segment(uint8_t start, char *rgb_hex_string)
{
    if (_extract_rgb_from_string(start, rgb_hex_string))
    {
        _rainbow_flow_brightness = 100;
        _show_from_buffer();
        return true;
    }
    else return false;
}

void GroveChainableRgbLed::_clear_buffer() {
    for (int led_num = 0; led_num < led_cnt; led_num++)
    {
        rgb_buffer.pixels[led_num].r = 0;
        rgb_buffer.pixels[led_num].g = 0;
        rgb_buffer.pixels[led_num].b = 0;
    }
    memset(_color_towards, 0, sizeof(_color_towards));
}

void GroveChainableRgbLed::_show_from_buffer()
{
    // Send data frame prefix (32x "0")
    _send_frame_envelope();

    for (int led_num = 0; led_num < led_cnt; led_num++)
    {
        _send_color(
            (rgb_buffer.pixels[led_num].r * _rainbow_flow_brightness / 100),
            (rgb_buffer.pixels[led_num].g * _rainbow_flow_brightness / 100),
            (rgb_buffer.pixels[led_num].b* _rainbow_flow_brightness / 100)
        );
    }

    // Terminate data frame (32x "0")
    _send_frame_envelope();
}

bool GroveChainableRgbLed::_extract_rgb_from_string(int start, char *str)
{
    int len = strlen(str);
    len = len - len % 6;
    int cnt = len / 6;

    uint8_t hex[3];
    hex[2] = '\0';
    uint32_t ul;

    for (int i = 0; i < cnt && (start + i)<led_cnt; i++)
    {
        memcpy(hex, (str + i * 6), 2);
        ul = strtoul((const char *)hex, NULL, 16);
        if (ul >= 0 && ul <= 255)
        {
            rgb_buffer.pixels[start + i].r = ul;
        }
        else
        {
            error_desc = "invalid R value";
            return false;
        }

        memcpy((void *)hex, (void *)(str + i * 6 + 2), 2);
        ul = strtoul((const char *)hex, NULL, 16);
        if (ul >= 0 && ul <= 255)
        {
            rgb_buffer.pixels[start + i].g = ul;
        }
        else
        {
            error_desc = "invalid G value";
            return false;
        }

        memcpy((void *)hex, (void *)(str + i * 6 + 4), 2);
        ul = strtoul((const char *)hex, NULL, 16);
        if (ul >= 0 && ul <= 255)
        {
            rgb_buffer.pixels[start + i].b = ul;
        }
        else
        {
            error_desc = "invalid B value";
            return false;
        }

    }

    return true;
}

void GroveChainableRgbLed::_send_color(byte red, byte green, byte blue)
{
    // Start by sending a byte with the format "1 1 /B7 /B6 /G7 /G6 /R7 /R6"
    byte prefix = 0b11000000;
    if ((blue & 0x80) == 0)     prefix |= 0b00100000;
    if ((blue & 0x40) == 0)     prefix |= 0b00010000;
    if ((green & 0x80) == 0)    prefix |= 0b00001000;
    if ((green & 0x40) == 0)    prefix |= 0b00000100;
    if ((red & 0x80) == 0)      prefix |= 0b00000010;
    if ((red & 0x40) == 0)      prefix |= 0b00000001;
    _send_byte(prefix);

    // Now must send the 3 colors
    _send_byte(blue);
    _send_byte(green);
    _send_byte(red);
}

void GroveChainableRgbLed::_send_byte(byte b)
{
    // Send one bit at a time, starting with the MSB
    for (byte i = 0; i<8; i++)
    {
        // If MSB is 1, write one and clock it, else write 0 and clock
        if ((b & 0x80) != 0)
            suli_pin_write(io_data, SULI_HIGH);
        else
            suli_pin_write(io_data, SULI_LOW);
        _send_clk();

        // Advance to the next bit to send
        b <<= 1;
    }
}

void GroveChainableRgbLed::_send_clk()
{
    suli_pin_write(io_clk, SULI_LOW);
    suli_delay_us(GCRGBLED_CLK_PULSE_DELAY);
    suli_pin_write(io_clk, SULI_HIGH);
    suli_delay_us(GCRGBLED_CLK_PULSE_DELAY);
}

void GroveChainableRgbLed::_send_frame_envelope()
{
    _send_byte(0x00);
    _send_byte(0x00);
    _send_byte(0x00);
    _send_byte(0x00);
}

bool GroveChainableRgbLed::write_start_rainbow_flow(uint8_t length, uint8_t brightness, uint8_t speed)
{
    led_cnt = length > GCRGBLED_MAX_LED_CNT ? GCRGBLED_MAX_LED_CNT : length;
    _rainbow_flow_brightness = constrain(brightness, 0, 100);

    int8_t step_r, step_g, step_b;
    uint8_t start_r, start_g, start_b;
    uint8_t next_towards = 0;

    memset(_color_towards, 0, led_cnt);

    for (int i = 0; i < led_cnt; i++)
    {
        _color_towards[i] = next_towards;
        switch (_color_towards[i])
        {
        case 0:
            start_r = 0;
            start_g = 0;
            start_b = 250;
            step_r = 25;
            step_g = 0;
            step_b = -25;
            break;
        case 1:
            start_r = 250;
            start_g = 0;
            start_b = 0;
            step_r = -25;
            step_g = 25;
            step_b = 0;
            break;
        case 2:
            start_r = 0;
            start_g = 250;
            start_b = 0;
            step_r = 0;
            step_g = -25;
            step_b = 25;
            break;
        default:
            break;
        }

        int ii = i % 10;
        if (ii == 0) ii = 10;

        rgb_buffer.pixels[i].r = start_r + ii*step_r;
        rgb_buffer.pixels[i].g = start_g + ii*step_g;
        rgb_buffer.pixels[i].b = start_b + ii*step_b;

        if (i % 10 == 0)
        {
            next_towards += 1;
            if (next_towards > 2)
            {
                next_towards = 0;
            }
            _color_towards[i] = next_towards;
        }
    }
    _show_from_buffer();

    long s = constrain(speed, 1, 10);
    int32_t interval = suli_map(s, 1, 10, 200000, 10000);

    suli_timer_install(timer, (uint32_t)interval, gcrgbled_timer_handler, this, true);
    return true;
}

bool GroveChainableRgbLed::write_stop_rainbow_flow()
{
    suli_timer_remove(timer);
    return true;
}

void GroveChainableRgbLed::_update_color()
{
    int8_t step_r = 25;
    int8_t step_g = 0;
    int8_t step_b = -25;

    for (int i = 0; i < led_cnt; i++)
    {
        switch (_color_towards[i])
        {
        case 0:
            step_r = 25;
            step_g = 0;
            step_b = -25;
            break;
        case 1:
            step_r = -25;
            step_g = 25;
            step_b = 0;
            break;
        case 2:
            step_r = 0;
            step_g = -25;
            step_b = 25;
            break;
        default:
            break;
        }
        rgb_buffer.pixels[i].r += step_r;
        rgb_buffer.pixels[i].g += step_g;
        rgb_buffer.pixels[i].b += step_b;

        if (rgb_buffer.pixels[i].r >= 250)
        {
            rgb_buffer.pixels[i].r = 250;
            rgb_buffer.pixels[i].g = 0;
            rgb_buffer.pixels[i].b = 0;
            _color_towards[i] = 1;
        }
        else if (rgb_buffer.pixels[i].g >= 250)
        {
            rgb_buffer.pixels[i].r = 0;
            rgb_buffer.pixels[i].g = 250;
            rgb_buffer.pixels[i].b = 0;
            _color_towards[i] = 2;
        }
        else if (rgb_buffer.pixels[i].b >= 250)
        {
            rgb_buffer.pixels[i].r = 0;
            rgb_buffer.pixels[i].g = 0;
            rgb_buffer.pixels[i].b = 250;
            _color_towards[i] = 0;
        }
    }
    _show_from_buffer();
}

static void gcrgbled_timer_handler(void *para)
{
    GroveChainableRgbLed *g = (GroveChainableRgbLed *)para;
    g->_update_color();
}