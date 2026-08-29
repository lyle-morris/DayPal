# DayPal 2.0.0 Handoff

Updated: August 29, 2026

Use this document to resume DayPal 2.0.0 without reconstructing the release decisions or accidentally promoting QA-only code.

## Release status

**DayPal 2.0.0 is active development and is not release-ready. Do not merge PR #7 to `main` yet.**

The last implementation/QA commit before this documentation cleanup was:

`18b3627f9d2cc71fff16fbb37c22f3d460878068` — `Bust cache for DayPal 2.0 app config QA`

Documentation cleanup commits follow that implementation commit on `daypal-2.0.0-dev`.

The stable DayPal source remains on `main`. Its head before this documentation cleanup was `9ce17508637784bd04138f45ce555cfd7f49c867`, the merge of the DayPal 1.6.1 weather cache fallback hotfix. Historical version metadata on the stable branch still says 1.6.0, so do not infer the published revision from `appinfo.json` alone.

## Quick resume prompt

```text
Continue DayPal 2.0.0 from branch daypal-2.0.0-dev. Read docs/2.0.0/README.md and docs/2.0.0/DayPal-Handoff-2.0.0.md first. Verify the latest Figma section 84:1722 before changing the theme model. Do not merge PR #7 until every release blocker in the handoff and QA checklist is resolved.
```

## Repository boundaries

### Watchface / companion

- Repository: `lyle-morris/DayPal`
- Stable branch: `main`
- 2.0 development branch: `daypal-2.0.0-dev`
- 2.0 pull request: #7
- Store/app UUID: `64d3ca97-9d6a-47d0-98e5-0cb9530ad920`
- Target: Pebble Time 2 / Emery / 200 × 228
- Figma file: `oRliygHF8qzrzs5THYQOMH`
- Approved 2.0 section used by the checked-in contract: `84:1722`

### Consolidated hosting

- Repository: `lyle-morris/Hosting`
- Branch: `main`
- QA: `apps/daypal/qa/app-config.html`
- Production: `apps/daypal/prod/app-config.html`
- Release snapshots: `apps/daypal/releases/`

Current DayPal 2.0 QA page is synchronized with the app-repository copy:

- `DayPal/app-config/index.html` blob: `dc6c4e1981ef63fdbb6b3bb2646d24b91062a0f2`
- `Hosting/apps/daypal/qa/app-config.html` blob: `dc6c4e1981ef63fdbb6b3bb2646d24b91062a0f2`

There is **no `apps/daypal/releases/2.0.0/` snapshot yet**. Only 1.6.0 and 1.6.1 snapshots exist. That is an intentional release gate.

### Legacy compatibility hosting

- Repository: `lyle-morris/DayPal-Hosting`
- Published compatibility URL: `https://lyle-morris.github.io/DayPal-Hosting/app-config.html`

The current stable DayPal companion on `main` still opens this legacy URL. Keep the repository and endpoint online for installed 1.6.x builds.

The consolidated stable production copy and legacy live page currently use the same app-config blob (`07d5f06a9530bf5043511cde6126fd32fd92b5b2`), but existing installations may still open the legacy URL directly.

## What is implemented in the 2.0 development branch

### Watchface geometry / assets

- Native 200 × 228 renderer.
- 70px information tray and 130px clock region.
- 3-slot geometry using 42 × 42 icon canvases.
- 4-slot geometry using 32 × 32 icon canvases.
- Purpose-built Color/White/Black resource treatments.
- Expanded weather and battery asset sets.
- Pixel-level layout tuning performed during development.

### Themes

The checked-in implementation currently supports the fixed 2.0 preset set:

| Theme | ID | Background | Foreground |
|---|---:|---|---|
| Default | 0 | #000000 | per-metric color |
| Orange | 5 | #FF5500 | white |
| Blue | 1 | #00AAFF | white |
| Purple | 12 | #5500FF | white |
| Yellow | 9 | #FFCC55 | black |
| Green | 3 | #005500 | white |
| Red | 8 | #FF0000 | white |
| Pink | 2 | #FF00AA | white |
| White | 4 | #FFFFFF | black |
| Black | 7 | #000000 | white |

Legacy theme IDs 6, 10, and 11 remain compatibility values and must not be repurposed.

`reverse_theme` remains persisted for upgrade compatibility but the fixed 2.0 palette does not apply the old reverse-theme visual behavior.

### Metrics currently represented

IDs in code/UI:

- 0 Weather
- 1 Heart Rate
- 2 Battery
- 3 Calories
- 4 Steps
- 5 None
- 6 Sleep
- 7 Activity Time
- 8 Distance

Weather, Heart Rate, Battery, Calories, and Steps have runtime data paths. Sleep, Activity Time, and Distance currently have assets/UI IDs but are not yet backed by live data in normal runtime.

### Weather / location

Already present:

