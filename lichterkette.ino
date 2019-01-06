#include <FastLED.h>
#define NUM_LEDS 100 // number of LEDS on the strip
#define DATA_PIN 3  // connection to the strip
#define MIC_PIN A3  // input for the microphone

// init led array
CRGB leds[NUM_LEDS + 1];

// SETUP

void setup() {
  // set mic pin to input
  pinMode(MIC_PIN, INPUT);
  // init serial line for debugging
  Serial.begin(115200);
  // set WS2811 led strip
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}


// HELPER FUNCTIONS

// all LEDS same color
void colorAll(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = color;
  FastLED.show();
}

// dims a color to percent
CRGB colorPercent(CRGB color, int percent) {
  if (percent > 100) percent = 100;
  if (percent < 0) percent = 0;
  return (CRGB(color.r * percent / 100, color.g * percent / 100, color.b * percent / 100));
}

// random color
CRGB randomColor() {
  return (CRGB(random8(), random8(), random8()));
}

// PATTERNS

// Let snow flake fill the strip.
void snowFlake(int wait_ms = 50) {
  colorAll(CRGB::Black);

  for (int pos = NUM_LEDS; pos >= 0; pos--) {
    for (int i = 0; i < pos; i++) {
      leds[i - 1] = CRGB::Black;
      leds[i] = CRGB::White;
      FastLED.show();
      delay(wait_ms / (NUM_LEDS / 50));
    }
  }
}

// random led colors
void randomFill(int iterations = 200, int wait_ms = 50) {
  while (iterations) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = randomColor();
    }
    FastLED.show();
    delay(wait_ms);
    iterations--;
  }
}

// random pixels
void randomPixel(int iterations = 5, int wait_ms = 500) {
  colorAll(CRGB::Black);

  // let leds randomly glow
  while (iterations) {
    colorAll(CRGB::Black);
    for (int k = 0; k < NUM_LEDS / 5; k++) {
      leds[random8(NUM_LEDS)] = randomColor();
      FastLED.show();
      delay(wait_ms);
    }
    iterations--;
  }
}

// strobe light
void strobe(int iterations = 10, int flash = 50) {
  // strobe lights
  while (iterations) {
    CRGB color = randomColor();
    for (int j = 0; j < 10; j++) {
      colorAll(color);
      delay(flash);
      colorAll(CRGB::Black);
      delay(flash);
    }
    delay(1000);
    iterations--;
  }
}

// fades a color in and out
void fadeInOut(int iterations = 5, int wait_ms = 10) {
  while (iterations) {
    CRGB color = randomColor();
    for (int percent = 0; percent <= 100; percent++) {
      colorAll(colorPercent(color, percent));
      delay(wait_ms);
    }
    for (int percent = 100; percent >= 0; percent--) {
      colorAll(colorPercent(color, percent));
      delay(wait_ms);
    }
    iterations--;
  }
}

// Generate rainbow colors across 0-255 positions.
CRGB wheelColor(int pos) {
  if (pos < 85) return (CRGB(pos * 3, 255 - pos * 3, 0));
  if (pos < 170) {
    pos -= 85;
    return (CRGB(255 - pos * 3, 0, pos * 3));
  }
  pos -= 170;
  return (CRGB(0, pos * 3, 255 - pos * 3));
}

// rainbow colors
void rainbow(int iterations = 3, int wait_ms = 20) {
  while (iterations) {
    for (int j = 0; j < 256; j++) {
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = wheelColor((i + j) & 255);
      FastLED.show();
      delay(wait_ms);
    }
    iterations --;
  }
}

// knight rider animation
void knightRider(int iterations = 5, int wait_ms = 50) {
  colorAll(CRGB::Black);

  while (iterations) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
      for (int p = 1; p <= 5; p++) {
        if (i - p < 0) continue;
        leds[i - p] = leds[i].nscale8(250 - (p * 50));
      }
      FastLED.show();
      delay(wait_ms);
    }
    for (int i = NUM_LEDS; i >= 0; i--) {
      leds[i] = CRGB::Red;
      for (int p = 1; p <= 5; p++) {
        if (i + p > NUM_LEDS) continue;
        leds[i + p] = leds[i].nscale8(250 - (p * 50));
      }
      FastLED.show();
      delay(wait_ms);
    }
    iterations--;
  }
}

// dips on the stripe
void dip(int iterations = 10, int wait_ms = 50) {
  colorAll(CRGB::Black);

  while (iterations) {
    int pos = random8(NUM_LEDS);
    CRGB color = randomColor();
    for (int i = 0; i <= 10; i++) {
      for (int j = 0; j < i; j++) {
        if ((i + j) * 10 <= 100) {
          leds[pos + j] = colorPercent(color, 100 - (i + j) * 10);
          leds[pos - j] = colorPercent(color, 100 - (i + j) * 10);
          FastLED.show();
          delay(wait_ms);
        }
      }
    }
    iterations--;
  }
}

