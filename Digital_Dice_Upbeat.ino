#include <Wire.h>
#include <LCD_I2C.h> 

LCD_I2C lcd(0x27, 16, 2); 

// Pin Definitions
const int joystickX = A0; // Joystick horizontal (X-axis)
const int triggerButton = 3; // Trigger button

// Variables
int currentProgram = 0; // 0 = Dice, 1 = Coin Toss, 2 = Stopwatch, 3 = Time & Date
const int totalPrograms = 4;

unsigned long lastJoystickTime = 0;
unsigned long lastTriggerTime = 0;
const unsigned long debounceDelay = 200;

unsigned long stopwatchStart = 0;
bool stopwatchRunning = false; // Tracks whether stopwatch is running

String dateTime = "No Data"; // Placeholder for received date/time

void setup() {
  lcd.begin();        // Initialize LCD
  lcd.backlight();    // Turn on backlight

  pinMode(joystickX, INPUT);
  pinMode(triggerButton, INPUT_PULLUP);

  Serial.begin(9600);

  lcd.clear();
  lcd.print("Dice: Roll it!"); // Start with Dice program
}

void loop() {
  unsigned long currentTime = millis();

  // Handle Joystick Movement (Program Navigation)
  int joystickValue = analogRead(joystickX);
  if (currentTime - lastJoystickTime > debounceDelay) {
    if (joystickValue < 400) { // Joystick moved left (backward program)
      lastJoystickTime = currentTime;
      previousProgram();
    } else if (joystickValue > 600) { // Joystick moved right (forward program)
      lastJoystickTime = currentTime;
      nextProgram();
    }
  }

  // Handle Trigger Button
  if (digitalRead(triggerButton) == LOW && (currentTime - lastTriggerTime > debounceDelay)) {
    lastTriggerTime = currentTime;
    triggerProgram();
  }

  // Continuously update stopwatch display if running
  if (stopwatchRunning && currentProgram == 2) {
    updateStopwatchDisplay();
  }

  // Read serial input for date and time
  if (Serial.available()) {
    dateTime = Serial.readStringUntil('\n');
  }
}

void nextProgram() {
  currentProgram = (currentProgram + 1) % totalPrograms; // Go to next program
  updateLCD();
}

void previousProgram() {
  currentProgram = (currentProgram - 1 + totalPrograms) % totalPrograms; // Go to previous program
  updateLCD();
}

void updateLCD() {
  lcd.clear();
  switch (currentProgram) {
    case 0:
      lcd.print("Dice: Roll it!");
      break;
    case 1:
      lcd.print("Coin Toss");
      break;
    case 2:
      lcd.print("Stopwatch");
      break;
    case 3:
      lcd.print("Time & Date");
      break;
  }
}

void triggerProgram() {
  lcd.clear();
  switch (currentProgram) {
    case 0:
      digitalDice();
      break;
    case 1:
      coinToss();
      break;
    case 2:
      toggleStopwatch();
      break;
    case 3:
      displayTimeAndDate();
      break;
  }
}

void digitalDice() {
  lcd.print("Rolling Dice...");
  delay(1000);
  int dice = random(1, 7);
  lcd.clear();
  lcd.print("Dice: ");
  lcd.print(dice);
  delay(2000);
  updateLCD();
}

void coinToss() {
  lcd.print("Tossing Coin...");
  delay(1000);
  int toss = random(0, 2);
  lcd.clear();
  lcd.print("Result: ");
  lcd.print(toss == 0 ? "Heads" : "Tails");
  delay(2000);
  updateLCD();
}

void toggleStopwatch() {
  if (!stopwatchRunning) {
    // Start the stopwatch
    stopwatchStart = millis();
    stopwatchRunning = true;
    lcd.print("Stopwatch Started");
    delay(1000);
  } else {
    // Stop the stopwatch and display elapsed time
    stopwatchRunning = false;
    unsigned long elapsedTime = millis() - stopwatchStart;
    lcd.clear();
    lcd.print("Elapsed Time:");
    lcd.setCursor(0, 1);
    lcd.print(elapsedTime / 1000);
    lcd.print("s");
    delay(3000);

    // Reset stopwatch
    stopwatchStart = 0;
    lcd.clear();
    lcd.print("Stopwatch Reset");
    delay(2000);
  }
  updateLCD();
}

void updateStopwatchDisplay() {
  lcd.clear();
  unsigned long elapsedTime = millis() - stopwatchStart;
  lcd.print("Time: ");
  lcd.print(elapsedTime / 1000);
  lcd.print("s");
  delay(200);
}

void displayTimeAndDate() {
  lcd.print("Time & Date:");
  lcd.setCursor(0, 1);
  lcd.print(dateTime);
  delay(3000);
  updateLCD();
}
