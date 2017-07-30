/* Low-power Two-circuit break detector using the ATtiny85
* 
* This program will every eight seconds check the two circuits 
* to see whether they're open or closed. This is typically the fridge
* and the freezer door. If the door is open for more than three minutes
* the piezo-buzzer will beep three times followed by four seconds pause if the
* fridge door is open, and continue until it's closed. For the freezer door, 
* nine beeps will be produced.
* In addition, a LED will blink briefly every 60 seconds to show it's alive.
* The ATtiny85 is using a 1MHz internal clock.
* 
* The program uses approximately 4.5uA without the LED, and 6.5uA with the
* current configuration (red LED, 330ohm resistor, 15ms blink and 60s interval)
* With 6.5uA average current consumption, a CR2032 coin cell battery with 220mAh
* will give 3.8 years life. Without the LED, 5.5 years.
* 
* Low-power modes are attained using the LowPower library:
* https://github.com/sej7278/LowPower which is based on the 
* LowPower library from Rocket Scream Electronics(www.rocketscream.com)
* 
* Written by: Fredrik Hekland
* Date: 2017-07-26
* 
* This code is licensed under Creative Commons Attribution-ShareAlike 3.0
* Unported License.
*/

#include <LowPower.h>

const uint8_t PIN_SWITCH_VCC    = 0;
const uint8_t PIN_FRIDGE_SENSE  = 1;
const uint8_t PIN_FREEZER_SENSE = 2;
const uint8_t PIN_LED           = 3;
const uint8_t PIN_BUZZER        = 4;

const uint16_t BUZZER_TIMEOUT_S = 176; // ~3min time-out before alarm sounds
const uint8_t LED_INTERVAL_S    = 60;  // Blink alive-LED once a minute

boolean isAlarmActive     = false;
uint16_t durationOpenDoor = 0;
uint8_t ageLEDblink       = LED_INTERVAL_S;

void setup() 
{
  pinMode(PIN_SWITCH_VCC, OUTPUT);
  digitalWrite(PIN_SWITCH_VCC, HIGH);

  pinMode(PIN_FRIDGE_SENSE, INPUT_PULLUP);
  pinMode(PIN_FREEZER_SENSE, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
  
  pinMode(5, INPUT_PULLUP); // power-save
}

void loop()
{
  boolean isFridgeOpen  = checkIsDoorOpen(PIN_FRIDGE_SENSE);
  boolean isFreezerOpen = checkIsDoorOpen(PIN_FREEZER_SENSE);
  if ( isFridgeOpen || isFreezerOpen )
  {
    if ( durationOpenDoor > BUZZER_TIMEOUT_S )
      isAlarmActive = true;
    if ( isAlarmActive )
    {
      uint8_t Nbeeps = (isFreezerOpen ? 9 : 3);
      for (uint8_t i = 0; i < Nbeeps; ++i)
      {
        togglePinHigh(SLEEP_120MS, PIN_BUZZER);
      }
    }
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    durationOpenDoor += 4;
    ageLEDblink += 4;
  }
  else // doors are closed
  {
    durationOpenDoor = 0;
    isAlarmActive = false;
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    ageLEDblink += 8;
  }
  
  if (ageLEDblink >= LED_INTERVAL_S)
  {
    togglePinHigh(SLEEP_15MS, PIN_LED);
    ageLEDblink = 0;
  }
}

/**
 * Toggle a pin high for a given period.
 * Used to blink the LED or sound the buzzer.
 * 
 * Must use idle mode since powerDown seems to provide
 * unstable/less pin current.
 * 
 * \param period The period to keep the pin high
 * \param pinNr The pin to toggle
 */
void togglePinHigh(period_t period, uint8_t pinNr)
{
  digitalWrite(pinNr, HIGH);
  LowPower.idle(period, ADC_OFF, TIMER1_OFF, TIMER0_OFF);
  digitalWrite(pinNr, LOW);
  LowPower.idle(period, ADC_OFF, TIMER1_OFF, TIMER0_OFF);
}

/**
 * Check whether a door has been opened, thus breaking
 * the circuit.
 * 
 * \param pin The input pin to check whether circuit has been broken
 */
uint8_t checkIsDoorOpen(uint8_t pin)
{
  digitalWrite(PIN_SWITCH_VCC, LOW); // run current through door circuit
  uint8_t val = digitalRead(pin); // high if open door, low if closed
  digitalWrite(PIN_SWITCH_VCC, HIGH); // shutoff current to save energy
  return val;
}

