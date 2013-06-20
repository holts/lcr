// Some macros used often

#ifndef MACROS_H
#define MACROS_H

// Set bit
#define sbi(port, bit) (port) |= (1 << (bit))
// Clear bit
#define cbi(port, bit) (port) &= ~(1 << (bit))

#endif
