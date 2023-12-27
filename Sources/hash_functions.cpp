#include "hash_functions.h"
#include <string>

using std::string;

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

unsigned hash_c_string(const char* s)
{
   unsigned h = FIRSTH;
   while (*s) {
     h = (h * A) ^ (s[0] * B);
     s++;
   }
   return h; // or return h % C;
}

int pow (int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if ((y % 2) == 0)
        return pow (x, y / 2) * pow (x, y / 2);
    else
        return x * pow (x, y / 2) * pow (x, y / 2);

}

const int PRIME_CONST = 31;

unsigned hash_string(string key) {
    int hashCode = 0;
    for (unsigned i = 0; i < key.length(); i++) {
        hashCode += key[i] * pow(PRIME_CONST, i);
    }
    return hashCode;
}