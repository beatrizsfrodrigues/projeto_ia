#include <FastLED.h>

#define LED_PIN     8
#define LED_WIDTH   8
#define LED_HEIGHT  16
#define NUM_LEDS    (LED_WIDTH * LED_HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS  10

#define LED_PIN2     10

const int JOY_X = A1;
const int JOY_Y = A0;
const int JOY_X2 = A2;
const int JOY_Y2 = A3;
const int JOY_THRESHOLD = 400;  // deadzone threshold

unsigned long lastMoveTime1 = 0;
unsigned long lastMoveTime2 = 0;
const unsigned long moveDelay = 100; // milliseconds between moves





CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];



const int BTN_START = 9;

const int NUM_MAZES = 6;
int currentMaze1 = 0;
int currentMaze2 = 0;
// int currentMaze1 = 6;
// int currentMaze2 = 6;
bool gameOver = false;
bool gameStarted = false;





const byte mazes[NUM_MAZES][16][8] PROGMEM  = {
 { 
    {1,0,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,1},
    {1,0,0,0,1,1,0,1},
    {1,1,1,1,1,1,0,1},
    {0,0,0,0,0,0,0,1},
    {1,1,0,1,1,1,1,1},
    {0,0,0,1,0,0,0,1},
    {0,1,1,1,0,1,0,1}, 
    {0,0,0,0,0,1,0,1},
    {1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1},
    {1,0,0,0,0,1,1,1},
    {1,1,0,1,0,0,0,1},
    {0,0,0,1,0,1,0,1},
    {1,1,1,1,1,1,0,2}
  },
  {
    {1,1,1,1,2,1,0,1},
    {0,0,0,1,0,1,0,1},
    {1,1,0,0,0,1,0,0},
    {0,0,0,1,1,1,1,0},
    {0,1,1,1,0,0,0,0},
    {0,0,1,1,0,1,1,1},
    {1,0,1,0,0,0,0,0},
    {1,0,0,0,1,1,1,0}, 
    {1,1,1,1,1,1,1,0},
    {0,1,1,1,0,0,0,0},
    {0,0,0,1,0,1,0,1},
    {0,1,0,0,0,1,0,1},
    {0,1,1,1,1,1,1,1},
    {0,1,1,0,0,0,1,1},
    {0,0,1,0,1,0,0,0},
    {1,0,0,0,1,1,1,1}
  },
  {
    {1,1,1,1,1,0,1,1},
    {0,0,0,0,0,0,0,0},
    {0,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,0,0},
    {0,0,0,1,1,0,1,1},
    {0,1,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,0},
    {1,1,0,0,0,0,1,0},
    {0,1,1,1,1,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,1,1,1,1,0,1,0},
    {0,0,0,1,1,1,1,0},
    {1,1,0,1,0,0,0,0},
    {0,0,0,1,0,1,1,0},
    {0,1,1,1,0,0,1,0},
    {0,1,2,0,0,1,1,0}
  },
  {
    {1,1,1,0,1,1,1,1},
    {2,0,0,0,0,1,0,1},
    {1,1,1,1,0,0,0,1},
    {1,0,0,0,0,1,0,0},
    {1,0,1,1,1,1,1,0},
    {1,0,0,0,0,0,1,0},
    {1,1,1,1,1,0,1,0},
    {0,0,0,1,0,0,1,1},
    {1,1,0,1,0,1,1,1},
    {0,0,0,1,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,1,0,0,0},
    {1,0,1,0,1,0,1,1},
    {1,0,0,0,1,0,0,0},
    {1,0,1,0,1,1,1,0},
    {0,0,1,0,0,0,0,0}
  },
  {
    {0,0,0,1,1,1,1,1},
    {0,1,0,0,0,0,1,1},
    {0,1,1,1,1,0,1,1},
    {0,0,0,0,1,0,0,0},
    {1,1,0,1,1,1,1,1},
    {0,0,0,0,0,1,1,1},
    {0,1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1,0},
    {1,0,0,0,1,0,0,0},
    {1,0,1,0,1,0,1,1},
    {1,0,1,0,0,0,0,0},
    {1,0,1,1,1,1,1,0},
    {1,0,0,0,1,1,1,0},
    {1,1,1,0,0,0,1,0},
    {2,0,1,1,1,0,1,1},
    {1,0,0,0,0,0,0,0}
  },
  {
    {0,0,0,0,1,1,1,1},
    {1,1,1,0,0,0,0,0},
    {1,0,0,0,1,0,1,0},
    {1,0,1,1,1,0,1,0},
    {1,0,0,1,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {0,0,0,1,0,0,0,2},
    {0,1,1,1,1,1,1,1},
    {0,1,0,0,0,1,1,1},
    {0,0,0,1,0,0,0,0},
    {1,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,1,1,1,0,1,0,0},
    {0,0,1,0,0,1,0,1},
    {1,0,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,1}
  }
};

int startPositions[NUM_MAZES][2] = {
  {1, 0},  
  {7, 14},
  {0, 15},
  {0, 7}, 
  {7, 3}, 
  {1, 15}   
};

int playerX = startPositions[0][0];
int playerY = startPositions[0][1];
// int playerX = startPositions[5][0];
// int playerY = startPositions[5][1];
int playerX2 = startPositions[0][0];
int playerY2 = startPositions[0][1];




int XY(int x, int y) {
  return y * LED_WIDTH + x; // row-major layout
}




void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<CHIPSET, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);


  pinMode(BTN_START, INPUT_PULLUP);

}

