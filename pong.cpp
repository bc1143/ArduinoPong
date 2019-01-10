#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


void setup() {
    init();
    Serial.begin(9600);
    tft.begin();
    pinMode(2, INPUT);
    digitalWrite(2, HIGH);
}

void startScreen(int x, int y) {
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRect(x, y, 10, 80, ILI9341_WHITE);
  tft.fillRect(310, 0, 10, 240, ILI9341_WHITE);
  tft.fillRect(260, 118, 5, 5, ILI9341_WHITE);
}

void drawPrompt() {
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(2);
  tft.println(" Welcome to PONG!");
  tft.println("Click the Joystick");
  tft.println("     To Begin     ");
}

void moveRect(int x, int y, int direction) {
  // drawRect(x, y, width, height, color)
  switch (direction) {
    case 1: // Move Down
      tft.fillRect(x, y + 79, 10, 1, ILI9341_WHITE); // Add in front (right)
      tft.fillRect(x, y - 1, 10, 1, ILI9341_BLACK); // Erase behind
      break;
    case 2: // Move Up
      tft.fillRect(x, y, 10, 1, ILI9341_WHITE);
      tft.fillRect(x, y + 80, 10, 1, ILI9341_BLACK);
      break;
  }
}

struct game_ball {
    int x_pos;
    int y_pos;
    int v_x;
    int v_y;
    int count;
};

struct game_ball moveBall(struct game_ball ball, int y) {
  ball.x_pos += ball.v_x;
  if (ball.x_pos >= 304) {
    ball.v_x = - ball.v_x;
  } else if (ball.x_pos <= 31 && (ball.y_pos >= (y - 5) && ball.y_pos <= (y +80))) {
    ball.v_x = - ball.v_x;
    ball.count += 1;
    //ball.v_x += 1;
    tft.fillRect(0, 0, 19, 19, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.println(ball.count);
  }
  if (ball.x_pos <= -6) {
    tft.setCursor(80, 100);
    tft.setTextSize(3);
    tft.println("GAME OVER");
    exit(0);
    //tft.println("Score: ");
  }
  tft.fillRect(ball.x_pos - ball.v_x, ball.y_pos, 5, 5, ILI9341_BLACK);
  tft.fillRect(ball.x_pos, ball.y_pos, 5, 5, ILI9341_WHITE);
  return ball;
}

int main() {
  setup();
  //testFillScreen();
  //draw_pixel();
  //print_string();
  int x = 20;
  int y = 80;
  tft.setRotation(3);
  startScreen(x, y);
  drawPrompt();
  
  struct game_ball ball;
  ball.x_pos = 260; 
  ball.y_pos = 118;
  ball.v_x = -1;
  ball.v_y = 0;
  ball.count = 0;
  
  
  while (digitalRead(2) == 1) {
    delay(8);
  }

  tft.fillRect(0, 0, 260, 60, ILI9341_BLACK);
  int direction = 0;

  while(true) {
    direction = 0;
    ball = moveBall(ball, y);
    
    if ((analogRead(1) > 700  || direction == 1) && (y + 81) <= 240) {
      direction = 1; // Move Down
      y += 1;
    } else if ((analogRead(1) < 300 || direction == 2) && (y - 1) >= 0) {
      direction = 2; // Move Up
      y -= 1;
    }
    moveRect(x, y, direction);
    delay(8);
  }
  Serial.end();

  return 0;
}