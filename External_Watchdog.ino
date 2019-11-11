/* Watchdog Processor
 * https://github.com/Andy4495/ExternalWatchdog

   Sketch monitors one pin to see if it toggles within a certain
   threshold timer.

   If no toggle is detected, then pull another line low to reset
   a companion processor.

   LED is flashed for half a second when sketch starts.

   LED flashes quickly (100ms on / 100ms off) when approaching timeout.

   The sketch should run on any MSP430 chip. Since the compiled
   sketch requires less than 1200 byts of program space, it can
   be programmed on low end G2 chips like G2231 or G2201.
*/

int currentWD_pin_state;
int lastWD_pin_state;
unsigned long lastWD_detected = 0;
long lastFlashTime; // Used when flashing LED
int  lastFlashState = HIGH;

const unsigned long timeout_ms       = 15L * 60L * 1000L; // Time before triggering watchdog reset
const unsigned long almostTimeout_ms = 14L * 60L * 1000L; // Time to start flashing LEDs
const int  reset_delay_ms   = 1 * 1000;   // Length of time to hold reset low

const int LED_pin        =  7;     // External LED
const int WD_monitor_pin =  6;     // Check if this pin toggles within timeout
const int WD_reset_pin   =  5;     // This pin connects to companion processor reset



void setup() {
  digitalWrite(LED_pin, HIGH);
  digitalWrite(WD_reset_pin, LOW);

  pinMode(LED_pin, OUTPUT);
  pinMode(WD_monitor_pin, INPUT);  // Assumes hardwired connection to companion processor, so no PULLUP
  pinMode(WD_reset_pin, INPUT);    // Keep this pin in HIGH-Z state unless pulling LOW

  digitalWrite(LED_pin, HIGH);      // Flash the LED on restart
  delay(500);
  digitalWrite(LED_pin, LOW);

  lastWD_pin_state = digitalRead(WD_monitor_pin);
  lastWD_detected = millis();
  lastFlashTime = lastWD_detected;
}

void loop() {
  currentWD_pin_state = digitalRead(WD_monitor_pin);
  if (currentWD_pin_state == lastWD_pin_state) {
    if ((millis() - lastWD_detected) > timeout_ms) {
      // Reset companion processor, hold reset low for 1 second
      digitalWrite(LED_pin, HIGH);
      pinMode(WD_reset_pin, OUTPUT);
      delay(reset_delay_ms);
      pinMode(WD_reset_pin, INPUT);
      digitalWrite(LED_pin, LOW);
      lastWD_detected = millis();
      lastFlashTime = lastWD_detected;
      currentWD_pin_state = digitalRead(WD_monitor_pin);
    }
    if ((millis() - lastWD_detected) > almostTimeout_ms) {
      if ((millis() - lastFlashTime) > 100) {
        lastFlashTime = millis();
        digitalWrite(LED_pin, lastFlashState);
        lastFlashState = !lastFlashState;
      }
    }
  }
  else  // Watchdog pin changed levels
  {
    lastWD_pin_state = currentWD_pin_state;
    lastWD_detected = millis();
    lastFlashTime = lastWD_detected;
    digitalWrite(LED_pin, LOW);            // Turn off LED
    lastFlashState = LOW;
  }
}
