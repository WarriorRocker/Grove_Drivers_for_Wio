/*
* grove_chainable_rgb_led.h
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


#ifndef __GROVE_CHAINABLE_RGB_LED_H__
#define __GROVE_CHAINABLE_RGB_LED_H__

#include "suli2.h"

//GROVE_NAME        "Grove - Chainable RGB LED"
//SKU               104030006
//IF_TYPE           UART
//IMAGE_URL         http://www.seeedstudio.com/wiki/images/thumb/9/94/Chanbalelednb1.jpg/400px-Chanbalelednb1.jpg
//DESCRIPTION       "Grove - Chainable RGB LED is based on P9813 chip which is a full-color LED driver. It provides 3 constant-current drivers as well as modulated output of 256 shades of gray."
//WIKI_URL          http://www.seeedstudio.com/wiki/Grove_-_Chainable_RGB_LED
//ADDED_AT          "2016-07-01"
//AUTHOR            "Travis Brown"

#define GCRGBLED_CL_RED             0
#define GCRGBLED_CL_GREEN           1
#define GCRGBLED_CL_BLUE            2
#define GCRGBLED_CLK_PULSE_DELAY    20
#define GCRGBLED_MAX_LED_CNT        10

struct gcrgbled_rgb_s
{
    uint8_t g, r, b;
}__attribute__((__packed__));

union gcrgbled_buffer_u
{
    uint8_t buff[GCRGBLED_MAX_LED_CNT * 3];
    struct gcrgbled_rgb_s pixels[GCRGBLED_MAX_LED_CNT];
};

class GroveChainableRgbLed
{
public:
    GroveChainableRgbLed(int pintx, int pinrx);

    /**
    * Set or reset or clear the led strip with a specified color.
    *
    * @param total_led_cnt - the total count of this strip, max: 60(too many leds will cause the power unstable)
    * @param rgb_hex_string - a rgb value in hex format, e.g. AA55CC (without # or 0x)
    *
    * @return bool
    */
    bool write_clear(uint8_t total_led_cnt, char *rgb_hex_string);

    /**
    * Change the color of some piece of segment of the led strip.
    * We need to specify a list of rgb hex value concatinated into a string.
    * The segment will be defined with a start index and the length.
    * The length equals rgb_hex_string's length / 6.
    *
    * @param start - the start index of the segment(included)
    * @param rgb_hex_string - a list of rgb hex value, e.g. FFFFFFBBBBBBCCCCCC000000111111, max length: 240 or 40 rgb hex
    *
    * @return bool
    */
    bool write_segment(uint8_t start, char *rgb_hex_string);

    /**
    * Make the strip blink in a rainbow flow
    *
    * @param length - the length of the flow, the flow will always begin with index 0.
    * @param brightness - 0~100
    * @param speed - 1~10, 10 is the fastest.
    *
    * @return bool
    */
    bool write_start_rainbow_flow(uint8_t length, uint8_t brightness, uint8_t speed);

    /**
    * Stop the rainbow flow.
    *
    * @return bool
    */
    bool write_stop_rainbow_flow();
private:
    void _clear_buffer();
    void _show_from_buffer();
    bool _extract_rgb_from_string(int start, char *str);

    void _send_color(byte red, byte green, byte blue);
    void _send_byte(byte b);
    void _send_clk();
    void _send_frame_envelope();

    IO_T *io_clk;
    IO_T *io_data;
    TIMER_T *timer;
    char *error_desc;

    uint8_t led_cnt;
    uint8_t _color_towards[GCRGBLED_MAX_LED_CNT];
    uint8_t _rainbow_flow_brightness;
    union gcrgbled_buffer_u rgb_buffer;

public:
    void _update_color();
};

static void gcrgbled_timer_handler(void *para);

#endif
