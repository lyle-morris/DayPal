# DayPal 1.5.0 Technical Plan

## Scope

DayPal 1.5.0 updates the watchface theme system, resource manifest, weather mapping, battery state mapping, and hosted configuration page while preserving the approved 200 x 228 watchface layout and metric slot behavior.

## Repositories

| Repo | Files | Purpose |
|---|---|---|
| `lyle-morris/DayPal` | `src/c/main.c`, `src/pkjs/index.js`, `appinfo.json`, `package.json` | Watchface runtime, weather mapping, resources |
| `lyle-morris/DayMate-config` | `index.html` | Hosted settings/configuration page |

## Theme Values

Keep numeric values stable to protect existing saved settings:

- `0` Default / Multi-Color
- `1` Blue
- `2` Pink
- `3` Green
- `4` White
- `5` Orange
- `7` Black
- `8` Red
- `9` Yellow
- `10` Legacy Khaki value; normalize to Yellow when encountered
- `11` Gray

## Theme Palette

| Theme | Background | Text/Icon | Border |
|---|---:|---:|---:|
| Default / Multi-Color | `#000000` | `#FFFFFF` | `#555555` |
| Blue | `#0055FF` | `#000000` | `#000000` |
| Orange | `#FF5500` | `#000000` | `#000000` |
| Green | `#00AA55` | `#000000` | `#000000` |
| Pink | `#FF00AA` | `#000000` | `#000000` |
| Yellow | `#FFCC55` | `#000000` | `#000000` |
| Red | `#FF0055` | `#000000` | `#000000` |
| Black | `#000000` | `#FFFFFF` | `#FFFFFF` |
| Gray | `#AAAAAA` | `#000000` | `#000000` |
| White | `#FFFFFF` | `#000000` | `#000000` |

## Runtime Implementation

- `main.c` should use helper functions for black text and white text themes to avoid repeated color tuples.
- The Default theme keeps individual metric colors:
  - Weather `#FFFF00`
  - Heart Rate `#FF0000`
  - Battery `#00FF00`
  - Calories `#FF5500`
  - Steps `#00AAFF`
- All non-default themes use a single foreground color for all text and icons.
- Blue, Orange, Green, Pink, Yellow, Red, Gray, and White use black icons.
- Black uses white icons.

## Battery Mapping

- `0%` -> `battery_0`
- `1-25%` -> `battery_25`
- `26-50%` -> `battery_50`
- `52-80%` -> `battery_75`
- `81-100%` -> `battery_100`

Charging must use the matching `battery_charging_*` icon.

## Weather Mapping

Weather code mapping must support:

- Sunny
- Partly Cloudy
- Cloudy
- Rain
- Storm
- Snow
- Fog

Open-Meteo overcast code `3` maps to Cloudy.

## Configuration Page

- Hosted from `DayMate-config/index.html`.
- The watch app opens `https://lyle-morris.github.io/DayMate-config/`.
- Theme selector uses tiles instead of a dropdown.
- Selected tile uses a centered checkmark overlay.
- Save settings is fixed to the footer.
- Khaki is renamed to Yellow.
- Legacy theme value `10` should be saved back as `9`.

## Validation

- Import branch into CloudPebble.
- Build for Emery.
- Open Settings and verify the new hosted config page appears.
- Change each theme and confirm foreground/icon contrast.
- Verify battery 75 state resources compile and render.
- Verify Cloudy weather resource compiles and renders.
