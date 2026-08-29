# DayPal 2.0.0 Development

This branch contains the in-progress DayPal 2.0.0 work for Pebble Time 2 / Emery.

**Branch:** `daypal-2.0.0-dev`  
**Status:** Active development / partial QA prototype / **not release-ready**  
**Base:** stable DayPal `main` 1.6.x line  
**Visual authority:** Figma section `84:1722`

## Start here

- Release index: [`docs/2.0.0/README.md`](docs/2.0.0/README.md)
- Current handoff and blockers: [`docs/2.0.0/DayPal-Handoff-2.0.0.md`](docs/2.0.0/DayPal-Handoff-2.0.0.md)
- Pixel-perfect gate: [`docs/2.0.0/DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md`](docs/2.0.0/DayPal-Pixel-Perfect-QA-Checklist-2.0.0.md)
- QA evidence: [`docs/2.0.0/qa/README.md`](docs/2.0.0/qa/README.md)
- Draft release notes: [`docs/2.0.0/DayPal-Release-Notes-2.0.0.md`](docs/2.0.0/DayPal-Release-Notes-2.0.0.md)

## What is already present

- 200 × 228 DayPal 2.0 watchface geometry work.
- 3-slot and 4-slot rendering code paths and matching 42px/32px asset sets.
- 10 fixed theme palette work: Default, Orange, Blue, Purple, Yellow, Green, Red, Pink, White, Black.
- Weather, Heart Rate, Battery, Calories, and Steps metric support from the 1.6.x line.
- Additional Sleep, Activity Time, and Distance IDs/assets/UI entries.
- Hosted 2.0 QA page in `lyle-morris/Hosting/apps/daypal/qa/`.
- Manual/current-location weather flow and last-successful-weather fallback.

The repository copy of `app-config/index.html` and the consolidated Hosting QA page currently share the same blob, so QA is using the checked-in development config source.

## Known release blockers

This branch is intentionally **not** merged to `main` because the 2.0 release contract is not complete.

- `DAYPAL_QA_SLOT_COUNT` still forces the watchface to three visible metrics for QA.
- The hosted 3/4-slot toggle does not have a real persisted layout key on the watch/companion.
- Three-slot mode currently saves Slot 4 as `None`; the hidden Slot 4 choice is not preserved across a save/reopen cycle.
- Companion/config defaults still describe four visible slots, conflicting with the planned new-install three-slot default.
- The exact three-metric new-install/Reset Layout default is still not frozen.
- Sleep, Activity Time, and Distance render `---` in normal runtime because live Health data is not wired yet.
- Language is currently presentation-only: the config shows only English and does not serialize a language setting to the companion/watch.
- The Country control is not part of the saved manual-location model.
- Analytics is still a local test-event buffer; GA4 collection/reporting is not implemented in the 2.0 runtime.
- Weather refresh is currently 30 minutes on-watch; the 2.0 contract calls for 15-minute scheduling plus dedupe/retry/reconnect behavior.
- `appinfo.json` and `package.json` still identify the branch as 1.6.0.
- The companion still points to the consolidated **QA** config URL.
- Hosting does not yet contain `apps/daypal/releases/2.0.0/`.
- Formal QA evidence and final physical Pebble Time 2 signoff are incomplete.

See the handoff for the full blocker history and resume sequence.

## Hosting

DayPal 2.0 development uses the consolidated Hosting repository:

- QA: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- Future production: `https://lyle-morris.github.io/Hosting/apps/daypal/prod/app-config.html`
- Future immutable 2.0 snapshot: `https://lyle-morris.github.io/Hosting/apps/daypal/releases/2.0.0/app-config.html`

The legacy `lyle-morris/DayPal-Hosting` repository must remain online for installed 1.6.x builds and is not the 2.0 development target.

## Important design check before resuming

The checked-in 2.0 contract uses the ten fixed preset themes above. Later design discussions explored a more granular custom-color theming model. Before doing substantial theme/config work, verify the latest approved Figma direction instead of assuming either model is final.
