# DayPal 2.0.0 Handoff

Use this document to resume DayPal 2.0.0 work in a later conversation.

## Quick resume prompt

```text
Continue DayPal 2.0.0 using docs/2.0.0/DayPal-Handoff-2.0.0.md as the source of truth. Verify GitHub and the Figma 2.0.0 section before making changes.
```

## Repositories and branch

- App: `lyle-morris/DayPal`
- Consolidated hosting: `lyle-morris/Hosting`
- Legacy compatibility hosting: `lyle-morris/DayPal-Hosting`
- Development branch: `daypal-2.0.0-dev`
- Target: Pebble Time 2 / Emery
- Figma: `oRliygHF8qzrzs5THYQOMH`, section `84:1722`

## Verified starting point — 2026-08-03

- DayPal `main` ends at merge commit `9ce17508637784bd04138f45ce555cfd7f49c867`.
- That commit contains the 1.6.1 last-successful-weather fallback.
- App metadata and the root README still identify 1.6.0 and need release cleanup during 2.0.0.
- The published companion on `main` still opens `DayPal-Hosting/app-config.html`.
- Consolidated DayPal QA, production, 1.6.0, and 1.6.1 config pages exist in `lyle-morris/Hosting` and share blob SHA `07d5f06a9530bf5043511cde6126fd32fd92b5b2`.
- The 2.0.0 branch opens the consolidated QA page with cache label `daypal-2.0.0-qa-1`.
- `DayPal-Hosting` must stay online for installed 1.6.x builds.

## Current Figma facts

Section `84:1722` is named `2.0.0` and contains:

- One 390 × 2296 configuration frame.
- Nine 200 × 228 DayPal watchface examples.
- A three-row metric tray measurement study.
- Theme labels: Default, Orange, Blue, Purple, Yellow, Green, Red, White, Black.
- Current information selections: Weather, Heart Rate, Battery.
- General, manual-location, language, analytics, support, reset, and save controls.

Wireframes are still in progress. Do not convert observed pixels or labels into final requirements until approved.

## Contracts that must not be renumbered

Current runtime contracts include:

- AppMessage theme key 0.
- AppMessage metric keys 1–4.
- AppMessage leading-zero key 5.
- AppMessage 24-hour key 6.
- AppMessage reverse-theme key 7.
- Weather keys 10–12.
- Weather request key 20 and settings-ready key 21.
- Persistence theme key 100 and slot keys 101–104.
- Theme IDs currently include Default 0, Blue 1, Pink 2, Green 3, White 4, Orange 5, legacy Dark Blue 6, Black 7, Red 8, Yellow 9, legacy Khaki 10, and Gray 11.

New behavior must append or explicitly migrate values. Never silently reuse an existing ID for a different meaning.

## Hosting release contract

1. Work only in `Hosting/apps/daypal/qa/app-config.html`.
2. Validate the QA page in a browser and through Pebble configuration.
3. Copy the exact validated bytes to `Hosting/apps/daypal/releases/2.0.0/app-config.html`.
4. Promote the same bytes to `Hosting/apps/daypal/prod/app-config.html`.
5. Verify matching blob SHAs.
6. Switch the app companion to production with a fresh production cache label.
7. Never edit the immutable 2.0.0 snapshot after release.

## Next work

1. Finish and approve the 2.0.0 wireframes.
2. Resolve fourth-slot migration and new-theme ID behavior.
3. Author PRD and technical specification from the approved design.
4. Inventory required icon colors/sizes before editing manifests.
5. Implement one release slice at a time and record QA in `docs/2.0.0/qa/README.md`.
