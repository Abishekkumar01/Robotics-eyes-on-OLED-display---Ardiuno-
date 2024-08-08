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

void sleep()
{
    left_eye_height = 2;
    right_eye_height = 2;
    draw_eyes(true);
    display.fillRect(left_eye_x - left_eye_width / 2, left_eye_y - 2, left_eye_width, 4, SSD1306_WHITE);
    display.fillRect(right_eye_x - right_eye_width / 2, right_eye_y - 2, right_eye_width, 4, SSD1306_WHITE);
    display.display();
    delay(2000);
}

void wakeup()
{
    sleep();
    for (int h = 0; h <= ref_eye_height; h += 2)
    {
        left_eye_height = h;
        right_eye_height = h;
        draw_eyes(true);
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

    display.display();
    delay(1000);
}

void saccade(int direction_x, int direction_y)
{
    int direction_x_movement_amplitude = 8;
    int direction_y_movement_amplitude = 6;
    int blink_amplitude = 8;

    for (int i = 0; i < 1; i++)
    {
        left_eye_x += direction_x_movement_amplitude * direction_x;
        right_eye_x += direction_x_movement_amplitude * direction_x;
        left_eye_y += direction_y_movement_amplitude * direction_y;
        right_eye_y += direction_y_movement_amplitude * direction_y;

        right_eye_height -= blink_amplitude;
        left_eye_height -= blink_amplitude;
        draw_eyes();
        delay(1);
    }

    for (int i = 0; i < 1; i++)
    {
        left_eye_x += direction_x_movement_amplitude * direction_x;
        right_eye_x += direction_x_movement_amplitude * direction_x;
        left_eye_y += direction_y_movement_amplitude * direction_y;
        right_eye_y += direction_y_movement_amplitude * direction_y;

        right_eye_height += blink_amplitude;
        left_eye_height += blink_amplitude;

        draw_eyes();
        delay(1);
    }
}

void move_right_big_eye()
{
    move_big_eye(1);
}

void move_left_big_eye()
{
    move_big_eye(-1);
}

void move_big_eye(int direction)
{
    int direction_oversize = 1;
    int direction_movement_amplitude = 2;
    int blink_amplitude = 5;

    for (int i = 0; i < 3; i++)
    {
        left_eye_x += direction_movement_amplitude * direction;
        right_eye_x += direction_movement_amplitude * direction;
        right_eye_height -= blink_amplitude;
        left_eye_height -= blink_amplitude;
        if (direction > 0)
        {
            right_eye_height += direction_oversize;
            right_eye_width += direction_oversize;
        }
        else
        {
            left_eye_height += direction_oversize;
            left_eye_width += direction_oversize;
        }

        draw_eyes();
        delay(1);
    }
    for (int i = 0; i < 3; i++)
    {
        left_eye_x += direction_movement_amplitude * direction;
        right_eye_x += direction_movement_amplitude * direction;
        right_eye_height += blink_amplitude;
        left_eye_height += blink_amplitude;
        if (direction > 0)
        {
            right_eye_height += direction_oversize;
            right_eye_width += direction_oversize;
        }
        else
        {
            left_eye_height += direction_oversize;
            left_eye_width += direction_oversize;
        }
        draw_eyes();
        delay(1);
    }

    delay(1000);

    for (int i = 0; i < 3; i++)
    {
        left_eye_x -= direction_movement_amplitude * direction;
        right_eye_x -= direction_movement_amplitude * direction;
        right_eye_height -= blink_amplitude;
        left_eye_height -= blink_amplitude;
        if (direction > 0)
        {
            right_eye_height -= direction_oversize;
            right_eye_width -= direction_oversize;
        }
        else
        {
            left_eye_height -= direction_oversize;
            left_eye_width -= direction_oversize;
        }
        draw_eyes();
        delay(1);
    }
    for (int i = 0; i < 3; i++)
    {
        left_eye_x -= direction_movement_amplitude * direction;
        right_eye_x -= direction_movement_amplitude * direction;
        right_eye_height += blink_amplitude;
        left_eye_height += blink_amplitude;
        if (direction > 0)
        {
            right_eye_height -= direction_oversize;
            right_eye_width -= direction_oversize;
        }
        else
        {
            left_eye_height -= direction_oversize;
            left_eye_width -= direction_oversize;
        }
        draw_eyes();
        delay(1);
    }

    center_eyes();
}

void look_left()
{
    left_eye_x -= 10;
    right_eye_x -= 10;
    draw_eyes();
}

void look_right()
{
    left_eye_x += 10;
    right_eye_x += 10;
    draw_eyes();
}

void happy_eyes()
{
    left_eye_height = 10;
    right_eye_height = 10;
    draw_eyes();
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
        myServo.write(180); // Move servo to 180 degrees
        happy_eye();        // Display happy eyes on OLED
    }
    else
    {
        myServo.write(0); // Reset servo position
        blink();          // Default blinking eyes animation
    }

    delay(1000); // Delay for readability
}

