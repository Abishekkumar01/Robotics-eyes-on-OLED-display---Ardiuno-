#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Servo
Servo myServo;

// Ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 6;

// Variables to store the duration and distance
long duration;
int distance;

// OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< Address for 128x64 display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Eye and expression variables
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;

int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = 32 + ref_eye_width + ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;

void draw_eyes(bool update = true)
{
    display.clearDisplay();
    int x = int(left_eye_x - left_eye_width / 2);
    int y = int(left_eye_y - left_eye_height / 2);
    display.fillRoundRect(x, y, left_eye_width, left_eye_height, ref_corner_radius, SSD1306_WHITE);
    x = int(right_eye_x - right_eye_width / 2);
    y = int(right_eye_y - right_eye_height / 2);
    display.fillRoundRect(x, y, right_eye_width, right_eye_height, ref_corner_radius, SSD1306_WHITE);
    if (update)
    {
        display.display();
    }
}

void center_eyes(bool update = true)
{
    left_eye_height = ref_eye_height;
    left_eye_width = ref_eye_width;
    right_eye_height = ref_eye_height;
    right_eye_width = ref_eye_width;

    left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
    left_eye_y = SCREEN_HEIGHT / 2;
    right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
    right_eye_y = SCREEN_HEIGHT / 2;

    draw_eyes(update);
}

void draw_smile()
{
    // Draw the smile using an arc approximation with lines
    int smileCenterX = SCREEN_WIDTH / 2;
    int smileCenterY = SCREEN_HEIGHT / 2 + 20;
    int smileRadius = 15;
    int startAngle = 0;
    int endAngle = 180;

    for (int angle = startAngle; angle <= endAngle; angle++)
    {
        float rad = angle * 3.14159 / 180.0;
        int x = smileCenterX + smileRadius * cos(rad);
        int y = smileCenterY + smileRadius * sin(rad);
        display.drawPixel(x, y, SSD1306_WHITE);
    }

    display.display();
    delay(1000);
}

void blink(int speed = 12)
{
    draw_eyes();

    for (int i = 0; i < 3; i++)
    {
        left_eye_height -= speed;
        right_eye_height -= speed;
        draw_eyes();
        delay(1);
    }
    for (int i = 0; i < 3; i++)
    {
        left_eye_height += speed;
        right_eye_height += speed;

        draw_eyes();
        delay(1);
    }
}

void happy_eye()
{
    center_eyes(false);
    int offset = ref_eye_height / 2;
    for (int i = 0; i < 10; i++)
    {
        display.fillTriangle(left_eye_x - left_eye_width / 2 - 1, left_eye_y + offset, left_eye_x + left_eye_width / 2 + 1, left_eye_y + 5 + offset, left_eye_x - left_eye_width / 2 - 1, left_eye_y + left_eye_height + offset, SSD1306_BLACK);
        display.fillTriangle(right_eye_x + right_eye_width / 2 + 1, right_eye_y + offset, right_eye_x - left_eye_width / 2 - 1, right_eye_y + 5 + offset, right_eye_x + right_eye_width / 2 + 1, right_eye_y + right_eye_height + offset, SSD1306_BLACK);
        offset -= 2;
        display.display();
        delay(1);
    }

    draw_smile();
}

void setup()
{
    Serial.begin(9600);

    // Servo setup
    myServo.attach(9);
    myServo.write(0); // Initial position

    // Ultrasonic sensor setup
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // OLED setup
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    display.display();
    delay(2000);
    display.clearDisplay();
    center_eyes();
}

void loop()
{
    // Measure distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; // Calculate the distance in cm

    if (distance <= 10)
    {
        myServo.write(0); // Move servo to 180 degrees
        happy_eye();        // Display happy eyes and smile on OLED
    }
    else
    {
        myServo.write(180); // Reset servo position
        blink();          // Default blinking eyes animation
    }

    delay(1000); // Delay for readability
}
