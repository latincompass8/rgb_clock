// RGB LED clock
// circle strip of 60 SK6812(GRBW)

// https://github.com/sonyhome/FAB_LED
#include <FAB_LED.h>

sk6812<B,2>  strip;
#define NUM_PIXEL   60
typedef struct grbw_t color_t;
color_t pixels[NUM_PIXEL] = {};

void clearPixels()
{
    for (uint8_t i = 0; i < NUM_PIXEL; i++) {
        pixels[i] = {};
    }
}

void setPixel(uint8_t n, color_t c)
{
    pixels[n] = c;
}

void setWatch(uint16_t min, uint16_t sec)
{
    clearPixels();
    // LED per 12min
    for (uint8_t i = 0; i < NUM_PIXEL; i++) {
        if (min < 720) {
            // a.m.
            if (i <= (min%720)/12) {
                setPixel(i, {0,16,0,0});
            }
        } else {
            // p.m.
            if (i >= (min%720)/12) {
                setPixel(i, {0,16,0,0});
            }
        }
    }

//#define SEC_TURN_AROUND
#ifdef SEC_TURN_AROUND
    // sec turn around
    setPixel(sec, {16,0,0,0});
#else
    // SEC_BLINK
    if (sec%2) {
        setPixel((min%720)/12, {});
    } else {
        setPixel((min%720)/12, {16,0,0,0});
    }
#endif
}

int ctoi(int c)
{
    switch(c) {
    case '0' ... '9':
        return c - '0';
#if 0
    case 'a' ... 'f':
        return c - 'a' + 10;
    case 'A' ... 'F':
        return c - 'A' + 10;
#endif
    default:
        return -1;
    }
}

int read_time(void)
{
    int i, t;

    i = t = 0;
    // Hhmm
    while(!Serial.available());
    if ((i = ctoi(Serial.read())) == -1) return 0;
    t += i*600;

    // hHmm
    while(!Serial.available());
    if ((i = ctoi(Serial.read())) == -1) return 0;
    t += i*60;

    // hhMm
    while(!Serial.available());
    if ((i = ctoi(Serial.read())) == -1) return 0;
    t += i*10;

    // hhmM
    while(!Serial.available());
    if ((i = ctoi(Serial.read())) == -1) return 0;
    t += i;

    if (t >= 1440) return 0;
    return t;
}

void setup()
{
    Serial.begin(9600);
    strip.clear(NUM_PIXEL);
}

// 1000 tick/sec
#define TICK_PER_SEC 1000
#define U32_MAX 2^32
uint32_t last;
uint16_t sec, min;
void loop()
{
    /* tick */
    uint32_t now, diff;
    now = millis();
    diff = now > last ? (now - last) : (UINT32_MAX + 1 - last + now);
    if (diff >= TICK_PER_SEC) {
        int s = diff/TICK_PER_SEC;
        last += s*TICK_PER_SEC;
        sec  += s;
        if (sec >= 60) {
            min = (min+sec/60)%1440;    // min per day
            sec = sec%60;
        }
#if 0
        Serial.print(min, DEC);
        Serial.print(" ");
        Serial.print(sec, DEC);
        Serial.println();
#endif
        setWatch(min, sec);
        strip.sendPixels(NUM_PIXEL, pixels);
    }

    if (Serial.available() > 0) {
        int c = Serial.read();
        switch (c) {
        case '?':
            Serial.print("Shhmm\t- set time\r\n");
            Serial.print("V\t- show variables\r\n");
            Serial.print("T\t- show time\r\n");
            break;
        case 'S':
            // read four digits and set time
            min = read_time();
            sec = 0;
            setWatch(min, sec);
            Serial.print("set min=");
            Serial.println(min, DEC);
            break;
        case 'V':
            Serial.print("min=");
            Serial.print(min, DEC);
            Serial.print(", sec=");
            Serial.print(sec, DEC);
            Serial.println();
            break;
        case 'T':
            Serial.print(min/60 > 9 ? "" : "0");
            Serial.print(min/60, DEC);
            Serial.print(":");
            Serial.print(min%60 > 9 ? "" : "0");
            Serial.print(min%60, DEC);
            Serial.print(".");
            Serial.print(sec%60 > 9 ? "" : "0");
            Serial.print(sec%60, DEC);
            Serial.println();
            break;
        default:
            Serial.print("? for help.\r\n");
            break;
        }
    }
}
