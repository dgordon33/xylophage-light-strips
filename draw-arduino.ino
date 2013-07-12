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

#define NCAT 3

struct cat_state {
  uint8_t  strip;
  int8_t  length;
  int8_t   head;
  uint8_t  velocity;
  int8_t   dir;
  uint8_t  brightness;
};

struct cat_state cat_table[NCAT];



void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  
  struct cat_state *catp = &cat_table[0];
  catp->brightness=128;
  catp->head=0;
  catp->length = 4;
  catp->dir = 1;
    
  catp = &cat_table[1];
  catp->brightness=128;
  catp->head=7;
  catp->length = 4;
  catp->dir = 1;
    
}

void move_cat(struct cat_state * catp) {
  if (catp->dir > 0) { 
    clear(catp->head, catp->head + (catp->length-1));
  }
  else {
    clear(catp->head - (catp->length-1), catp->head);
  }
  catp->head += catp->dir;
  if (catp->dir > 0) {
    if ((catp->head - (catp->length-1)) >= STRIPLENGTH) {
      catp->head = 0;
    }
  } else {
    if (catp->head + (catp->length-1) < 0) {
      catp->head = STRIPLENGTH-1;
    }
  }
    
  drawcat(catp->head, catp->length, catp->dir);
  strip.show();
}

void loop() {
//  color(5, 0, 0, 255);
//  strip.show();
//  delay(1000);
//  color(3, 0, 0, 20);
//  strip.show();
//  delay(1000);
//  clear(0, 18);
//  delay(1000);
//  drawcat(3, 4, 1); 
//  strip.show();
//  for (int i=3; i<18; i++) {
//    drawcat(i, 4, 1);
//    strip.show();
//    delay(100);
//    clear(0, 17); 
//  }
//  for (int i=14; i>=0; i--) {
//    drawcat(i, 4, -1);
//    strip.show();
//    delay(100);
//    clear(0,17);
//  }
  move_cat(&cat_table[0]);
  move_cat(&cat_table[1]);
  delay(200);
}

//direction 1 = forward (increasing indices)
//direction -1 = reverse (decreasing indices)

void drawcat(int head, int length, int direct) {
  color(head, 255, 0, 0);
  for (int i = 1; i < length-1; i++)  {
    color(head-i*direct, 0, 255, 0);
  }
  color(head-(length-1)*direct, 0, 0, 255);
}
    
    

void color(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(pixel, strip.Color(r, g, b));
}

void clear(int start, int stop) {
  while (start <= stop) {
    color(start, 0,0,0);
    start++;
  }
  strip.show();
}
