#ifndef __TEXT_DISPLAY__
#define __TEXT_DISPLAY__

// Set the font type for display(eg.TXT0508)
void SetFont(unsigned char fontx);
// Put an ASCII char onto the screen
void putchar(unsigned char AscChar);
// Display xNumber AscChars
void putchars(unsigned char AscChar, unsigned char xNumber);
// Tile screen with one letter
void aTile(unsigned char AscChar);

#endif
