# DayPal 2.0.0 Pixel-Perfect QA Checklist

**Release gate:** Required  
**Visual authority:** [Figma 2.0.0 section 84:1722](https://www.figma.com/design/oRliygHF8qzrzs5THYQOMH/Pebble?node-id=84-1722&m=dev)  
**Comparison size:** Native 200 × 228 only

A functionally correct build is not sufficient for 2.0.0. The approved Figma render is the visual contract.

## Reference freeze

- [ ] Record the Figma file key: `oRliygHF8qzrzs5THYQOMH`.
- [ ] Record the approved section: `84:1722`.
- [ ] Export each required watchface golden at exactly 200 × 228.
- [ ] Store/export date and a reference to the Figma revision used for final QA.
- [ ] Re-export goldens if the approved Figma section changes after QA begins.

## Required visual matrix

The Figma section contains approved examples for both layouts across ten selectable themes:

- Default
- Orange
- Blue
- Purple
- Yellow
- Green
- Red
- Pink
- White
- Black

Minimum release visual matrix: **20 native goldens** — 10 themes × 2 layouts.

Also exercise metric/state variants not represented by the theme goldens, especially weather conditions, battery charge states, Calories, and edge-case value widths.

## Structural geometry

### Shared

- [ ] Canvas is exactly 200 × 228.
- [ ] Information tray is exactly 70px wide.
- [ ] Time region starts at x=70 and is 130px wide.
- [ ] Separator is exactly 1px and does not steal an extra pixel from the time region.
- [ ] Hour is x81/y16/w108/h72.
- [ ] Minute is x81/y96/w108/h72.
- [ ] Date optical position matches the Figma golden at y184.

### Three slots

- [ ] Icon canvases are 42 × 42.
- [ ] Icon x=14.
- [ ] Icon y positions are 12, 86, 160.
- [ ] Visual glyphs are 28px target.
- [ ] Value x=14/w42/h14.
- [ ] Value y positions are 54, 128, 202.
- [ ] Value typography visually matches Roboto Bold 14.

### Four slots

- [ ] Icon canvases are 32 × 32.
- [ ] Icon x=19.
- [ ] Icon y positions are 8, 64, 120, 176.
- [ ] Visual glyphs are 20px target.
- [ ] Value x=19/w32/h12.
- [ ] Value y positions are 40, 96, 152, 208.
- [ ] Value typography visually matches Roboto Bold 12.

## Typography

- [ ] Hour/minute visually match Roboto Black 93 and approved right alignment.
- [ ] Hour/minute baseline and inter-line gap match the golden.
- [ ] Date visually matches Roboto Bold 18.
- [ ] Slot values remain centered for narrow and wide data.
- [ ] Leading-zero on/off passes.
- [ ] 12-hour and 24-hour modes pass.
- [ ] No clipping at worst-case supported values.

Pebble rasterization can differ from Figma. A ≤1px optical text compensation is allowed only when it makes the rendered output match the golden more closely. Every such compensation must be recorded in `qa/README.md` with element, axis, amount, and evidence.

## Theme/palette

- [ ] Default #000000 with per-metric colors.
- [ ] Orange #FF5500 / white foreground.
- [ ] Blue #00AAFF / white foreground.
- [ ] Purple #5500FF / white foreground.
- [ ] Yellow #FFCC55 / black foreground.
- [ ] Green #005500 / white foreground.
- [ ] Red #FF0000 / white foreground.
- [ ] Pink #FF00AA / white foreground.
- [ ] White #FFFFFF / black foreground.
- [ ] Black #000000 / white foreground.
- [ ] No unintended legacy Reverse Theme visual is applied.

## Asset checks

- [ ] 3-slot icons use the 42px canvas / 28px glyph resources.
- [ ] 4-slot icons use the 32px canvas / 20px glyph resources.
- [ ] Default theme uses Color treatment.
- [ ] Each fixed theme uses the correct White/Black treatment.
- [ ] No runtime asset scaling is visible.
- [ ] Transparent edges are clean at native scale.

## Capture method

For each test case:

1. Set the exact theme, layout, time/date, and representative metric values used by the golden.
2. Capture the emulator at native 200 × 228 without browser/image scaling.
3. Preserve the raw capture.
4. Overlay the capture and Figma golden at 50% opacity.
5. Generate/inspect an absolute difference image.
6. Classify mismatches as geometry, typography, color, asset, or data-formatting.
7. Fix the implementation, recapture, and repeat.

Do not “approve by eye” a scaled screenshot when an overlay is possible.

## Acceptance criteria

### Structural

**Zero-pixel structural geometry differences** are allowed for rectangles, regions, separator placement, and icon-canvas placement.

### Text optical exception

Only documented ≤1px text optical compensation is allowed, because Figma and Pebble font rasterizers differ. This exception does not allow resizing or repositioning structural regions.

### Color

Theme background and fixed foreground colors must match the approved hex values. Antialiasing edge pixels are assessed visually and by source asset/font behavior rather than treated as theme-color changes.

## Functional visual states

- [ ] Weather displays integer + degree symbol.
- [ ] Battery displays number without percent sign.
- [ ] Missing values display `---`.
- [ ] Weather states render all supported condition icons.
- [ ] Battery renders all bucket and charging states.
- [ ] Sleep renders in each visible slot.
- [ ] Repeated metrics do not share incorrect mutable render state.
- [ ] Hidden slot 4 creates no visual residue in three-slot mode.

## Configuration page visual QA

The hosted page must also be compared to Figma at its 390px design width:

- [ ] Section order and spacing.
- [ ] General-setting labels and controls.
- [ ] Ten-theme selector.
- [ ] Four-slot toggle and metric controls.
- [ ] Manual-location fields.
- [ ] Language section.
- [ ] Analytics copy/consent control.
- [ ] Support, Reset Layout, Save Settings, My DayPal.
- [ ] Focus/pressed/validation states are coherent even where not shown in static Figma.
- [ ] Mobile viewport has no unintended horizontal overflow.

## Physical device signoff

After emulator visual QA passes:

- [ ] Install the release candidate on Pebble Time 2.
- [ ] Review both layouts.
- [ ] Review light and dark backgrounds.
- [ ] Confirm fine icon strokes remain legible.
- [ ] Confirm no device-only clipping/raster artifacts.
- [ ] Record physical-test date, PBW identity, and result.

## Evidence record template

For each approved case record:

| Field | Value |
|---|---|
| Date | |
| Commit/PBW | |
| Figma reference | |
| Layout | |
| Theme | |
| Raw capture | |
| Golden | |
| Diff/overlay | |
| Result | PASS / FAIL |
| Optical exception | none or exact ≤1px adjustment |
| Notes | |

2.0.0 cannot be tagged/released while any required visual case remains FAIL.
