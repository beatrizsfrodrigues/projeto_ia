#include <FastLED.h>

#define LED_PIN     8
#define LED_WIDTH   8
#define LED_HEIGHT  16
#define NUM_LEDS    (LED_WIDTH * LED_HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS  30

#define LED_PIN2     10

const int JOY_X = A0;
const int JOY_Y = A1;
const int JOY_X2 = A2;
const int JOY_Y2 = A3;
const int JOY_THRESHOLD = 400;  // deadzone threshold

const int segmentPins[7] = {1,2,3,4,5,6,7}; // a to g


CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];



const int BTN_START = 9;

const int NUM_MAZES = 6;
int currentMaze1 = 0;
int currentMaze2 = 0;
bool gameOver = false;
bool gameStarted = false;

// Segment order: a, b, c, d, e, f, g
const byte digits[7][7] PROGMEM = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
};



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

  for (int i = 0; i < 7; i++) {
  pinMode(segmentPins[i], OUTPUT);
}

}

void desenharLabirinto();

void displayDigit(int digit) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[digit][i]);
  }
}



void loop() {
  
  if (!gameStarted) {
    if (digitalRead(BTN_START) == LOW) {
      gameStarted = true;
      gameOver = false;
      currentMaze1 = 0;
      currentMaze2 = 0;
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

    desenharLabirinto();    // Player 1
    desenharLabirinto2();   // Player 2

    FastLED.show(); 

    // Check for win
    if (currentMaze1 >= NUM_MAZES) {
      Serial.println("🏁 Player 1 wins!");
      gameOver = true;
    } else if (currentMaze2 >= NUM_MAZES) {
      Serial.println("🏁 Player 2 wins!");
      gameOver = true;
    }
  } else {
    FastLED.clear();
    FastLED.show();
    delay(1000);
  }

  displayDigit(NUM_MAZES - max(currentMaze1, currentMaze2));
  delay(200);
}




void moverJogador() {
  int novoX = playerX;
  int novoY = playerY;

  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);

  // Prioritize vertical movement
  if (yVal < 512 - JOY_THRESHOLD) {
    novoY--;  // up
    Serial.println("UP");
  } else if (yVal > 512 + JOY_THRESHOLD) {
    novoY++;  // down
    Serial.println("DOWN");
  } else if (xVal < 512 - JOY_THRESHOLD) {
    novoX--;  // left
    Serial.println("LEFT");
  } else if (xVal > 512 + JOY_THRESHOLD) {
    novoX++;  // right
    Serial.println("RIGHT");
  }

  // Ensure the new position is within bounds
  if (novoX >= 0 && novoX < LED_WIDTH && novoY >= 0 && novoY < LED_HEIGHT) {
    if (mazes[currentMaze1][novoY][novoX] != 1) {
      playerX = novoX;
      playerY = novoY;
    }
  }

  // Check if player reached the goal
  if (mazes[currentMaze1][playerY][playerX] == 2) {
    Serial.println("Player 1 reached the goal!");
    currentMaze1++;
    if (currentMaze1 < NUM_MAZES) {
      playerX = startPositions[currentMaze1][0];
      playerY = startPositions[currentMaze1][1];
    }
    delay(500);
  }
}

void moverJogador2() {
  int novoX = playerX2;
  int novoY = playerY2;

  int xVal = analogRead(JOY_X2);
  int yVal = analogRead(JOY_Y2);

  if (yVal < 512 - JOY_THRESHOLD) novoY--; // up
  else if (yVal > 512 + JOY_THRESHOLD) novoY++; // down
  else if (xVal < 512 - JOY_THRESHOLD) novoX--; // left
  else if (xVal > 512 + JOY_THRESHOLD) novoX++; // right

  if (novoX >= 0 && novoX < LED_WIDTH && novoY >= 0 && novoY < LED_HEIGHT) {
    if (mazes[currentMaze2][novoY][novoX] != 1) {
      playerX2 = novoX;
      playerY2 = novoY;
    }
  }

  if (mazes[currentMaze2][playerY2][playerX2] == 2) {
    Serial.println("Player 2 reached the goal!");
    currentMaze2++;
    if (currentMaze2 < NUM_MAZES) {
      playerX2 = startPositions[currentMaze2][0];
      playerY2 = startPositions[currentMaze2][1];
    }
    delay(500);
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
