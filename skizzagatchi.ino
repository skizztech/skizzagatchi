#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define W 64
#define H 32
Adafruit_SSD1306 display(W, H, &Wire, -1);

// ======================================================
// NORMAL 16×16
// ======================================================
const uint8_t frogNormal[] = {
  0x00,0x00,
  0x00,0x00,
  0x0C,0x30,
  0x1E,0x78,
  0x1E,0x78,
  0x0F,0xF0,
  0x08,0x10,
  0x14,0x50,
  0x10,0x10,
  0x3B,0xC8,
  0x10,0x10,
  0x08,0x30,
  0x07,0xF0,
  0x0E,0x10,
  0x00,0x00,
  0x00,0x00
};

// ======================================================
// BLINK
// ======================================================
const uint8_t frogBlink[] = {
  0x00,0x00,
  0x00,0x00,
  0x0C,0x30,
  0x1E,0x78,
  0x1E,0x78,
  0x0F,0xF0,
  0x00,0x00,
  0x14,0x50,
  0x00,0x00,
  0x3B,0xC8,
  0x10,0x10,
  0x08,0x30,
  0x07,0xF0,
  0x0E,0x10,
  0x00,0x00,
  0x00,0x00
};

// ======================================================
// SIDE SLEEP FROG
// ======================================================
const uint8_t frogSleep[] = {
  0x00,0x00,
  0x3C,0x00,
  0x7E,0x00,
  0xFF,0x00,
  0x81,0x80,
  0xA5,0x80,
  0x81,0x80,
  0x93,0xC0,
  0x81,0x80,
  0xC0,0x80,
  0x78,0x00,
  0x1C,0x00,
  0x00,0x00,
  0x00,0x00,
  0x00,0x00,
  0x00,0x00
};

// ======================================================
// PILLOW
// ======================================================
const uint8_t pillowBitmap[] = {
  0x3F,0xC0,
  0x21,0x40,
  0x21,0x40,
  0x21,0x40,
  0x21,0x40,
  0x3F,0xC0
};

// ======================================================
// BUBBLE
// ======================================================
const uint8_t bubbleBitmap[] = {
  0x1C,
  0x22,
  0x41,
  0x41,
  0x41,
  0x22,
  0x1C,
  0x00
};

// ======================================================
// Z
// ======================================================
const uint8_t zBitmap[] = {
  0x1F,
  0x02,
  0x04,
  0x08,
  0x10,
  0x1F,
  0x00
};

// ======================================================
// TIMERS
// ======================================================
unsigned long lastBlink = 0;
bool blinkState = false;

unsigned long lastSleepStart = 0;
bool isSleeping = false;

int bubbleY = 10;
unsigned long bubbleTimer = 0;

const unsigned long SLEEP_TIME = 180000;   // 3 minutes awake
const unsigned long SLEEP_DURATION = 30000; // 30 sec sleep
unsigned long awakeAccum = 0;
unsigned long lastUpdate = 0;

// ======================================================
// LEVEL & XP
// ======================================================
int level = 1;
unsigned long xpAccum = 0;
const unsigned long XP_PER_LEVEL = 600000; // 10 minutes

// ======================================================
// STAR EFFECT
// ======================================================
#define STAR_COUNT 6
int starX[STAR_COUNT];
int starY[STAR_COUNT];
int starLife[STAR_COUNT];

void spawnStar() {
  for (int i = 0; i < STAR_COUNT; i++) {
    if (starLife[i] <= 0) {
      starX[i] = random(0, 64);
      starY[i] = random(0, 10);
      starLife[i] = random(300, 900);
      return;
    }
  }
}

void updateStars(unsigned long dt) {
  if (random(0, 35) == 0) spawnStar();

  for (int i = 0; i < STAR_COUNT; i++) {
    if (starLife[i] > 0) {
      starLife[i] -= dt;
      if (starLife[i] > 0)
        display.drawPixel(starX[i], starY[i], SSD1306_WHITE);
    }
  }
}

