# DayPal 2.0.0 Asset Inventory

**Status:** Intake / validation  
**Owner input:** New 2.0.0 exports are being added to `daypal-2.0.0-dev`.

This document defines what the watchface renderer needs. Do not mark an asset complete merely because a file exists; it must also match the Figma sizing/treatment and render correctly on Emery.

## Asset contract

Two layout sizes are required:

| Layout | Canvas | Visual glyph target |
|---|---:|---:|
| 3 slots | 42 × 42 | 28px |
| 4 slots | 32 × 32 | 20px |

Each visual state needs three treatments:

- **Color** — used by Default theme.
- **White** — used by Orange, Blue, Purple, Green, Red, Pink, Black.
- **Black** — used by Yellow and White.

Assets must be centered in their canvas according to the approved Figma examples. Do not scale a 32px raster at runtime to simulate the 42px resource or vice versa.

## Default-theme colors

| Metric | Color |
|---|---|
| Weather | #FFFF00 |
| Heart Rate | #FF0000 |
| Battery | #00FF00 |
| Steps | #00AAFF |
| Calories | #FF5500 |
| Sleep | #8A38F5 |

Unavailable values use `#666666`; icon fallback behavior must be visually checked against the approved state.

## Required metric families

### Weather

Conditions currently required by the runtime:

- Sunny
- Partly Cloudy
- Rainy
- Storm
- Snow
- Fog
- Cloudy

Unknown weather continues to use the approved fallback condition unless the implementation spec is explicitly changed.

Required count: 7 conditions × 2 sizes × 3 treatments = **42 assets**.

### Battery

Buckets currently required:

- 0
- 25
- 50
- 75
- 100

Each bucket requires normal and charging states.

Current bucket mapping remains:

- ≤0 → 0
- 1–25 → 25
- 26–50 → 50
- 51–80 → 75
- 81–100 → 100

Required count: 5 buckets × 2 charging states × 2 sizes × 3 treatments = **60 assets**.

### Simple metrics

Each simple metric needs 2 sizes × 3 treatments = 6 assets:

- Heart Rate
- Steps
- Calories
- Sleep

Required count: 4 metrics × 6 = **24 assets**.

### Total target

If all metric families use distinct raster resources, the complete 2.0.0 matrix is **126 assets**:

- Weather: 42
- Battery: 60
- Heart Rate/Steps/Calories/Sleep: 24

Existing resources may be reused only when they are visually identical to the 2.0.0 approved target. Reuse does not waive the 42/28 and 32/20 validation.

## Intake checklist

For every exported family:

- [ ] Color 42 canvas / 28 glyph
- [ ] White 42 canvas / 28 glyph
- [ ] Black 42 canvas / 28 glyph
- [ ] Color 32 canvas / 20 glyph
- [ ] White 32 canvas / 20 glyph
- [ ] Black 32 canvas / 20 glyph
- [ ] Transparent background and expected alpha
- [ ] Optical centering matches Figma
- [ ] No unintended antialiasing halo
- [ ] Manifest resource path added
- [ ] Emulator renders exact size without runtime scaling
- [ ] Both Default and theme-foreground treatments verified

## File naming

Keep names deterministic and machine-searchable. The implementation may retain existing names for compatible assets, but new names should encode:

`metric[_state]_<color|white|black>_<42|32>`

Examples:

- `weather_sunny_color_42.png`
- `battery_75_charging_white_32.png`
- `heart_black_42.png`
- `sleep_color_32.png`

Do not mass-rename already referenced resources solely for naming consistency; manifest stability is more valuable than cosmetic cleanup.

## Validation workflow

1. Inventory the files added to the branch.
2. Read actual PNG dimensions; do not trust filenames.
3. Compare transparent bounds to the 28px/20px visual glyph target.
4. Map every resource through the manifest.
5. Render all three treatments on their required theme backgrounds.
6. Capture at native 200 × 228.
7. Compare against the corresponding Figma golden.
8. Record exceptions in `qa/README.md`.

## Asset acceptance

The asset set is release-ready only when:

- There are no missing renderer states.
- No asset is runtime-scaled between the 42 and 32 layouts.
- Color/White/Black treatment selection matches the theme table.
- Edge pixels/alpha do not introduce visible halos.
- Every visible Figma example passes overlay/diff review.
