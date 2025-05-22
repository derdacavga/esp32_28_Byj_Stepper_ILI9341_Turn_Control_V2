#include <TFT_eSPI.h>
#include <AccelStepper.h>

const int motorPin1 = 18;
const int motorPin2 = 8;
const int motorPin3 = 16;
const int motorPin4 = 17;

AccelStepper stepper(AccelStepper::FULL4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

TFT_eSPI tft = TFT_eSPI();

const int TURNS_TEXT_X = 80;
const int TURNS_TEXT_Y = 20;
const int TUR_INC_X = 20;
const int turnButtonY = 50;
const int TUR_DEC_X = 120;
const int SPEED_TEXT_X = 80;
const int SPEED_TEXT_Y = 110;
const int SPEED_INC_X = 20;
const int SPEED_DEC_X = 120;
const int SPEED_BUT_Y = 140;
const int startStopButtonX = 70;
const int startStopButtonY = 280;
const int buttonW = 80;
const int buttonH = 40;
const int maxMotorSpeed = 700;
const int minMotorSpeed = 10;
const int cwButtonX = 20;
const int dirButtonY = 220;
const int ccwButtonX = 120;

volatile int targetTurns = 1;
volatile int displayedSpeed = 20;
volatile bool motorRunning = false;
volatile bool motorDir = true;     
volatile long currentMotorTargetPosition = 0; 
volatile long totalStepsToMove = 0;
volatile long turnCounter = 0;
volatile long stepCounter = 0;

const long stepsPerTurn = 2048;

const unsigned long touchDelay = 100;
unsigned long lastTouchTime = 0;

void motorTask(void *parameter);
void uiTask(void *parameter);
void drawUI();
bool inArea(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void logo();
void drawStartStopButton();
void drawDirectionButtons();
void drawSpeedValue();
void updateCounterDisplay();

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(2);
  logo();
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  tft.calibrateTouch(nullptr, TFT_MAGENTA, TFT_GREEN, 15);

  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(50);
  stepper.setCurrentPosition(0);

  xTaskCreatePinnedToCore(
    motorTask,    // Task function
    "MotorTask",  // Task name
    4096,         // Stack size in bytes
    NULL,         // Task parameters
    2,            // Task priority (higher number means higher priority)
    NULL,         // Task handle (optional)
    0             // Core ID to run the task on (0 for Core 0)
  );

  xTaskCreatePinnedToCore(
    uiTask,    // Task function
    "UITask",  // Task name
    4096,      // Stack size in bytes
    NULL,      // Task parameters
    1,         // Task priority
    NULL,      // Task handle (optional)
    1          // Core ID to run the task on (1 for Core 1)
  );
}

void loop() {
  delay(10);
}

void motorTask(void *parameter) {
  for (;;) {
    if (motorRunning) {
      float actualMotorSpeed = map(displayedSpeed, 0, 100, minMotorSpeed, maxMotorSpeed);
      stepper.setMaxSpeed(actualMotorSpeed);
      stepper.setSpeed(motorDir ? actualMotorSpeed : -actualMotorSpeed); 

      stepper.run(); 

      if (stepper.distanceToGo() == 0) { 
        motorRunning = false;
        stepper.setCurrentPosition(0); 
      }

      long currentPosition = stepper.currentPosition();
      turnCounter = currentPosition / stepsPerTurn;
      stepCounter = abs(currentPosition % stepsPerTurn);

    } else {
      stepper.setSpeed(0);
    }
    vTaskDelay(1);
  }
}

void uiTask(void *parameter) {
  drawUI(); 
  for (;;) {
    uint16_t x, y;
    unsigned long currentTime = millis();

    if (tft.getTouch(&x, &y) && (currentTime - lastTouchTime >= touchDelay)) {
      lastTouchTime = currentTime;

      if (inArea(x, y, startStopButtonX, startStopButtonY, buttonW, buttonH)) {
        if (!motorRunning) { 
          totalStepsToMove = (long)targetTurns * stepsPerTurn;
          stepper.moveTo(motorDir ? totalStepsToMove : -totalStepsToMove);
          motorRunning = true; 
        } else { 
          motorRunning = false;
          stepper.stop(); 
          stepper.setCurrentPosition(0); 
        }
        drawStartStopButton(); 
      } else if (inArea(x, y, cwButtonX, dirButtonY, buttonW, buttonH)) {
        if (!motorRunning) { 
          motorDir = true;
          drawDirectionButtons();
        }
      } else if (inArea(x, y, ccwButtonX, dirButtonY, buttonW, buttonH)) {
        if (!motorRunning) { 
          motorDir = false;
          drawDirectionButtons();
        }
      } else if (inArea(x, y, SPEED_INC_X, SPEED_BUT_Y, buttonW, buttonH)) {
        if (displayedSpeed < 100) {
          displayedSpeed += 10;
          drawSpeedValue();
        }
      } else if (inArea(x, y, SPEED_DEC_X, SPEED_BUT_Y, buttonW, buttonH)) {
        if (displayedSpeed > 0) {
          displayedSpeed -= 10;
          drawSpeedValue();
        }
      } else if (inArea(x, y, TUR_INC_X, turnButtonY, buttonW, buttonH)) {
        targetTurns++;
        drawUI(); 
      } else if (inArea(x, y, TUR_DEC_X, turnButtonY, buttonW, buttonH) && targetTurns > 1) {
        targetTurns--;
        drawUI(); 
      }
    }
    updateCounterDisplay();
    vTaskDelay(10); 
  }
}

void drawUI() {
  tft.fillScreen(TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);

  tft.drawString("Turns: ", 10, 0);
  tft.drawString(" Step: ", 120, 0);

  tft.drawString("Target Turns:", 10, TURNS_TEXT_Y);
  tft.drawRect(TUR_INC_X, turnButtonY, buttonW, buttonH, TFT_BLUE);
  tft.drawString("+", TUR_INC_X + buttonW / 2 - 10, turnButtonY + buttonH / 2 - 10);
  tft.drawRect(TUR_DEC_X, turnButtonY, buttonW, buttonH, TFT_BLUE);
  tft.drawString("-", TUR_DEC_X + buttonW / 2 - 10, turnButtonY + buttonH / 2 - 10);
  tft.drawString(String(targetTurns), 145, TURNS_TEXT_Y);

  tft.drawString("Speed:", SPEED_TEXT_X - 40, SPEED_TEXT_Y);
  tft.drawRect(SPEED_INC_X, SPEED_BUT_Y, buttonW, buttonH, TFT_BLUE);
  tft.drawString("+", SPEED_INC_X + buttonW / 2 - 10, SPEED_BUT_Y + buttonH / 2 - 10);
  tft.drawRect(SPEED_DEC_X, SPEED_BUT_Y, buttonW, buttonH, TFT_BLUE);
  tft.drawString("-", SPEED_DEC_X + buttonW / 2 - 10, SPEED_BUT_Y + buttonH / 2 - 10);
  drawSpeedValue();
  tft.drawString("%", SPEED_TEXT_X + 85, SPEED_TEXT_Y);

  tft.drawString("Direction:", 50, 190);

  drawDirectionButtons();
  drawStartStopButton();
  updateCounterDisplay();
}

void drawDirectionButtons() {
  tft.fillRect(cwButtonX, dirButtonY, buttonW, buttonH, motorDir ? TFT_YELLOW : TFT_WHITE);
  tft.drawRect(cwButtonX, dirButtonY, buttonW, buttonH, TFT_BLUE);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("CW", cwButtonX + buttonW / 2 - 15, dirButtonY + buttonH / 2 - 5);

  tft.fillRect(ccwButtonX, dirButtonY, buttonW, buttonH, motorDir ? TFT_WHITE : TFT_YELLOW);
  tft.drawRect(ccwButtonX, dirButtonY, buttonW, buttonH, TFT_BLUE);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("CCW", ccwButtonX + buttonW / 2 - 20, dirButtonY + buttonH / 2 - 5);
}

void drawStartStopButton() {
  tft.fillRect(startStopButtonX, startStopButtonY, buttonW, buttonH, motorRunning ? TFT_RED : TFT_GREEN);
  tft.drawRect(startStopButtonX, startStopButtonY, buttonW, buttonH, TFT_BLUE);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString(motorRunning ? "STOP" : "START", startStopButtonX + buttonW / 2 - 28, startStopButtonY + buttonH / 2 - 7);
}

void drawSpeedValue() {
  tft.fillRect(SPEED_TEXT_X + buttonW / 2 - 15, SPEED_TEXT_Y, 40, 20, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString(String(displayedSpeed), SPEED_TEXT_X + buttonW / 2 - 15, SPEED_TEXT_Y);
}

void updateCounterDisplay() {
  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.fillRect(80, 0, 40, 20, TFT_GREEN);
  tft.drawString(String(turnCounter), 80, 0);
  tft.fillRect(190, 0, 50, 20, TFT_GREEN);
  tft.drawString(String(stepCounter), 190, 0);
}

bool inArea(uint16_t tx, uint16_t ty, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  return (tx >= x && tx <= (x + w) && ty >= y && ty <= (y + h));
}

void logo() {
  tft.fillScreen(TFT_GREEN);
  int centerX = 120;
  int centerY = 160;
  int radius = 70;
  int thickness = 7;

  tft.fillCircle(centerX, centerY, radius, TFT_RED);
  for (int r = radius - thickness / 2; r <= radius + thickness / 2; r++) {
    tft.drawCircle(centerX, centerY, r, TFT_BLACK);
  }
  tft.setTextColor(TFT_BLACK, TFT_RED);
  tft.setFreeFont(&FreeSansBold24pt7b);
  tft.drawString("DSN", 70, 142);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("Stepper motor Control", 20, 300);
}