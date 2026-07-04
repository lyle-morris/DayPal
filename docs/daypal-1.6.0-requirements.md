# DayPal 1.6.0 Product Requirements Document

Status: Release source of truth
Branch: `daypal-1.6.0-dev`
Visual source of truth: `DayPal-Wireframes-1.6.0-Final.pdf`

## 1. Product summary

DayPal 1.6.0 is a configurable Pebble Time 2 watchface release focused on theme accuracy, user-controlled display settings, weather/location control, privacy-forward analytics, and a refreshed configuration page.

The release must match the final 1.6.0 wireframes. The watchface and configuration page must not depend on older release behavior, old labels, or stale manifest/resource references.

## 2. Goals

- Match the 1.6.0 watchface wireframes for all active themes.
- Render metric icons in the correct color for each theme and reverse-theme state.
- Support configurable metric slots.
- Support leading-zero and 24-hour time display settings.
- Support Celsius temperature display through configuration and weather handling.
- Support manual weather location entry by ZIP/postal code or city.
- Provide clear configuration states for default layout, manual-location off, manual-location on, validation error, and success.
- Provide an analytics opt-in setting that is anonymous and excludes personal, health, and location data.
- Keep all active release documentation, manifests, and code references aligned to DayPal 1.6.0.

## 3. Non-goals

- Public app store release automation.
- Account sign-in.
- Collection of personal, health, or precise location data.
- Theme colors outside the final 1.6.0 palette.
- Runtime image tinting on the Pebble device.

## 4. Supported platforms

- Pebble Time 2 target platform: `emery`.
- CloudPebble-compatible project structure.
- Watchface runtime in C.
- PebbleKit JS companion for configuration, weather, location, and analytics communication.
- Hosted configuration page for user settings.

## 5. Watchface layout requirements

### 5.1 Canvas

- Target screen: Pebble Time 2, 200 x 228.
- Metric tray: left column.
- Clock area: right column.
- Divider: vertical rule between metric tray and clock area.
- Date: bottom of clock area.

### 5.2 Default visible metric slots

The default metric slot order must be:

1. Weather
2. Heart rate
3. Battery
4. Steps

Calories must be available as a selectable metric, but it is not part of the default layout.

### 5.3 Metric value formatting

- Weather: numeric temperature only; no degree symbol.
- Battery: numeric charge only; no percent symbol.
- Heart rate: numeric BPM only.
- Steps: compact count; large values may abbreviate to `k`.
- Calories: compact count; large values may abbreviate to `k`.
- Missing/unavailable values: `---`.
- Unavailable value color: `#666666`.

### 5.4 Metric data examples from wireframes

The release must support these display ranges and examples:

| Metric | Examples |
| --- | --- |
| Steps | `---`, `0`, `99`, `999`, `9999`, `99k`, `999k` |
| Battery | `---`, `0`, `25`, `50`, `75`, `100` |
| Heart rate | `---`, `60`, `100`, `150`, `200` |
| Weather | `---`, `-8`, `-32`, `60`, `100` |
| Calories | `---`, `9`, `99`, `999`, `9999`, `99k`, `999k` |

## 6. Weather requirements

### 6.1 Weather conditions

The weather icon system must support:

- Sunny
- Partly cloudy
- Rain
- Storm
- Snow
- Fog
- Cloudy

### 6.2 Weather display

- Weather metric shows the weather icon above the numeric temperature value.
- Temperature displays without degree symbol.
- Temperature unit is controlled by the configuration setting.
- Weather must update based on current location when manual location is off.
- Weather must update based on manually entered ZIP/postal code or city when manual location is on.
- Manual location must support international postal/city lookup where the weather provider supports it.

## 7. Theme requirements

### 7.1 Active selectable themes

The configuration page must expose these active themes:

- Default
- Blue
- Orange
- Green
- Pink
- Red
- Yellow
- Black

### 7.2 Default theme

Default theme is a multi-color watchface theme.

| Property | Color |
| --- | --- |
| Background | `#000000` |
| Font | `#FFFFFF` |
| Border/divider | `#555555` |
| Weather | `#FFFF00` |
| Heart rate | `#FF0000` |
| Battery | `#00FF00` |
| Steps | `#00AAFF` |
| Calories | `#FF5500` |

### 7.3 Non-reverse themes

For non-reverse color themes, the background uses the selected theme color and all text, borders, and metric icons use black.

| Theme | Background | Text | Border | Metric icons |
| --- | --- | --- | --- | --- |
| Blue | `#0055FF` | `#000000` | `#000000` | `#000000` |
| Orange | `#FF5500` | `#000000` | `#000000` | `#000000` |
| Green | `#00AA55` | `#000000` | `#000000` | `#000000` |
| Pink | `#FF00AA` | `#000000` | `#000000` | `#000000` |
| Yellow | `#FFCC55` | `#000000` | `#000000` | `#000000` |
| Red | `#FF0055` | `#000000` | `#000000` | `#000000` |
| Black | `#FFFFFF` | `#000000` | `#000000` | `#000000` |

### 7.4 Reverse themes

For reverse themes, the background is black and text, borders, and metric icons use the selected theme color.

