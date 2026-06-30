# DayPal 1.5.0 PRD

## Overview

DayPal 1.5.0 is a visual contrast and polish release for the Pebble Time 2 watchface. The release keeps the approved 200 x 228 layout, metric tray behavior, configuration flow, and existing metric set intact while improving icon readability and making battery, heart rate, and divider colors clearer at a glance.

## Goals

- Improve weather icon contrast by replacing outlined weather icons with filled icon artwork.
- Improve battery readability by changing the default battery accent from purple `#AA55FF` to green `#00FF00`.
- Improve heart rate readability by changing the default heart rate accent from green `#00FF00` to red `#FF0000`.
- Confirm the date text is rendered with bold font weight `700`.
- Reduce the visual harshness of the vertical metric divider by changing it from white `#FFFFFF` to gray `#555555`.

## Non-Goals

- No layout redesign.
- No metric slot behavior changes.
- No configuration page changes unless required by implementation.
- No changes to weather, health, battery, or settings data pipelines.
- No new metric types.
- No changes to clock typography, clock placement, or metric tray dimensions.

## User-Facing Requirements

### Weather Icons

Weather icons must use filled artwork instead of outlined artwork for stronger visibility on Pebble Time 2.

Affected weather states:

- Sunny
- Partly cloudy
- Rainy
- Storm
- Snow
- Fog

Existing theme behavior should remain:

- Default theme uses yellow weather icons.
- Color themes use white weather icons.
- White theme uses black weather icons.

### Battery Accent

Battery icons and text must change from purple `#AA55FF` to green `#00FF00` in the default theme.

Affected battery states:

- `0`
- `25`
- `50`
- `100`
- `charging_0`
- `charging_25`
- `charging_50`
- `charging_100`

The green battery icon filenames should use `00FF00` in the file name. Purple `AA55FF` battery resources should no longer be referenced by runtime code after the update.

### Heart Rate Accent

Heart rate icon and text must change from green `#00FF00` to red `#FF0000` in the default theme.

The red heart rate icon filename should use `FF0000` in the file name. Green `00FF00` heart rate resources should no longer be referenced by runtime code after the update.

### Date Font Weight

The date text must be validated as bold font weight `700`. If the current date font already uses a bold resource, no visual change is required; the implementation should document that validation.

### Vertical Divider

The vertical divider between the metric tray and clock area must change from white `#FFFFFF` to gray `#555555`. This should preserve the divider width and position.

## Acceptance Criteria

- Weather icons render as filled icons across all weather conditions and supported theme variants.
- Default theme battery icon and text render in green `#00FF00`.
- Default theme heart rate icon and text render in red `#FF0000`.
- Battery resource names and references use `GREEN` or `00FF00` naming instead of purple naming for the default colored variant.
- Heart rate resource names and references use `RED` or `FF0000` naming instead of green naming for the default colored variant.
- Date text is confirmed to use `Roboto-Bold.ttf` or another bold `700` font resource.
- Divider renders as `#555555` without changing layout dimensions.
- Existing black and white icon variants remain available for white and color themes.
- CloudPebble or compatible Pebble SDK build succeeds.

## Release Risks

- Icon files may need manual visual QA because contrast depends on both fill shape and Pebble display rendering.
- Renaming resources requires synchronized updates across `resources/images`, `appinfo.json`, and C resource references.
- If old resource names remain in code, the app may build against stale purple or green assets.

