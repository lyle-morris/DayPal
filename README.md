# DayPal

DayPal is a configurable Pebble Time 2 watchface built for fast, glanceable daily context. It pairs a bold stacked clock with a compact metric tray, selectable high-contrast themes, and native Pebble services for weather, health, and battery status.

## Current Release

**DayPal 1.5.0**

DayPal 1.5.0 is a readability and polish release focused on stronger Pebble Time 2 contrast, updated icon states, improved battery visibility, and a cleaner configuration experience.

## Platform

- Target device: Pebble Time 2
- Target platform: Emery
- SDK: Pebble SDK 3
- App type: Watchface
- Companion: PebbleKit JS
- Configuration: Hosted Pebble configuration page

## Features

- Large stacked hour and minute display
- Centered date line
- Four configurable metric slots
- Supported metrics:
  - Weather
  - Heart Rate
  - Battery
  - Calories
  - Steps
- `None` option for hiding individual metric slots
- Clock-only mode when all metric slots are hidden
- Duplicate metric selections supported
- Pebble system 12-hour and 24-hour time support
- Optional leading zero
- Persistent settings on the phone and watch
- Open-Meteo weather integration with cached fallback
- Pebble Health integration for steps, heart rate, and active calories
- Battery percentage and charging-state icon support

## Themes

DayPal 1.5.0 includes a high-contrast theme palette tuned for better Pebble Time 2 visibility.

| Theme | Background | Text/Icon Behavior |
|---|---:|---|
| Default / Multi-Color | `#000000` | White clock/date with colorful metric icons |
| Blue | `#0055FF` | Black text/icons |
| Orange | `#FF5500` | Black text/icons |
| Green | `#00AA55` | Black text/icons |
| Pink | `#FF00AA` | Black text/icons |
| Yellow | `#FFCC55` | Black text/icons |
| Red | `#FF0055` | Black text/icons |
| Black | `#000000` | White text/icons |
| Gray | `#AAAAAA` | Black text/icons |
| White | `#FFFFFF` | Black text/icons |

Default / Multi-Color metric colors:

- Weather: `#FFFF00`
- Heart Rate: `#FF0000`
- Battery: `#00FF00`
- Steps: `#00AAFF`
- Calories: `#FF5500`

## Design

DayPal uses a fixed 200 x 228 layout tuned for Pebble Time 2.

- Metric tray width: 58 px
- Divider width: 1 px
- Clock region width with metrics: 141 px
- Full-screen clock region when all metrics are hidden
- Slot positions remain fixed, even when other slots are set to `None`
- Default theme uses colored metric icons
- Solid-color themes use black or white icons based on theme contrast
- Default divider color: `#555555`

The visual direction is minimal, high-contrast, and readable at a glance.

## Configuration

DayPal is configured from the Pebble mobile app configuration flow. Settings are sent through PebbleKit JS, applied on the watch through AppMessage, and persisted locally.

Configurable options:

- Theme selection using color tiles
- Leading zero
- Slot 1 metric
- Slot 2 metric
- Slot 3 metric
- Slot 4 metric
- Reset layout

Default layout:

- Slot 1: Weather
- Slot 2: Heart Rate
- Slot 3: Battery
- Slot 4: Steps

The configuration page uses a selected tile checkmark and a fixed Save settings footer.

Hosted configuration repo:

```text
https://github.com/lyle-morris/DayMate-config
```

Hosted configuration page:

```text
https://lyle-morris.github.io/DayMate-config/
```

## Weather

Weather uses the phone companion to request location and fetch current conditions from Open-Meteo. The watch displays the current temperature and a themed weather icon. The last valid weather result is cached so a temporary location or network failure does not immediately clear the weather display.

Supported weather states:

- Sunny
- Partly Cloudy
- Cloudy
- Rain
- Storm
- Snow
- Fog

## Battery

DayPal 1.5.0 adds a new 75% battery icon state and matching charging icon state.

Battery icon mapping:

- `0%` → 0 icon
- `1-25%` → 25 icon
- `26-50%` → 50 icon
- `52-80%` → 75 icon
- `81-100%` → 100 icon

## Health Metrics

DayPal reads supported Pebble Health metrics directly on the watch:

- Steps
- Heart rate
- Active calories

If a health metric is unavailable on the device, DayPal shows `---` for that metric instead of failing or displaying stale placeholder data.

## Development

Primary development branch for DayPal 1.5.0:

```text
daypal-1.5.0-dev
```

The project is intended to build in CloudPebble or a compatible Pebble/Rebble SDK environment.

Required resource categories:

- Roboto font resources
- Weather icons
- Battery icons
- Heart rate icon
- Calories icon
- Steps icon

## Release Status

DayPal 1.5.0 has been tested successfully in CloudPebble with updated themes, weather icons, battery icons, heart rate icons, configuration tile selection, fixed Save settings footer, and Yellow theme naming.

## Release Notes Summary

DayPal 1.5.0 includes:

- New high-contrast theme colors
- Khaki renamed to Yellow
- New Cloudy weather icon
- Filled weather icons for improved contrast
- Battery color changed from purple to green in Default / Multi-Color
- Heart rate color changed from green to red in Default / Multi-Color
- New 75% battery and charging battery icon state
- Updated settings page with color tiles, selected checkmark, Reset layout, and fixed Save settings footer
