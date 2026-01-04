#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <SPI.h>

// SPI OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define OLED_DC     9     // Data/Command
#define OLED_CS     10    // Chip Select
#define OLED_RESET  7     // Reset pin

Adafruit_SSD1327 display(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Thumbstick pins
#define P1_Y_PIN A0       // Player 1 Y-axis
#define P2_Y_PIN A1       // Player 2 Y-axis

// Paddle and ball properties
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 20
#define BALL_SIZE 4

// Game variables
int p1_y, p2_y;                
int ball_x = SCREEN_WIDTH / 2; 
int ball_y = SCREEN_HEIGHT / 2;
int ball_dx = 3, ball_dy = 3;  // Ball direction and speed

// Score variables
int p1_score = 0;
int p2_score = 0;
const int SCORE_LIMIT = 10;    

bool game_started = false;
bool game_over = false;

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin()) {
    Serial.println(F("SSD1327 allocation failed"));
    for (;;);
  }

  display.fillScreen(0x0000);  // Clear screen (black)

  // Initialize thumbstick pins
  pinMode(P1_Y_PIN, INPUT);
  pinMode(P2_Y_PIN, INPUT);

  // Display the start screen
  showStartScreen();
}

void loop() {
  if (game_over) {
    checkJoystickMovement();  // Allow restart after game over
    return;
  }

  if (!game_started) {
    checkJoystickMovement();
    return;
  }

  // Read Y-axis values
  int p1_value = analogRead(P1_Y_PIN);
  int p2_value = analogRead(P2_Y_PIN);

  // Map Y-axis values to paddle positions
  int p1_target = map(p1_value, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
  int p2_target = map(p2_value, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

  float speed_factor = 0.4;

  p1_y += (p1_target - p1_y) * speed_factor;
  p2_y += (p2_target - p2_y) * speed_factor;

  // Update ball position
  ball_x += ball_dx;
  ball_y += ball_dy;

  // Ball collision with top and bottom
  if (ball_y <= 0 || ball_y >= SCREEN_HEIGHT - BALL_SIZE) {
    ball_dy = -ball_dy;
  }

  // Ball collision with paddles
  if (ball_x <= PADDLE_WIDTH && ball_y >= p1_y && ball_y <= p1_y + PADDLE_HEIGHT) {
    ball_dx = -ball_dx;
  }

  if (ball_x >= SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE && ball_y >= p2_y && ball_y <= p2_y + PADDLE_HEIGHT) {
    ball_dx = -ball_dx;
  }

  // Ball goes out of bounds - scoring logic
  if (ball_x < 0) {
    p2_score++;
    resetBall();
  }
  if (ball_x > SCREEN_WIDTH) {
    p1_score++;
    resetBall();
  }

  // Check for score limit
  if (p1_score >= SCORE_LIMIT || p2_score >= SCORE_LIMIT) {
    game_over = true;
    showGameOverScreen();
    return;
  }

  // Draw everything
  display.fillScreen(0x0000);  // Clear the screen

  // Draw scoreboard
  display.setTextSize(1);
  display.setTextColor(0xFFFF);  // White
  display.setCursor(10, 5);
  display.print("P1: "); 
  display.print(p1_score);
  display.setCursor(75, 5);
  display.print("P2: "); 
  display.print(p2_score);

  // Draw paddles (grayscale)
  display.fillRect(0, p1_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xAAAA);      // Light gray paddle
  display.fillRect(SCREEN_WIDTH - PADDLE_WIDTH, p2_y, PADDLE_WIDTH, PADDLE_HEIGHT, 0xAAAA);

  // Draw ball
  display.fillCircle(ball_x, ball_y, BALL_SIZE / 2, 0xFFFF);  // White ball

  display.display();
  delay(8);  // Adjust game speed
}

// --- FUNCTIONS ---

// Display the start screen
void showStartScreen() {
  display.fillScreen(0x0000);
  display.setTextSize(2);
  display.setTextColor(0xFFFF);
  display.setCursor(20, 25);
  display.println("2-Player");
  display.setCursor(40, 43);
  display.println("Pong");

  display.setTextSize(1);
  display.setCursor(28, 70);
  display.println("(First to 10!)");
  display.setCursor(25, 90);
  display.println("Move a Joystick");
  display.setCursor(40, 100);
  display.println("To Start");

  display.display();
}

// Display the game over screen
void showGameOverScreen() {
  display.fillScreen(0x0000);
  display.setTextSize(2);
  display.setTextColor(0xFFFF);
  display.setCursor(25, 50);

  if (p1_score >= SCORE_LIMIT) {
    display.println("P1 Wins!");
  } else {
    display.println("P2 Wins!");
  }

  display.setTextSize(1);
  display.setCursor(25, 90);
  display.println("Move a Joystick");
  display.setCursor(35, 100);
  display.println("to Restart");  

  display.display();
}

// Check for joystick movement to start or restart the game
void checkJoystickMovement() {
  int p1_value = analogRead(P1_Y_PIN);
  int p2_value = analogRead(P2_Y_PIN);

  if (abs(p1_value - 512) > 50 || abs(p2_value - 512) > 50) {
    if (game_over) {
      // Reset the game
      p1_score = 0;
      p2_score = 0;
      game_over = false;
      showStartScreen();
    } else {
      game_started = true;
    }
    Serial.println("Game Started!");
  }
}

// Reset the ball after scoring
void resetBall() {
  ball_x = SCREEN_WIDTH / 2;
  ball_y = SCREEN_HEIGHT / 2;

  // Randomize ball direction
  ball_dx = (random(2) == 0) ? 2 : -2;
  ball_dy = (random(2) == 0) ? 2 : -2;

  delay(500);  // Brief pause after scoring
}

// Print Boxes
// Add lining to wires
// Put on PCB
// Download Code onto Microcontroller