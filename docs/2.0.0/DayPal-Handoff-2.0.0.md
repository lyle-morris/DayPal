# DayPal 2.0.0 Handoff

Use this document to resume DayPal 2.0.0 work without reconstructing the release decisions.

## Quick resume prompt

```text
Continue DayPal 2.0.0 from branch daypal-2.0.0-dev. Read docs/2.0.0/README.md and DayPal-Handoff-2.0.0.md, verify the current GitHub branch and Figma section 84:1722, then continue from the first incomplete release gate.
```

## Repositories and branch

- App: `lyle-morris/DayPal`
- Development branch: `daypal-2.0.0-dev`
- Consolidated hosting: `lyle-morris/Hosting`
- Legacy compatibility hosting: `lyle-morris/DayPal-Hosting`
- Target: Pebble Time 2 / Emery / 200 × 228
- Figma file: `oRliygHF8qzrzs5THYQOMH`
- Approved Figma section: `84:1722` (`2.0.0`)

## Starting point

- Branch baseline is DayPal `main` commit `9ce17508637784bd04138f45ce555cfd7f49c867`, containing the 1.6.1 last-successful-weather fallback.
- Root version/readme references still require final 2.0.0 release cleanup.
- The 2.0.0 branch companion points to consolidated DayPal QA with cache label `daypal-2.0.0-qa-1`.
- Published 1.6.x builds still use `DayPal-Hosting`; that host must remain available.
- The owner is exporting the final 2.0.0 image assets to the development branch.

## Approved Figma structure

Section `84:1722` contains:

- 390px hosted configuration frame.
- Ten 200 × 228 three-slot theme goldens.
- Ten 200 × 228 four-slot theme goldens.
- A technical specification frame.
- Ten selectable themes: Default, Orange, Blue, Purple, Yellow, Green, Red, Pink, White, Black.
- Configuration controls for general settings, theme, information/layout, manual location, language, analytics, support, reset, and save.

Pixel-perfect conformance to these watchface goldens is a release blocker.

## Locked layout contract

### Shared

- Tray x0/y0/w70/h228.
- Clock region x70/y0/w130/h228.
- 1px separator at the boundary.
- Hour x81/y16/w108/h72.
- Minute x81/y96/w108/h72.
- Date optical target around x87.5/y184/w95.
- Hour/minute: Roboto Black 93 target.
- Date: Roboto Bold 18 target.

### Three slots

- 42 × 42 icon canvas; 28px visual glyph.
- icon x14; y12/86/160.
- value x14/w42/h14; y54/128/202.
- Roboto Bold 14 target.

### Four slots

- 32 × 32 icon canvas; 20px visual glyph.
- icon x19; y8/64/120/176.
- value x19/w32/h12; y40/96/152/208.
- Roboto Bold 12 target.

Native Figma golden beats a textual annotation if the two disagree. Pebble integer/font rasterization permits only a documented ≤1px optical text compensation.

## Layout migration

- New installs: three-slot mode.
- Upgrade from legacy persisted DayPal: four-slot mode when new layout key is absent.
- Slot 4 stays saved while hidden.
- Switching layouts never shifts slot values.
- Proposed new AppMessage key: 8.
- Proposed new persistence key: 111.
- Browser key: `use_four_slots`.
- Still to freeze: exact three metric selections used by new install/Reset Layout.

## Metrics

Preserve:

- 0 Weather
- 1 Heart Rate
- 2 Battery
- 3 Calories
- 4 Steps
- 5 None

Append:

- 6 Sleep

Weather 2.0 display matches Figma with an integer plus degree symbol. Battery remains numeric without `%`. Missing value remains `---`.

## Theme contract

| Theme | ID | Background | Foreground |
|---|---:|---|---|
| Default | 0 | #000000 | metric Color |
| Orange | 5 | #FF5500 | white |
| Blue | 1 | #00AAFF | white |
| Purple | 12 | #5500FF | white |
| Yellow | 9 | #FFCC55 | black |
| Green | 3 | #005500 | white |
| Red | 8 | #FF0000 | white |
| Pink | 2 | #FF00AA | white |
| White | 4 | #FFFFFF | black |
| Black | 7 | #000000 | white |

Legacy IDs 6, 10, 11 remain reserved/compatible and are never repurposed.

Reverse Theme is absent from the final 2.0 UI. Preserve its existing message/persistence keys so stored data is not misinterpreted, but 2.0 selectable themes render with their fixed Figma palette.

## Asset contract

- Treatments: Color, White, Black.
- 3 slots: 42px canvas / 28px glyph.
- 4 slots: 32px canvas / 20px glyph.
- Weather: 7 conditions × 2 sizes × 3 treatments = 42.
- Battery: 5 buckets × 2 charge states × 2 sizes × 3 treatments = 60.
- Heart Rate, Steps, Calories, Sleep: 24 combined.
- Full distinct-resource target: 126.

Validate actual PNG dimensions/transparent bounds and manifest mappings when exported assets arrive.

## Weather reliability

Carry forward Essential Redux 2.0.0 lessons:

- 15-minute scheduled requests.
- Settings-ready/config/reconnect triggers.
- 60-second equivalent-request dedupe.
- Persist/show last successful data during temporary failures.
- Bounded retry around 1/5/15 minutes.
- Engineering logging for reason/timing/ACK/NACK/cache age without sending readings/location to analytics.

## Analytics

Consented hosted-page GA4 taxonomy:

Events:

- `settings_loaded`
- `settings_saved`

Key parameters:

- `app_name=daypal`
- `app_version`
- `theme_id`, `theme_name`
- `layout_mode`, `visible_slot_count`
- `slot_1_metric` … `slot_4_metric`
- `layout_signature`
- `reverse_theme` only as legacy compatibility visibility

Never send health readings, weather values, battery readings, location fields/coordinates, device IDs, account IDs, or free-form input.

Use the same taxonomy later for Essential Redux with `app_name=essential_redux`.

## Hosting release contract

1. Develop only at `Hosting/apps/daypal/qa/app-config.html`.
2. Validate normal-browser and Pebble configuration flows.
3. Copy exact validated bytes to `Hosting/apps/daypal/releases/2.0.0/app-config.html`.
4. Promote the same bytes to `Hosting/apps/daypal/prod/app-config.html`.
5. Verify matching blob SHAs.
6. Switch the app companion to prod with a new production cache label.
7. Never edit the immutable 2.0.0 snapshot after release.
8. Keep `DayPal-Hosting` online for installed 1.6.x.

## Release docs

Read in this order:

1. `docs/2.0.0/README.md`
2. `DayPal-PRD-2.0.0.md`
3. `DayPal-Tech-Spec-2.0.0.md`
4. `DayPal-Asset-Inventory-2.0.0.md`
5. `DayPal-Analytics-Spec-2.0.0.md`
6. `DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md`
7. `qa/README.md`
8. `DayPal-Release-Notes-2.0.0.md`

## Next implementation work

1. Inventory/validate the assets arriving on `daypal-2.0.0-dev`.
2. Freeze the new-install/reset three-metric default.
3. Implement shared 3/4-slot renderer, palettes, Sleep metric, and font/resources.
4. Implement consolidated hosted config + settings migration + analytics.
5. Apply scheduled weather reliability work.
6. Run upgrade and native pixel-perfect QA continuously, not only at the end.
