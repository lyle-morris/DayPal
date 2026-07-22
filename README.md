# DayPal

DayPal is a configurable Pebble Time 2 watchface built for fast, glanceable daily context. It pairs a bold stacked clock with a compact metric tray, selectable high-contrast themes, and native Pebble services for weather, health, and battery status.

## Current Release

**DayPal 1.6.0**

## Upcoming Patch

**DayPal 1.6.1**

DayPal 1.6.1 moves the companion configuration page to the consolidated production directory. Watchface behavior and the configuration UI are otherwise unchanged.

## Release History

- **DayPal 1.0.0** — Released
- **DayPal 1.5.0** — Released
- **DayPal 1.6.0** — Released
- **DayPal 1.6.1** — In development

## Platform

- Target device: Pebble Time 2
- Target platform: Emery
- SDK: Pebble SDK 3
- App type: Watchface
- Companion: PebbleKit JS
- Configuration: Hosted Pebble configuration page
- Hosting repository: `lyle-morris/Hosting`
- Legacy compatibility host: `lyle-morris/DayPal-Hosting`

## 1.6.0 Scope

DayPal 1.6.0 includes the following planned updates:

- Match final DayPal 1.6.0 wireframes
- Restore original metric icon shapes
- Correct icon color mapping for Default, non-reverse, and reverse theme states
- Add Use 24-hour format setting
- Add Weather location setting with current-location and manual-location behavior
- Add Manual location input using ZIP/postal code or city
- Add Celsius temperature setting
- Add Reverse theme colors option
- Add anonymous analytics opt-in/out
- Add donation/support panel in the configuration page

## Not in Scope for 1.6.0

The following items should not be carried into the DayPal 1.6.0 backlog:

- Saved custom themes
- User-created theme colors
- Account sign-in
- Collection of personal, health, or precise location data

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
- 12-hour and 24-hour time support
- Optional leading zero
- Persistent settings on the phone and watch
- Open-Meteo weather integration with cached fallback
- Pebble Health integration for steps, heart rate, and active calories
- Battery charge and charging-state icon support

## Themes

DayPal 1.6.0 includes the active theme set shown in the final wireframes.

| Theme | Non-reverse behavior | Reverse behavior |
|---|---|---|
| Default / Multi-Color | `#000000` background, white clock/date, multi-color metric icons | Same as Default unless changed by an approved requirement |
| Blue | `#0055FF` background, black text/icons | `#000000` background, `#0055FF` text/icons |
| Orange | `#FF5500` background, black text/icons | `#000000` background, `#FF5500` text/icons |
| Green | `#00AA55` background, black text/icons | `#000000` background, `#00AA55` text/icons |
| Pink | `#FF00AA` background, black text/icons | `#000000` background, `#FF00AA` text/icons |
| Yellow | `#FFCC55` background, black text/icons | `#000000` background, `#FFCC55` text/icons |
| Red | `#FF0055` background, black text/icons | `#000000` background, `#FF0055` text/icons |
| Black | `#FFFFFF` background, black text/icons | `#000000` background, white text/icons |

Default / Multi-Color metric icon colors:

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
- Non-reverse color themes use black icons
- Reverse color themes use selected theme-color icons
- Default divider color: `#555555`

The visual direction is minimal, high-contrast, and readable at a glance.

## Configuration

DayPal is configured from the Pebble mobile app configuration flow. Settings are sent through PebbleKit JS, applied on the watch through AppMessage, and persisted locally.

Configurable options:

- Theme selection using color tiles
- Use 24-hour format
- Leading zero
- Weather location: Current or Manual
- Manual location: ZIP/postal code or city
- Celsius temperature
- Reverse theme colors
- Slot 1 metric
- Slot 2 metric
- Slot 3 metric
- Slot 4 metric
- Reset layout
- Anonymous analytics opt-in/out

Default layout:

- Slot 1: Weather
- Slot 2: Heart Rate
- Slot 3: Battery
- Slot 4: Steps

The configuration page uses a selected tile checkmark and a fixed Save settings footer.

Canonical hosted configuration:

```text
https://lyle-morris.github.io/Hosting/apps/daypal/prod/app-config.html
```

QA configuration:

```text
https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html
```

The original `DayPal-Hosting` URL remains online for compatibility with DayPal 1.6.0 and earlier installations.

## Weather

Weather uses the phone companion to request location and fetch current conditions from Open-Meteo. DayPal 1.6.0 supports choosing either the current phone location or a manually entered location using ZIP/postal code or city. The watch displays the current temperature and a themed weather icon. The last valid weather result is cached so a temporary location or network failure does not immediately clear the weather display.

Supported weather states:

- Sunny
- Partly Cloudy
- Cloudy
- Rain
- Storm
- Snow
- Fog

## Battery

Battery icon mapping:

- `0%` → 0 icon
- `1-25%` → 25 icon
- `26-50%` → 50 icon
- `51-80%` → 75 icon
- `81-100%` → 100 icon

The watchface displays the numeric battery charge without a percent symbol.

## Health Metrics

DayPal reads supported Pebble Health metrics directly on the watch:

- Steps
- Heart rate
- Active calories

If a health metric is unavailable on the device, DayPal shows `---` for that metric instead of failing or displaying stale placeholder data.

## Analytics

DayPal 1.6.0 includes anonymous analytics to help understand configuration usage and general adoption patterns. Analytics should not collect personally identifiable information, exact user location, health values, or unique personal data. The configuration page must include an analytics opt-in/out section.

## Development

Primary development branch for DayPal 1.6.0:

```text
daypal-1.6.0-dev
```

Hosted configuration, analytics dashboard, and location QA work belongs in:

```text
lyle-morris/DayPal-Hosting
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

DayPal 1.6.0 is currently in development.

## Release Notes Summary

DayPal 1.6.0 planned updates include:

- Use 24-hour format setting
- Weather location setting with Current and Manual options
- Manual ZIP/postal code or city location input
- Celsius temperature setting
- Reverse theme colors option
- Anonymous analytics
- DayPal-Hosting configuration, analytics, and location QA support
