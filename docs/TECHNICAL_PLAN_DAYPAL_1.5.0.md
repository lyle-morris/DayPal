# DayPal 1.5.0 Technical Plan

## Scope

This release updates the DayPal watchface theme system, configuration page, and icon resource set while preserving the approved 200 x 228 watchface layout and existing metric slot behavior.

## Repositories

| Repo | Files | Purpose |
|---|---|---|
| `lyle-morris/DayPal` | `src/c/main.c`, `src/pkjs/index.js`, `appinfo.json`, `package.json` | Watchface runtime, weather mapping, resources |
| `lyle-morris/DayMate-config` | `index.html` | Hosted settings/configuration page |

## Theme Values

Keep numeric theme values stable where possible:

- `0` Default / Multi-Color
- `1` Blue
- `2` Pink
- `3` Green
- `4` White
- `5` Orange
- `7` Black
- `8` Red
- `9` Yellow
- `10` Legacy Yellow alias for prior Khaki work
- `11` Gray

## Final Theme Palette

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

## `src/c/main.c`

Implementation requirements:

- Use `black_text_theme()` helper for Blue, Orange, Green, Pink, Yellow, Red, Gray, and White.
- Use `white_text_theme()` helper for Black.
- Keep Default as individual metric colors:
  - Weather `#FFFF00`
  - Heart rate `#FF0000`
  - Battery `#00FF00`
  - Steps `#00AAFF`
  - Calories `#FF5500`
- Normalize legacy Khaki theme value `10` to Yellow behavior.
- Use black icon variants for all black-text themes.
- Use white icon variants for Black.
- Use color icon variants for Default.

## Battery Logic

Battery buckets:

- `0%` => 0
- `1-25%` => 25
- `26-50%` => 50
- `52-80%` => 75
- `81-100%` => 100

Charging states must follow the same bucket mapping.

## Weather Logic

Weather condition values:

- `0` Sunny
- `1` Partly Cloudy
- `2` Rain
- `3` Storm
- `4` Snow
- `5` Fog
- `6` Cloudy
- `7` Unknown / unsupported

Open-Meteo weather code `3` should map to Cloudy.

## Configuration Page

Hosted URL remains:

`https://lyle-morris.github.io/DayMate-config/`

Implementation requirements:

- Remove theme dropdown.
- Render color tiles.
- Add centered checkmark overlay on selected tile.
- Rename Khaki to Yellow.
- Use fixed Save settings footer.
- Keep Reset layout as secondary action in Metric slots.
- Normalize incoming theme value `10` to Yellow value `9`.

## Resource Manifest

Declare resources in both `appinfo.json` and `package.json` when required by the active build/import path.

Required resource families:

- Weather: sunny, partly cloudy, cloudy, rainy, storm, snow, fog in black/yellow/white.
- Battery: 0, 25, 50, 75, 100 and charging 0, 25, 50, 75, 100 in black/green/white.
- Heart rate: black/red/white.
- Calories: black/orange/white.
- Steps: black/blue/white.

## Build Validation

- Import `daypal-1.5.0-dev` into CloudPebble.
- Confirm no manifest asset errors.
- Build for Emery.
- Open settings page and confirm tile UI, checkmark selected state, Yellow naming, and fixed Save footer.
- Save each theme and verify watchface colors update.