void desenharLabirinto();



void loop() {
  
  if (!gameStarted) {
    if (digitalRead(BTN_START) == LOW) {
      gameStarted = true;
      gameOver = false;
      currentMaze1 = 0;
      currentMaze2 = 0;
      // currentMaze1 = 6;
      // currentMaze2 = 6;
      playerX = startPositions[0][0];
      playerY = startPositions[0][1];
      playerX2 = startPositions[0][0];
      playerY2 = startPositions[0][1];
      delay(300);
    }
    return;
  }

  if (!gameOver) {
  if (currentMaze1 < NUM_MAZES) moverJogador();
  if (currentMaze2 < NUM_MAZES) moverJogador2();

  desenharLabirinto();    
  desenharLabirinto2();   
  FastLED.show();

  if (currentMaze1 >= NUM_MAZES || currentMaze2 >= NUM_MAZES) {
    gameOver = true;

    // Display animations based on who won
    if (currentMaze1 >= NUM_MAZES) {
      Serial.println("🏁 Player 1 wins!");
      dualWinAnimation(leds, leds2);
    } else if (currentMaze2 >= NUM_MAZES) {
      Serial.println("🏁 Player 2 wins!");
      dualWinAnimation(leds2, leds);
    }



    // Clear LEDs after animation
    FastLED.clear();
    FastLED.show();

    gameStarted = false;
  }
} else {
    FastLED.clear();
    FastLED.show();
    delay(1000);
  }


  delay(200);
}




void moverJogador() {
  if (millis() - lastMoveTime1 < moveDelay) return; // wait before allowing next move

  int novoX = playerX;
  int novoY = playerY;

  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);

  bool moved = false;

  if (yVal < 512 - JOY_THRESHOLD) {
    novoY++;
    moved = true;
  } else if (yVal > 512 + JOY_THRESHOLD) {
    novoY--;
    moved = true;
  } else if (xVal < 512 - JOY_THRESHOLD) {
    novoX--;
    moved = true;
  } else if (xVal > 512 + JOY_THRESHOLD) {
    novoX++;
    moved = true;
  }

  if (moved) {
    lastMoveTime1 = millis();  // reset timer only on attempted move

    if (novoX >= 0 && novoX < LED_WIDTH && novoY >= 0 && novoY < LED_HEIGHT) {
      if (pgm_read_byte(&(mazes[currentMaze1][novoY][novoX])) != 1) {
        playerX = novoX;
        playerY = novoY;
      }
    }

    if (pgm_read_byte(&(mazes[currentMaze1][playerY][playerX])) == 2) {
      Serial.println("Player 1 reached the goal!");
      currentMaze1++;
      if (currentMaze1 < NUM_MAZES) {
        playerX = startPositions[currentMaze1][0];
        playerY = startPositions[currentMaze1][1];
      }
      delay(500);
    }
  }
}


