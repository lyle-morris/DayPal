# DayPal

DayPal is a configurable Pebble Time 2 watchface built for fast, glanceable daily context. It pairs a bold stacked clock with a compact metric tray, selectable themes, and native Pebble services for weather, health, and battery status.

## Release

**DayPal 1.0.0**

This release focuses on the core watchface experience: reliable configuration, stable layout behavior, live metrics, and a polished visual foundation for Pebble Time 2.

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
- Duplicate metric selections
- Pebble system 12-hour and 24-hour time support
- Optional leading zero
- Eight visual themes:
  - Default
  - Blue
  - Pink
  - Green
  - White
  - Orange
  - Dark Blue
  - Black
- Persistent settings on the phone and watch
- Open-Meteo weather integration with cached fallback
- Pebble Health integration for steps, heart rate, and active calories
- Battery percentage and charging-state icon support

## Design

DayPal uses a fixed 200 x 228 layout tuned for Pebble Time 2.

- Metric tray width: 58 px
- Divider width: 1 px
- Clock region width with metrics: 141 px
- Full-screen clock region when all metrics are hidden
- Slot positions remain fixed, even when other slots are set to `None`
- Default theme uses colored metric icons
- Solid-color themes use white metric icons
- White theme uses black text and icons

The approved visual direction is minimalist, high-contrast, and readable at a glance.

## Configuration

DayPal is configured from the Pebble mobile app configuration flow. Settings are sent through PebbleKit JS, applied on the watch through AppMessage, and persisted locally.

Configurable options:

- Theme
- Leading zero
- Slot 1 metric
- Slot 2 metric
- Slot 3 metric
- Slot 4 metric

## Weather

Weather uses the phone companion to request location and fetch current conditions from Open-Meteo. The watch displays the current temperature and a themed weather icon. The last valid weather result is cached so a temporary location or network failure does not immediately clear the weather display.

## Health Metrics

DayPal reads supported Pebble Health metrics directly on the watch:

- Steps
- Heart rate
- Active calories

If a health metric is unavailable on the device, DayPal shows `---` for that metric instead of failing or displaying stale placeholder data.

## Development

Primary development branch:

```text
daymate-1.7.0-dev
```

The repository name and branch name still reflect the original development project. The product name for this release is **DayPal**.

## Build Notes

The project is intended to build in CloudPebble or a compatible Pebble/Rebble SDK environment.

Required resource categories:

- Roboto font resources
- Weather icons
- Battery icons
- Heart rate icon
- Calories icon
- Steps icon

## Release Status

DayPal 1.0.0 has validated the core configuration pipeline, theme switching, persistent settings, clock layout modes, fixed metric slot positioning, battery display, steps, and heart rate on hardware. Weather and calories should be included in the final hardware smoke test before release packaging.
