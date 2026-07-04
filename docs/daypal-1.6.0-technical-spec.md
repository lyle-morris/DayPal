# DayPal 1.6.0 Technical Specification

Status: Release source of truth
Branch: `daypal-1.6.0-dev`
Visual source of truth: `DayPal-Wireframes-1.6.0-Final.pdf`

## 1. Scope

This specification defines the DayPal 1.6.0 watchface, configuration page, theme system, icon resource mapping, weather/location behavior, analytics setting, and release hygiene requirements.

All active release files must align to DayPal 1.6.0. Stale manifest references, old image references, old release numbers, and old document content must be removed from the active development branch.

## 2. Runtime surfaces

DayPal 1.6.0 has three runtime surfaces:

1. Watchface C app: `src/c/main.c`
2. PebbleKit JS companion: `src/pkjs/index.js`
3. Hosted configuration page: app-config web UI

Responsibilities:

| Surface | Responsibility |
| --- | --- |
| Watchface C app | Layout, theme resolution, icon resource selection, metric rendering, persisted watch settings |
| PebbleKit JS | AppMessage bridge, settings sync, weather requests, geocoding/manual location, temperature conversion, analytics dispatch |
| Configuration page | User settings UI, theme selector, metric slot selector, manual location inputs, analytics opt-in, save/reset actions |

## 3. Watchface layout constants

Target display:

- Width: `200`
- Height: `228`
- Metric tray: left column
- Clock/date area: right column
- Divider: vertical rule between metric tray and clock area
- Metric icon size: `32 x 32`
- Date text: abbreviated month, day, weekday

Current implementation constants should remain aligned to the rendered wireframes:

```c
#define SCREEN_W 200
#define SCREEN_H 228
#define METRIC_TRAY_W 58
#define DIVIDER_X 58
#define DIVIDER_W 1
#define CLOCK_X 59
#define CLOCK_W 141
#define ICON_SIZE 32
#define METRIC_ROW_H 52
#define METRIC_ICON_X 13
#define METRIC_VALUE_Y_OFFSET 30
```

## 4. Settings model

### 4.1 Watch-facing settings

These values must be sent to the watch when relevant:

| Setting | Type | Watch key |
| --- | --- | --- |
| `theme` | integer | `0` |
| `slot_1_metric` | integer | `1` |
| `slot_2_metric` | integer | `2` |
| `slot_3_metric` | integer | `3` |
| `slot_4_metric` | integer | `4` |
| `show_leading_zero` | boolean | `5` |
| `use_24_hour` | boolean | `6` |
| `reverse_theme` | boolean | `7` |
| `weather_temp` | integer | `10` |
| `weather_code` | integer | `11` |
| `weather_valid` | boolean | `12` |
| `request_weather` | boolean | `20` |
| `settings_ready` | boolean | `21` |

### 4.2 Companion/config-only settings

These values may remain in the configuration page and PebbleKit JS layer if they do not need watch-side persistence:

| Setting | Type | Notes |
| --- | --- | --- |
| `use_celsius` | boolean | JS converts or requests weather in the selected unit before sending `weather_temp`. |
| `manual_location` | boolean | Controls whether current location or manual lookup is used. |
| `manual_postal_code` | string | Optional manual location input. |
| `manual_city` | string | Optional manual location input. |
| `analytics_enabled` | boolean | Enables anonymous analytics only. |

## 5. Enums

### 5.1 Metrics

```c
typedef enum {
  METRIC_WEATHER = 0,
  METRIC_HEART_RATE = 1,
  METRIC_BATTERY = 2,
  METRIC_CALORIES = 3,
  METRIC_STEPS = 4,
  METRIC_NONE = 5
} MetricType;
```

Default slot order:

```c
{ METRIC_WEATHER, METRIC_HEART_RATE, METRIC_BATTERY, METRIC_STEPS }
```

### 5.2 Themes

Active selectable themes:

```c
typedef enum {
  THEME_DEFAULT = 0,
  THEME_BLUE = 1,
  THEME_PINK = 2,
  THEME_GREEN = 3,
  THEME_ORANGE = 5,
  THEME_BLACK = 7,
  THEME_RED = 8,
  THEME_YELLOW = 9
} ThemeType;
```

