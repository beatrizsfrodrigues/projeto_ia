#include <FastLED.h>

#define LED_PIN     8
#define LED_WIDTH   8
#define LED_HEIGHT  8
#define NUM_LEDS    (LED_WIDTH * LED_HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS  30

CRGB leds[NUM_LEDS];

const int BTN_UP = 13;
const int BTN_DOWN = 12;
const int BTN_LEFT = 11;
const int BTN_RIGHT = 10;

const int BTN_UP2 = 9;
const int BTN_DOWN2 = 7;
const int BTN_LEFT2 = 6;
const int BTN_RIGHT2 = 5;

const int BTN_START = A5;

const int NUM_MAZES = 6;
int currentMaze1 = 0;
int currentMaze2 = 0;
bool gameOver = false;

byte mazes[NUM_MAZES][16][8] = {
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



int XY(int x, int y) {
  return y * LED_WIDTH + x; // row-major layout
}




void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  pinMode(BTN_UP2, INPUT_PULLUP);
  pinMode(BTN_DOWN2, INPUT_PULLUP);
  pinMode(BTN_LEFT2, INPUT_PULLUP);
  pinMode(BTN_RIGHT2, INPUT_PULLUP);

  pinMode(BTN_START, INPUT_PULLUP);
}

void desenharLabirinto();


void loop() {
  if (!gameOver) {
    if (currentMaze1 < NUM_MAZES) {
      moverJogador();
    }
 
    desenharLabirinto();

    if (currentMaze1 >= NUM_MAZES) {
      Serial.println("🏁 Player 1 wins!");
      gameOver = true;
    } 
  
  } else {
    FastLED.clear();
    FastLED.show();
    delay(1000);
  }


delay(200);

}

void moverJogador() {
  int novoX = playerX;
  int novoY = playerY;

  if (digitalRead(BTN_UP) == LOW) {
  novoY--;
  Serial.println("UP");
}
if (digitalRead(BTN_DOWN) == LOW) {
  novoY++;
  Serial.println("DOWN");
}
if (digitalRead(BTN_LEFT) == LOW) {
  novoX--;
  Serial.println("LEFT");
}
if (digitalRead(BTN_RIGHT) == LOW) {
  novoX++;
  Serial.println("RIGHT");
}


  // Ensure the new position is within bounds
  if (novoX >= 0 && novoX < LED_WIDTH && novoY >= 0 && novoY < LED_HEIGHT) {
    if (mazes[currentMaze1][novoY][novoX] != 1) { // Check if it's not a wall (1)
      playerX = novoX;
      playerY = novoY;
    }
  }

  // Check if the player has reached the goal (2)
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



void desenharLabirinto() {
  FastLED.clear();

  // Draw maze 1 (Player 1)
  if (currentMaze1 < NUM_MAZES) {
    for (int x = 0; x < LED_WIDTH; x++) {
      for (int y = 0; y < LED_HEIGHT; y++) {
        byte tile = mazes[currentMaze1][y][x];

        if (tile == 1) leds[XY(x, y)] = CRGB::White; //cor das paredes
        else if (tile == 2) leds[XY(x, y)] = CRGB::Green; //cor da meta
      }
    }
    leds[XY(playerX, playerY)] = CRGB::Blue; //cor player 1
  }

  

  FastLED.show();
}
