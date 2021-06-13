
#include <math.h>
#include <stdint.h>

#define VERT 0
#define HORZ 1

#define LD 8
#define V_CS 9
#define H_CS 10

#define STEP_DELAY 1000

#define LETTER_END {0, 0, 0}
#define IS_LETTER_END(li, i) (LETTERS[li][i][0] == 0 && LETTERS[li][i][1] == 0)

const int8_t LETTERS[][8][3] = {
  { {10, 0, 1}, {-10, -10, 1}, {10, 0, 1}, {-10, 10, 1}, LETTER_END }, // Z
  { {10, 0, 1}, {-10, 0, 1}, {0, -10, 1}, {10, 0, 1}, {-10, 0, 1}, {0, 10, 1}, LETTER_END },
  { {10, 0, 1}, {-10,  0, 1}, {10, 10, 1}, {-10, -10, 1}, {0, 10, 1}, {0, -10, 1}, LETTER_END }, // TESTING
  { {10, 10, 1}, {-20, -20, 1}, {10, 10, 1}, {10, 0, 1}, {-10, 0, 1}, LETTER_END },
  { {10, 0, 1}, {-10, -10, 1}, {10, 0, 1}, {-10, 0, 1}, {0, 10, 1}, LETTER_END },
  { {3, 12, 1}, {3, -12, 1}, {-1, 4, 1}, {-4, 0, 1}, {-1, -4, 1}, LETTER_END }, // 5 - SPACESHIP
  { {10, 0, 1}, {0, 10, 1}, {-10, 0, 1}, {0, -10, 1}, LETTER_END }, // 6 - SQUARE
  { {50, 50, 1}, {-50, -50, 1}, LETTER_END }
};

byte letter = 0;
int8_t x_pos = 0, y_pos = 0;

void initDAC() {
  for (int i = 0; i < 8; i++)
    pinMode(i, OUTPUT);
  pinMode(LD, OUTPUT); // /LD signal
  pinMode(V_CS, OUTPUT); // /CS for Vertical
  pinMode(H_CS, OUTPUT); // /CS for Hirizontal
 
  digitalWrite(LD, HIGH);
  digitalWrite(V_CS, HIGH);
  digitalWrite(H_CS, HIGH);
}

void writePos() {
//  Serial.println(y_pos + 0x80, HEX);
  writeAxis(x_pos + 0x80, HORZ);
  writeAxis(y_pos + 0x80, VERT);
    
  digitalWrite(LD, LOW); // LoaD in value to DAC
  delayMicroseconds(1);
  digitalWrite(LD, HIGH);
  delayMicroseconds(1);
}

inline void writeAxis(byte val, byte axis) {
  if (axis == VERT) {
    digitalWrite(V_CS, LOW);
    writeDAC(val);
    digitalWrite(V_CS, HIGH);
  }
  else if (axis == HORZ) {
    digitalWrite(H_CS, LOW);
    writeDAC(val);
    digitalWrite(H_CS, HIGH);
  }
}

inline void writeDAC(byte val) {
  for (int i = 0; i < 8; i++)
    digitalWrite(i, val & (1 << i));
}


// Bresenham's Algorithm (Wikipedia and middle-engine.com)
void nextLetter(byte letter_index) {
  for (uint8_t i = 0; i < 8 && !IS_LETTER_END(letter_index, i); i++) {
    
    int8_t end_x = x_pos + LETTERS[letter_index][i][0];
    int8_t end_y = y_pos + LETTERS[letter_index][i][1];
    
    int8_t dx = abs(LETTERS[letter_index][i][0]);
    int8_t dy = abs(LETTERS[letter_index][i][1]);
    int8_t sx = LETTERS[letter_index][i][0] < 0 ? -1 : 1;
    int8_t sy = LETTERS[letter_index][i][1] < 0 ? -1 : 1;

    int16_t err = 0;

    if (dx > dy) {
      for (; sx < 0 ? x_pos >= end_x : x_pos <= end_x; x_pos += sx) {
        err += dy;
        if (err * 2 >= dx) {
          y_pos += sy;
          err -= dx;
        }
        writePos();
        delayMicroseconds(STEP_DELAY);
      }
    } 
    else {
     for (; sy < 0 ? y_pos >= end_y : y_pos <= end_y; y_pos += sy) {
        err += dx;
        if (err * 2 >= dy) {
          x_pos += sx;
          err -= dy;
        }
        writePos();
        delayMicroseconds(STEP_DELAY);
      } 
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  initDAC();
//  Serial.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly:
  nextLetter(4);
//  Serial.println(x_pos, HEX);
//  Serial.println(y_pos, HEX);
//  Serial.println("---");
//  x_pos = y_pos = 0;
//  writePos();
//    delay(2000);

}