Implementation may keep fallback enum values for stored settings compatibility, but the configuration page must only expose the active selectable themes listed above.

### 5.3 Weather conditions

```c
typedef enum {
  WEATHER_SUNNY = 0,
  WEATHER_PARTLY_CLOUDY = 1,
  WEATHER_RAINY = 2,
  WEATHER_STORM = 3,
  WEATHER_SNOW = 4,
  WEATHER_FOG = 5,
  WEATHER_CLOUDY = 6,
  WEATHER_UNKNOWN = 7
} WeatherCondition;
```

## 6. Theme color tokens

### 6.1 Watchface tokens

| Token | Hex |
| --- | --- |
| `COLOR_BLACK` | `#000000` |
| `COLOR_WHITE` | `#FFFFFF` |
| `COLOR_DIVIDER_DEFAULT` | `#555555` |
| `COLOR_UNAVAILABLE` | `#666666` |
| `COLOR_BLUE` | `#0055FF` |
| `COLOR_ORANGE` | `#FF5500` |
| `COLOR_GREEN` | `#00AA55` |
| `COLOR_PINK` | `#FF00AA` |
| `COLOR_RED` | `#FF0055` |
| `COLOR_YELLOW` | `#FFCC55` |
| `COLOR_DEFAULT_WEATHER` | `#FFFF00` |
| `COLOR_DEFAULT_HEART_RATE` | `#FF0000` |
| `COLOR_DEFAULT_BATTERY` | `#00FF00` |
| `COLOR_DEFAULT_STEPS` | `#00AAFF` |
| `COLOR_DEFAULT_CALORIES` | `#FF5500` |

### 6.2 Configuration page tokens

| Token | Hex |
| --- | --- |
| Page background | `#000000` |
| Panel background | near-black/dark gray per wireframe |
| Primary text | `#FFFFFF` |
| Secondary text | muted gray |
| Focus border | `#00AAFF` |
| Error background | dark red |
| Error border/text | red accent |
| Primary action | blue accent |

## 7. Theme resolver

### 7.1 Default theme

Default is a multi-color theme.

```c
background: #000000
divider: #555555
clock_text: #FFFFFF
metric_text: #FFFFFF
unavailable_text: #666666
weather: #FFFF00
heart_rate: #FF0000
battery: #00FF00
steps: #00AAFF
calories: #FF5500
individual_metric_colors: true
```

### 7.2 Non-reverse themes

For Blue, Orange, Green, Pink, Yellow, Red, and Black in non-reverse mode:

- Background is the selected theme background.
- Clock text is black.
- Metric text is black.
- Divider/border is black.
- Metric icons are black.
- Unavailable metric values are `#666666`.

| Theme | Background | Foreground |
| --- | --- | --- |
| Blue | `#0055FF` | `#000000` |
| Orange | `#FF5500` | `#000000` |
| Green | `#00AA55` | `#000000` |
| Pink | `#FF00AA` | `#000000` |
| Yellow | `#FFCC55` | `#000000` |
| Red | `#FF0055` | `#000000` |
| Black | `#FFFFFF` | `#000000` |

### 7.3 Reverse themes

For reverse mode:

- Background is black.
- Clock text, metric text, divider/border, and metric icons use the selected theme color.
- Reverse Black uses white foreground.
- Reverse Default remains the default multi-color theme on black unless explicitly changed later.

| Theme | Background | Foreground |
| --- | --- | --- |
| Default | `#000000` | default multi-color metric set |
| Blue | `#000000` | `#0055FF` |
| Orange | `#000000` | `#FF5500` |
| Green | `#000000` | `#00AA55` |
| Pink | `#000000` | `#FF00AA` |
| Yellow | `#000000` | `#FFCC55` |
| Red | `#000000` | `#FF0055` |
| Black | `#000000` | `#FFFFFF` |

## 8. Icon Manager

### 8.1 Requirement

The Icon Manager must select pre-generated PNG resources. It must not runtime-tint Pebble bitmaps.

### 8.2 Required asset colors

To match the wireframes, the resource directory and manifest must include icon assets for these colors:

