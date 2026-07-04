# DayPal 1.6.0 App-Config Handoff

Status: Release-cycle reference
Branch: `daypal-1.6.0-dev`
Visual source of truth: `docs/daypal-1.6.0-wireframes.pdf`
Hosting repository: `lyle-morris/DayPal-Hosting`

## 1. Purpose

This document captures the app-configuration contract for DayPal 1.6.0.

The configuration page must match the final 1.6.0 wireframes and support settings, theme selection, reverse theme colors, metric slot customization, manual weather location, anonymous analytics opt-in, donation support, reset layout, and save settings.

## 2. Current hosting structure

DayPal-Hosting uses separate dev and production paths.

Development URLs:

```text
https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html
https://lyle-morris.github.io/DayPal-Hosting/dev/analytics.html
https://lyle-morris.github.io/DayPal-Hosting/dev/location-testing.html
```

Production URLs:

```text
https://lyle-morris.github.io/DayPal-Hosting/app-config.html
https://lyle-morris.github.io/DayPal-Hosting/analytics.html
https://lyle-morris.github.io/DayPal-Hosting/location-testing.html
```

During the 1.6.0 release cycle, the watchface dev branch must open the dev app-config URL:

```text
https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html
```

Release requirement:

- Development testing uses the `/dev/` hosted files.
- Production release approval must confirm promotion from `/dev/` to the production root files.
- Production root must not receive QA-only changes before release approval.

## 3. App-config location in repo

The source implementation is staged in the DayPal watchface repo at:

```text
app-config/index.html
```

The hosted copy in `DayPal-Hosting` must be named:

```text
app-config.html
```

Recommended hosting structure:

```text
/
  app-config.html
  analytics.html
  location-testing.html
  dev/
    app-config.html
    analytics.html
    location-testing.html
```

## 4. Settings payload contract

The configuration page returns settings through:

```text
pebblejs://close#<encoded-json>
```

Returned object:

```json
{
  "theme": 0,
  "slot_1_metric": 0,
  "slot_2_metric": 1,
  "slot_3_metric": 2,
  "slot_4_metric": 4,
  "show_leading_zero": true,
  "use_24_hour": false,
  "use_celsius": false,
  "manual_location": false,
  "manual_postal_code": "",
  "manual_city": "",
  "reverse_theme": false,
  "analytics_enabled": true
}
```

## 5. Metric values

```text
0 = Weather
1 = Heart rate
2 = Battery
3 = Calories
4 = Steps
5 = None
```

Default slot order:

```text
Slot 1 = Weather
Slot 2 = Heart rate
Slot 3 = Battery
Slot 4 = Steps
```

## 6. Theme values

```text
0 = Default
1 = Blue
2 = Pink
3 = Green
5 = Orange
7 = Black
8 = Red
9 = Yellow
```

Only these active themes should be exposed in the configuration page:

```text
Default, Blue, Orange, Green, Pink, Red, Yellow, Black
```

## 7. Configuration page panels

The page must include:

1. DayPal title and intro copy.
2. Support DayPal panel.
3. Donate action.
4. Help improve DayPal analytics panel.
5. Anonymous analytics opt-in.
6. Settings panel.
7. Theme panel.
8. Metric slots panel.
9. Reset layout action.
10. Save settings action.

## 8. Settings panel

Required controls:

- Display leading zero on the hours.
- Use 24-hour format.
- Use Celsius for temperature.
- Set location manually.

## 9. Manual location behavior

When manual location is off:

- Hide ZIP/postal code and City fields.
- Use current-device location for weather.

When manual location is on:

- Show ZIP/postal code field.
- Show City field.
- Accept either ZIP/postal code or City.

Error copy:

```text
Enter a ZIP/postal code or city to set the location manually.
```

Field-level error copy:

```text
Missing/invalid information
```

## 10. Theme panel behavior

The Theme panel must include:

- Theme options: Default, Blue, Orange, Green, Pink, Red, Yellow, Black.
- Reverse theme colors checkbox.
- Selected theme visual state.
- Focus border color: `#00AAFF`.

Theme preview chips must follow the 1.6.0 wireframe redlines:

- Non-reverse color themes use selected color background and black text.
- Non-reverse Black uses white background and black text.
- Reverse color themes use black background and selected color text.
- Reverse Black uses black background and white text.

## 11. Analytics copy

Analytics opt-in must communicate:

- Usage analytics are anonymous.
- No personal data is collected.
- No health data is collected.
- No location data is collected.

## 12. Donation link

Donate action opens:

```text
https://ko-fi.com/lylemorris
```

Payment processing remains external to DayPal.

## 13. Handoff checklist

1. Confirm `app-config/index.html` in the DayPal repo matches the final 1.6.0 configuration page wireframes.
2. Confirm `https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html` loads.
3. Confirm `https://lyle-morris.github.io/DayPal-Hosting/dev/analytics.html` loads.
4. Confirm `https://lyle-morris.github.io/DayPal-Hosting/dev/location-testing.html` loads.
5. Confirm settings render from a sample `settings` query string.
6. Confirm Save settings returns `pebblejs://close#<encoded-json>`.
7. Confirm Reset layout restores default slot order and default theme.
8. Confirm manual location validation states.
9. Confirm analytics opt-in can be saved.
10. Confirm Donate opens Ko-fi externally.
11. Confirm final production root files before release approval.
