#include <Adafruit_NeoPixel.h>

#define PIN 6
#define STRIPLENGTH 18
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPLENGTH, PIN, NEO_GRB + NEO_KHZ800);

/*
 * Convert an RGB value to a color.  This can be done by the compiler
 * to populate the color scheme array.
 */
static uint32_t
NeoPixelColor(
  uint8_t  red,
  uint8_t  green,
  uint8_t  blue)
{
  return ( (uint32_t)red << 16) | ( (uint32_t)green << 8) | blue;
};

/*
 * Add a color to an existing pixel.
 */
void
addToPixelColor(
    uint16_t            idx,
    uint32_t            c1)
{
    uint32_t            c2 = strip.getPixelColor(idx);

    uint32_t            red = (c1 >> 16) + (c2 >> 16);
    if (red > 255) {
        red = 255;
    }
    uint32_t            green = ( (c1 >> 8) & 255) + ( (c2 >> 8) & 255);
    if (green > 255) {
        green = 255;
    }
    uint32_t            blue = (c1 & 255) + (c2 & 255);
    if (blue > 255) {
        blue = 255;
    }
    strip.setPixelColor(idx, red, green, blue);
}

/*
 * A catepillar that walks back and forth along a route
 *
 * Pixel numbers start at 0.
 * direction +1 = forward (increasing indices)
 * direction -1 = reverse (decreasing indices)
 */

/*
 * Different color schemes for catepillars:
 * Each catepillar has a color for head, then body, then tail
 * 0 is a green catepillar
 *
 */
uint32_t     cat_color_scheme[][3] = {
    { NeoPixelColor(0, 128, 0), NeoPixelColor(0, 128, 0), NeoPixelColor(0, 128, 0) }
    { NeoPixelColor(128, 0, 0), NeoPixelColor(128, 0, 0), NeoPixelColor(128, 0, 0) }
};

class catepillar {
public:
    uint8_t     route;          // route being walked
    uint8_t     scheme;         // color scheme
    int8_t      head;           // location of head
    int8_t      length;         // length of beasty
    int8_t      dir;            // direction walking
    uint8_t     speed;          // speed (not used)
    uint8_t     brightness;     // brightness (not used)

    // Constructor for a catepillar
    catepillar(
        uint8_t         rte,    // route
        uint8_t         sch,    // scheme
        int8_t          hd,     // position of head
        int8_t          len,    // length
        int8_t          d,      // direction (1 or -1)
        uint8_t         s,      // speed (ignored for now)
        uint8_t         brght)  // brightness (ignored for now)
      : route(rte), scheme(sch), head(hd), length(len),
        dir(d), speed(s), brightness(brght)
    {
        if (scheme >= ncolor_scheme) {
            scheme = 0;
        }
    }

    // Number of color schemes
    static const unsigned       ncolor_scheme =
        sizeof(cat_color_scheme) / (3 * sizeof(uint32_t) );

    // Draw the catepillar at its current postion
    //
    // color determines the color scheme --
    // 1 draws the standard red head, blue body, green tail
    void
    draw()
    {
        addToPixelColor(head, cat_color_scheme[scheme][0]);
        for (int i = 1; i < length - 1; i++)  {
            addToPixelColor(head - (i * dir),
                           cat_color_scheme[scheme][1]);
        }
        addToPixelColor(head - (length - 1) * dir,
                       cat_color_scheme[scheme][2]);
    }

    // Move the catepillar and draw it at the new position
    void
    move()
    {
        // move one step in the direction
        head += dir;
        if (dir > 0) {
            if ((head - (length - 1) ) >= STRIPLENGTH) {
                head = 0;
            }
        } else {
            if (head + (length - 1) < 0) {
                head = STRIPLENGTH - 1;
            }
        }

        // draw the new position
        draw(0);
    }

    // Update the catepillar (change position, length, color, etc.)
    // Called once each "cycle"
    void
    update()
    {
        move();
    }
};

#define NCATEPILLAR     3
class catepillar *      cat_table[NCATEPILLAR];

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Create two catepillars
  // route 0, length 3, head 0, dir 1, speed 0, brightness 0
  cat_table[0] = new catepillar(0, 0, 6, 3, +1, 0, 0);
  cat_table[1] = new catepillar(0, 1, 3, 3, -1, 0, 0);
}

void loop() {
     // start by wiping all pixels to black
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0);
    }

    for (int i = 0; i < NCATEPILLAR; i++) {
        if (cat_table[i] != NULL) {
            cat_table[i]->update();
        }
    }
    strip.show();
    delay(100);
}

// Clear (make black) all pixels from start to stop, inclusive
void
clear(int start, int stop) {
  while (start <= stop) {
    strip.setPixelColor(start, 0, 0, 0);
    start++;
  }
  strip.show();
}