void moverJogador2() {
  if (millis() - lastMoveTime2 < moveDelay) return;

  int novoX = playerX2;
  int novoY = playerY2;

  int xVal = analogRead(JOY_X2);
  int yVal = analogRead(JOY_Y2);

  bool moved = false;

  if (yVal < 512 - JOY_THRESHOLD) {
    novoY++;
    moved = true;
  } else if (yVal > 512 + JOY_THRESHOLD) {
    novoY--;
    moved = true;
  } else if (xVal < 512 - JOY_THRESHOLD) {
    novoX--;
    moved = true;
  } else if (xVal > 512 + JOY_THRESHOLD) {
    novoX++;
    moved = true;
  }

  if (moved) {
    lastMoveTime2 = millis();

    if (novoX >= 0 && novoX < LED_WIDTH && novoY >= 0 && novoY < LED_HEIGHT) {
      if (pgm_read_byte(&(mazes[currentMaze2][novoY][novoX])) != 1) {
        playerX2 = novoX;
        playerY2 = novoY;
      }
    }

    if (pgm_read_byte(&(mazes[currentMaze2][playerY2][playerX2])) == 2) {
      Serial.println("Player 2 reached the goal!");
      currentMaze2++;
      if (currentMaze2 < NUM_MAZES) {
        playerX2 = startPositions[currentMaze2][0];
        playerY2 = startPositions[currentMaze2][1];
      }
      delay(500);
    }
  }
}



void desenharLabirinto() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);  // Only clears leds[] for matrix 1


  // Draw maze 1 (Player 1)
  if (currentMaze1 < NUM_MAZES) {
    for (int x = 0; x < LED_WIDTH; x++) {
      for (int y = 0; y < LED_HEIGHT; y++) {
        byte tile = pgm_read_byte(&(mazes[currentMaze1][y][x]));


        if (tile == 1) leds[XY(x, y)] = CRGB::White; //cor das paredes
        else if (tile == 2) leds[XY(x, y)] = CRGB::Green; //cor da meta
      }
    }
    leds[XY(playerX, playerY)] = CRGB::Blue; //cor player 1
  }

}

void desenharLabirinto2() {
  fill_solid(leds2, NUM_LEDS, CRGB::Black);

  if (currentMaze2 < NUM_MAZES) {
    for (int x = 0; x < LED_WIDTH; x++) {
      for (int y = 0; y < LED_HEIGHT; y++) {
        byte tile = pgm_read_byte(&(mazes[currentMaze2][y][x]));


        if (tile == 1) leds2[XY(x, y)] = CRGB::White;
        else if (tile == 2) leds2[XY(x, y)] = CRGB::Green;
      }
    }
    leds2[XY(playerX2, playerY2)] = CRGB::Purple;
  }

}

void dualWinAnimation(CRGB* leds1, CRGB* leds2) {
  for (int t = 0; t < 60; t++) {  // Runs for ~3 seconds

    uint8_t baseHue = t * 8;

    // === Rainbow animation for leds1 ===
    fill_rainbow(leds1, NUM_LEDS, baseHue, 10);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (random8() < 40) {
        leds1[i] = CHSV(random8(), 255, 255);
      } else {
        leds1[i].fadeToBlackBy(random8(10, 40));
      }
    }

    // === Red flashing animation for leds2 ===
    CRGB flashColor = (t % 10 < 5) ? CRGB::Red : CRGB::Black;
    fill_solid(leds2, NUM_LEDS, flashColor);

    FastLED.show();
    delay(50);
  }
}


// void rainbowAnimation(CRGB* ledsArray) {
//   for (int t = 0; t < 60; t++) {  // ~3 seconds at 50ms/frame
//     uint8_t baseHue = t * 8;      // Rapid hue shifting

//     // Fill rainbow pattern
//     fill_rainbow(ledsArray, NUM_LEDS, baseHue, 10);

//     // Add sparkle/flicker effect
//     for (int i = 0; i < NUM_LEDS; i++) {
//       if (random8() < 40) { // ~15% chance to sparkle
//         ledsArray[i] = CHSV(random8(), 255, 255); // Random color sparkle
//       } else {
//         ledsArray[i].fadeToBlackBy(random8(10, 40)); // Random dimming for flicker
//       }
//     }

//     FastLED.show();
//     delay(50); // Faster for more excitement
//   }
// }



// void redFlashAnimation(CRGB* ledsArray) {
//   for (int t = 0; t < 10; t++) { // Flash red 10 times over 5 seconds
//     fill_solid(ledsArray, NUM_LEDS, t % 2 == 0 ? CRGB::Red : CRGB::Black);
//     FastLED.show();
//     delay(500);
//   }
// }

