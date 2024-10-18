#include <FastLED.h>
#include "Keyboard.h"

const int buttonPin = 2;
const int secondButtonPin = 3; 
#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
int previousButtonState = HIGH;
int previousSecondButtonState = HIGH;
int palleteCount = 0;

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePaletteOnPress() {
    if(palleteCount ==  0)  { 
        currentPalette = RainbowColors_p;         
        currentBlending = LINEARBLEND; 
    }
    if(palleteCount == 1)  { 
        currentPalette = RainbowStripeColors_p;   
        currentBlending = NOBLEND;  
    }
    if(palleteCount == 2)  { 
        currentPalette = RainbowStripeColors_p;   
        currentBlending = LINEARBLEND; 
    }
    if(palleteCount == 3)  { 
        currentPalette = PartyColors_p;           
        currentBlending = LINEARBLEND; 
    }
    palleteCount = (palleteCount + 1) % 4;
}

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    pinMode(buttonPin, INPUT_PULLUP);  
    pinMode(secondButtonPin, INPUT_PULLUP);
}

void loop()
{
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    FillLEDsFromPaletteColors(startIndex);

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);

      //checking the state of the button

    int buttonState = digitalRead(buttonPin);
    int secondButtonState = digitalRead(secondButtonPin);
    //replaces button press with UP arrow
    if (buttonState == LOW && previousButtonState == HIGH) {
        // and it's currently pressed:
        Serial.println("Button Pressed");
        Keyboard.press(KEY_UP_ARROW);
        delay(50);
    }

    if (buttonState == HIGH && previousButtonState == LOW) {
        // and it's currently released:
        Keyboard.release(KEY_UP_ARROW);
        ChangePaletteOnPress();
        delay(50);
    }

    previousButtonState = buttonState;

    if (secondButtonState == LOW && previousSecondButtonState == HIGH) {
        Serial.println("Button 2 Pressed");
        Keyboard.press(69);
        delay(50);
    }

    if (secondButtonState == HIGH && previousSecondButtonState == LOW) {
        Keyboard.release(69);
        ChangePaletteOnPress();
        delay(50);
    }

    previousSecondButtonState = secondButtonState;
}