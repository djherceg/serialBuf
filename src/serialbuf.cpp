//
//  SerialBuf v1.0.002
//  17.12.2020
//  Djordje Herceg
//

#include "serialbuf.h"
#include "Arduino.h"

void SerialBuf::init(size_t buflen)
{
    init(buflen, 50);
    textMode();
}

void SerialBuf::init(size_t buflen, uint32_t stopInterval)
{
    clear();
    if (buffer != nullptr)
    {
        delete[] buffer;            // prevent memory leaks!
    }
    buffer = new char[buflen];
    maxbuf = buflen;
    this->stopInterval = stopInterval;
    binaryMode();
}

SerialBuf::~SerialBuf()
{
    delete[] buffer;
}

void SerialBuf::clear()
{
    overflow = false;
    finished = false;
    length = 0;
    position = 0;
    buffer[0] = 0; // null-terminate
    mils = millis();
}

void SerialBuf::loop()
{
    if (finished)
    {
        return;
    }

    if ((mode == SERIALBUF_BINARYMODE) && (length > 0) && (millis() - mils > stopInterval))
    {
        finished = true;
        return;
    }

    while (Serial.available())
    {
        mils = millis();

        int r = Serial.read();
        if (mode == SERIALBUF_TEXTMODE)
        {
            if (r == 13)
                continue;

            if (r == 10)
            {
                finished = true;
                return;
            }
        }

        if (length < maxbuf)
        {
            //int r = Serial.read();
            if (r > -1)
            {
                buffer[length++] = (char)r;
                buffer[length] = 0; // null-terminate
            }
        }
        else
        {
            overflow = true;
            clear();
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
    if (position + offset < length)
    {
        return buffer[position + offset];
    }
    else
    {
        return -1;
    }
}

bool SerialBuf::iscurrent()
{
    return position < length;
}

bool SerialBuf::isnext()
{
    return (position + 1) < length;
}

bool SerialBuf::isnextn(size_t n)
{
    return (position + n) <= length;
}

/**
 * Cita karakter iz bafera ili vraca -1 ako ne postoji
 */
int SerialBuf::read()
{
    if (!isnext())
        return -1;

    return buffer[position++];
}

bool SerialBuf::isoverflow()
{
    return overflow;
}

bool SerialBuf::isavailable()
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