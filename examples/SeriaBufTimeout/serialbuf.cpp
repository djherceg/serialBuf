//
//  SerialBuf v1.0.006
//  15.4.2021
//  Djordje Herceg
//

/*
   Serial buffer is in TEXTMODE, i.e. reception finishes when LF is received.
   CR is ignored. Data exceeding buffer length is ignored, but the overflow flag is set.
*/

#include "serialbuf.h"
#include "Arduino.h"

void SerialBuf::init(size_t buflen)
{
  init(buflen, SERIALBUF_TEXTMODE, 50);
}

void SerialBuf::init(size_t buflen, int Mode, uint32_t Timeout)
{
  if (buffer != nullptr)
  {
    delete[] buffer;              // prevent memory leaks!
  }
  buffer = new char[buflen + 1];  // plus one byte for the null char!
  maxbuf = buflen;
  this->timeout = Timeout;
  mode = Mode;                    // SERIALBUF_BINARYMODE or SERIALBUF_TEXTMODE
  clear();
}

SerialBuf::~SerialBuf()
{
  delete[] buffer;
}

size_t SerialBuf::getSize() {
  return maxbuf;
}

void SerialBuf::clear()
{
  overflow = false;
  finished = false;
  length = 0;
  position = 0;
  if (buffer != nullptr) {
    buffer[0] = 0; // null-terminate
  }
  mils = millis();
}

void SerialBuf::loop()
{
  if (finished)
  {
    return;
  }

  if ((mode == SERIALBUF_BINARYMODE) && (length > 0) && (millis() - mils > timeout))
  {
    finished = true;
    buffer[length] = 0;
    return;
  }

  if (Serial.available()) {
    while (Serial.available())
    {
      int r = Serial.read();
      if (mode == SERIALBUF_TEXTMODE)
      {
        if (r == 13)
          continue;

        if (r == 10)
        {
          finished = true;
          buffer[length] = 0; // null-terminate
          return;
        }
      }

      if (length < maxbuf)
      {
        if (r > -1) {
          buffer[length++] = (char)r;
        }
      }
      else
      {
        overflow = true;
      }
    }
    mils = millis();
  }
}

void SerialBuf::textMode()
{
  mode = SERIALBUF_TEXTMODE;
  clear();
}

void SerialBuf::binaryMode()
{
  mode = SERIALBUF_BINARYMODE;
  clear();
}

int SerialBuf::getMode()
{
  return mode;
}

int SerialBuf::peek(size_t offset)
{
  if (position + offset < length)
  {
    return buffer[position + offset];
  }
  else
  {
    return -1;
  }
}

bool SerialBuf::isCurrent()
{
  return position < length;
}

bool SerialBuf::isNext()
{
  return (position + 1) < length;
}

bool SerialBuf::isNextn(size_t n)
{
  return (position + n) <= length;
}


/** Returns the next charater from the buffer or -1 if it doesn't exist. */
int SerialBuf::read()
{
  if (!isCurrent())
    return -1;

  return buffer[position++];
}

bool SerialBuf::isOverflow()
{
  return overflow;
}

bool SerialBuf::isAvailable()
{
  return finished;
}

size_t SerialBuf::getLength()
{
  return length;
}

size_t SerialBuf::getPosition()
{
  return position;
}


uint32_t SerialBuf::getTimeout()
{
  return timeout;
}
