# DayPal 2.0.0 Technical Specification

**Status:** Implementation baseline  
**Visual authority:** Figma file `oRliygHF8qzrzs5THYQOMH`, section `84:1722`  
**Target:** Emery / Pebble Time 2 / 200 × 228

## Compatibility contract

2.0.0 is a major visual release, not a numeric-protocol reset. Existing IDs keep their meanings.

### AppMessage keys

| Key | Meaning | Action |
|---:|---|---|
| 0 | Theme | Preserve |
| 1–4 | Metric slots 1–4 | Preserve |
| 5 | Leading zero | Preserve |
| 6 | Use 24-hour format | Preserve; configuration UI may invert wording |
| 7 | Reverse theme | Reserve for compatibility; not exposed in 2.0 UI |
| 8 | Use four slots | New |
| 10–12 | Weather payload/settings | Preserve |
| 20 | Weather request | Preserve |
| 21 | Settings ready | Preserve |

Do not reuse gaps or existing keys for different meanings after this release.

### Persistence keys

| Key | Meaning | Action |
|---:|---|---|
| 100 | Theme | Preserve |
| 101–104 | Metric slots 1–4 | Preserve |
| 105 | Leading zero | Preserve |
| 106–108 | Weather state | Preserve |
| 109 | Use 24-hour format | Preserve |
| 110 | Reverse theme | Preserve/reserve |
| 111 | Use four slots | New |

Recommended browser key: `use_four_slots`.

### Metric IDs

`0 Weather`, `1 Heart Rate`, `2 Battery`, `3 Calories`, `4 Steps`, `5 None` remain unchanged. Append `6 Sleep`.

### Theme IDs

Preserve current values:

- 0 Default
- 1 Blue
- 2 Pink
- 3 Green
- 4 White
- 5 Orange
- 6 legacy Dark Blue compatibility value
- 7 Black
- 8 Red
- 9 Yellow
- 10 legacy Khaki compatibility value
- 11 Gray compatibility/reserved value
- 12 Purple — new

IDs 6, 10, and 11 are not available for Purple or any other new theme.

## Migration

On launch/config application:

1. Read existing persisted settings.
2. If persistence key 111 exists, use it.
3. If key 111 is absent and legacy DayPal persisted settings are present, initialize `use_four_slots=true`.
4. If key 111 is absent and no legacy settings are present, initialize `use_four_slots=false`.
5. Never mutate slot 4 merely because it is hidden.
6. Persist the new layout flag after initialization so future launches are deterministic.

The exact new-install/reset metric selections remain an explicit product-default decision; do not infer them from an illustrative Figma instance.

## Layout renderer

Use a shared slot renderer parameterized by layout mode rather than duplicate metric logic.

### Three slots

| Element | Geometry |
|---|---|
| Tray | x0 y0 w70 h228 |
| Slot icon boxes | x14 w42 h42; y12/y86/y160 |
| Value boxes | x14 w42 h14; y54/y128/y202 |
| Value font target | Roboto Bold 14, centered |
| Visual glyph target | 28px within 42px canvas |

### Four slots

| Element | Geometry |
|---|---|
| Tray | x0 y0 w70 h228 |
| Slot icon boxes | x19 w32 h32; y8/y64/y120/y176 |
| Value boxes | x19 w32 h12; y40/y96/y152/y208 |
| Value font target | Roboto Bold 12, centered |
| Visual glyph target | 20px within 32px canvas |

### Clock

| Element | Geometry/typography |
|---|---|
| Time region | x70 y0 w130 h228 |
| Separator | 1px at tray/time boundary; draw within tray so time begins at x70 |
| Hour | x81 y16 w108 h72; Roboto Black 93; right aligned |
| Minute | x81 y96 w108 h72; Roboto Black 93; right aligned |
| Date | optical target approx x87.5 y184 w95; Roboto Bold 18 |

Figma's date node and its technical annotation disagree slightly on line-height. The rendered golden, not the annotation, decides acceptance. Pebble uses integer rectangles; tune the date rectangle by at most 1px if required to reproduce the golden and record that exception in QA.

Do not assume the 1.6.x custom fonts are visually interchangeable with the Figma targets. Add/rebuild font resources if necessary and validate actual Pebble rasterization.

