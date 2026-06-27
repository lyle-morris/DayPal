# DayPal 1.0.0 Implementation Log

## Release

Product name: **DayPal**

Release title: **DayPal 1.0.0**

Development branch: `daymate-1.7.0-dev`

The repository and branch names still reflect the original DayMate development project. The public product name for this release is DayPal.

## Completed

- Added Pebble SDK 3 watchface metadata for Pebble Time 2 / Emery.
- Added DayPal app naming and 1.0.0 release metadata.
- Added native C watchface foundation.
- Added bold stacked clock layout.
- Added centered date rendering.
- Added full-height metric divider.
- Added fixed 58 px metric tray.
- Added four configurable metric slots.
- Added duplicate metric support.
- Added `None` support for hiding individual metric slots.
- Added fixed slot positioning so each metric remains anchored to its configured slot row.
- Added clock-only layout when all slots are `None`.
- Added full-screen centered clock mode for clock-only layout.
- Added leading-zero toggle.
- Added right-positioned single-digit hour behavior when leading zero is disabled.
- Added eight selectable themes.
- Added themed weather, battery, heart rate, calories, and steps icon resources.
- Added phone-side PebbleKit JS settings bridge.
- Added hosted configuration page support.
- Added settings persistence on phone and watch.
- Added Open-Meteo weather integration.
- Added weather fallback cache.
- Added persisted weather state on watch.
- Added live Pebble Health reads for steps, heart rate, and active calories.
- Added battery percentage and charging-state handling.

## Validated On Hardware

- CloudPebble build succeeds.
- Theme selection propagates from configuration page to watchface.
- Theme changes repaint immediately.
- Settings persist after watchface restart.
- Blue, Orange, Black, and White themes have been validated.
- Clock-only mode centers correctly.
- Single-digit hour alignment works with leading zero disabled.
- Metric slots remain in their configured row positions.
- Heart rate reads correctly.
- Steps read correctly.
- Battery reads correctly.
- UI layout is holding with current validated metric and theme combinations.

## Pending Validation

- Weather location permission and first successful weather fetch.
- Weather cached fallback after restart or temporary network/location failure.
- Active calories availability and display on target hardware.
- Full theme and slot matrix smoke test.
- One-day beta wear test for battery usage and refresh behavior.

## Notes

- Preserve the approved 200 x 228 layout.
- Do not redesign the clock, tray, divider, typography, or slot spacing unless fixing a verified defect.
- Keep commits small and buildable.
- CloudPebble remains the current compile gate for this workspace.