// moves randomly around and stays
void randomStay(int iterations = 10, int wait_ms = 200) {
  colorAll(CRGB::Black);

  int oldpos = 0;
  while (iterations) {
    CRGB color = randomColor();
    int pos = random(NUM_LEDS);
    if (pos > oldpos) {
      for (int i = oldpos + 1; i < pos + 1; i++) {
        leds[i - 1] = CRGB::Black;
        leds[i] = color;
        FastLED.show();
        delay(wait_ms * abs(pos - i) / NUM_LEDS);
      }
    } else {
      for (int i = oldpos - 1; i > pos - 1; i--) {
        leds[i + 1] = CRGB::Black;
        leds[i] = color;
        FastLED.show();
        delay(wait_ms * abs(pos - i) / NUM_LEDS);
      }
    }
    oldpos = pos;
    delay(1000);
    iterations--;
  }
}

// simulates fireflys
void fireflys(int iterations = 200, int wait_ms = 100, int fly_count = 5) {
  colorAll(CRGB::Black);

  int flys[fly_count];
  for (int i = 0; i < fly_count; i++) flys[i] = random8(NUM_LEDS);

  while (iterations) {
    for (int f = 0; f < fly_count; f++) {
      int direction = random8(3);
      int pos = flys[f];
      if (direction < 1 && pos > 0) {
        leds[pos] = CRGB::Black;
        pos--;
        leds[pos] = CRGB::Yellow;
      } else if (direction > 1 && pos < NUM_LEDS) {
        leds[pos] = CRGB::Black;
        pos++;
        leds[pos] = CRGB::Yellow;
      }
      flys[f] = pos;
    }
    FastLED.show();
    delay(wait_ms);

    iterations--;
  }

}

// Simulates TV turning off
void tvOff(int iterations = 10, int wait_ms = 10) {
  while (iterations) {
    CRGB color = randomColor();
    colorAll(color);
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      leds[i] = CRGB::Black;
      leds[NUM_LEDS - i] = CRGB::Black;
      FastLED.show();
      delay(wait_ms);
    }
    delay(100);
    for (int percent = 100; percent >= 0; percent--) {
      leds[NUM_LEDS / 2 - 1] = colorPercent(color, percent);
      leds[NUM_LEDS / 2] = colorPercent(color, percent);
      leds[NUM_LEDS / 2 + 1] = colorPercent(color, percent);
      leds[NUM_LEDS / 2 + 2] = colorPercent(color, percent);
      FastLED.show();
      delay(wait_ms);
    }
    delay(2000);
    iterations--;
  }
}

// simulates rocket start
void rocket(int iterations = 5) {
  while (iterations) {
    colorAll(CRGB::Black);
    CRGB color = randomColor();
    // fired on
    for (int i = 0; i < 5; i++) {
      leds[i] = CRGB::Yellow;
      if (i > 0) leds[i - 1] = CRGB::Red;
      if (i > 1) leds[i - 2] = CRGB::Black;
      FastLED.show();
      delay(500);
    }
    // start
    for (int i = 5; i < NUM_LEDS; i++) {
      leds[i] = color;
      if (i > 9) leds[i - 10] = CRGB::Black;
      FastLED.show();
      delay(5);
    }
    // explodes
    for (int i = 0; i < 20; i++) {
      for (int j = NUM_LEDS - i; j > NUM_LEDS - i - 10; j--) {
        leds[j] = colorPercent(randomColor(), 100 - (i * 5));
        FastLED.show();
        delay(20 - i);
      }
      leds[NUM_LEDS - i] = CRGB::Black;
    }
    iterations--;
  }
}

// pixel hopping from left to right
void pixelLeftRight(int iterations = 20) {
  while (iterations) {
    colorAll(CRGB::Black);

    CRGB color = randomColor();

    // first direction
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
      leds[i - 1] = CRGB::Black;
      FastLED.show();
      delay(5);
    }
    // second direction
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
      leds[i] = color;
      leds[i + 1] = CRGB::Black;
      FastLED.show();
      delay(5);
    }
    iterations--;
  }
}

// pixel hopping from left to right until mid is reached
void pixelToMid(int iterations = 5) {
  while (iterations) {
    colorAll(CRGB::Black);

    CRGB color = randomColor();

    for (int x = 0; x < NUM_LEDS / 2; x++) {

      // first direction
      if (x % 2 == 0) {
        for (int i = x; i < NUM_LEDS - x; i++) {
          leds[i] = color;
          leds[i - 1] = CRGB::Black;
          FastLED.show();
          delay(5);
        }
      }

      if (x % 2 == 1) {
        // second direction
        for (int i = NUM_LEDS - x - 1; i >= x; i--) {
          leds[i] = color;
          leds[i + 1] = CRGB::Black;
          FastLED.show();
          delay(5);
        }
      }
    }

    iterations--;
  }
}

