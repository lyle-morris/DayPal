# DayPal 2.0.0 Release Notes

**Status:** Draft — finalize after release-candidate QA

DayPal 2.0.0 is a major visual refresh for Pebble Time 2, with a new pixel-precise layout system, expanded theme choices, more flexible information density, and a rebuilt configuration/release pipeline.

## What’s new

- Choose between **3 or 4 informational slots**.
- New installs start with the cleaner three-slot layout; existing DayPal users retain the familiar four-slot layout when upgrading.
- Slot 4 is preserved when switching to three slots, so changing layouts does not discard the saved metric.
- Ten selectable themes: Default, Orange, Blue, Purple, Yellow, Green, Red, Pink, White, and Black.
- New **Purple** theme.
- **Sleep** joins the metric set shown in the 2.0.0 design.
- Redesigned 70px information tray and 130px clock region for Pebble Time 2.
- New purpose-built 42px assets for three-slot mode and 32px assets for four-slot mode.
- Weather display updated to the 2.0.0 design with a degree symbol.
- Hosted settings page refreshed to match the 2.0.0 design.
- Anonymous, opt-in configuration analytics can help guide future theme/layout decisions without collecting health, location, weather readings, or device-identifying data.

## Reliability

DayPal 2.0.0 carries forward the weather-reliability work proven during Essential Redux 2.0.0:

- Scheduled weather refresh.
- Last-successful-weather fallback during temporary failures.
- Reconnect/settings triggers.
- Request deduplication and bounded retries.

## Compatibility

2.0.0 preserves existing AppMessage, persistence, metric, and theme ID meanings. New values are appended rather than reusing old IDs.

The configuration page moves to the consolidated DayPal path in `lyle-morris/Hosting`. The legacy `DayPal-Hosting` site remains available for installed 1.6.x builds.

## Visual quality

The watchface is being validated against the approved Figma designs at native 200 × 228 resolution. Both layouts and all ten selectable themes must pass overlay/difference review before release, followed by physical Pebble Time 2 signoff.

## Before publishing

Replace this section with the final store-facing copy after QA and record:

- Final version/build identity.
- Final PBW.
- Upgrade test result.
- Physical-device signoff.
- Hosted production/snapshot SHA verification.
- Any known limitations.