| Purpose | Required colors |
| --- | --- |
| White icons | `#FFFFFF` |
| Black icons | `#000000` |
| Reverse Blue | `#0055FF` |
| Reverse Orange | `#FF5500` |
| Reverse Green | `#00AA55` |
| Reverse Pink | `#FF00AA` |
| Reverse Yellow | `#FFCC55` |
| Reverse Red | `#FF0055` |
| Default weather | `#FFFF00` |
| Default heart rate | `#FF0000` |
| Default battery | `#00FF00` |
| Default steps | `#00AAFF` |
| Default calories | `#FF5500` |

### 8.3 Icon resource naming

Naming pattern:

```text
IMAGE_<METRIC_OR_WEATHER_OR_BATTERY>_<COLOR_NAME>
```

File pattern:

```text
resources/images/<category>/<base_name>_32_<HEX>.png
```

Examples:

```text
resources/images/weather/sunny_32_FFFFFF.png
resources/images/weather/sunny_32_000000.png
resources/images/weather/sunny_32_0055FF.png
resources/images/battery/battery_100_32_000000.png
resources/images/heart_rate/heart_rate_32_FF0000.png
resources/images/steps/steps_32_00AAFF.png
```

### 8.4 Color selection rules

`resource_id_for_metric()` must accept both metric and resolved icon color intent.

Required mapping:

| Theme state | Weather icon | Heart icon | Battery icon | Steps icon | Calories icon |
| --- | --- | --- | --- | --- | --- |
| Default | `#FFFF00` | `#FF0000` | `#00FF00` | `#00AAFF` | `#FF5500` |
| Non-reverse color themes | `#000000` | `#000000` | `#000000` | `#000000` | `#000000` |
| Non-reverse Black | `#000000` | `#000000` | `#000000` | `#000000` | `#000000` |
| Reverse Blue | `#0055FF` | `#0055FF` | `#0055FF` | `#0055FF` | `#0055FF` |
| Reverse Orange | `#FF5500` | `#FF5500` | `#FF5500` | `#FF5500` | `#FF5500` |
| Reverse Green | `#00AA55` | `#00AA55` | `#00AA55` | `#00AA55` | `#00AA55` |
| Reverse Pink | `#FF00AA` | `#FF00AA` | `#FF00AA` | `#FF00AA` | `#FF00AA` |
| Reverse Yellow | `#FFCC55` | `#FFCC55` | `#FFCC55` | `#FFCC55` | `#FFCC55` |
| Reverse Red | `#FF0055` | `#FF0055` | `#FF0055` | `#FF0055` | `#FF0055` |
| Reverse Black | `#FFFFFF` | `#FFFFFF` | `#FFFFFF` | `#FFFFFF` | `#FFFFFF` |

### 8.5 Weather icon selection

Weather icon resource selection must combine weather condition and icon color.

Conditions:

- Sunny
- Partly cloudy
- Cloudy
- Rain
- Storm
- Snow
- Fog
- Unknown fallback: partly cloudy

### 8.6 Battery icon selection

Battery icon resource selection must combine charge bucket, charging state, and icon color.

Buckets:

| Charge percent | Bucket icon |
| --- | --- |
| `0` to `25` | `25` bucket, except exactly empty may use `0` bucket |
| `26` to `50` | `50` bucket |
| `51` to `80` | `75` bucket |
| `81` to `100` | `100` bucket |

Charging icon variants must exist for each bucket.

## 9. Metric rendering

### 9.1 Text formatting

| Metric | Formatting |
| --- | --- |
| Weather | integer temperature, no degree symbol |
| Heart rate | integer BPM |
| Battery | integer charge, no percent symbol |
| Steps | compact integer with `k` abbreviation |
| Calories | compact integer with `k` abbreviation |
| Missing/unavailable | `---` |

### 9.2 Text color

- Available text follows theme metric text or per-metric color.
- Unavailable text is `#666666`.
- Icons follow the Icon Manager mapping, not the unavailable text color.

## 10. Time and date

- `show_leading_zero = true`: hour renders as two digits.
- `show_leading_zero = false`: hour renders without leading zero.
- `use_24_hour = true`: hour renders in 24-hour format.
- `use_24_hour = false`: hour renders in 12-hour format.
- Date format: `%b %d, %a`.

## 11. Weather and location

### 11.1 Current-location mode

When manual location is off:

- JS requests device coordinates.
- JS requests weather for current coordinates.
- JS sends `weather_temp`, `weather_code`, and `weather_valid` to the watch.