// real snow
void realSnow(int wait_ms = 50, int iterations = 3) {
  colorAll(CRGB::Black);

  // let leds randomly glow
  while (iterations) {
    colorAll(CRGB::Black);

    for (int k = 0; k < 500; k++) {
      // get next position
      int pos = random8(NUM_LEDS);

      // get color of this pixel
      CRGB color = leds[pos];

      // increase white value
      if (color.red < 255) color.red += 1;
      if (color.blue < 255) color.blue += 1;
      if (color.green < 255) color.green += 1;

      // set pixel
      leds[pos] = color;

      FastLED.show();
      delay(wait_ms);
    }
    iterations--;
  }
}

// Let snow flake fill the strip with RGB white
void snowFlakeFill(int wait_ms = 1) {
  colorAll(CRGB::Black);

  CRGB lowWhite;
  lowWhite.setRGB(1, 1, 1);

  int x = 0;
  while (x < NUM_LEDS) {
    for (int i = NUM_LEDS - 1; i >= x; i--) {

      // final pos reached?
      if (i == x) {
        leds[i].red += 10;
        leds[i].blue += 10;
        leds[i].green += 10;
        CRGB color = leds[i];
        if (color.blue == 50) {
          x += 2;
        }
      }

      // normal pos
      else {
        leds[i] = lowWhite;
      }

      leds[i + 1] = CRGB::Black;

      FastLED.show();
      delay(wait_ms);
    }
  }
}

// snake game
void snake(int wait_ms = 50, int iterations = 1000) {
  colorAll(CRGB::Black);

  int head = 0;
  int len = 1;
  int dir = 1;
  int fruit = -1;

  while (iterations > 0) {
    // paint old values black
    // head
    leds[head] = CRGB::Black;
    // tail
    int tail;
    for (int pos = 1; pos <= len; pos++) {
      tail = head - (pos * dir);
      
      if (tail < 0 || tail >= NUM_LEDS) break;
      leds[tail] = CRGB::Black;
    }

    // spawn fruit if edge is reached
    if (head == 0 || head == NUM_LEDS-1) {
      fruit = random8(NUM_LEDS-10) + 5;
      leds[fruit] = CRGB::Yellow;
    }

    // change values
    head = head + dir;

    // check if fruit was eaten
    if (head == fruit) len++;

    // calc direction
    if (dir == 1 && head == NUM_LEDS-1) dir = -1;
    if (dir == -1 && head == 0) dir = 1;

    // paint head
    leds[head] = CRGB(0, 0, 255);
    // paint tail
    for (int pos = 1; pos <= len; pos++) {
      tail = head - (pos * dir);

      if (tail < 0 || tail >= NUM_LEDS) break;
      leds[tail] = CRGB(0, 0, 10);
    }

    FastLED.show();
    delay(wait_ms);

    iterations--;
  }
}

// sound meter
void sound2light(int runtime = 20000) {
  int start = millis();
  int oldPos = 0;
  int maxval = 0;
  while (millis() - start < runtime) {

    int lastval = 1024;
    for (int runs = 0; runs <= 100; runs++) {
      int val = analogRead(MIC_PIN);
      if (val < lastval) lastval = val;
    }

    if (lastval > maxval) maxval = lastval;
    int pos = map(lastval, maxval, 0, 0, NUM_LEDS + 3);

    if (oldPos > pos) pos = oldPos - 1;

    // First, clear the existing led values
    FastLED.clear();
    for (int led = 0; led < pos; led++) {
      leds[led] = CRGB::Blue;
    }
    FastLED.show();
    oldPos = pos;
  }
}

// sound lets pixels walk the strip
void sound2pixel() {
  while (true) {
    int lastval = 1024;
    for (int runs = 0; runs <= 100; runs++) {
      int val = analogRead(MIC_PIN);
      if (val < lastval) lastval = val;
    }

    for (int i = NUM_LEDS - 1; i > 0; i--) {
      leds[i] = leds[i - 1];
    }

    if (lastval < 512) leds[0] = CRGB::Blue;
    else leds[0] = CRGB::Black;
    FastLED.show();
  }
}

// MAIN
void loop() {
  // music mode
  sound2pixel();

/*
  // christmas mode
  snake();
  snowFlakeFill();
  realSnow();
  pixelToMid();
  rocket();
  tvOff();
  fireflys();
  randomStay();
  dip();
  pixelLeftRight();
  knightRider();
  rainbow();
  fadeInOut();
  strobe();
  randomPixel();
  randomFill();
  snowFlake();

  // test mode
  // sound2light();

*/
}