// ======================================================
// SPARKLE EXPLOSION
// ======================================================
#define SPARKLE_COUNT 40
int spX[SPARKLE_COUNT];
int spY[SPARKLE_COUNT];
int spVX[SPARKLE_COUNT];
int spVY[SPARKLE_COUNT];
int spLife[SPARKLE_COUNT];
bool doSparkles = false;

void startSparkleExplosion() {
  doSparkles = true;
  for (int i = 0; i < SPARKLE_COUNT; i++) {
    spX[i] = 32;
    spY[i] = 16;
    spVX[i] = random(-2, 3);
    spVY[i] = random(-2, 3);
    spLife[i] = random(150, 400);
  }
}

void updateSparkles(unsigned long dt) {
  if (!doSparkles) return;

  bool anyAlive = false;

  for (int i = 0; i < SPARKLE_COUNT; i++) {
    if (spLife[i] > 0) {
      anyAlive = true;
      spLife[i] -= dt;
      spX[i] += spVX[i];
      spY[i] += spVY[i];
      if (spLife[i] > 0)
        display.drawPixel(spX[i], spY[i], SSD1306_WHITE);
    }
  }

  if (!anyAlive)
    doSparkles = false;
}

// ======================================================
// DRAW AWAKE
// ======================================================
void drawAwake() {
  if (millis() - lastBlink > 2500) {
    blinkState = true;
    lastBlink = millis();
  }

  if (blinkState && millis() - lastBlink > 150) {
    blinkState = false;
  }

  const uint8_t* s = blinkState ? frogBlink : frogNormal;
  display.drawBitmap(24, 8, s, 16, 16, SSD1306_WHITE);
}

// ======================================================
// DRAW SLEEPING
// ======================================================
void drawSleeping() {
  display.drawBitmap(20, 14, pillowBitmap, 12, 6, SSD1306_WHITE);
  display.drawBitmap(26, 8, frogSleep, 16, 16, SSD1306_WHITE);

  display.drawBitmap(45, bubbleY, bubbleBitmap, 8, 8, SSD1306_WHITE);

  display.drawBitmap(55, bubbleY - 2,  zBitmap, 5, 7, SSD1306_WHITE);
  display.drawBitmap(55, bubbleY - 10, zBitmap, 5, 7, SSD1306_WHITE);
  display.drawBitmap(55, bubbleY - 18, zBitmap, 5, 7, SSD1306_WHITE);

  if (millis() - bubbleTimer > 120) {
    bubbleY--;
    bubbleTimer = millis();
  }
  if (bubbleY < -20)
    bubbleY = 10;
}

// ======================================================
// LEVEL BAR
// ======================================================
void drawLevelBar() {
  float p = (float)xpAccum / XP_PER_LEVEL;
  if (p > 1) p = 1;

  int width = p * 62;

  display.drawRect(0, 29, 64, 3, SSD1306_WHITE);
  display.fillRect(1, 30, width, 1, SSD1306_WHITE);
}

// ======================================================
// SETUP (the missing part)
// ======================================================
void setup() {
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  randomSeed(analogRead(0));

  lastUpdate = millis();
}

// ======================================================
// LOOP
// ======================================================
void loop() {
  unsigned long now = millis();
  unsigned long dt = now - lastUpdate;
  lastUpdate = now;

  if (!isSleeping)
    xpAccum += dt;

  if (xpAccum >= XP_PER_LEVEL) {
    xpAccum = 0;
    level++;
    startSparkleExplosion();
  }

  if (!isSleeping)
    awakeAccum += dt;

  if (!isSleeping && awakeAccum >= SLEEP_TIME) {
    isSleeping = true;
    bubbleY = 10;
    lastSleepStart = now;
  }

  if (isSleeping && now - lastSleepStart > SLEEP_DURATION) {
    isSleeping = false;
    awakeAccum = 0;
    for (int i = 0; i < STAR_COUNT; i++) starLife[i] = 0;
  }

  display.clearDisplay();

  if (isSleeping) {
    drawSleeping();
    updateStars(dt);
  } else {
    drawAwake();
  }

  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.print("LV:");
  display.print(level);

  drawLevelBar();
  updateSparkles(dt);

  display.display();
  delay(30);
}
