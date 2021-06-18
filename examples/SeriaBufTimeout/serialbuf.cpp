#include "serialbuf.h"
#include "Arduino.h"


SerialBuf::SerialBuf(size_t buflen, int Mode = SERIALBUF_TEXTMODE, uint32_t Timeout = 50)
{
  maxlen = buflen;
  array = new ByteArray(maxlen + 1);

  this->timeout = Timeout;
  mode = Mode;                    // SERIALBUF_BINARYMODE or SERIALBUF_TEXTMODE
  clear();
}

SerialBuf::~SerialBuf()
{
  delete array;
}



void SerialBuf::clear()
{
  overflow = false;
  finished = false;
  position = 0;
  array->clear();

  mils = millis();
}

void SerialBuf::loop()
{
  if (finished)
  {
    return;
  }

  if ((mode == SERIALBUF_BINARYMODE) && (array->getLen() > 0) && (millis() - mils > timeout))
  {
    finished = true;
    array->nullTerminate();
    return;
  }

  while (Serial.available())
  {
    mils = millis();

    int r = Serial.read();
    if (mode == SERIALBUF_TEXTMODE)
    {
      if (r == 10)
        continue;

      if (r == 13)
      {
        finished = true;
        array->nullTerminate();
        return;
      }
    }

    if (r > -1) {
      if (array->getLen() < maxlen)
      {
        array->append((char)r);
      }
      else
      {
        overflow = true;
      }
    }
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
  if (position + offset < array->getLen())
  {
    return (int)(*array)[position + offset];
  }
  else
  {
    return -1;
  }
}

bool SerialBuf::isCurrent()
{
  return position < array->getLen();
}

bool SerialBuf::isNext()
{
  return (position + 1) < array->getLen();
}

bool SerialBuf::isNextn(size_t n)
{
  return (position + n) <= array->getLen();
}


/** Returns the next charater from the buffer or -1 if it doesn't exist. */
int SerialBuf::read()
{
  if (!isCurrent())
    return -1;

  return (int)(*array)[position++];
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
  return array->getLen();
}

size_t SerialBuf::getPosition()
{
  return position;
}

size_t SerialBuf::getSize() {
  return maxlen;
}

uint32_t SerialBuf::getTimeout()
{
  return timeout;
}


char *SerialBuf::getBuffer()
{
  return array->buffer;
}