| Theme | Background | Text | Border | Metric icons |
| --- | --- | --- | --- | --- |
| Default | `#000000` | `#FFFFFF` | `#555555` | default metric colors |
| Blue | `#000000` | `#0055FF` | `#0055FF` | `#0055FF` |
| Orange | `#000000` | `#FF5500` | `#FF5500` | `#FF5500` |
| Green | `#000000` | `#00AA55` | `#00AA55` | `#00AA55` |
| Pink | `#000000` | `#FF00AA` | `#FF00AA` | `#FF00AA` |
| Yellow | `#000000` | `#FFCC55` | `#FFCC55` | `#FFCC55` |
| Red | `#000000` | `#FF0055` | `#FF0055` | `#FF0055` |
| Black | `#000000` | `#FFFFFF` | `#FFFFFF` | `#FFFFFF` |

### 7.5 Icon color behavior

Metric icons must not always render white. Icon selection must follow the active theme state:

- Default: per-metric icon colors.
- Non-reverse Blue/Orange/Green/Pink/Yellow/Red/Black: black icons.
- Reverse Blue/Orange/Green/Pink/Yellow/Red: selected theme-color icons.
- Reverse Black: white icons.
- Unavailable values use `#666666` text; icon color continues to follow the active theme unless a future requirement explicitly changes unavailable icon styling.

## 8. Time and date requirements

- Display hour and minute as stacked clock text.
- Support leading zero on/off for hours.
- Support 12-hour and 24-hour display.
- Date format: abbreviated month, day, and weekday, for example `Jul 12, Wed`.

## 9. Configuration page requirements

### 9.1 Default layout

The configuration page must include:

- DayPal title.
- Introductory helper text.
- Support DayPal panel.
- Donate button.
- Help improve DayPal analytics panel.
- Anonymous analytics opt-in checkbox.
- Settings panel.
- Theme panel.
- Metric slots panel.
- Reset layout action.
- Save settings action.

### 9.2 Settings panel

Settings must include:

- Display leading zero on the hours.
- Use 24-hour format.
- Use Celsius for temperature.
- Set location manually.

### 9.3 Manual location states

When manual location is off:

- ZIP/postal code and city fields are hidden.
- Weather uses current location.

When manual location is on:

- ZIP/postal code field is shown.
- City field is shown.
- User may enter either ZIP/postal code or city.

Error state:

- Show helper text: `Enter a ZIP/postal code or city to set the location manually.`
- Mark invalid/missing fields with `Missing/invalid information`.

Success state:

- Preserve the entered location value.
- Continue to show manual location fields.
- Save settings without resetting unrelated settings.

### 9.4 Theme panel

The theme panel must include:

- Theme options: Default, Blue, Orange, Green, Pink, Red, Yellow, Black.
- Reverse theme colors checkbox.
- Selected theme visual state.
- Focus border color: `#00AAFF`.

Theme panel CSS redlines:

| Theme state | Background | Text | Focus border | Focus icon |
| --- | --- | --- | --- | --- |
| Default, non-reverse | `#000000` | `#FFFFFF` | `#00AAFF` | `#FFFFFF` |
| Blue, non-reverse | `#0055FF` | `#000000` | `#00AAFF` | `#FFFFFF` |
| Orange, non-reverse | `#FF5500` | `#000000` | `#00AAFF` | `#FFFFFF` |
| Green, non-reverse | `#00AA55` | `#000000` | `#00AAFF` | `#FFFFFF` |
| Pink, non-reverse | `#FF00AA` | `#000000` | `#00AAFF` | `#FFFFFF` |
| Red, non-reverse | `#FF0055` | `#000000` | `#00AAFF` | `#FFFFFF` |
| Yellow, non-reverse | `#FFCC55` | `#000000` | `#00AAFF` | `#FFFFFF` |
| Black, non-reverse | `#FFFFFF` | `#000000` | `#00AAFF` | `#000000` |
| Default, reverse | `#000000` | `#FFFFFF` | `#00AAFF` | `#FFFFFF` |
| Blue, reverse | `#000000` | `#0055FF` | `#00AAFF` | `#0055FF` |
| Orange, reverse | `#000000` | `#FF5500` | `#00AAFF` | `#FF5500` |
| Green, reverse | `#000000` | `#00AA55` | `#00AAFF` | `#00AA55` |
| Pink, reverse | `#000000` | `#FF00AA` | `#00AAFF` | `#FF00AA` |
| Red, reverse | `#000000` | `#FF0055` | `#00AAFF` | `#FF0055` |
| Yellow, reverse | `#000000` | `#FFCC55` | `#00AAFF` | `#FFCC55` |
| Black, reverse | `#000000` | `#FFFFFF` | `#00AAFF` | `#FFFFFF` |

### 9.5 Metric slots panel

- User can customize which metric appears in each watchface slot.
- Slots support Weather, Heart rate, Battery, Steps, Calories, and empty/null.
- Empty/null slots render no active metric value and must not break layout.
- Default slot values must be Weather, Heart rate, Battery, Steps.

## 10. Analytics requirements

- Analytics opt-in must be disabled unless the user explicitly enables it.
- Analytics copy must clearly state anonymous usage data only.
- Analytics must not collect personal data, health data, or location data.
- Analytics should support unique users, repeat unique users, and broad geographic insight only when privacy-safe.

## 11. Acceptance criteria

- No active docs, manifests, or release files reference an older release number.
- Watchface matches 1.6.0 wireframe theme colors in default, non-reverse, and reverse states.
- Metric icons are not white-only; icon color follows theme requirements.
- Weather displays without a degree symbol.
- Battery displays without a percent symbol.
- Configuration page includes all final 1.6.0 panels and states.
- Manual location works for ZIP/postal code and city.
- Analytics opt-in copy matches privacy requirements.
- CloudPebble import/build does not fail due to stale manifest resource references.
