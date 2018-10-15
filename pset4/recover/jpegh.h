#include <stdint.h>

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;

//Structure for jpeg Header

 typedef struct
{
    BYTE aByte;
    BYTE bByte;
    BYTE cByte;
    BYTE dByte;
} __attribute__((__packed__))
JPEGHEADER;