#include <Wire.h>
#include <LCD_I2C.h> 

// Initialize I2C LCD (address, columns, rows)
LCD_I2C lcd(0x27, 16, 2); 

// Button Pins
const int button1 = 2; // Navigation Button
const int button2 = 3; // Trigger Button

// Variables
int menuIndex = 0;
const char* menuItems[] = {"Digital Dice", "Coin Toss", "Stopwatch", "Time & Date"};
const int totalItems = 4;

unsigned long stopwatchStart = 0;
bool stopwatchRunning = false;

// Debounce settings
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

String dateTime = "No Data"; // Placeholder for received date/time

void setup() {
  lcd.begin();        // Initialize LCD
  lcd.backlight();    // Turn on backlight

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  Serial.begin(9600);
  lcd.print("Project Ready!");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read buttons with debouncing
  if (digitalRead(button1) == LOW && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    menuIndex = (menuIndex + 1) % totalItems;
    displayMenu();
  }

  if (digitalRead(button2) == LOW && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    triggerAction();
  }

  // Read serial input for date and time
  if (Serial.available()) {
    dateTime = Serial.readStringUntil('\n');
  }
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu:");
  lcd.setCursor(0, 1);
  lcd.print(menuItems[menuIndex]);
}

void triggerAction() {
  lcd.clear();
  switch (menuIndex) {
    case 0:
      digitalDice();
      break;
    case 1:
      coinToss();
      break;
    case 2:
      stopwatch();
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
  displayMenu();
}

void coinToss() {
  lcd.print("Tossing Coin...");
  delay(1000);
  int toss = random(0, 2);
  lcd.clear();
  lcd.print("Result: ");
  lcd.print(toss == 0 ? "Heads" : "Tails");
  delay(2000);
  displayMenu();
}

void stopwatch() {
  if (!stopwatchRunning) {
    lcd.print("Stopwatch On");
    stopwatchStart = millis();
    stopwatchRunning = true;
  } else {
    unsigned long elapsedTime = millis() - stopwatchStart;
    lcd.clear();
    lcd.print("Time: ");
    lcd.print(elapsedTime / 1000);
    lcd.print("s");
    stopwatchRunning = false;
    delay(2000);
  }
  displayMenu();
}

void displayTimeAndDate() {
  lcd.clear();
  lcd.print("Time/Date:");
  lcd.setCursor(0, 1);
  lcd.print(dateTime);
  delay(3000);
  displayMenu();
}
