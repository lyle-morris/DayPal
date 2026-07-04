# DayPal 1.6.0 Icon Manager

Status: Release source of truth
Branch: `daypal-1.6.0-dev`
Visual source of truth: `docs/daypal-1.6.0-wireframes.pdf`

## 1. Decision

DayPal 1.6.0 continues to use an Icon Manager.

The Icon Manager is responsible for selecting the correct pre-generated PNG resource for each metric, weather condition, battery state, theme, and reverse-theme state.

The Icon Manager must not rely on runtime bitmap tinting or pixel recoloring on the Pebble device. Runtime recoloring was tested during development and is not reliable enough for release because Pebble bitmap formats can render corrupted output when directly manipulated.

## 2. Responsibilities

The Icon Manager owns:

- Weather icon resource selection.
- Battery icon resource selection.
- Metric icon resource selection.
- Theme-to-icon-color mapping.
- Battery bucket mapping.
- Weather condition fallback mapping.
- Bitmap creation, drawing, and cleanup.

The main drawing layer should not hard-code white resources. It should request the correct icon resource from the Icon Manager using the current metric and resolved theme state.

## 3. Required theme color behavior

### 3.1 Default theme

Default is a multi-color theme.

| Metric | Required icon color | Hex |
| --- | --- | --- |
| Weather | Yellow | `#FFFF00` |
| Heart rate | Red | `#FF0000` |
| Battery | Green | `#00FF00` |
| Steps | Cyan/blue | `#00AAFF` |
| Calories | Orange | `#FF5500` |

### 3.2 Non-reverse themes

For all non-reverse color themes, icons must be black.

| Theme | Background | Icon color |
| --- | --- | --- |
| Blue | `#0055FF` | `#000000` |
| Orange | `#FF5500` | `#000000` |
| Green | `#00AA55` | `#000000` |
| Pink | `#FF00AA` | `#000000` |
| Yellow | `#FFCC55` | `#000000` |
| Red | `#FF0055` | `#000000` |
| Black option | `#FFFFFF` | `#000000` |

### 3.3 Reverse themes

For reverse themes, the watchface background is black and icons use the selected theme foreground color.

| Theme | Background | Icon color |
| --- | --- | --- |
| Blue reverse | `#000000` | `#0055FF` |
| Orange reverse | `#000000` | `#FF5500` |
| Green reverse | `#000000` | `#00AA55` |
| Pink reverse | `#000000` | `#FF00AA` |
| Yellow reverse | `#000000` | `#FFCC55` |
| Red reverse | `#000000` | `#FF0055` |
| Black reverse | `#000000` | `#FFFFFF` |

Reverse Default keeps the default multi-color metric behavior on black unless a later approved requirement changes it.

## 4. Required resource colors

The resource directory and manifests must include pre-generated PNGs for these color families:

| Family | Hex | Used for |
| --- | --- | --- |
| White | `#FFFFFF` | Reverse Black and fallback white icons |
| Black | `#000000` | All non-reverse color themes |
| Blue | `#0055FF` | Reverse Blue |
| Orange | `#FF5500` | Reverse Orange and default Calories |
| Green | `#00AA55` | Reverse Green |
| Pink | `#FF00AA` | Reverse Pink |
| Red | `#FF0055` | Reverse Red |
| Yellow | `#FFCC55` | Reverse Yellow |
| Default weather yellow | `#FFFF00` | Default Weather |
| Default heart red | `#FF0000` | Default Heart rate |
| Default battery green | `#00FF00` | Default Battery |
| Default steps cyan/blue | `#00AAFF` | Default Steps |

## 5. Resource naming pattern

Resource names should use stable, explicit names.

```text
IMAGE_WEATHER_<CONDITION>_<COLOR>
IMAGE_BATTERY_<BUCKET>_<COLOR>
IMAGE_BATTERY_CHARGING_<BUCKET>_<COLOR>
IMAGE_HEART_RATE_<COLOR>
IMAGE_CALORIES_<COLOR>
IMAGE_STEPS_<COLOR>
```

File names must follow the icon source naming convention:

```text
resources/images/<category>/<base>_32_<HEX>.png
```

Examples:

```text
resources/images/weather/sunny_32_000000.png
resources/images/weather/sunny_32_FFFF00.png
resources/images/battery/battery_100_32_00FF00.png
resources/images/heart_rate/heart_rate_32_FF0000.png
resources/images/steps/steps_32_00AAFF.png
```

## 6. Weather icon selection

Weather resource selection combines weather condition and selected icon color.

Supported conditions:

- Sunny
- Partly cloudy
- Cloudy
- Rain
- Storm
- Snow
- Fog

Unknown weather condition falls back to Partly cloudy.

## 7. Battery icon selection

Battery resource selection combines charging state, battery bucket, and selected icon color.

| Battery percent | Bucket |
| --- | --- |
| `0` | `0` |
| `1` to `25` | `25` |
| `26` to `50` | `50` |
| `51` to `80` | `75` |
| `81` to `100` | `100` |

Charging state uses the `IMAGE_BATTERY_CHARGING_<BUCKET>_<COLOR>` resource family.

## 8. Suggested implementation shape

The Icon Manager may be implemented inside `src/c/main.c` or extracted later, but the logic should be centralized.

Suggested internal model:

```c
typedef enum {
  ICON_COLOR_WHITE,
  ICON_COLOR_BLACK,
  ICON_COLOR_BLUE,
  ICON_COLOR_ORANGE,
  ICON_COLOR_GREEN,
  ICON_COLOR_PINK,
  ICON_COLOR_RED,
  ICON_COLOR_YELLOW,
  ICON_COLOR_DEFAULT_WEATHER,
  ICON_COLOR_DEFAULT_HEART,
  ICON_COLOR_DEFAULT_BATTERY,
  ICON_COLOR_DEFAULT_STEPS
} IconColor;
```

Suggested selector responsibilities:

```c
static IconColor icon_color_for_metric(DayPalTheme theme, MetricType metric);
static uint32_t weather_resource_id(IconColor color);
static uint32_t battery_resource_id(IconColor color);
static uint32_t resource_id_for_metric(MetricType metric, IconColor color);
```

## 9. Acceptance criteria

- No metric uses white-only resources across all themes.
- Default theme uses the approved multi-color metric palette.
- Non-reverse color themes use black icon resources.
- Reverse color themes use selected theme-color resources.
- Reverse Black uses white icon resources.
- Weather condition changes preserve icon color mapping.
- Battery bucket and charging state preserve icon color mapping.
- CloudPebble build succeeds with no missing `RESOURCE_ID_*` values.
- `package.json` and `appinfo.json` register every icon resource used by the Icon Manager.
