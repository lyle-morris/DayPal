# DayPal 2.0.0 Product Requirements

**Status:** Design-approved implementation baseline  
**Release:** 2.0.0  
**Target:** Pebble Time 2 / Emery, 200 × 228  
**Development branch:** `daypal-2.0.0-dev`  
**Visual authority:** [Figma 2.0.0 section 84:1722](https://www.figma.com/design/oRliygHF8qzrzs5THYQOMH/Pebble?node-id=84-1722&m=dev)

## Goal

DayPal 2.0.0 is a major visual and configuration refresh that keeps existing DayPal users compatible while adopting the release discipline proven in Essential Redux 2.0.0.

The release must:

- Match the approved Figma watchface layouts pixel-for-pixel.
- Support both three- and four-information-slot layouts.
- Preserve installed-user settings and all existing numeric contracts.
- Add the approved Purple theme without reusing an existing theme ID.
- Add the Sleep metric required by the 2.0.0 Figma examples without renumbering existing metrics.
- Move DayPal 2.0.0 configuration hosting to the consolidated `lyle-morris/Hosting` repository.
- Improve scheduled weather reliability using the Essential Redux 2.0.0 approach.
- Add consented, anonymous configuration analytics that can answer which themes, layouts, and metric arrangements are actually used.

## Product principles

1. **Figma is the visual source of truth.** Section `84:1722` is authoritative for 2.0.0 geometry, palette, typography, and configuration-page structure.
2. **Compatibility IDs are immutable.** Existing AppMessage, persistence, theme, and metric meanings are never silently repurposed.
3. **Upgrade behavior and new-install behavior are separate.** Existing users keep four visible slots; new installs start in three-slot mode.
4. **Hiding a slot does not destroy it.** Slot 4 remains saved while three-slot mode is active and returns unchanged when four-slot mode is restored.
5. **Privacy is a feature.** Analytics never contains personal, location, health, weather-value, or device-identifying data.
6. **Pixel-perfect conformance is a release gate.** A build that functions correctly but does not match the approved 200 × 228 goldens is not release-ready.

## Watchface requirements

### Canvas and layout

The Pebble Time 2 canvas is fixed at 200 × 228.

Both layouts use:

- Information tray: x=0, y=0, width=70, height=228.
- Time region: x=70, y=0, width=130, height=228.
- A 1px vertical separator at the tray/time boundary.
- Stacked hour and minute with the date below.

### Three-slot mode

- Three visible information rows.
- 42 × 42 icon canvases.
- 28px visual icon glyph target inside each 42px canvas.
- Slot icon top positions: y=12, 86, 160.
- Value text top positions: y=54, 128, 202.
- Value text box: x=14, width=42, height=14.
- Value typography target: Roboto Bold, 14px, centered.

### Four-slot mode

- Four visible information rows.
- 32 × 32 icon canvases.
- 20px visual icon glyph target inside each 32px canvas.
- Slot icon top positions: y=8, 64, 120, 176.
- Value text top positions: y=40, 96, 152, 208.
- Value text box: x=19, width=32, height=12.
- Value typography target: Roboto Bold, 12px, centered.

### Time and date

The approved visual target is shared by both layouts:

- Hour: x=81, y=16, width=108, height=72.
- Minute: x=81, y=96, width=108, height=72.
- Hour/minute typography target: Roboto Black 93, right aligned.
- Date optical target: centered in the time region at approximately x=87.5, y=184, width=95.
- Date typography target: Roboto Bold 18.

Pebble integer coordinates and font rendering may require a documented ≤1px optical adjustment. The exported Figma golden remains authoritative.

## Layout behavior

- New installs default to **three slots**.
- Upgrades from 1.6.x default to **four slots** when the new layout setting does not yet exist.
- The fourth metric selection is retained while three-slot mode is active.
- Switching between three and four slots must not automatically shift or renumber metric selections.
- Reset Layout must restore the approved new-install defaults.
- **Open before implementation freeze:** choose the exact three metric selections restored for a brand-new install/reset. The wireframes demonstrate several metric combinations but do not unambiguously define this persistence default.

## Metrics

Existing metric IDs remain stable:

| ID | Metric | 2.0.0 requirement |
|---:|---|---|
| 0 | Weather | Preserve |
| 1 | Heart Rate | Preserve |
| 2 | Battery | Preserve |
| 3 | Calories | Preserve |
| 4 | Steps | Preserve |
| 5 | None | Preserve |
| 6 | Sleep | New; append |

Metric values must retain established fallback behavior unless the final Figma explicitly changes presentation:

- Missing/unavailable value: `---`.
- Battery: numeric value without `%`.
- Weather: 2.0.0 Figma target is integer temperature with a degree symbol, e.g. `72°`.
- Default-theme metric colors follow the approved asset/color spec.

## Themes

The 2.0.0 configuration page exposes ten themes:

| Selectable theme | Stable/new ID | Background | Foreground |
|---|---:|---|---|
| Default | 0 | #000000 | Per-metric colors |
| Orange | 5 | #FF5500 | #FFFFFF |
| Blue | 1 | #00AAFF | #FFFFFF |
| Purple | 12 | #5500FF | #FFFFFF |
| Yellow | 9 | #FFCC55 | #000000 |
| Green | 3 | #005500 | #FFFFFF |
| Red | 8 | #FF0000 | #FFFFFF |
| Pink | 2 | #FF00AA | #FFFFFF |
| White | 4 | #FFFFFF | #000000 |
| Black | 7 | #000000 | #FFFFFF |

Purple uses new ID 12. Existing IDs 6, 10, and 11 remain compatibility/reserved values and must never be repurposed.

The final 2.0.0 UI does not expose Reverse Theme. Existing AppMessage/persistence keys remain reserved for upgrade compatibility; 2.0.0 visual rendering must follow the fixed approved palettes above.

## Configuration page

The consolidated hosted configuration page must match the approved 390px Figma frame and preserve its section/order semantics:

- General settings
  - Display leading zero on the hours
  - Use 12-hour format
  - Use Celsius for temperature
- Theme
- Information
  - Use 4 informational slots
  - Metric selections
- Manual location
  - Country
  - ZIP/postal code
  - City
- Language
- Analytics
- Support
- Reset layout
- Save settings
- My DayPal

The user-facing “Use 12-hour format” checkbox may invert the existing stored `use_24_hour` boolean; the stored key meaning must not change.

## Weather reliability

Carry forward the Essential Redux reliability pattern:

- Request scheduled weather updates every 15 minutes.
- Trigger on settings readiness/config changes and reconnect where appropriate.
- Deduplicate requests inside a short window to prevent bursts.
- Persist and continue showing the last successful weather during transient location/network/provider failures.
- Use bounded retry timing (1, 5, then 15 minutes) after failures.
- Log enough timing and ACK/NACK state for QA without logging location or weather values into analytics.

## Analytics

Analytics is anonymous and consented. It exists to measure configuration usage, not user behavior or health.

Required questions:

- Which themes are selected?
- How often are three vs four slots used?
- Which metrics are placed in each slot?
- Which overall layout combinations are common?

Approved events and parameters are defined in `DayPal-Analytics-Spec-2.0.0.md`.

## Localization

The 2.0.0 configuration page retains a Language control. The approved wireframe shows English, but it does not define a complete 2.0.0 supported-language set. Do not claim the Essential Redux language list for DayPal until that set is explicitly approved.

All UI implementation must keep strings separable from layout logic so additional languages can be added without changing persisted setting meanings.

## Hosting

DayPal 2.0.0 uses:

- QA: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- Production: `https://lyle-morris.github.io/Hosting/apps/daypal/prod/app-config.html`
- Immutable 2.0.0 snapshot: `https://lyle-morris.github.io/Hosting/apps/daypal/releases/2.0.0/app-config.html`

The legacy `DayPal-Hosting` site remains online for published 1.6.x builds.

## Acceptance gates

2.0.0 is ready only when:

- All visible layouts/themes pass native 200 × 228 pixel comparison.
- All required assets are present, correctly sized, and validated.
- Upgrade from 1.6.x preserves existing settings and four-slot visibility.
- New installs use three-slot mode.
- Slot 4 survives three-slot mode without mutation.
- Weather schedule/cache/retry scenarios pass.
- Analytics consent and data-exclusion tests pass.
- QA configuration is validated, then promoted byte-for-byte to production and the immutable release snapshot.
- Final Emery PBW passes emulator and physical Pebble Time 2 QA.
