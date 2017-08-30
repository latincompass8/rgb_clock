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

void setPixels(uint8_t n, color_t c)
{
    pixels[n] = c;
}

void setWatch(uint16_t min, uint16_t sec)
{
    clearPixels();
    // LED per 12min
    if (min < 720) {
        // a.m.
        for (uint8_t i = 0; i <= (min%720)/12; i++) {
            setPixels(i, {0,16,0,0});
        }
    } else {
        // p.m.
        for (uint8_t i = NUM_PIXEL; i >= (min%720)/12; i++) {
            setPixels(i, {0,16,0,0});
        }
    }
    // sec
    setPixels(sec, {16,0,0,0});
}

void setup()
{
    Serial1.begin(9600);
    strip.clear(NUM_PIXEL);
}

// 1000 tick/sec
#define TICK_PER_SEC 1
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

        // update LED
        //strip.clear(NUM_PIXEL);
        //clearPixels();
        //setPixels(sec, {16,0,0,0});
        setWatch(min, sec);
        strip.sendPixels(NUM_PIXEL, pixels);
        
        Serial.print(min, DEC);
        Serial.print(" ");
        Serial.print(sec, DEC);
        Serial.println();
    }
}
