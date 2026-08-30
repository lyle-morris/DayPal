# DayPal 2.0.0

**Status:** Active development / partial QA prototype / **not release-ready**  
**Development branch:** `daypal-2.0.0-dev`  
**Pull request:** #7 — keep Draft until the release gate is complete  
**Stable baseline:** DayPal `main` 1.6.x line  
**Visual source:** [Figma 2.0.0 section 84:1722](https://www.figma.com/design/oRliygHF8qzrzs5THYQOMH/Pebble?node-id=84-1722&m=dev)

This folder is the working release source of truth for DayPal 2.0.0. The approved Figma section is the visual authority for the checked-in design contract, but the latest Figma theme/config direction should be reconfirmed before substantial new theming work.

## Read first

1. [Current Handoff / Blockers](DayPal-Handoff-2.0.0.md)
2. [Product Requirements](DayPal-PRD-2.0.0.md)
3. [Technical Specification](DayPal-Tech-Spec-2.0.0.md)
4. [Asset Inventory](DayPal-Asset-Inventory-2.0.0.md)
5. [Analytics Specification](DayPal-Analytics-Spec-2.0.0.md)
6. [Pixel-Perfect QA Checklist](DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md)
7. [QA Evidence Index](qa/README.md)
8. [Draft Release Notes](DayPal-Release-Notes-2.0.0.md)

The handoff is more current than the original baseline documents and records implementation drift discovered during the August 29 cleanup audit.

## What exists today

- 200 × 228 DayPal 2.0 watchface geometry.
- 3-slot and 4-slot rendering work.
- 42px/32px Color/White/Black asset families.
- Ten fixed preset themes in the checked-in implementation.
- Weather, Heart Rate, Battery, Calories, and Steps runtime metrics.
- Sleep, Activity Time, and Distance IDs/assets/UI entries, but no live runtime data yet.
- Consolidated DayPal 2.0 QA app-config in `lyle-morris/Hosting`.
- Current/manual-location weather and last-successful-weather fallback.

The checked-in `DayPal/app-config/index.html` and `Hosting/apps/daypal/qa/app-config.html` currently have the same blob SHA (`dc6c4e1981ef63fdbb6b3bb2646d24b91062a0f2`).

## Locked compatibility decisions

- Pebble Time 2 canvas remains 200 × 228.
- Existing AppMessage IDs and stored meanings must not be repurposed.
- Existing metric IDs 0–5 retain their meanings.
- Existing theme IDs retain their meanings; Purple uses ID 12.
- Reverse Theme remains a legacy compatibility value even though the fixed 2.0 palette does not expose it in the final UI.
- `DayPal-Hosting` must remain online for installed 1.6.x clients.
- DayPal 2.0 development/promotions use `lyle-morris/Hosting`.

## Current release blockers

Do not merge or publish while any of these remain:

- Watchface still forces three visible slots through `DAYPAL_QA_SLOT_COUNT`.
- No dedicated persisted/AppMessage layout key exists for 3/4-slot mode.
- Hiding Slot 4 currently loses its saved metric after Save/reopen.
- Fresh-install/Reset Layout three-metric default is not frozen or implemented.
- Sleep, Activity Time, and Distance are not connected to live Pebble Health data.
- Language UI is not wired to companion/watch settings.
- Country UI is not part of the saved manual-location model.
- Analytics is development-local only, not GA4.
- Watch weather schedule is 30 minutes rather than the intended 15-minute release contract, and retry/dedupe work remains incomplete.
- `appinfo.json` and `package.json` still say 1.6.0.
- Companion still targets consolidated QA.
- No immutable `Hosting/apps/daypal/releases/2.0.0/` snapshot exists.
- Formal native visual QA evidence and physical PT2 signoff are incomplete.

See the handoff for root causes and the exact resume sequence.

## Hosting paths

- QA: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- Future production: `https://lyle-morris.github.io/Hosting/apps/daypal/prod/app-config.html`
- Future immutable 2.0.0 snapshot: `https://lyle-morris.github.io/Hosting/apps/daypal/releases/2.0.0/app-config.html`
- Legacy 1.6.x compatibility host: `https://lyle-morris.github.io/DayPal-Hosting/app-config.html`

The development companion currently targets QA and must remain there until promotion is complete.

## Release sequence

1. Reconfirm the latest Figma theme/config direction.
2. Freeze the new-install/Reset Layout three-metric default.
3. Replace the QA slot-count override with a real layout setting and migration contract.
4. Fix hidden Slot 4 persistence.
5. Complete or remove unfinished metrics and placeholder controls.
6. Implement GA4 analytics with privacy QA.
7. Complete weather schedule/retry/dedupe reliability work.
8. Run the full functional, upgrade, asset, and native pixel-perfect QA matrix.
9. Complete physical Pebble Time 2 signoff.
10. Promote the exact approved QA page to `releases/2.0.0` and `prod`, verifying matching blob SHAs.
11. Set versions to 2.0.0 and switch the companion to production with a fresh cache token.
12. Build and record the final Emery PBW.
13. Mark PR #7 ready and merge to `main` only after all gates pass.
