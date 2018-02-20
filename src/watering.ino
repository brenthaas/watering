
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN // 4
Adafruit_SSD1306 display(OLED_RESET);

#define SECONDS_WATERING 20
#define HOURS_BETWEEN_WATERING 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

unsigned long wait_duration = 0;
unsigned long last_water_time = 0;
unsigned int watering_count = 0;

void setup()   {
  Serial.begin(9600);

  watering_count = 0;
  wait_duration = HOURS_BETWEEN_WATERING * 60 * 60 * 1000;

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Brent's Watering");
  display.println();
  display.print("Count: ");
  display.println(watering_count);
  display.println();
  print_next_water_time();
  display.print("");
  display.println();
  display.print(millis());
  display.println("ms");
  display.display();
  if(last_water_time == 0 || times_up(millis())) {
    water();
  } else {
    delay(1000);
  }
}

void water() {
  for(int i = 0; i < SECONDS_WATERING; i++) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("~WATERING~");
    display.println();
    display.print("  ");
    display.print(SECONDS_WATERING - i);
    display.println("sec");
    display.display();
    delay(1000);
  }
  display.setTextSize(1);
  watering_count++;
  last_water_time = millis();
}

void print_next_water_time() {
  unsigned long next_time = last_water_time + wait_duration;
  unsigned long duration = next_time - millis();
  display.print("Next: ");
  unsigned long hours = duration / (1000 * 60 * 60);
  display.print(hours);
  display.print("H:");
  unsigned long min = (duration - (hours * 1000 * 60 * 60)) / (1000 * 60);
  display.print(min);
  display.print("M:");
  unsigned long sec = (duration - (hours * 1000 * 60 * 60) - (min * 1000 * 60)) / 1000;
  display.print(sec);
  display.println("S");
}

bool times_up(unsigned long at_time) {
  return (millis() - last_water_time) > wait_duration;
}
