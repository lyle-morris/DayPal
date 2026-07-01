# DayPal 1.5.0 PRD

## Overview

DayPal 1.5.0 is a Pebble Time 2 contrast, theme, and configuration polish release. The release keeps the existing 200 x 228 watchface layout and metric slot model intact while improving visibility with a stronger theme palette, refreshed icons, and a simplified configuration page.

Source of truth: updated DayPal 1.5.0 wireframe.

## Goals

- Improve watchface readability on Pebble Time 2 with higher-contrast theme colors.
- Rename Khaki to Yellow across user-facing surfaces.
- Keep Default as the Multi-Color theme.
- Use black text/icons on Blue, Orange, Green, Pink, Yellow, Red, Gray, and White themes.
- Use white text/icons on Black theme.
- Add a centered checkmark overlay to the selected configuration tile.
- Make Save settings a fixed footer in the configuration page.
- Keep Reset layout inside Metric slots.
- Add Cloudy as a distinct weather state.
- Add a 75 battery icon state for 52-80%, including charging.

## Non-Goals

- No change to screen size, clock placement, or metric tray dimensions.
- No new metric types.
- No change to the current settings transport model.
- No touch interaction or app-launch behavior in this release.

## Theme Palette

| Theme | Background | Text/Icon | Border | Notes |
|---|---:|---:|---:|---|
| Default / Multi-Color | `#000000` | `#FFFFFF` | `#555555` | Individual metric colors |
| Blue | `#0055FF` | `#000000` | `#000000` | Black text/icons |
| Orange | `#FF5500` | `#000000` | `#000000` | Black text/icons |
| Green | `#00AA55` | `#000000` | `#000000` | Black text/icons |
| Pink | `#FF00AA` | `#000000` | `#000000` | Black text/icons |
| Yellow | `#FFCC55` | `#000000` | `#000000` | Replaces Khaki |
| Red | `#FF0055` | `#000000` | `#000000` | Black text/icons |
| Black | `#000000` | `#FFFFFF` | `#FFFFFF` | White text/icons |
| Gray | `#AAAAAA` | `#000000` | `#000000` | Black text/icons |
| White | `#FFFFFF` | `#000000` | `#000000` | Black text/icons |

## Default Theme Metric Colors

- Weather: `#FFFF00`
- Heart Rate: `#FF0000`
- Battery: `#00FF00`
- Steps: `#00AAFF`
- Calories: `#FF5500`

## Configuration Page Requirements

- Remove the theme dropdown.
- Display theme tiles in this order: Default, Blue, Orange, Green, Pink, Red, Yellow, Black, White, Gray.
- Show selected tile using a centered checkmark overlay.
- Keep section background at white with 25% opacity.
- Keep section padding at 16px and border radius at 4px.
- Make Save settings a fixed footer.
- Reset layout restores:
  - Slot 1: Weather
  - Slot 2: Heart rate
  - Slot 3: Battery
  - Slot 4: Steps

## Battery Requirements

- `0%` uses 0 icon.
- `1-25%` uses 25 icon.
- `26-50%` uses 50 icon.
- `52-80%` uses 75 icon.
- `81-100%` uses 100 icon.
- Charging states must have matching 0, 25, 50, 75, and 100 icons.

## Weather Requirements

Supported weather states:

- Sunny
- Partly Cloudy
- Cloudy
- Rain
- Storm
- Snow
- Fog

## Success Criteria

- App imports into CloudPebble without missing manifest resources.
- Build succeeds for the Emery target.
- Theme changes save from the config page and apply to the watchface.
- Yellow appears everywhere instead of Khaki.
- Save settings remains visible at the bottom of the config page.
- Selected theme tile shows a checkmark overlay.
