#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ModbusMaster.h>

ModbusMaster node;

#define SLAVE_ID 2

#define VOLTAGE_ADDR 0x0000
#define VOLTAGE_LEN 2
#define CURRENT_ADDR 0x0E
#define CURRENT_LEN 2
#define POWER_ADDR 0x14
#define POWER_LEN 2
#define KILOWATTS_HOUR_ADDR 0x112
#define KILOWATTS_HOUR_LEN 2

#define VOLTAGE_WEIGHT 0.1
#define CURRENT_WEIGHT 0.001
#define POWER_WEIGHT 0.1
#define KILOWATTS_HOUR_WEIGHT 0.1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial1.begin(9600);
  Serial.begin(9600);

  node.begin(SLAVE_ID, Serial1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop()
{
  uint8_t result;
  float voltage, current, power, kilowatts_hour;

  result = node.readHoldingRegisters(VOLTAGE_ADDR, VOLTAGE_LEN);
  voltage = (float)node.getResponseBuffer(0) + ((float)node.getResponseBuffer(1) / 100.0);
  voltage = voltage * VOLTAGE_WEIGHT;

  delay(500);
  result = node.readHoldingRegisters(CURRENT_ADDR, CURRENT_LEN);
  current = (float)node.getResponseBuffer(0) + ((float)node.getResponseBuffer(1) / 100.0);
  current = current * CURRENT_WEIGHT;

  delay(500);
  result = node.readHoldingRegisters(POWER_ADDR, POWER_LEN);
  power = (float)node.getResponseBuffer(0) + ((float)node.getResponseBuffer(1) / 100.0);
  power = power * POWER_WEIGHT;

  delay(500);
  result = node.readHoldingRegisters(KILOWATTS_HOUR_ADDR, KILOWATTS_HOUR_LEN);
  kilowatts_hour = (float)node.getResponseBuffer(0) + ((float)node.getResponseBuffer(1) / 100.0);
  kilowatts_hour = kilowatts_hour * KILOWATTS_HOUR_WEIGHT;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Voltage: ");
  display.println(voltage);
  display.print("Current: ");
  display.println(current);
  display.print("Power: ");
  display.println(power);
  display.print("KWh: ");
  display.println(kilowatts_hour);
  display.display();

  delay(10000);
}
