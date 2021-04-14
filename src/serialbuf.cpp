#include "serialbuf.h"
#include "Arduino.h"

void SerialBuf::init()
{
    init(50);
}

void SerialBuf::init(uint32_t stopInterval)
{
    clear();
    this->stopInterval = stopInterval;
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

    if ((length > 0) && (millis() - mils > stopInterval))
    {
        finished = true;
    }

    while (Serial.available())
    {
        mils = millis();
        if (length < maxbuf)
        {
            int r = Serial.read();
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

int SerialBuf::peek()
{
    if (position < length)
    {
        return buffer[position];
    }
    else
    {
        return -1;
    }
}

int SerialBuf::peeknext()
{
    if ((position + 1) < length)
    {
        return buffer[position + 1];
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

bool SerialBuf::isnextn(int n)
{
    return (position + n) <= length;
}

/**
 * Čita karakter iz bafera ili vraća -1 ako ne postoji
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

int SerialBuf::getLength()
{
    return length;
}

int SerialBuf::getPosition()
{
    return position;
}