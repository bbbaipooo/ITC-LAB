#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WIDTH 128
#define HEIGHT 32
#define Button_L 5
#define Button_R 6
#define OLED_RESET -1
int speakerPin = 3;
Adafruit_SSD1306 OLED(OLED_RESET);

#define TIME_MOVE 50
struct Ball
{
    int x, y;        // position
    const int r = 2; // radius
    int dx, dy;      // direction x, direction y
    int time;        // millis
};
Ball ball;

struct Bar
{
    int x, y, w, h;
};
Bar sticky;
Bar sticker;

// Condition: stick must be sticked to wall
bool intersects(Ball ball, Bar sticky)
{
    return (ball.y - ball.r < sticky.y + sticky.h && ball.y + ball.r > sticky.y
        && (ball.x + ball.r > WIDTH - sticky.w || ball.x - ball.r < sticky.w));
}

void Bar_init(Bar* stick, int x, int y, int w, int h)
{
    stick->x = x;
    stick->y = y;
    stick->w = w;
    stick->h = h;
}

void fibonacci(int maximum)
{
  long fib = 1;
  long fib1 = 1;  
  long fib2 = 2;
  for (int i=0; i < maximum; i++) {
    fib = fib1+fib2;
    fib1 = fib2;
    fib2 = fib;
    tone(speakerPin,fib);
    delay(200);
  }
  noTone(speakerPin);
}

void setup()
{
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3c);//ชื่อรุ่น
    pinMode(Button_L, INPUT_PULLUP);
    pinMode(Button_R, INPUT_PULLUP);
    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;
    ball.dx = 1;
    ball.dy = 1;
    Bar_init(&sticky, 0, 0, 5, 10);
    Bar_init(&sticker, WIDTH - 5, 0, 5, 10);
    pinMode(speakerPin, OUTPUT);
}

void loop()
{
    OLED.clearDisplay();
    if (ball.x - ball.r < 0 || ball.x + ball.r > WIDTH)
    {
        OLED.setTextColor(WHITE);
        OLED.setCursor(10, 1);
        OLED.setTextSize(2);
        OLED.println("GameOver!");
        fibonacci(20);
    }
    else {
        if (digitalRead(Button_L) == 0 && sticky.y > 0)
        {
            sticky.y -= 1;
            sticker.y -= 1;
        }
        if (digitalRead(Button_R) == 0 && sticky.y < HEIGHT - sticky.h)
        {
            sticky.y += 1;
            sticker.y += 1;
        }

        // ball state
        if (millis() - ball.time > TIME_MOVE)
        {
            ball.time = millis();
            if (ball.y > HEIGHT - ball.r)//เช็คชนเพื่อเปลี่ยนทิศลูกบอล
            {
                ball.dy = -ball.dy;
                ball.y = HEIGHT - ball.r;
            }
            if (ball.y < ball.r)
            {
                ball.dy = -ball.dy;
                ball.y = ball.r;
            }
            // ball moving
            ball.x += ball.dx;
            ball.y += ball.dy;
            if (intersects(ball, sticky))
            {
                ball.dx = -ball.dx;
                tone(speakerPin,1050);
                delay(50);
                noTone(speakerPin);
            }
        }

        OLED.fillCircle(ball.x, ball.y, ball.r, WHITE);
        OLED.fillRect(sticky.x, sticky.y, sticky.w, sticky.h, WHITE);
        OLED.fillRect(sticker.x, sticker.y, sticker.w, sticker.h, WHITE);
    }
    OLED.display();
}