- Open-Meteo current-weather requests.
- Current phone location flow.
- Manual postal/city geocoding.
- US/Canada postal fallback via Zippopotam.
- Cached weather keyed by location/unit.
- Last-successful-weather fallback when a current request fails.

### Hosted app-config

The current QA page includes:

- General settings.
- Ten preset themes.
- 3/4-slot presentation toggle.
- Slot metric selectors.
- Manual postal/city UI.
- Country UI placeholder.
- Language section placeholder.
- Analytics consent toggle.
- Support link.
- Reset Layout and Save.

The page is a QA implementation, not a finished production contract.

## Release blockers — must resolve before merging PR #7

### 1. Three-slot mode is still forced by a compile-time QA override

`src/c/main.c` currently contains:

```c
#define DAYPAL_QA_SLOT_COUNT 3
```

and caps visible metrics at three under that macro. The source comment explicitly says this is temporary QA behavior.

**Required:** remove the QA override and let a real persisted layout setting control 3/4-slot rendering.

### 2. No real 3/4-slot AppMessage/persistence contract yet

The 2.0 design contract calls for a real layout setting while preserving the existing slot values. The current manifest and companion have no layout AppMessage key.

The hosted page currently simulates three-slot mode by saving Slot 4 as `None`.

**Required:** implement and freeze a layout key/persistence key without reusing existing IDs.

### 3. Hidden Slot 4 is not preserved across save/reopen

The hosted page remembers the previous Slot 4 value only in the current browser session, then saves `slot_4_metric = None` when three-slot mode is selected. After closing/reopening, the previous Slot 4 choice is lost.

**Required:** store layout separately and preserve Slot 4 exactly while hidden.

### 4. Fresh-install / Reset Layout defaults are not frozen

The design contract says new installs default to three slots, but the checked-in companion/config defaults still have four active metrics:

- Weather
- Heart Rate
- Battery
- Steps

The exact three-metric trio for fresh install and Reset Layout was left open in the original 2.0 documentation.

**Required:** freeze the intended trio and implement it consistently in watch, companion, and hosted config while preserving upgrade behavior for existing four-slot users.

### 5. Sleep, Activity Time, and Distance are not live metrics yet

Normal runtime currently displays `---` for metric IDs 6, 7, and 8. `update_health_metrics()` only populates Steps, Active Calories, and Heart Rate.

**Required:** wire the intended Pebble Health metrics and formatting, or remove the unfinished choices from the 2.0 selector/release scope.

### 6. Language section is presentation-only

The hosted page currently exposes only English and does not serialize a language field. The companion/watch AppMessage contract has no language key.

**Required:** implement the approved language contract end-to-end or remove the Language section from the release UI.

### 7. Country control is not part of the settings model

The QA page shows a Country selector, but the collected/manual-location settings do not carry `manual_country` to the companion.

**Required:** implement country-aware location end-to-end or remove the dead control.

### 8. Analytics is still a local test buffer, not GA4

The development companion's `trackAnalyticsEvent()` appends events to `localStorage` (`daypal_analytics_events`) and logs them. The consolidated QA page does not load `gtag.js`.

The approved analytics spec calls for consented configuration-only reporting and explicitly prohibits health, weather, battery, location, identifiers, and free-form values.

**Required:** implement actual GA4 collection, opt-out behavior, privacy checks, and the agreed taxonomy before release.

### 9. Weather refresh contract is incomplete

The watch currently requests weather when `tm_min % 30 == 0`. The 2.0 contract calls for a 15-minute schedule plus dedupe/reconnect and bounded retry behavior.

**Required:** align runtime behavior with the final weather reliability contract and QA it across real time boundaries.

### 10. Version metadata still says 1.6.0

`appinfo.json` on the 2.0 branch still has `versionLabel: 1.6.0`. `package.json` also remains 1.6.0.

**Required:** change both to 2.0.0 only when the release candidate is actually ready.

### 11. Companion still targets QA hosting

Current development companion:

```js
var CONFIG_URL = 'https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html';
var CONFIG_CACHE_LABEL = 'daypal-2.0.0-app-config-qa-1';
```

This is correct for development and a release blocker by design.

**Required:** after QA promotion, switch to production with a new production cache label and build a fresh PBW.

### 12. No immutable 2.0.0 hosted snapshot

`Hosting/apps/daypal/releases/` currently contains only 1.6.0 and 1.6.1.

**Required:** after QA is approved, copy the exact validated bytes to `releases/2.0.0/app-config.html` and `prod/app-config.html`, then verify matching blob SHAs.

### 13. Formal QA evidence is incomplete

The repository QA checklist remains largely unchecked and does not contain the required final PBW, native golden matrix, diff/overlay evidence, physical-device signoff, or release record.

Development conversations included successful visual review of the 4-slot layout and a positive 3-slot review, but those are **informal QA notes**, not substitutes for the repository's evidence requirements.

