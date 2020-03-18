/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "numtostr.h"

#include "../inc/MarlinConfig.h"
#include "../core/utility.h"
char conv[8] = { 0 };

#define DIGIT(n) ('0' + (n))
#define DIGIMOD(n, f) DIGIT((n)/(f) % 10)
#define RJDIGIT(n, f) ((n) >= (f) ? DIGIMOD(n, f) : ' ')
#define MINUSOR(n, alt) (n >= 0 ? (alt) : (n = -n, '-'))

// Convert a full-range unsigned 8bit int to a percentage
const char* ui8tostr4pctrj(const uint8_t i) {
  const uint8_t n = ui8_to_percent(i);
  snprintf_P(conv + 3, 5, PSTR("%3d%%"), n);
  return &conv[3];
}

// Convert unsigned 8bit int to string 123 format
const char* ui8tostr3rj(const uint8_t i) {
  snprintf_P(conv + 4, 4, PSTR("%3d"), i);
  return &conv[4];
}

// Convert signed 8bit int to rj string with 123 or -12 format
const char* i8tostr3rj(const int8_t x) {
  snprintf_P(conv + 4, 4, PSTR("%3d"), x);
  return &conv[4];
}

#if HAS_PRINT_PROGRESS_PERMYRIAD
  // Convert unsigned 16-bit permyriad to percent with 100 / 23 / 23.4 / 3.45 format
  const char* permyriadtostr4(const uint16_t xx) {
    if (xx >= 10000)
      return "100";
    else if (xx >= 1000) {
      conv[3] = DIGIMOD(xx, 1000);
      conv[4] = DIGIMOD(xx, 100);
      conv[5] = '.';
      conv[6] = DIGIMOD(xx, 10);
      return &conv[3];
    }
    else if (xx % 100 == 0) {
      conv[4] = ' ';
      conv[5] = RJDIGIT(xx, 1000);
      conv[6] = DIGIMOD(xx, 100);
      return &conv[4];
    }
    else {
      conv[3] = DIGIMOD(xx, 100);
      conv[4] = '.';
      conv[5] = DIGIMOD(xx, 10);
      conv[6] = RJDIGIT(xx, 1);
      return &conv[3];
    }
  }
#endif

// Convert unsigned 16bit int to string 12345 format
const char* ui16tostr5rj(const uint16_t xx) {
  snprintf_P(conv + 2, 6, PSTR("%5d"), xx);
  return &conv[2];
}

// Convert unsigned 16bit int to string 1234 format
const char* ui16tostr4rj(const uint16_t xx) {
  snprintf_P(conv + 3, 5, PSTR("%4d"), xx);
  return &conv[3];
}


// Convert unsigned 16bit int to string 123 format
const char* ui16tostr3rj(const uint16_t xx) {
  snprintf_P(conv + 4, 4, PSTR("%3d"), xx);
  return &conv[4];
}

// Convert signed 16bit int to rj string with 123 or -12 format
const char* i16tostr3rj(const int16_t x) {
  snprintf_P(conv + 4, 4, PSTR("%3d"), x);
  return &conv[4];
}

// Convert unsigned 16bit int to lj string with 123 format
const char* i16tostr3left(const int16_t i) {
  snprintf_P(conv + 4, 4, PSTR("%d"), i);
  return &conv[4];
}

// Convert signed 16bit int to rj string with 1234, _123, -123, _-12, or __-1 format
const char* i16tostr4signrj(const int16_t i) {
  snprintf_P(conv + 3, 5, PSTR("%4d"), i);
  return &conv[3];
}
// Convert unsigned float to string with 1.23 format
const char* ftostr12ns(const float &f) {
  snprintf_P(conv + 3, 5, PSTR("%.2f"), fabs(f));
  return &conv[3];
}

// Convert signed float to fixed-length string with 12.34 / -2.34 or 023.45 / -23.45 format
const char* ftostr42_52(const float &f) {
  if (f <= -10 || f >= 100) return ftostr52(f); // need more digits
  snprintf_P(conv + 2, 6, PSTR("%5.2f"), (double)f);
  return &conv[2];
}

// Convert signed float to fixed-length string with 023.45 / -23.45 format
const char* ftostr52(const float &f) {
  snprintf_P(conv + 1, 7, PSTR("%06.2f"), (double)f);
  return &conv[1];
}

// Convert signed float to fixed-length string with 12.345 / -2.345 or 023.456 / -23.456 format
const char* ftostr43_53(const float &f) {
  if (f <= -10 || f >= 100) return ftostr53(f); // need more digits
  snprintf_P(conv + 1, 7, PSTR("%6.3f"), (double)f);
  return &conv[1];
}

// Convert signed float to fixed-length string with 023.456 / -23.456 format
const char* ftostr53(const float &f) {
  snprintf_P(conv, 8, PSTR("%07.3f"), (double)f);
  return &conv[0];
}

#if ENABLED(LCD_DECIMAL_SMALL_XY)

  // Convert float to rj string with 1234, _123, -123, _-12, 12.3, _1.2, or -1.2 format
  const char* ftostr4sign(const float &f) {
    if (f <= -10 || f >= 100) return i16tostr4signrj((int)f);
    snprintf_P(conv + 3, 5, PSTR( "%f"), f);
    return &conv[3];
  }

#endif

// Convert float to fixed-length string with +123.4 / -123.4 format
const char* ftostr41sign(const float &f) {
  snprintf_P(conv + 1, 7, PSTR("%+06.1f"), (double)f);
  return &conv[1];
}

// Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
const char* ftostr43sign(const float &f, char plus/*=' '*/) {
  snprintf_P(conv + 1, 7, PSTR("%c%.3f"), f < 0 ? '-' : plus, fabs(f));
  return &conv[1];
}

// Convert signed float to string (5 digit) with -1.2345 / _0.0000 / +1.2345 format
const char* ftostr54sign(const float &f, char plus/*=' '*/) {
  snprintf_P(conv, 8, PSTR("%c%.4f"), f < 0 ? '-' : plus, fabs(f));
  return &conv[0];
}

// Convert unsigned float to rj string with 12345 format
const char* ftostr5rj(const float &f) {
  return ui16tostr5rj(uint16_t(fabs(f) + 0.5f));
}

// Convert signed float to string with +1234.5 format
const char* ftostr51sign(const float &f) {
  snprintf_P(conv, 8, PSTR("%+07.1f"), (double)f);
  return conv;
}

// Convert signed float to string with +123.45 format
const char* ftostr52sign(const float &f) {
  snprintf_P(conv, 8, PSTR("%+07.2f"), (double)f);
  return conv;
}

// Convert unsigned float to string with ____4.5, __34.5, _234.5, 1234.5 format
const char* ftostr51rj(const float &f) {
  snprintf_P(conv, 8, PSTR("%7.1f"), fabs(f));
  return conv;
}

// Convert signed float to space-padded string with -_23.4_ format
const char* ftostr52sp(const float &f) {
  snprintf(conv, 8, "%c%6.2f", f < 0 ? '-' : ' ', fabs(f));
  // strip extraneous decimals
  for (int i = 6; i > 3; i--) {
    if (conv[i] != '0' && conv[i] != '.') break;
    conv[i] = 0;
  }

  return conv;
}
