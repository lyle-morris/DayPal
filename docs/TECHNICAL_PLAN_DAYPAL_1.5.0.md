# DayPal 1.5.0 Technical Plan

## Scope

This release updates visual resources and color constants only. The implementation should avoid changes to layout geometry, app message keys, settings persistence, weather fetching, health reads, or configuration UI behavior.

## Current Implementation Notes

- Main watchface rendering is in `src/c/main.c`.
- Resource declarations are in `appinfo.json`.
- Weather assets live in `resources/images/weather/`.
- Battery assets live in `resources/images/battery/`.
- Heart rate assets live in `resources/images/heart_rate/`.
- Date text currently loads `RESOURCE_ID_FONT_ROBOTO_BOLD_20` from `Roboto-Bold.ttf`, which satisfies the bold `700` validation target.

## Asset Updates

### Weather

Replace outlined weather PNGs with filled PNGs while preserving existing filenames and resource names where possible:

- `sunny_32_000000.png`
- `sunny_32_FFFF00.png`
- `sunny_32_FFFFFF.png`
- `partly_cloudy_32_000000.png`
- `partly_cloudy_32_FFFF00.png`
- `partly_cloudy_32_FFFFFF.png`
- `rainy_32_000000.png`
- `rainy_32_FFFF00.png`
- `rainy_32_FFFFFF.png`
- `storm_32_000000.png`
- `storm_32_FFFF00.png`
- `storm_32_FFFFFF.png`
- `snow_32_000000.png`
- `snow_32_FFFF00.png`
- `snow_32_FFFFFF.png`
- `fog_32_000000.png`
- `fog_32_FFFF00.png`
- `fog_32_FFFFFF.png`

Keeping filenames stable for weather minimizes code churn because the color naming does not change.

### Battery

Create green default battery assets and update resource references from purple to green.

New/renamed filenames:

- `battery_0_32_00FF00.png`
- `battery_25_32_00FF00.png`
- `battery_50_32_00FF00.png`
- `battery_100_32_00FF00.png`
- `battery_charging_0_32_00FF00.png`
- `battery_charging_25_32_00FF00.png`
- `battery_charging_50_32_00FF00.png`
- `battery_charging_100_32_00FF00.png`

Recommended resource name changes in `appinfo.json`:

- `IMAGE_BATTERY_0_PURPLE` -> `IMAGE_BATTERY_0_GREEN`
- `IMAGE_BATTERY_25_PURPLE` -> `IMAGE_BATTERY_25_GREEN`
- `IMAGE_BATTERY_50_PURPLE` -> `IMAGE_BATTERY_50_GREEN`
- `IMAGE_BATTERY_100_PURPLE` -> `IMAGE_BATTERY_100_GREEN`
- `IMAGE_BATTERY_CHARGING_0_PURPLE` -> `IMAGE_BATTERY_CHARGING_0_GREEN`
- `IMAGE_BATTERY_CHARGING_25_PURPLE` -> `IMAGE_BATTERY_CHARGING_25_GREEN`
- `IMAGE_BATTERY_CHARGING_50_PURPLE` -> `IMAGE_BATTERY_CHARGING_50_GREEN`
- `IMAGE_BATTERY_CHARGING_100_PURPLE` -> `IMAGE_BATTERY_CHARGING_100_GREEN`

Update `battery_resource_id()` in `src/c/main.c` to reference the new `GREEN` resource identifiers.

### Heart Rate

Create red default heart rate asset and update resource references from green to red.

New/renamed filename:

- `heart_rate_32_FF0000.png`

Recommended resource name change in `appinfo.json`:

- `IMAGE_HEART_RATE_GREEN` -> `IMAGE_HEART_RATE_RED`

Update `resource_id_for_metric()` in `src/c/main.c` to reference `RESOURCE_ID_IMAGE_HEART_RATE_RED`.

## Color Updates

Update the default theme in `get_theme()`:

- `heart_rate`: `GColorFromHEX(0x00FF00)` -> `GColorFromHEX(0xFF0000)`
- `battery`: `GColorFromHEX(0xAA55FF)` -> `GColorFromHEX(0x00FF00)`
- `divider`: `GColorWhite` or `GColorFromHEX(0xFFFFFF)` -> `GColorFromHEX(0x555555)`

Apply the divider change consistently to themes that currently use a white divider. The white theme may also use `#555555` if visual QA confirms it remains readable on a white background; otherwise document any theme-specific exception before implementation.

## Date Font Validation

Validate that `s_font_date` is loaded from `RESOURCE_ID_FONT_ROBOTO_BOLD_20` and that `appinfo.json` maps that resource to `fonts/Roboto-Bold.ttf`.

Current expected mapping:

```text
FONT_ROBOTO_BOLD_20 -> fonts/Roboto-Bold.ttf
```

No code change is required if this remains true.

## Implementation Checklist

- Add filled weather PNG artwork.
- Add green battery PNG artwork.
- Add red heart rate PNG artwork.
- Update `appinfo.json` battery resource names and file paths.
- Update `appinfo.json` heart rate resource name and file path.
- Update `src/c/main.c` battery resource identifiers.
- Update `src/c/main.c` heart rate resource identifier.
- Update `src/c/main.c` default battery text color to `#00FF00`.
- Update `src/c/main.c` default heart rate text color to `#FF0000`.
- Update `src/c/main.c` divider color to `#555555`.
- Validate date font mapping remains bold.
- Build in CloudPebble or compatible Pebble SDK.

## Validation Plan

- Build succeeds with all renamed resources.
- Default theme screenshot or hardware view confirms:
  - Weather icons are filled.
  - Battery icon and text are green.
  - Heart rate icon and text are red.
  - Divider is gray.
  - Date appears bold.
- White theme confirms black icons still render correctly.
- One solid-color theme confirms white icons still render correctly.
- Battery charging and non-charging icon states each resolve to valid resources.
- Weather condition mapping still resolves all supported conditions to valid resources.

