#include "Godmode.h"
#include "HardwareSerial.h"
#include "SPI.h"

GodmodeState godmode = GodmodeState();

GodmodeState* GODMODE() {
  return &godmode;
}

unsigned long millis() {
  GodmodeState* godmode = GODMODE();
  return godmode->micros / 1000;
}

unsigned long micros() {
  GodmodeState* godmode = GODMODE();
  return godmode->micros;
}

void delay(unsigned long millis) {
  GodmodeState* godmode = GODMODE();
  godmode->micros += millis * 1000;
}

void delayMicroseconds(unsigned long micros) {
  GodmodeState* godmode = GODMODE();
  godmode->micros += micros;
}


void randomSeed(unsigned long seed)
{
  GodmodeState* godmode = GODMODE();
  godmode->seed = seed;
}

long random(long vmax)
{
  GodmodeState* godmode = GODMODE();
  godmode->seed += 4294967291;  // it's a prime that fits in 32 bits
  godmode->seed = godmode->seed % 4294967296; // explicitly wrap in case we're on a 64-bit impl
  return godmode->seed % vmax;
}

long random(long vmin, long vmax)
{
  return vmin < vmax ? (random(vmax - vmin) + vmin) : vmin;
}

void digitalWrite(unsigned char pin, unsigned char val) {
  GodmodeState* godmode = GODMODE();
  godmode->digitalPin[pin] = val;
}

int digitalRead(unsigned char pin) {
  GodmodeState* godmode = GODMODE();
  return godmode->digitalPin[pin].retrieve();
}

void analogWrite(unsigned char pin, int val) {
  GodmodeState* godmode = GODMODE();
  godmode->analogPin[pin] = val;
}

int analogRead(unsigned char pin) {
  GodmodeState* godmode = GODMODE();
  return godmode->analogPin[pin].retrieve();
}

void attachInterrupt(uint8_t interrupt, void ISR(void), uint8_t mode) {
  GodmodeState* godmode = GODMODE();
  godmode->interrupt[interrupt].attached = true;
  godmode->interrupt[interrupt].mode = mode;
}

void detachInterrupt(uint8_t interrupt) {
  GodmodeState* godmode = GODMODE();
  godmode->interrupt[interrupt].attached = false;
}

// Serial ports
#if defined(HAVE_HWSERIAL0)
  HardwareSerial Serial(&godmode.serialPort[0].dataIn, &godmode.serialPort[0].dataOut, &godmode.serialPort[0].readDelayMicros);
#endif
#if defined(HAVE_HWSERIAL1)
  HardwareSerial Serial1(&godmode.serialPort[1].dataIn, &godmode.serialPort[1].dataOut, &godmode.serialPort[1].readDelayMicros);
#endif
#if defined(HAVE_HWSERIAL2)
  HardwareSerial Serial2(&godmode.serialPort[2].dataIn, &godmode.serialPort[2].dataOut, &godmode.serialPort[2].readDelayMicros);
#endif
#if defined(HAVE_HWSERIAL3)
  HardwareSerial Serial3(&godmode.serialPort[3].dataIn, &godmode.serialPort[3].dataOut, &godmode.serialPort[3].readDelayMicros);
#endif

template <typename T>
inline std::ostream& operator << ( std::ostream& out, const PinHistory<T>& ph ) {
  out << ph;
  return out;
}

// defined in SPI.h
SPIClass SPI = SPIClass(&godmode.spi.dataIn, &godmode.spi.dataOut);
