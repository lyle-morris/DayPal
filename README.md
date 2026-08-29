# DayPal

DayPal is a configurable Pebble Time 2 watchface built for fast, glanceable daily context. It pairs a stacked clock with a metric tray, configurable themes, weather, Pebble Health data, and a hosted settings page.

## Repository status

### Stable / production baseline

`main` is the stable 1.6.x source line. The current head includes the last-successful-weather fallback hotfix that was merged as the **DayPal 1.6.1 weather cache fallback hotfix**.

Important historical metadata note: `appinfo.json` and `package.json` on `main` still identify the app as `1.6.0`. Do not use those two files alone to infer the published Store revision of the 1.6.x line.

The published 1.6.x companion still opens the legacy compatibility host:

`https://lyle-morris.github.io/DayPal-Hosting/app-config.html`

Do not remove that endpoint while installed 1.6.x builds may still depend on it.

### DayPal 2.0.0 development

DayPal 2.0.0 is **not release-ready** and must not be merged to `main` yet.

- Development branch: `daypal-2.0.0-dev`
- Pull request: #7
- Figma authority: section `84:1722`
- Consolidated QA config: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- Current handoff: [DayPal 2.0.0 Handoff](https://github.com/lyle-morris/DayPal/blob/daypal-2.0.0-dev/docs/2.0.0/DayPal-Handoff-2.0.0.md)
- QA evidence: [DayPal 2.0.0 QA Evidence](https://github.com/lyle-morris/DayPal/blob/daypal-2.0.0-dev/docs/2.0.0/qa/README.md)

The 2.0 branch contains substantial layout, asset, theme, and hosted-config work, but it still contains explicit QA-only behavior and incomplete release contracts. The handoff is the source of truth for what is implemented versus what remains blocked.

## Repository boundaries

### Watchface and companion

Repository: `lyle-morris/DayPal`

- `main` — stable 1.6.x source line
- `daypal-2.0.0-dev` — active 2.0 development only

### Consolidated hosting

Repository: `lyle-morris/Hosting`

- `apps/daypal/qa/` — DayPal 2.0 development/QA
- `apps/daypal/prod/` — consolidated stable production copy
- `apps/daypal/releases/` — immutable release snapshots

There is currently **no `releases/2.0.0/` snapshot**. That is a release blocker, not a cleanup omission.

### Legacy hosting

Repository: `lyle-morris/DayPal-Hosting`

This repository remains online for compatibility with published 1.6.x builds. It should not be repurposed for DayPal 2.0 development.

## DayPal 2.0 release gate

Do not promote DayPal 2.0.0 until all of the following are true:

1. QA-only slot-count forcing is removed.
2. 3/4-slot mode has a real persisted/AppMessage contract.
3. Fresh-install and Reset Layout defaults are frozen and implemented.
4. Hidden Slot 4 survives 3-slot mode across save/reopen.
5. Sleep, Activity Time, and Distance use live Pebble Health data or are removed from the release selector.
6. Language UI has a real settings/watch implementation or is removed from the release UI.
7. Manual-location country behavior is fully implemented or the dead control is removed.
8. GA4 analytics is implemented and privacy-tested; local test-event buffering is not sufficient.
9. Weather refresh/retry behavior matches the approved release contract.
10. The app and package versions are set to 2.0.0.
11. Native visual QA and physical Pebble Time 2 signoff are recorded.
12. The exact approved QA config is copied to `prod` and `releases/2.0.0` and verified.
13. The companion is switched from QA to the production config URL with a fresh cache token.
14. A final Emery PBW is built and recorded.

## Development guidance

For 2.0 work, start with the [DayPal 2.0.0 release index](https://github.com/lyle-morris/DayPal/blob/daypal-2.0.0-dev/docs/2.0.0/README.md) and then read the handoff. Do not infer implementation status from the PR title or the Figma alone.
