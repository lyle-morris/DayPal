# DayPal 2.0.0 QA Evidence

**Status:** Pre-implementation / asset intake  
**Visual gate:** [Pixel-Perfect QA Checklist](../DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md)

Do not mark an item complete without evidence, a date, and the tested commit/PBW where applicable.

## Evidence convention

For each completed test record:

- Date
- App commit/PBW identity
- Hosting commit/blob SHA when configuration is involved
- Emulator/device
- Inputs/setup
- Expected result
- Actual result
- PASS/FAIL
- Screenshot/golden/diff reference for visual checks
- Any documented ≤1px text optical compensation

## Visual QA

- [ ] Approved Figma section `84:1722` revision/export date recorded.
- [ ] 20 minimum native 200 × 228 goldens exported: 10 themes × 2 layouts.
- [ ] Emulator captures stored unscaled.
- [ ] Structural geometry has zero-pixel mismatch.
- [ ] Overlay/difference review passes every required layout/theme.
- [ ] Additional metric/weather/battery state matrix passes.
- [ ] Any text optical compensation is ≤1px and documented.
- [ ] Physical Pebble Time 2 review passes.

### Optical exceptions

Record approved exceptions here. Empty means none.

| Date | Commit | Element | Adjustment | Evidence | Reason |
|---|---|---|---|---|---|

## Asset QA

- [ ] Exported branch assets inventoried against `../DayPal-Asset-Inventory-2.0.0.md`.
- [ ] 42px canvases / 28px glyph targets validated.
- [ ] 32px canvases / 20px glyph targets validated.
- [ ] Color/White/Black treatments complete.
- [ ] Weather state matrix complete.
- [ ] Battery bucket/charging state matrix complete.
- [ ] Heart Rate, Steps, Calories, Sleep variants complete.
- [ ] Manifest paths resolve.
- [ ] No unintended runtime raster scaling.

## Upgrade and persistence QA

- [ ] Upgrade from published 1.6.x preserves existing theme and slot settings.
- [ ] AppMessage IDs retain meanings.
- [ ] Metric IDs 0–5 retain meanings; Sleep is appended at 6.
- [ ] Existing theme IDs retain meanings; Purple is appended at 12.
- [ ] Persistence IDs retain meanings.
- [ ] Legacy user with no layout key migrates to four slots.
- [ ] Fresh install with no legacy state migrates/defaults to three slots.
- [ ] Hiding slot 4 does not mutate its metric.
- [ ] Returning to four slots restores saved slot 4 exactly.
- [ ] Browser settings migration is validated.
- [ ] Legacy Reverse Theme stored data is not misinterpreted.

## Functional QA

- [ ] Every supported metric renders in every visible position.
- [ ] Duplicate metric selections behave independently where supported.
- [ ] Missing values display `---`.
- [ ] Weather displays integer + degree symbol.
- [ ] Battery remains numeric without percent sign.
- [ ] Leading zero passes.
- [ ] 12/24-hour behavior passes.
- [ ] Current/manual location flows pass.
- [ ] Scheduled weather refresh crosses multiple 15-minute real-time boundaries.
- [ ] Weather request dedupe passes.
- [ ] Cached last-successful weather survives temporary location/network/provider failures.
- [ ] Bounded retry/reconnect scenarios pass.

## Analytics/privacy QA

- [ ] Analytics disabled emits no usage events.
- [ ] Consented load emits `settings_loaded` with configuration only.
- [ ] Consented save emits `settings_saved` after successful Save.
- [ ] Save that disables analytics emits no post-disable usage event.
- [ ] 3-slot and 4-slot `layout_signature` values are correct.
- [ ] Hidden slot 4 selection is preserved.
- [ ] No location field/coordinates are transmitted as analytics parameters.
- [ ] No weather, health, sleep, steps, calories, or battery readings are transmitted.
- [ ] No device/account identifier or free-form user input is transmitted.
- [ ] GA4 custom dimensions are registered for reporting.

## Hosted configuration QA

- [ ] QA URL opens in a normal mobile browser.
- [ ] QA URL opens through Pebble configuration.
- [ ] 390px layout matches approved Figma configuration frame.
- [ ] Ten themes are selectable and save/restore.
- [ ] “Use 4 informational slots” saves/restores.
- [ ] “Use 12-hour format” correctly maps to the preserved stored 24-hour boolean.
- [ ] Metric selectors save/restore without shifting.
- [ ] Manual location validation passes.
- [ ] Analytics consent copy/control passes.
- [ ] Reset Layout matches the frozen new-install defaults.
- [ ] Save returns settings correctly.
- [ ] QA, production, and immutable 2.0.0 snapshot are byte-identical at promotion.
- [ ] QA and production cache labels are distinct.

## Build and release QA

- [ ] Emery build succeeds without new warnings.
- [ ] app version is 2.0.0.
- [ ] Resource IDs resolve and every manifest path exists.
- [ ] Runtime/RAM behavior is validated.
- [ ] Final PBW identity is recorded.
- [ ] Physical Pebble Time 2 signoff is recorded.
- [ ] Root documentation/version references are updated.
- [ ] Store screenshots/banner/release notes are approved.
- [ ] Consolidated production/snapshot blob SHA verification recorded.

## Final release record

| Field | Value |
|---|---|
| Release date | |
| Final app commit | |
| Final PBW | |
| Hosting QA SHA | |
| Hosting prod SHA | |
| Hosting 2.0.0 snapshot SHA | |
| Figma reference/revision | |
| Emulator visual QA | |
| Physical PT2 QA | |
| Release approver | |
