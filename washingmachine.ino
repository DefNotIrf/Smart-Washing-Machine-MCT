#include <Wire.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN 2
#define SERVO_PIN 6
#define MOTOR_PIN_A 3
#define MOTOR_PIN_B 4
#define WATER_LEVEL_SENSOR_PIN A0

TM1637Display countdownDisplay(9, 8);
Servo doorLockServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  

bool buttonPressed = false;
bool washing = false;
unsigned long countdownTime = 60000; // 60 seconds
int minimumWaterLevel = 500; 

void setup() {
  Serial.begin(9600); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_PIN_A, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  stopMotor(); 

  doorLockServo.attach(SERVO_PIN);
  doorLockServo.write(0); 

  countdownDisplay.setBrightness(0x0f);
  countdownDisplay.showNumberDec(0);

  lcd.init();                      
  lcd.backlight();                 

  // Display the welcome message
  displayWelcomeMessage();

  Wire.begin();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW && !washing) {
    buttonPressed = true;
  }

  if (buttonPressed) {
    buttonPressed = false;
    washing = true;
    doorLockServo.write(90); // Lock door

    // Display washing message with water level
    displayWashingMessage(analogRead(WATER_LEVEL_SENSOR_PIN));

    runMotor(); // Start motor

    unsigned long startTime = millis();
    while (millis() - startTime < countdownTime) {
      unsigned long remainingTime = countdownTime - (millis() - startTime);
      int seconds = remainingTime / 1000;
      countdownDisplay.showNumberDec(seconds);

      // Check water level
      int waterLevel = analogRead(WATER_LEVEL_SENSOR_PIN);
      if (waterLevel < minimumWaterLevel) 
      }

    }

    // Stop motor
    stopMotor();

    doorLockServo.write(0); // Unlock door

    // Display done message
    displayDoneMessage();

    
    delay(5000);
    washing = false;
    displayWelcomeMessage(); // Display the welcome message for the next cycle
  }
}

void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
}

void displayWashingMessage(int waterLevel) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Washing");
  lcd.setCursor(0, 1);
  lcd.print("Water Level:");
  lcd.setCursor(12, 1);
  lcd.print(waterLevel);
}

void displayDoneMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done :)");
}

void stopMotor() {
  digitalWrite(MOTOR_PIN_A, LOW);
  digitalWrite(MOTOR_PIN_B, LOW); // Ensure both pins are LOW to stop the motor
  Serial.println("Motor stopped"); // Debug print
}

// Function to run the motor
void runMotor() {
  digitalWrite(MOTOR_PIN_A, HIGH);
  digitalWrite(MOTOR_PIN_B, LOW); // Adjust these values if needed for your motor direction
  Serial.println("Motor running"); // Debug print
}
