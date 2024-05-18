#ifndef DEBUG_UTILS_H

/**
 * Currently facing an issue that
 * when using serial prints over the USB Serial
 * the ESP32 is not working w/o an open USB Serial terminal/ monitor
 * activate serial in platform.ini if needed:

[env]
build_flags =
  '-DSERIAL_DEBUG'
*/

#ifdef SERIAL_DEBUG
#define serialBegin(x) Serial.begin(x)
#define serialPrint(x) Serial.print(x)
#define serialPrintln(x) Serial.println(x)
#define serialFlush() Serial.flush()
#else
#define serialBegin(x)
#define serialPrint(x)
#define serialPrintln(x)
#define serialFlush()
#endif

#endif
