# DayPal 1.6.0 Technical Specification

Status: Development baseline
Target release: July 5, 2026
Branch: `daypal-1.6.0-dev`

## 1. Architecture

DayPal has three primary runtime surfaces:

1. Pebble watchface C application (`src/c/main.c`)
2. PebbleKit JS companion (`src/pkjs/index.js`)
3. Hosted app-config web page (`DayPal-config`)

The app-config page owns user input. PebbleKit JS owns configuration persistence on the phone, weather/geocoding requests, analytics dispatch, and AppMessage communication. The C application owns watch-side persistence, theme resolution, metric rendering, and the Icon Manager.

## 2. Naming cleanup

All active 1.6.0 code and documentation must use DayPal naming.

Legacy identifiers currently present in code, including `DayMateSettings`, `DayMateTheme`, `DAYMATE_QA_*`, `daymate_settings`, `daymate_weather`, and the DayMate config URL, must be migrated to DayPal equivalents without changing user behavior.

## 3. Settings model

Canonical settings object:

```text
theme: integer
slot_1_metric: integer
slot_2_metric: integer
slot_3_metric: integer
slot_4_metric: integer
show_leading_zero: boolean
use_24_hour: boolean
use_celsius: boolean
manual_location: boolean
manual_postal_code: string
manual_city: string
reverse_theme: boolean
analytics_enabled: boolean
```

Defaults:

```text
theme = 0
slot_1_metric = Weather
slot_2_metric = Heart Rate
slot_3_metric = Battery
slot_4_metric = Steps
show_leading_zero = true
use_24_hour = true
use_celsius = false
manual_location = false
manual_postal_code = ""
manual_city = ""
reverse_theme = false
analytics_enabled = true
```

Phone persistence key: `daypal_settings`
Weather cache key: `daypal_weather`

## 4. AppMessage contract

Existing keys remain stable for backward compatibility.

| Key | Name | Direction | Type |
|---|---|---|---|
| 0 | theme | Phone -> Watch | int |
| 1 | slot_1_metric | Phone -> Watch | int |
| 2 | slot_2_metric | Phone -> Watch | int |
| 3 | slot_3_metric | Phone -> Watch | int |
| 4 | slot_4_metric | Phone -> Watch | int |
| 5 | show_leading_zero | Phone -> Watch | bool/int |
| 6 | use_24_hour | Phone -> Watch | bool/int |
| 7 | reverse_theme | Phone -> Watch | bool/int |
| 10 | weather_temp | Phone -> Watch | int |
| 11 | weather_code | Phone -> Watch | int |
| 12 | weather_valid | Phone -> Watch | bool/int |
| 20 | request_weather | Watch -> Phone | signal |
| 21 | settings_ready | Watch -> Phone | signal |

`use_celsius`, manual-location fields, and `analytics_enabled` remain phone-side settings. The watch only needs the resolved temperature value and does not need location or analytics data.

## 5. Time behavior

The watch must not rely solely on the Pebble system clock preference. DayPal 1.6.0 explicitly respects `use_24_hour` from configuration.

- `use_24_hour = true`: format hour with `%H`.
- `use_24_hour = false`: format hour with `%I`, then apply the leading-zero preference.
- `show_leading_zero = false`: remove the leading zero from a single-digit hour.
- `show_leading_zero = true`: preserve two-digit hour formatting.

No AM/PM indicator is displayed.

## 6. Weather and location

### Current location

When `manual_location = false`, use `navigator.geolocation` as in 1.5.0.

### Manual location

When `manual_location = true`:

1. Trim ZIP/postal code and City values.
2. If both are empty, reject the config response and show the approved validation state in app-config.
3. Build a geocoding query from the provided value.
4. Either field alone is sufficient.
5. Resolve the query to latitude/longitude.
6. Request weather from Open-Meteo using the resolved coordinates.

The unused manual-location field must not be treated as invalid when the other field resolves successfully.

### Units

Weather request uses:

- `temperature_unit=celsius` when `use_celsius = true`
- `temperature_unit=fahrenheit` when `use_celsius = false`

The watch receives only the rounded numeric temperature. No degree symbol or unit suffix is rendered.

### Cache

Cache must be unit-aware and location-mode-aware. A cached Fahrenheit value must never be reused after switching to Celsius, and a current-location cache must not be reused for a different manual location.

Recommended cache identity:

```text
mode + normalized manual query + temperature unit
```

## 7. Theme model

Public 1.6.0 theme set:

