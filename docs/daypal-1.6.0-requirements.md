# DayPal 1.6.0 Requirements Baseline

Status: Approved for development
Target release: July 5, 2026
Visual source of truth: DayPal-Wireframes-1.6.0-Final.pdf

## Release objective

DayPal 1.6.0 expands user configuration, weather location control, theme behavior, privacy controls, and internal icon handling while preserving the core DayPal watchface layout established in 1.5.0.

## In scope

- Display leading zero setting
- Use 24-hour format setting
- Use Celsius for temperature setting
- Current-location weather behavior when manual location is off
- Manual weather location using either ZIP/postal code or City
- Manual-location validation, error, and success states
- Theme selection: Default, Blue, Orange, Green, Pink, Red, Yellow, Black
- Reverse theme colors setting
- Four configurable metric slots
- Reset layout action
- Anonymous usage analytics
- Visible analytics opt-out control
- Support DayPal donation section linked to Ko-fi
- Icon Manager architecture
- One white-on-transparent PNG source asset per icon state, recolored at runtime
- App-config page implementation targeting approximately 98% visual fidelity to the approved wireframe
- Google Material icons / Material Symbols for app-config UI icons where shown in the approved wireframe

## Out of scope

- Fallback/unavailable styling fixes; resolved in 1.5.0
- Time clipping/alignment fixes; resolved in 1.5.0
- New metrics beyond Weather, Heart Rate, Battery, Steps, Calories, and None
- Donation payment processing inside DayPal
- Collection of personal, health, or location data for analytics

## Approved behavior rules

### Time

- When Use 24-hour format is on, the watch uses 24-hour time.
- When Use 24-hour format is off, the watch uses 12-hour time.
- Display leading zero applies to the hour display.

### Temperature

- When Use Celsius for temperature is on, weather temperature is displayed in Celsius.
- When off, temperature is displayed in Fahrenheit.
- DayPal does not display a degree symbol or temperature-unit suffix on the watchface.

### Location

- When Set location manually is off, weather uses the phone's current location.
- When Set location manually is on, the user may provide either ZIP/postal code or City.
- Either field alone is sufficient.
- Both fields blank is invalid.
- Error copy: Enter a ZIP/postal code or city to set the location manually.
- The unused field must not show an error when the other field resolves successfully.

### Themes

- Default uses black background, white clock/date text, #555555 divider, and metric-specific icon colors.
- Non-reversed solid themes use the selected theme color as background and black foreground.
- Reversed solid themes use black background and the selected theme color as foreground.
- Black non-reversed uses white background with black foreground.
- Black reversed uses black background with white foreground.
- Default reverse is visually equivalent to Default.

### Icons

- Maintain one white-on-transparent PNG source asset per icon state.
- The Icon Manager owns resource selection, runtime recoloring, draw behavior, and cleanup.
- Default theme uses metric-specific colors.
- Solid themes use the resolved foreground color.
- Unavailable states use the existing 1.5.0 unavailable behavior and must not be reworked as part of 1.6.0.

### Analytics

- Checkbox label: Share anonymous usage analytics.
- Supporting copy: Share anonymous usage data to help improve DayPal. No personal, health, or location data is collected.
- Default state: checked.
- Analytics must not collect personal information, health values, location values, ZIP/postal code, city, or other location identifiers.
- If the user opts out, no analytics events may be sent after the preference is applied.

### Donation

- Section title: Support DayPal.
- Copy: DayPal is free to use. If you enjoy it, consider leaving a small tip to support future updates and improvements.
- Button label: Donate.
- Donation service: Ko-fi.
- Donation URL: https://ko-fi.com/lylemorris
- The Donate button opens the Ko-fi page externally.
- Payment processing and payment data remain external to DayPal.

## Configuration defaults

- Display leading zero: on
- Use 24-hour format: on
- Use Celsius for temperature: off
- Set location manually: off
- Reverse theme colors: off
- Share anonymous usage analytics: on
- Theme: Default
- Slot 1: Weather
- Slot 2: Heart rate
- Slot 3: Battery
- Slot 4: Steps

## Implementation sections

1. Documentation baseline
2. App-config structure and CSS
3. Settings persistence and AppMessage contract
4. Weather location and temperature-unit logic
5. Theme resolution and Icon Manager
6. Anonymous analytics and opt-out enforcement
7. Integration, CloudPebble build, visual QA, and regression QA