### 14. Theme direction should be reconfirmed before substantial new work

The checked-in 2.0 contract uses ten fixed presets. Later product discussion explored a more granular custom-color system for tray/time box/stripe/text/icons.

**Required:** verify the latest approved Figma direction before refactoring the theme engine or app-config. Do not assume the older preset-only contract or later custom-theme discussion automatically wins.

### 15. App-config preview colors need final visual reconciliation

The current QA CSS uses some preview values that do not exactly match the watch/source contract (for example the Yellow preview token and Blue preview text treatment).

**Required:** reconcile hosted preview colors with the final approved Figma/watch palette during visual QA.

## Issues / blockers encountered during the 2.0 development cycle

Preserve these lessons when resuming:

- 4-slot layout required several pixel-level adjustments to time, date, and information text before informal visual approval.
- 3-slot mode was initially hard-coded for QA because the configuration/layout contract was not yet implemented.
- Informational text sizing went through multiple iterations.
- Theme contrast/readability required review; this contributed to later discussion of a different theming model.
- App-config was rebuilt to match the Figma direction and the consolidated Hosting path.
- Asset naming/resource strategy changed from many theme-colored files toward Color/White/Black treatments and separate 32/42px sizes.
- Weather reliability must preserve the successful last-read fallback from the stable 1.6.x hotfix.
- Analytics was specified but never completed as real GA4 telemetry in the 2.0 implementation.
- Version/root documentation drift remained because 2.0 development was never promoted to a true release candidate.

## Current AppMessage contract

Existing meanings must be preserved:

| Key | Meaning |
|---:|---|
| 0 | theme |
| 1 | slot_1_metric |
| 2 | slot_2_metric |
| 3 | slot_3_metric |
| 4 | slot_4_metric |
| 5 | show_leading_zero |
| 6 | use_24_hour |
| 7 | reverse_theme (legacy compatibility) |
| 10 | weather_temp |
| 11 | weather_code |
| 12 | weather_valid |
| 20 | request_weather |
| 21 | settings_ready |

Do not repurpose existing keys. Append new layout/language/etc. keys only after the contract is frozen.

## Settings migration constraints

- `SETTINGS_KEY`: `daypal_settings`
- legacy settings key: `daymate_settings`
- Existing slot/theme meanings must remain stable.
- Legacy Reverse Theme data must not be reinterpreted.
- A new layout setting must distinguish a fresh install from an upgrade that has no layout key.
- Slot 4 must remain stored while hidden in three-slot mode.

## Analytics target contract

The 2.0 analytics specification intends consented configuration-only events such as:

- `settings_loaded`
- `settings_saved`

Useful parameters include theme, layout mode, visible slot count, selected metric IDs, and a layout signature.

Never send:

- Health readings.
- Weather readings.
- Battery readings.
- Location text or coordinates.
- Device/account identifiers.
- Free-form user input.

Treat the existing local event buffer as development instrumentation only.

## Hosting promotion contract

1. Develop only against `Hosting/apps/daypal/qa/app-config.html`.
2. Validate normal-browser and real Pebble configuration flows.
3. Freeze the approved QA blob.
4. Copy those exact bytes to `Hosting/apps/daypal/releases/2.0.0/app-config.html`.
5. Copy the same bytes to `Hosting/apps/daypal/prod/app-config.html`.
6. Verify QA/release/prod blob SHAs match at promotion time.
7. Switch the companion to production with a fresh production cache label.
8. Build a fresh Emery PBW.
9. Complete physical Pebble Time 2 signoff.
10. Do not edit the immutable 2.0.0 snapshot after release.

Keep `DayPal-Hosting` online for installed 1.6.x clients.

## Recommended resume sequence

1. Verify the latest Figma theme/config direction.
2. Freeze the fresh-install/reset three-metric default.
3. Implement a real 3/4-slot layout key and migration logic; remove `DAYPAL_QA_SLOT_COUNT`.
4. Fix hidden Slot 4 persistence.
5. Complete or remove Sleep/Activity/Distance.
6. Complete or remove Language/Country UI.
7. Implement real GA4 analytics with privacy QA.
8. Finish the 15-minute weather reliability contract.
9. Reconcile app-config theme previews with Figma/watch colors.
10. Run the full functional and 20-case native visual matrix plus physical PT2 signoff.
11. Promote exact QA bytes to immutable 2.0.0 + prod.
12. Set app/package version to 2.0.0, switch companion to prod, and build the final PBW.
13. Only then mark PR #7 ready and merge to `main`.

## Branch policy

- `main` is stable 1.6.x source until 2.0 passes the release gate.
- `daypal-2.0.0-dev` remains the only active 2.0 branch.
- PR #7 should remain Draft while blockers remain.
- Do not delete the 2.0 development branch until after a completed release merge/tag/archive.