- Default
- Blue `#0055FF`
- Orange `#FF5500`
- Green `#00AA55`
- Pink `#FF00AA`
- Red `#FF0055`
- Yellow `#FFCC55`
- Black

Legacy theme IDs may be normalized during migration, but the app-config page must expose only the approved 1.6.0 theme set.

### Default

```text
background = #000000
clock/date = #FFFFFF
divider = #555555
weather = #FFFF00
heart rate = #FF0000
battery = #00FF00
steps = #00AAFF
calories = #FF5500
```

### Solid, non-reversed

```text
background = selected theme color
foreground = #000000
```

Black is the special light treatment shown in the approved wireframe:

```text
background = #FFFFFF
foreground = #000000
```

### Solid, reversed

```text
background = #000000
foreground = selected theme color
```

Black reversed:

```text
background = #000000
foreground = #FFFFFF
```

Default reverse is a no-op visually.

## 8. Icon Manager

The current 1.5.0 implementation selects separate black, metric-color, and white PNG resources. 1.6.0 replaces that variant-selection model.

### Source assets

- One white-on-transparent PNG per icon state.
- Weather retains one source image per weather condition.
- Battery retains one source image per charge bucket and charging state.
- Heart Rate, Calories, and Steps each retain one source image.

### Responsibilities

The Icon Manager owns:

- icon resource selection by metric/state
- bitmap loading
- runtime recoloring
- draw positioning
- cleanup

Suggested interface:

```c
void icon_manager_draw_metric(
  GContext *ctx,
  MetricType metric,
  WeatherCondition weather,
  BatteryChargeState battery,
  GColor color,
  GRect bounds
);
```

### Recoloring

Source white pixels are replaced with the resolved icon color before drawing. Transparent pixels remain transparent. The implementation must be validated on Emery for supported bitmap formats before duplicate resource variants are removed from `package.json`.

Migration must occur in two steps:

1. Implement and validate runtime recoloring while existing variants remain available.
2. Remove duplicate black/color resources only after visual QA passes.

This prevents a resource cleanup from blocking recovery if palette replacement behaves differently in CloudPebble or on device.

## 9. App-config implementation

Visual source of truth: final 1.6.0 wireframe.

Target: approximately 98% visual fidelity at the intended Pebble configuration viewport.

Implementation rules:

- Use semantic HTML controls underneath custom visual styling.
- Use Material Symbols / Google Material icons for UI icons shown in the wireframe.
- Do not export those app-config icons as PNGs unless a browser compatibility issue requires it.
- Preserve keyboard focus and visible focus treatment.
- Theme tiles use the documented background, text, focus-border, and focus-icon colors.
- Manual-location fields are conditionally shown only when manual location is enabled.
- Fixed Save settings footer behavior should match the wireframe.
- Donation button opens `https://ko-fi.com/lylemorris` externally.
- No payment processing or payment data may pass through DayPal.

## 10. Analytics

Analytics is anonymous and opt-out capable.

Allowed event categories:

- config page opened
- settings saved
- theme selected
- reverse theme enabled/disabled
- 24-hour preference enabled/disabled
- Celsius preference enabled/disabled
- manual/current location mode selected
- metric type assigned to a slot
- reset layout used

Prohibited analytics payloads:

- ZIP/postal code
- City
- latitude/longitude
- weather values
- health values
- personal information
- persistent cross-service identifiers

`analytics_enabled = false` must prevent future event dispatch immediately after the setting is applied.

The analytics service is not selected by this technical specification. Service selection must be reviewed for privacy, cost, browser support, and compatibility with the hosted app-config environment.

## 11. Migration and compatibility

1. Read existing `daymate_settings` if `daypal_settings` does not exist.
2. Normalize the legacy object into the 1.6.0 settings model.
3. Save the migrated object under `daypal_settings`.
4. Preserve existing slot and theme selections where they map to the approved theme set.
5. Use defaults for new settings.
6. Migrate weather cache only if its unit and location identity are known; otherwise discard it safely.

## 12. Build and QA gates

Before release:

- CloudPebble build succeeds with no missing resource IDs.
- 12-hour and 24-hour modes pass.
- Leading-zero combinations pass.
- Fahrenheit/Celsius switching passes.
- Current location and manual ZIP/postal/city paths pass.
- Manual-location validation passes.
- All theme and reverse combinations match the wireframe.
- All metric slots and None pass.
- Icon recoloring passes for every weather and battery state.
- Analytics opt-out prevents dispatch.
- No analytics payload contains prohibited data.
- 1.5.0 fallback/unavailable styling remains unchanged.
- 1.5.0 time alignment remains unchanged.
