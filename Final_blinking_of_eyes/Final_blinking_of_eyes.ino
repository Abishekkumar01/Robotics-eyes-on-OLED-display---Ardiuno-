#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void smiley_eyes()
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

void setup()
{
    // initialize with the I2C addr 0x3C (for the 128x64)
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }

    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
}

void loop()
{
    center_eyes();
    delay(1000);
    
    blink(10);   // Blinking
    delay(500);  // Delay after blink

    look_left();
    blink(10);   // Blink after looking left
    delay(1000);
    center_eyes();
    blink(10);   // Blink after centering eyes
    delay(1000);

    look_right();
    blink(10);   // Blink after looking right
    delay(1000);
    center_eyes();
    blink(10);   // Blink after centering eyes
    delay(1000);

    happy_eye();
    blink(10);   // Blink after happy expression
    delay(1000);

    move_left_big_eye();
    blink(10);   // Blink after moving left
    delay(1000);

    move_right_big_eye();
    blink(10);   // Blink after moving right
    delay(1000);

    sleep();
    delay(2000);

    wakeup();
    blink(10);   // Blink after waking up
    delay(1000);
}
