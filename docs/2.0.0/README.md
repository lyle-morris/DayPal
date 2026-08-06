# DayPal 2.0.0

**Status:** Design approved / documentation baseline frozen / implementation beginning  
**Development branch:** `daypal-2.0.0-dev`  
**Baseline:** DayPal `main` at `9ce17508637784bd04138f45ce555cfd7f49c867`  
**Visual source:** [Figma 2.0.0 section 84:1722](https://www.figma.com/design/oRliygHF8qzrzs5THYQOMH/Pebble?node-id=84-1722&m=dev)

This folder is the release source of truth for DayPal 2.0.0. The approved Figma section is the visual authority; these documents define the product, compatibility, analytics, asset, QA, and release contracts around it.

## Release documents

- [Product Requirements](DayPal-PRD-2.0.0.md)
- [Technical Specification](DayPal-Tech-Spec-2.0.0.md)
- [Asset Inventory](DayPal-Asset-Inventory-2.0.0.md)
- [Analytics Specification](DayPal-Analytics-Spec-2.0.0.md)
- [Pixel-Perfect QA Checklist](DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md)
- [Release Notes](DayPal-Release-Notes-2.0.0.md)
- [Handoff](DayPal-Handoff-2.0.0.md)
- [QA Evidence Index](qa/README.md)

## Locked 2.0.0 decisions

- Pebble Time 2 canvas remains 200 × 228.
- Both 3-slot and 4-slot layouts are supported.
- New installs default to 3 slots.
- Upgrades from 1.6.x retain 4 visible slots when the new layout setting is absent.
- Slot 4 remains stored while hidden; switching layout never shifts slot assignments.
- Information tray is 70px wide; clock region is 130px wide.
- 3-slot icons use 42px canvases with 28px visual glyphs.
- 4-slot icons use 32px canvases with 20px visual glyphs.
- Ten themes are selectable: Default, Orange, Blue, Purple, Yellow, Green, Red, Pink, White, Black.
- Pink keeps ID 2; Purple appends ID 12.
- Existing compatibility IDs are never reused.
- Sleep appends metric ID 6.
- Final 2.0 themes follow the fixed Figma palettes; Reverse Theme is not exposed in the 2.0 configuration UI.
- Weather uses the Essential Redux reliability pattern: 15-minute schedule, last-success fallback, dedupe/reconnect handling, bounded retries.
- Analytics is consented and configuration-only; no health readings, location, weather values, device IDs, or free-form user data.
- Pixel-perfect comparison against native 200 × 228 Figma goldens is a release blocker.
- DayPal 2.0.0 configuration is developed/promoted through `lyle-morris/Hosting`; `DayPal-Hosting` stays online for published 1.6.x builds.

## One product default still open

The wireframes establish that a new install starts in 3-slot mode but do not unambiguously establish which three metrics Reset Layout/new install should select. Freeze that exact metric trio before implementation of reset/default migration. Do not infer it from one illustrative Figma instance.

## Hosting paths

- QA: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- Production: `https://lyle-morris.github.io/Hosting/apps/daypal/prod/app-config.html`
- Immutable 2.0.0 snapshot: `https://lyle-morris.github.io/Hosting/apps/daypal/releases/2.0.0/app-config.html`
- Legacy 1.6.x compatibility host: `https://lyle-morris.github.io/DayPal-Hosting/app-config.html`

The development companion currently targets consolidated QA using cache label `daypal-2.0.0-qa-1`.

## Release sequence

1. Receive and validate the final 32/42px asset exports.
2. Freeze the new-install/reset metric trio.
3. Implement the shared 3/4-slot renderer and 2.0 palette.
4. Implement the hosted configuration page and settings migration.
5. Implement/verify weather reliability and anonymous analytics.
6. Run functional, upgrade, asset, and native pixel-perfect QA.
7. Promote the exact validated QA config to production and the immutable 2.0.0 snapshot.
8. Switch the companion to production with a fresh cache label.
9. Build the final Emery PBW and complete physical Pebble Time 2 signoff.
10. Finalize release notes and close QA evidence.
