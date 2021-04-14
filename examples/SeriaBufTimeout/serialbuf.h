//
//  SerialBuf v1.0.005
//  14.4.2021
//  Djordje Herceg
//

#ifndef __SERIALBUF_H
#define __SERIALBUF_H

#include <Arduino.h>

#define SERIALBUF_TEXTMODE 0
#define SERIALBUF_BINARYMODE 1

class SerialBuf
{
private:
    size_t maxbuf;                          // buffer size
    bool overflow;                          // data overflow detected on reception
    uint32_t mils;
    uint32_t timeout = 50;                  // timeout interval in milliseconds for reception in BINARYMODE
    bool finished;                          // reception finished
    size_t length;                          // actual content length
    size_t position;                        // current position
    int mode = SERIALBUF_TEXTMODE;          // BINARYMODE or TEXTMODE (recognize CR+LF or LF as End-of-line and End-of-reception)

public:
    /** Initialize buffer of specified size in TEXTMODE */
    void init(size_t buflen);

    /** Intialize buffer with the specified size, mode and timeout */
    void init(size_t buflen, int Mode, uint32_t Timeout);

    /** Disposes of the buffer */
    ~SerialBuf();

    /** Clears the buffer */
    void clear();

    /** Switch to TEXT mode. Reception is finished when LF is encountered. CR is ignored. */
    void textMode();

    /** Switch to BINARY mode. Receptions is finished when nothing is received for [timeout] milliseconds. */
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
    
    /** Buffer array */
    char *buffer = nullptr;        
    
    /** Returns the length of content in the buffer */
    size_t getLength();            
    
    /** Returns the current position in the buffer */
    size_t getPosition();

    /** Return the maximum content size. */
    size_t getSize();

    /** Return binary mode timeout in milliseconds */
    uint32_t getTimeout();
};

#endif
