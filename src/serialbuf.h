//
//  SerialBuf v1.0.002
//  17.12.2020
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
    size_t maxbuf;                          // velicina bafera
    bool overflow;
    uint32_t mils;
    uint32_t stopInterval = 50;             // interval nakon zadnjeg primljenog karaktera kada se poruka racuna kao zavrsena
    bool finished;
    size_t length;                          // duzina sadrzaja u baferu
    size_t position;                        // trenutna pozicija u baferu
    int mode = SERIALBUF_TEXTMODE;          // BINARYMODE or TEXTMODE (recognize CR+LF or LF as End-of-line)

public:
    /** Inicijalizuje bafer zadate velicine u TEXTMODE */
    void init(size_t buflen);

    /** Inicijalizuje bafer zadate velicine u BINARYMODE sa zadatim stop intervalom */
    void init(size_t buflen, uint32_t stopinterval);

    /** Disposes of the buffer */
    ~SerialBuf();

    /** Brise bafer */
    void clear();

    /** Prebacuje bafer u TEXT mod. ceka da se red zavrsi znakom LF, ignorise CR */
    void textMode();

    /** Prebacuje bafer u BINARY mod. ceka stopInterval milisekundi a onda proglasava kraj unosa */
    void binaryMode();

    /** Vraca mod bafera: SERIALBUF_TEXTMODE ili SERIALBUF_BINARYMODE */
    int getMode();

    /** Poziva se stalno radi prihvata karaktera sa serijskog porta */
    void loop();

    /** Vraca karakter na poziciji+offset ili -1 ako ne postoji */
    int peek(size_t offset);                 // karakter na position+offset ili -1 ako ne postoji

    
    /** Da li postoji karakter na poziciji */
    bool iscurrent();           // da li postoji karakter na position

    /** Da li postoji karakter na poziciji+1 */
    bool isnext();              // da li postoji karakter na position+1

    /** cita jedan karakter iz bafera ili vraca -1 ako nema vise */
    int read();                 // cita jedan karakter iz bafera ili vraca -1 ako nema vise

    /** Da li bafer sadrzi jos najmanje n karaktera */
    bool isnextn(size_t n);        

    /** Da li je bafer prepunjen */
    bool isoverflow();          // da li je bafer prepunjen preko kapaciteta

    /** Da li je bafer zavrsio prijem i sadrzi poruku */
    bool isavailable();         // da li je bafer zavrsio prijem i sadrzi poruku
    

    char *buffer = nullptr;         // bafer
    
    /** vraca duzinu sadrzaja u baferu */
    size_t getLength();            
    
    /** vraca trenutnu poziciju u baferu */
    size_t getPosition();
};

#endif