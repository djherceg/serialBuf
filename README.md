# serialBuf

Description
===========
Arduino buffer for receiving incoming serial data

The SerialBuf class implements a simple byte-array buffer of a fixed size, which stores bytes received over Serial. When reception is complete, the buffer can be read by the application.


Modes: Binary and Text
======================
SerialBuf works in one of the two modes: binary or text. 

In binary mode, SerialBuf stores any bytes received over Serial. Reception ends when nothing is received for a specified time (timeout).
In text mode, SerialBuf stores any bytes received over Serial, except CR, which is ignored, and LF, which signals the end of reception.
Afterwards, the contents of the buffer can be read by the main program.

Timeout depends on the sender - how long does the sender wait between transmissions? Values between 20 and 500ms are reasonalbe, but anything goes.


Usage
=====

First, include the header file and create the SerialBuf object.

    #include "serialbuf.h"
    SerialBuf sbuf;
  
Next, in setup(), initialize the buffer. Specify maximum buffer size, mode and timeout interval. Have in mind that one extra byte will be allocated, so that received data can be null-terminated. For example, if you specify the buffer of 10 bytes, 11 bytes will be reserved, and the last received byte will always be followed by the null character.

    sbuf.init(10, SERIALBUF_BINARYMODE, 2000);    // NOTE: buffer acknowledges reception after 2000ms in binary mode
    sbuf.init(10, SERIALBUF_TEXTMODE, 2000);
  
Then, in loop(), make sure to call SerialBuffer::loop often enough so that it doesn't miss incoming serial data. 
Check SerialBuffer::isAvailable() to determine if there is a message  waiting to be processed in the buffer. 
Determine message length by calling SerialBuffer::getLength(). 
Access the message contents by either calling SerialBuffer::read() or by accessing SerialBuffer::buffer directly.
If the buffer has overflowed, SerialBuf::isOverflow() will return true.

    sbuf.loop();

    if (sbuf.isAvailable()) {
      Serial.print(sbuf.getLength());
      Serial.println(F(" bytes available in buffer"));
      if (sbuf.isOverflow()) {
        Serial.println(F("Overflow occurred."));
      }
      Serial.println(sbuf.buffer);
    }
  
