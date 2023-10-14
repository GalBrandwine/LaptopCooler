#ifndef TFT_CONTROLLER_H
#define TFT_CONTROLLER_H "Gal is using old header guard mechanism"
/***************************************************
  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
#include "FastLED.h"
#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

// According to https://forum.arduino.cc/t/facing-problem-while-interfacing-with-qvga-display-2-2-tft-spi-240x320/427581
// The pins must have 3.3V logic. The Uno/Nano has 5v logic so:
// Votlage deviding the pins with:
// V_out = (V_source X R2) / ( R1 + R2)
#define R1 4K7 // Ohm
#define R2 10K // Ohm

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
static const uint8_t sclk = 13;
static const uint8_t miso = 12;
static const uint8_t mosi = 11;
static const uint8_t cs = 10;
static const uint8_t dc = 9;
static const uint8_t rst = 8;

Adafruit_ILI9340 *tft;
namespace tft_ctrl
{
    // Using software SPI is really not suggested, its incredibly slow
    // Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
    // Use hardware SPI

    void setup()
    {
        // Serial.begin(9600);
        // while (!Serial)
        //     ;

        Serial.println("Adafruit 2.2\" SPI TFT setting up!");
        tft = new Adafruit_ILI9340(cs, dc, mosi, sclk, rst, miso);
        tft->begin();
        tft->setRotation(1);

        // Serial.println(F("Benchmark                Time (microseconds)"));
        // Serial.print(F("Screen fill              "));
        // Serial.println(testFillScreen());
        // delay(500);

        // Serial.print(F("Text                     "));
        // Serial.println(testText());
        // delay(3000);

        // Serial.print(F("Lines                    "));
        // Serial.println(testLines(ILI9340_CYAN));
        // delay(500);

        // Serial.print(F("Horiz/Vert Lines         "));
        // Serial.println(testFastLines(ILI9340_RED, ILI9340_BLUE));
        // delay(500);

        // Serial.print(F("Rectangles (outline)     "));
        // Serial.println(testRects(ILI9340_GREEN));
        // delay(500);

        // Serial.print(F("Rectangles (filled)      "));
        // Serial.println(testFilledRects(ILI9340_YELLOW, ILI9340_MAGENTA));
        // delay(500);

        // Serial.print(F("Circles (filled)         "));
        // Serial.println(testFilledCircles(10, ILI9340_MAGENTA));

        // Serial.print(F("Circles (outline)        "));
        // Serial.println(testCircles(10, ILI9340_WHITE));
        // delay(500);

        // Serial.print(F("Triangles (outline)      "));
        // Serial.println(testTriangles());
        // delay(500);

        // Serial.print(F("Triangles (filled)       "));
        // Serial.println(testFilledTriangles());
        // delay(500);

        // Serial.print(F("Rounded rects (outline)  "));
        // Serial.println(testRoundRects());
        // delay(500);

        // Serial.print(F("Rounded rects (filled)   "));
        // Serial.println(testFilledRoundRects());
        // delay(500);

        Serial.println(F("TFT controller Done setup!"));
    }

    // void loop(void)
    // {
    //     for (uint8_t rotation = 0; rotation < 4; rotation++)
    //     {
    //         tft->setRotation(rotation);
    //         testText();
    //         delay(2000);
    //     }
    // }
    unsigned long clearScreen()
    {
        unsigned long start = micros();
        tft->fillScreen(ILI9340_BLACK);
        return micros() - start;
    }
    unsigned long testFillScreen()
    {
        unsigned long start = micros();
        tft->fillScreen(ILI9340_BLACK);
        tft->fillScreen(ILI9340_RED);
        tft->fillScreen(ILI9340_GREEN);
        tft->fillScreen(ILI9340_BLUE);
        tft->fillScreen(ILI9340_BLACK);
        return micros() - start;
    }

    unsigned long testText()
    {
        tft->fillScreen(ILI9340_BLACK);
        unsigned long start = micros();
        tft->setCursor(0, 0);
        tft->setTextColor(ILI9340_WHITE);
        tft->setTextSize(1);
        tft->println("Hello World!");
        tft->setTextColor(ILI9340_YELLOW);
        tft->setTextSize(2);
        tft->println(1234.56);
        tft->setTextColor(ILI9340_RED);
        tft->setTextSize(3);
        tft->println(0xDEADBEEF, HEX);
        tft->println();
        tft->setTextColor(ILI9340_GREEN);
        tft->setTextSize(5);
        tft->println("Groop");
        tft->setTextSize(2);
        tft->println("I implore thee,");
        tft->setTextSize(1);
        tft->println("my foonting turlingdromes.");
        tft->println("And hooptiously drangle me");
        tft->println("with crinkly bindlewurdles,");
        tft->println("Or I will rend thee");
        tft->println("in the gobberwarts");
        tft->println("with my blurglecruncheon,");
        tft->println("see if I don't!");
        return micros() - start;
    }

    unsigned long testLines(uint16_t color)
    {
        unsigned long start, t;
        int x1, y1, x2, y2,
            w = tft->width(),
            h = tft->height();

        tft->fillScreen(ILI9340_BLACK);

        x1 = y1 = 0;
        y2 = h - 1;
        start = micros();
        for (x2 = 0; x2 < w; x2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        x2 = w - 1;
        for (y2 = 0; y2 < h; y2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        t = micros() - start; // fillScreen doesn't count against timing

        tft->fillScreen(ILI9340_BLACK);

        x1 = w - 1;
        y1 = 0;
        y2 = h - 1;
        start = micros();
        for (x2 = 0; x2 < w; x2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        x2 = 0;
        for (y2 = 0; y2 < h; y2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        t += micros() - start;

        tft->fillScreen(ILI9340_BLACK);

        x1 = 0;
        y1 = h - 1;
        y2 = 0;
        start = micros();
        for (x2 = 0; x2 < w; x2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        x2 = w - 1;
        for (y2 = 0; y2 < h; y2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        t += micros() - start;

        tft->fillScreen(ILI9340_BLACK);

        x1 = w - 1;
        y1 = h - 1;
        y2 = 0;
        start = micros();
        for (x2 = 0; x2 < w; x2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);
        x2 = 0;
        for (y2 = 0; y2 < h; y2 += 6)
            tft->drawLine(x1, y1, x2, y2, color);

        return micros() - start;
    }

    unsigned long testFastLines(uint16_t color1, uint16_t color2)
    {
        unsigned long start;
        int x, y, w = tft->width(), h = tft->height();

        tft->fillScreen(ILI9340_BLACK);
        start = micros();
        for (y = 0; y < h; y += 5)
            tft->drawFastHLine(0, y, w, color1);
        for (x = 0; x < w; x += 5)
            tft->drawFastVLine(x, 0, h, color2);

        return micros() - start;
    }

    unsigned long testRects(uint16_t color)
    {
        unsigned long start;
        int n, i, i2,
            cx = tft->width() / 2,
            cy = tft->height() / 2;

        tft->fillScreen(ILI9340_BLACK);
        n = min(tft->width(), tft->height());
        start = micros();
        for (i = 2; i < n; i += 6)
        {
            i2 = i / 2;
            tft->drawRect(cx - i2, cy - i2, i, i, color);
        }

        return micros() - start;
    }

    unsigned long testFilledRects(uint16_t color1, uint16_t color2)
    {
        unsigned long start, t = 0;
        int n, i, i2,
            cx = tft->width() / 2 - 1,
            cy = tft->height() / 2 - 1;

        tft->fillScreen(ILI9340_BLACK);
        n = min(tft->width(), tft->height());
        for (i = n; i > 0; i -= 6)
        {
            i2 = i / 2;
            start = micros();
            tft->fillRect(cx - i2, cy - i2, i, i, color1);
            t += micros() - start;
            // Outlines are not included in timing results
            tft->drawRect(cx - i2, cy - i2, i, i, color2);
        }

        return t;
    }

    unsigned long testFilledCircles(uint8_t radius, uint16_t color)
    {
        unsigned long start;
        int x, y, w = tft->width(), h = tft->height(), r2 = radius * 2;

        tft->fillScreen(ILI9340_BLACK);
        start = micros();
        for (x = radius; x < w; x += r2)
        {
            for (y = radius; y < h; y += r2)
            {
                tft->fillCircle(x, y, radius, color);
            }
        }

        return micros() - start;
    }

    unsigned long drawCircle(int x, int y, uint8_t radius, uint16_t color)
    {
        unsigned long start;
        int r2 = radius * 2;
        // w = tft->width() + radius,
        // h = tft->height() + radius;

        // Screen is not cleared for this one -- this is
        // intentional and does not affect the reported time.
        start = micros();

        tft->drawCircle(x, y, radius, color);

        return micros() - start;
    }
    unsigned long fitCircle()
    {
        auto center_x{ILI9340_TFTWIDTH / 2};
        auto center_y{ILI9340_TFTHEIGHT / 2};
        auto radius{ILI9340_TFTWIDTH / 2};
        return tft_ctrl::drawCircle(center_x, center_y, radius, ILI9340_WHITE);
    }
    void clearTempOnHueCircle(int temperature)
    {
        int center_x;
        int center_y;
        int radius;
        switch (tft->getRotation())
        {
        case 0:
            center_x = ILI9340_TFTWIDTH / 2;
            center_y = ILI9340_TFTHEIGHT / 2;
            radius = ILI9340_TFTWIDTH / 2 - 5;
            break;
        case 1:
            center_x = ILI9340_TFTHEIGHT / 2;
            center_y = ILI9340_TFTWIDTH / 2;
            radius = ILI9340_TFTWIDTH / 2 - 5;
            break;
        default:
            break;
        }

        auto hue = map(temperature, 50, 0, 0, 160); // Convert tmp to hue
        auto angle_deg = map(hue, 0, 255, 0, 360);  // Convert hue to angle
        auto x = center_x + radius * cos(angle_deg * PI / 180);
        auto y = center_y + radius * sin(angle_deg * PI / 180);
        tft->fillCircle(x, y, 5, ILI9340_BLACK);
    }
    void drawTempOnHueCircle(int temperature)
    {
        CRGB rgb;
        Serial.print("got temp: ");
        Serial.print(temperature);
        // Convert temp to hue
        auto hue = map(temperature, 50, 0, 0, 160); // Lower TEMP gets bluer HUE
        Serial.print("converted to hue: ");
        Serial.print(hue);
        // Convert hue to angle
        auto angle_deg = map(hue, 0, 255, 0, 360); // Lower TEMP gets bluer HUE
        Serial.print("converted to angle: ");
        Serial.print(angle_deg);

        int center_x;
        int center_y;
        int radius;
        switch (tft->getRotation())
        {
        case 0:
            center_x = ILI9340_TFTWIDTH / 2;
            center_y = ILI9340_TFTHEIGHT / 2;
            radius = ILI9340_TFTWIDTH / 2 - 5;
            break;
        case 1:
            center_x = ILI9340_TFTHEIGHT / 2;
            center_y = ILI9340_TFTWIDTH / 2;
            radius = ILI9340_TFTWIDTH / 2 - 5;
            break;
        default:
            break;
        }

        auto x = center_x + radius * cos(angle_deg * PI / 180);
        auto y = center_y + radius * sin(angle_deg * PI / 180);
        auto hsv_hue = map(angle_deg, 0, 359, 0, 255);

        // tft->drawCircle(x, y, 5, ILI9340_WHITE);
        rgb.setHue(hsv_hue);
        auto tft_color = tft->Color565(rgb.r, rgb.g, rgb.b);
        tft->fillCircle(x, y, 5, tft_color);

        // Write temperature on screen
        // tft->setCursor(center_x - 50, center_y);
        // tft->setTextSize(2);
        // tft->print("Tmp: ");
        // tft->print(temperature);
        // tft->print(" [C]");
        // tft->setTextSize(1);
    }
    /**
     * @brief Draw a HUE representative circle
     *
     * @return
     */
    void drawHueCircle()
    {
        int center_x;
        int center_y;
        int radius;
        switch (tft->getRotation())
        {
        case 0:
            center_x = ILI9340_TFTWIDTH / 2;
            center_y = ILI9340_TFTHEIGHT / 2;
            radius = ILI9340_TFTWIDTH / 2 - 5;
            break;
        case 1:
            center_x = ILI9340_TFTHEIGHT / 2;
            center_y = ILI9340_TFTWIDTH / 2;
            radius = ILI9340_TFTWIDTH / 2 - 5;
            break;
        default:
            break;
        }

        auto res{1};
        tft->drawPixel(center_x, center_y, ILI9340_DISPON);
        tft->drawLine(0, 0, 0, 50, ILI9340_YELLOW);
        tft->drawLine(0, 0, 50, 0, ILI9340_RED);
        CRGB rgb;
        for (size_t theta_deg = 0; theta_deg < 359; theta_deg++)
        {
            auto x = center_x + radius * cos(theta_deg * PI / 180) * res;
            auto y = center_y + radius * sin(theta_deg * PI / 180) * res;
            if (theta_deg % 60 == 0)
            {
                if (theta_deg == 0)
                {
                    tft->setCursor(x - 5, y);
                }
                else
                {
                    tft->setCursor(x, y);
                }

                tft->print(theta_deg);
            }

            auto hsv_hue = map(theta_deg, 0, 359, 0, 255);
            rgb.setHue(hsv_hue);
            auto tft_color = tft->Color565(rgb.r, rgb.g, rgb.b);
            tft->drawPixel(x, y, tft_color);
        }
    }

    unsigned long testCircles(uint8_t radius, uint16_t color)
    {
        unsigned long start;
        int x, y, r2 = radius * 2,
                  w = tft->width() + radius,
                  h = tft->height() + radius;

        // Screen is not cleared for this one -- this is
        // intentional and does not affect the reported time.
        start = micros();
        for (x = 0; x < w; x += r2)
        {
            for (y = 0; y < h; y += r2)
            {
                tft->drawCircle(x, y, radius, color);
            }
        }

        return micros() - start;
    }

    unsigned long testTriangles()
    {
        unsigned long start;
        int n, i, cx = tft->width() / 2 - 1,
                  cy = tft->height() / 2 - 1;

        tft->fillScreen(ILI9340_BLACK);
        n = min(cx, cy);
        start = micros();
        for (i = 0; i < n; i += 5)
        {
            tft->drawTriangle(
                cx, cy - i,     // peak
                cx - i, cy + i, // bottom left
                cx + i, cy + i, // bottom right
                tft->Color565(0, 0, i));
        }

        return micros() - start;
    }

    unsigned long testFilledTriangles()
    {
        unsigned long start, t = 0;
        int i, cx = tft->width() / 2 - 1,
               cy = tft->height() / 2 - 1;

        tft->fillScreen(ILI9340_BLACK);
        start = micros();
        for (i = min(cx, cy); i > 10; i -= 5)
        {
            start = micros();
            tft->fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                              tft->Color565(0, i, i));
            t += micros() - start;
            tft->drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                              tft->Color565(i, i, 0));
        }

        return t;
    }

    unsigned long testRoundRects()
    {
        unsigned long start;
        int w, i, i2,
            cx = tft->width() / 2 - 1,
            cy = tft->height() / 2 - 1;

        tft->fillScreen(ILI9340_BLACK);
        w = min(tft->width(), tft->height());
        start = micros();
        for (i = 0; i < w; i += 6)
        {
            i2 = i / 2;
            tft->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft->Color565(i, 0, 0));
        }

        return micros() - start;
    }

    unsigned long testFilledRoundRects()
    {
        unsigned long start;
        int i, i2,
            cx = tft->width() / 2 - 1,
            cy = tft->height() / 2 - 1;

        tft->fillScreen(ILI9340_BLACK);
        start = micros();
        for (i = min(tft->width(), tft->height()); i > 20; i -= 6)
        {
            i2 = i / 2;
            tft->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft->Color565(0, i, 0));
        }

        return micros() - start;
    }
} // tft_ctrl
#endif /*TFT_CONTROLLER_H*/