### 11.2 Manual-location mode

When manual location is on:

- User can enter ZIP/postal code or city.
- Either field can satisfy manual location.
- JS geocodes the manual input.
- JS requests weather for the resolved location.
- JS sends weather data to the watch.

### 11.3 Temperature unit

- `use_celsius = false`: weather temperature is Fahrenheit.
- `use_celsius = true`: weather temperature is Celsius.
- Unit conversion should happen before the numeric temperature is sent to the watch.
- Watchface renders the numeric value without a degree symbol.

## 12. Configuration page

### 12.1 Page structure

The final configuration page must include:

1. DayPal title and intro copy.
2. Support DayPal donation panel.
3. Anonymous analytics opt-in panel.
4. Settings panel.
5. Theme panel.
6. Metric slots panel.
7. Reset layout action.
8. Save settings action.

### 12.2 Settings panel controls

- Display leading zero on the hours.
- Use 24-hour format.
- Use Celsius for temperature.
- Set location manually.

### 12.3 Manual location UI states

| State | Behavior |
| --- | --- |
| Off | Hide ZIP/postal code and City fields. |
| On | Show ZIP/postal code and City fields. |
| Error | Show helper text and field-level missing/invalid information. |
| Success | Preserve entered value and allow save. |

Error copy:

```text
Enter a ZIP/postal code or city to set the location manually.
```

Field-level error copy:

```text
Missing/invalid information
```

### 12.4 Theme panel controls

- Theme chips: Default, Blue, Orange, Green, Pink, Red, Yellow, Black.
- Reverse theme colors checkbox.
- Selected theme state must be visually clear.
- Focus border: `#00AAFF`.

### 12.5 Metric slots

Slot selectors must support:

- Weather
- Heart rate
- Battery
- Steps
- Calories
- Empty/null

Default slot values:

```text
Slot 1: Weather
Slot 2: Heart rate
Slot 3: Battery
Slot 4: Steps
```

## 13. App manifest requirements

Both Pebble manifests must remain aligned:

- `package.json`
- `appinfo.json`

Rules:

- Both manifests must reference the same active resource set.
- Neither manifest may reference missing files.
- Neither manifest may reference stale icon paths.
- Both manifests must expose the DayPal 1.6.0 settings keys needed by CloudPebble.
- CloudPebble import/build must not fail due to manifest/file mismatch.

## 14. Release hygiene

Before release approval:

- Search active branch for old release-number references and remove them.
- Search active branch for stale image references and remove them.
- Verify `/docs` reflects DayPal 1.6.0 only.
- Verify manifest resources exist in `resources/images`.
- Verify CloudPebble can import and build the branch.
- Verify watchface icon colors across all theme states.
- Verify configuration page theme redlines.

## 15. Acceptance test checklist

### 15.1 Watchface

- Default theme shows multi-color metric icons and values.
- Non-reverse Blue shows blue background with black text, border, and icons.
- Non-reverse Orange shows orange background with black text, border, and icons.
- Non-reverse Green shows green background with black text, border, and icons.
- Non-reverse Pink shows pink background with black text, border, and icons.
- Non-reverse Yellow shows yellow background with black text, border, and icons.
- Non-reverse Red shows red background with black text, border, and icons.
- Non-reverse Black shows white background with black text, border, and icons.
- Reverse Blue shows black background with blue text, border, and icons.
- Reverse Orange shows black background with orange text, border, and icons.
- Reverse Green shows black background with green text, border, and icons.
- Reverse Pink shows black background with pink text, border, and icons.
- Reverse Yellow shows black background with yellow text, border, and icons.
- Reverse Red shows black background with red text, border, and icons.
- Reverse Black shows black background with white text, border, and icons.

### 15.2 Metrics

- Weather has no degree symbol.
- Battery has no percent symbol.
- Empty data shows `---`.
- Unavailable values render `#666666`.
- Large step and calorie values compact correctly.

### 15.3 Configuration

- Save settings persists theme, reverse theme, slots, time format, temperature unit, and manual location settings.
- Reset layout restores default slot order and default theme behavior.
- Manual location validates ZIP/postal code or city.
- Analytics copy states anonymous usage only.
- Donation/support panel appears as designed.
