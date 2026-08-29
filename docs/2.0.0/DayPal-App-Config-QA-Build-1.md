# DayPal 2.0.0 — App-Config QA Build 1

Updated: August 29, 2026

## Purpose

This checkpoint begins the DayPal app-config modernization on the existing `daypal-2.0.0-dev` / PR #7 line without changing the already pixel-tuned watchface renderer.

The goal of Build 1 is to validate the new configuration shell and the settings round trip before adding new watch-side protocol behavior.

## Source / hosting identity

- DayPal branch: `daypal-2.0.0-dev`
- Pull request: #7 (`DRAFT — DayPal 2.0.0 development`)
- Companion cache label: `daypal-2.0.0-app-config-qa-2`
- QA URL: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- DayPal source page: `app-config/index.html`
- Hosting QA page: `apps/daypal/qa/app-config.html`
- Exact app-config blob SHA in both repositories: `b26b31d61f118b12a7b25f6abc6fd4914b4d785b`
- Hosting deployment commit: `ae2a97e5f065591dadef8bb9df8cbff827e10cc6`
- GitHub Pages workflow run: `33253661866` — success

## What Build 1 changes

### Redux-style configuration shell

The DayPal page now uses the proven Redux visual/control language:

- 390px centered settings shell.
- Dark gradient page background.
- Card/header treatment.
- Redux-style switches, checkboxes, selects, text fields, focus states, fixed action bar, and spacing.
- Information section first, followed by General Settings, Theme, Manual Location, Language, Analytics, and Support.
- Fixed Reset Layout / Save Settings controls.

This is a DayPal implementation, not a runtime dependency on Redux files.

### Theme selector

Build 1 preserves all existing DayPal 2.0 theme IDs and meanings. It also corrects the app-config preview values to match the checked-in watch/theme contract:

- Blue `#00AAFF` with white foreground.
- Yellow `#FFCC55` with black foreground.
- No theme IDs are added, removed, or repurposed.

### Manual Location

The configuration/companion state now includes:

- `manual_postal_code`
- `manual_city`
- `manual_country`

Behavior:

- Country defaults to `US` for users without the new field.
- ZIP/postal and City have in-field clear controls.
- Entering ZIP/postal clears City.
- Entering City clears ZIP/postal.
- `manual_location` is derived from whether ZIP/postal or City contains a value.
- Weather cache identity now includes country.
- Open-Meteo geocoding now receives the selected country code.
- US/Canada Zippopotam fallback now uses the selected country rather than inferring country only from input shape.

No location value is added to analytics event data.

### Language staging

Build 1 adds and persists the same 30 language codes already proven in Redux:

`en, es, fr, de, pt, it, nl, da, nb, sv, fi, is, ca, eu, gl, pl, cs, sk, sl, hr, bs, sr-Latn, ro, hu, et, lv, lt, tr, sq, mt`

This build intentionally does **not** add a DayPal watch AppMessage language key yet. The selection survives config Save/reopen through the companion, but watch-side date localization remains a Build 2 task.

This staging is deliberate: it lets us validate browser/Pebble WebView serialization first without changing `appinfo.json`, `package.json`, font resources, or the pixel-approved date renderer in the same step.

## Deliberately unchanged in Build 1

- `src/c/main.c` geometry and typography.
- The temporary `DAYPAL_QA_SLOT_COUNT 3` watch override.
- AppMessage key set.
- Persistence key set.
- `appinfo.json` version (`1.6.0` remains until release-candidate time).
- `package.json` version.
- Theme IDs / metric IDs.
- Sleep / Activity Time / Distance runtime behavior.
- Weather refresh cadence/retry state machine.
- GA4 implementation.
- Production hosting.
- Immutable `releases/2.0.0` snapshot.

## Known Build 1 limitations

1. The 3/4-slot UI still uses the legacy Slot 4 = `None` compatibility behavior because the real key 8 / persistence 111 bridge is intentionally deferred to Build 2. Do not use Build 1 to approve Slot 4 persistence.
2. The watch is still compile-time forced to the 3-slot QA renderer. The configuration toggle does not yet constitute final watch-side layout behavior.
3. Language is persisted in config/companion state but is not applied to the watch yet.
4. The settings page labels themselves remain English in Build 1. Full settings-page localization belongs with the watch language bridge.
5. Analytics remains the existing local test buffer; Build 1 does not claim GA4 completion.

## Build 1 QA checklist

### Page / visual

- [ ] Open the QA page through Pebble configuration, not only a desktop browser.
- [ ] Confirm the DayPal heading, cards, spacing, controls, and fixed action bar match the Redux look/feel direction.
- [ ] Confirm no horizontal overflow at Pebble/mobile WebView width.
- [ ] Confirm all ten DayPal themes appear and selection highlight follows the saved theme.
- [ ] Confirm Blue preview uses white text and Yellow preview is `#FFCC55` with black text.

### Existing settings regression

- [ ] Leading-zero value loads, saves, and restores.
- [ ] 12/24-hour value loads, saves, and restores.
- [ ] Celsius value loads, saves, and restores.
- [ ] Theme loads, saves, and restores without changing its numeric ID.
- [ ] Slot selections load, save, and restore under the existing protocol.
- [ ] Analytics consent loads, saves, and restores.

### Manual Location

- [ ] Existing user with no `manual_country` opens with United States selected.
- [ ] Enter ZIP/postal → City clears.
- [ ] Enter City → ZIP/postal clears.
- [ ] ZIP clear X clears only ZIP/postal.
- [ ] City clear X clears only City.
- [ ] Save and reopen preserves Country + active ZIP/City field.
- [ ] US ZIP weather resolves.
- [ ] Canadian postal weather resolves when Canada is selected.
- [ ] City + Country weather resolves to the expected country.
- [ ] Changing location invalidates mismatched cached weather rather than showing another country's cached result.

### Language staging

- [ ] English defaults for users without a saved language.
- [ ] All 30 language options appear.
- [ ] Select a non-English language → Save → reopen → same language remains selected.
- [ ] Confirm the watch remains unchanged in Build 1; this is expected and documented.

## Build 2 gate

Do not start production promotion from Build 1.

After Build 1 config/WebView regression passes, Build 2 should implement the protocol pieces already defined by the audit:

1. AppMessage key `8` / persistence `111` for `use_four_slots`.
2. Remove `DAYPAL_QA_SLOT_COUNT` forcing.
3. Preserve Slot 4 while hidden.
4. Add an append-only DayPal language AppMessage/persistence key without repurposing existing IDs.
5. Expand the date font glyph contract for supported language characters.
6. Localize watch month/weekday labels while preserving the approved English pixel geometry.
7. Localize settings-page labels from the same language selection.
8. Re-run upgrade/persistence QA before touching the remaining health/weather/analytics blockers.
