# DayPal 2.0.0 QA Evidence

**Status:** Active development / partial informal QA / formal release evidence incomplete  
**Visual gate:** [Pixel-Perfect QA Checklist](../DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md)

Do not mark a formal release item complete without evidence, a date, and the tested commit/PBW where applicable.

## Important distinction: informal QA vs release evidence

During development, several visual reviews were performed interactively and the watchface geometry was iterated against screenshots/overlays. Those conversations are useful product history, but the repository does not currently contain the complete release evidence required by this checklist.

Informal observations recorded from the development cycle:

- Four-slot layout went through repeated time/date/info-text alignment passes and was eventually accepted visually in conversation as looking correct.
- Three-slot QA was later reviewed and accepted informally as looking good.
- General Settings in the hosted config received an informal functional pass during app-config QA.
- Theme contrast and app-config styling remained active design work after those checks.

These observations **do not check off** the formal boxes below because the required capture/PBW/commit/date evidence has not been stored here.

## Current known blockers discovered in the August 29 audit

- [ ] Remove `DAYPAL_QA_SLOT_COUNT` compile-time slot forcing.
- [ ] Implement a real 3/4-slot AppMessage/persistence key.
- [ ] Preserve hidden Slot 4 across Save/reopen.
- [ ] Freeze and implement fresh-install/Reset Layout three-metric defaults.
- [ ] Wire Sleep, Activity Time, and Distance to live data or remove them from release UI.
- [ ] Implement or remove the Language control.
- [ ] Implement country-aware manual location or remove the dead Country control.
- [ ] Replace local analytics test buffering with consented GA4 telemetry.
- [ ] Align weather scheduling/retry/dedupe with the final release contract.
- [ ] Set app/package versions to 2.0.0 at release-candidate time.
- [ ] Promote approved QA config to immutable 2.0.0 + production and verify identical blobs.
- [ ] Switch companion from QA to production with a fresh cache token.

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

## Current QA source alignment

At the August 29 cleanup audit, the checked-in development config and consolidated QA page were byte-identical by Git blob SHA:

- `DayPal/app-config/index.html`: `dc6c4e1981ef63fdbb6b3bb2646d24b91062a0f2`
- `Hosting/apps/daypal/qa/app-config.html`: `dc6c4e1981ef63fdbb6b3bb2646d24b91062a0f2`

Use that relationship as the expected source/published-QA check until the next intentional QA edit.

## Visual QA

- [ ] Approved Figma section `84:1722` revision/export date recorded.
- [ ] Latest Figma theme/config direction reconfirmed before final QA.
- [ ] 20 minimum native 200 × 228 goldens exported: 10 themes × 2 layouts, if the fixed 10-theme model remains final.
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
- [ ] Heart Rate, Steps, Calories, Sleep, Activity Time, and Distance release assets validated for whatever metrics remain in scope.
- [ ] Manifest paths resolve.
- [ ] No unintended runtime raster scaling.

## Upgrade and persistence QA

- [ ] Upgrade from published 1.6.x preserves existing theme and slot settings.
- [ ] AppMessage IDs retain meanings.
- [ ] Existing metric IDs retain meanings; new IDs are appended only.
- [ ] Existing theme IDs retain meanings; Purple remains appended at 12.
- [ ] Persistence IDs retain meanings.
- [ ] Legacy user with no new layout key migrates to four slots.
- [ ] Fresh install with no legacy state defaults to the frozen three-slot layout.
- [ ] Hiding Slot 4 does not mutate its metric.
- [ ] Returning to four slots restores saved Slot 4 exactly after closing/reopening settings.
- [ ] Browser settings migration is validated.
- [ ] Legacy Reverse Theme stored data is not misinterpreted.

## Functional QA

- [ ] Every release-supported metric renders in every visible position.
- [ ] Duplicate metric selections behave independently where supported.
- [ ] Missing values display `---`.
- [ ] Weather displays integer + degree symbol.
- [ ] Battery remains numeric without percent sign.
- [ ] Leading zero passes.
- [ ] 12/24-hour behavior passes.
- [ ] Current/manual location flows pass.
- [ ] Country behavior passes if Country remains in the final UI.
- [ ] Language behavior passes if Language remains in the final UI.
- [ ] Scheduled weather refresh crosses multiple 15-minute real-time boundaries if 15 minutes remains the approved contract.
- [ ] Weather request dedupe passes.
- [ ] Cached last-successful weather survives temporary location/network/provider failures.
- [ ] Bounded retry/reconnect scenarios pass.

## Analytics/privacy QA

- [ ] Analytics disabled emits no usage events.
- [ ] Consented load emits the approved load event with configuration only.
- [ ] Consented save emits the approved save event after successful Save.
- [ ] Save that disables analytics emits no post-disable usage event.
- [ ] 3-slot and 4-slot layout dimensions/signatures are correct.
- [ ] Hidden Slot 4 selection is preserved.
- [ ] No location field/coordinates are transmitted as analytics parameters.
- [ ] No weather, health, sleep, steps, calories, or battery readings are transmitted.
- [ ] No device/account identifier or free-form user input is transmitted.
- [ ] GA4 Realtime receives the expected events.
- [ ] GA4 custom dimensions are registered for reporting as needed.

Current development instrumentation only writes test events to browser local storage; that does not satisfy these items.

## Hosted configuration QA

- [ ] QA URL opens in a normal mobile browser.
- [ ] QA URL opens through Pebble configuration.
- [ ] 390px layout matches the latest approved configuration frame.
- [ ] Theme selector matches the final approved theme model.
- [ ] “Use 4 informational slots” saves/restores through a real layout key.
- [ ] “Use 12-hour format” correctly maps to the preserved stored 24-hour boolean.
- [ ] Metric selectors save/restore without shifting.
- [ ] Hidden Slot 4 survives Save/reopen.
- [ ] Manual location validation passes.
- [ ] Language/Country placeholder controls are either fully wired or absent from the release UI.
- [ ] Analytics consent copy/control passes.
- [ ] Reset Layout matches the frozen new-install defaults.
- [ ] Save returns settings correctly.
- [ ] QA, production, and immutable 2.0.0 snapshot are byte-identical at promotion.
- [ ] QA and production cache labels are distinct.

## Build and release QA

- [ ] Emery build succeeds without new warnings.
- [ ] `appinfo.json` version is 2.0.0.
- [ ] `package.json` version is 2.0.0.
- [ ] Resource IDs resolve and every manifest path exists.
- [ ] Runtime/RAM behavior is validated.
- [ ] Final PBW identity is recorded.
- [ ] Physical Pebble Time 2 signoff is recorded.
- [ ] Root documentation/version references are updated.
- [ ] Store screenshots/banner/release notes are approved.
- [ ] Consolidated production/snapshot blob SHA verification recorded.
- [ ] Companion production URL/cache token verified in the final PBW.

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

**Do not mark DayPal 2.0.0 released until this final record and all required gates are complete.**
