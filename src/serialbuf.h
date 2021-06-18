/*
    Serial buffer V2
    Djordje Herceg, 17.6.2021.

    GitHub: https://github.com/djherceg/serialBuf

    Requires: ByteArray library
*/

#ifndef __SERIALBUF_H
#define __SERIALBUF_H


#include "ByteArray.h"
#include <Arduino.h>
//#include <stdint.h>

#define SERIALBUF_TEXTMODE 0
#define SERIALBUF_BINARYMODE 1

class SerialBuf
{
private:
    size_t maxlen;                  // buffer size
    ByteArray *array;
    bool overflow;                  // data overflow detected on reception
    uint32_t mils;
    uint32_t timeout = 50;          // timeout interval in milliseconds for reception in BINARYMODE
    bool finished;                  // reception finished
    size_t position;                // current position
    int mode = SERIALBUF_TEXTMODE;  // BINARYMODE or TEXTMODE (recognize CR+LF or LF as End-of-line and End-of-reception)

public:
    /** Initialize buffer of specified size in TEXTMODE with a 100ms timeout */
    SerialBuf(size_t buflen);

    /** Intialize buffer with the specified size, mode and timeout */
    SerialBuf(size_t buflen, int Mode, uint32_t Timeout);
    
    /** Dispose of the buffer */
    ~SerialBuf();


    /** Clear the buffer */
    void clear();
    
    /** Switch to TEXT mode. Reception is finished when CR is encountered. LF is ignored. */
    void textMode();

    /** Switch to BINARY mode. Reception is finished when nothing is received for [timeout] milliseconds. */
    void binaryMode();

    /** Gets buffer mode: SERIALBUF_TEXTMODE or SERIALBUF_BINARYMODE */
    int getMode();

    /** Must be called frequently to receive serial data. */
    void loop();

    /** Returns the character at [position]+offset or -1 if it doesn't exist. */
    int peek(size_t offset); 
    
    /** A character exists at [position] */
    bool isCurrent();           

    /** A character exists at [position]+1 */
    bool isNext();              

    /** Returns the next charater from the buffer or -1 if it doesn't exist. */
    int read(); 

    /** At least n more characters exist at [position] */
    bool isNextn(size_t n);        

    /** Data overflow occured on reception */
    bool isOverflow();          

    /** Reception finished, there is a message in the buffer */
    bool isAvailable(); 
    
    /** Returns the length of content in the buffer */
    size_t getLength();            
    
    /** Returns the current position in the buffer */
    size_t getPosition();

    /** Return the maximum content size. */
    size_t getSize();

    /** Return binary mode timeout in milliseconds */
    uint32_t getTimeout();

    /**
     * @brief Get the underlying buffer. Consult getLen() and getMaxLen() for the length of the buffer.
     * 
     * @return char* 
     */
    char* getBuffer();
};

#endif
