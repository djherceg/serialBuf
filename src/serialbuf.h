#ifndef __SERIALBUF_H
#define __SERIALBUF_H

#include <stdint.h>

class SerialBuf
{
private:
    static const int maxbuf = 80;
    bool overflow;
    uint32_t mils;
    uint32_t stopInterval = 50;      // interval nakon zadnjeg primljenog karaktera kada se poruka računa kao završena
    bool finished;
    int length;                      // dužina sadržaja u baferu
    int position;                    // trenutna pozicija u baferu

public:
    /** Inicijalizuje bafer sa stop intervalom od 50ms */
    void init();

    /** Inicijalizuje bafer sa zadatim stop intervalom */
    void init(uint32_t stopinterval);

    /** Briše bafer */
    void clear();

    /** Poziva se stalno radi prihvata karaktera sa serijskog porta */
    void loop();

    /** Vraća karakter na poziciji ili -1 ako ne postoji */
    int peek();                 // karakter na position ili -1 ako ne postoji

    /** Vraća karakter na poziciji+1 ili -1 ako ne postoji */
    int peeknext();             // karakter na position+1 ili -1 ako ne postoji

    /** Da li postoji karakter na poziciji */
    bool iscurrent();           // da li postoji karakter na position

    /** Da li postoji karakter na poziciji+1 */
    bool isnext();              // da li postoji karakter na position+1

    /** Čita jedan karakter iz bafera ili vraća -1 ako nema više */
    int read();                 // čita jedan karakter iz bafera ili vraća -1 ako nema više

    /** Da li bafer sadrži još najmanje n karaktera */
    bool isnextn(int n);        

    /** Da li je bafer prepunjen */
    bool isoverflow();          // da li je bafer prepunjen preko kapaciteta

    /** Da li je bafer završio prijem i sadrži poruku */
    bool isavailable();         // da li je bafer završio prijem i sadrži poruku
    

    char buffer[maxbuf + 1];    // bafer
    
    /** vraća dužinu sadržaja u baferu */
    int getLength();            
    
    /** vraća trenutnu poziciju u baferu */
    int getPosition();
};

#endif