## Theme resolver

Use one semantic theme resolver that returns background and foreground treatment.

| Theme | ID | BG | Foreground |
|---|---:|---|---|
| Default | 0 | #000000 | Color |
| Blue | 1 | #00AAFF | White |
| Pink | 2 | #FF00AA | White |
| Green | 3 | #005500 | White |
| White | 4 | #FFFFFF | Black |
| Orange | 5 | #FF5500 | White |
| Black | 7 | #000000 | White |
| Red | 8 | #FF0000 | White |
| Yellow | 9 | #FFCC55 | Black |
| Purple | 12 | #5500FF | White |

The palette is intentionally the approved 2.0.0 Figma palette even where it differs from 1.6.x color values.

Reverse Theme is no longer a visible 2.0.0 control. Keep key 7 / persistence 110 reserved so old data is not misread. The fixed Figma palette controls 2.0 rendering.

Default-theme metric colors:

- Weather: `#FFFF00`
- Heart Rate: `#FF0000`
- Battery: `#00FF00`
- Steps: `#00AAFF`
- Calories: `#FF5500`
- Sleep: `#8A38F5`
- Unavailable: `#666666`

Non-default themes use the theme foreground treatment for icons and values.

## Asset selection

Each supported metric icon must resolve by:

1. metric/state,
2. active layout size (42 or 32 canvas),
3. treatment (Color, White, Black).

Weather additionally resolves condition; Battery additionally resolves bucket and charging state.

See `DayPal-Asset-Inventory-2.0.0.md` for the filename-level inventory contract.

## Metric formatting

- Weather: integer plus degree symbol, e.g. `72°`, to match 2.0.0 Figma.
- Battery: integer only; no percent sign.
- Heart Rate, Steps, Calories, Sleep: compact numeric/text representation that fits the approved value box.
- Missing/unavailable: `---`.

Formatting must be exercised at worst-case widths in both 14px and 12px slot text.

## Weather state machine

Carry forward the Essential Redux 2.0.0 reliability model.

### Triggers

- 15-minute scheduled refresh.
- Settings-ready handshake.
- Location/config change.
- Reconnect where supported.
- Bounded retry after failure.

### Guardrails

- Deduplicate equivalent requests inside 60 seconds.
- Keep the last successful value visible during temporary failures.
- Persist sufficient last-success state for restart behavior.
- Retry after approximately 1, 5, and 15 minutes; do not create an unbounded tight retry loop.
- Log request reason, timestamp, age, ACK/NACK, and cache/fallback state for engineering diagnostics.

Weather diagnostics are local/debug information; they are not GA4 event payloads.

## Configuration bridge

2.0.0 development uses:

`https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`

Current QA cache label:

`daypal-2.0.0-qa-1`

The companion should append cache/version and serialized settings as currently implemented.

At release:

1. Validate QA.
2. Copy the exact QA file to `Hosting/apps/daypal/releases/2.0.0/app-config.html`.
3. Copy the same exact bytes to `Hosting/apps/daypal/prod/app-config.html`.
4. Verify matching blob SHAs.
5. Switch companion URL to prod.
6. Use a fresh production cache label.
7. Never edit the 2.0.0 snapshot afterward.

Keep `DayPal-Hosting` online for installed 1.6.x releases.

## Analytics bridge

Use GA4 only when anonymous analytics is enabled. Required event schema is in `DayPal-Analytics-Spec-2.0.0.md`.

No analytics event may contain:

- country, ZIP/postal code, city, coordinates, or derived precise location;
- temperature/weather values;
- heart-rate, step, calorie, sleep, or battery readings;
- device/account identifiers;
- free-form user-entered strings.

## Build/release cleanup

Before release candidate:

- Set app version to 2.0.0.
- Update root README/version references.
- Verify all manifest resource paths.
- Ensure 32px and 42px resources compile for Emery.
- Remove no compatibility IDs merely because their UI is hidden.
- Build without new warnings.
- Record final PBW SHA/identity in QA evidence.

## Verification

A functional emulator pass is insufficient. Release requires the workflow in `DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md`: native Figma goldens, unscaled 200 × 228 captures, overlays/diffs, documented optical exceptions, and physical Pebble Time 2 signoff.
