# DayPal 2.0.0

**Status:** Planning / wireframes in progress  
**Development branch:** `daypal-2.0.0-dev`  
**Baseline:** DayPal `main` at `9ce17508637784bd04138f45ce555cfd7f49c867` (1.6.1 weather-cache fallback hotfix)  
**Visual source:** [Figma section 84:1722](https://www.figma.com/design/oRliygHF8qzrzs5THYQOMH/Pebble?node-id=84-1722&m=dev)

This folder is the source of truth for the DayPal 2.0.0 release. Requirements remain draft until the wireframes are approved.

## Current design direction

The current 2.0.0 Figma section shows:

- A fixed 200 × 228 Pebble Time 2 watchface.
- A three-row metric tray beside the stacked time and date.
- Weather, Heart Rate, and Battery in the illustrated default layout.
- Nine visible theme choices: Default, Orange, Blue, Purple, Yellow, Green, Red, White, and Black.
- A 390px-wide hosted configuration page with General settings, Theme, Information, Manual location, Language, Analytics, Support, Reset, and Save sections.
- Manual location fields for Country, ZIP/postal code, and City.
- Localization controls carried into the configuration page.

These observations describe the current wireframes; they are not final acceptance criteria.

## Essential Redux controls carried forward

DayPal 2.0.0 will reuse the release discipline proven in Essential Redux 2.0.0:

1. Preserve existing AppMessage, metric, theme, and persistence meanings. New values are appended.
2. Treat the approved Figma frame as the sole visual authority.
3. Compare native 200 × 228 emulator captures against Figma goldens; document any 1px optical compensation.
4. Keep the hosted config self-contained unless shared assets are explicitly versioned.
5. Develop at the consolidated QA URL, then promote the exact validated file to production and an immutable 2.0.0 snapshot.
6. Use a new cache label for QA and another for the production release.
7. Keep cached weather visible during temporary failures and test scheduled refresh across real time boundaries.
8. Record build, emulator, configuration, upgrade, and physical-watch evidence before release closeout.

## Hosting paths

- QA: `https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html`
- Production: `https://lyle-morris.github.io/Hosting/apps/daypal/prod/app-config.html`
- Planned immutable snapshot: `https://lyle-morris.github.io/Hosting/apps/daypal/releases/2.0.0/app-config.html`
- Legacy compatibility host: `https://lyle-morris.github.io/DayPal-Hosting/app-config.html`

Published 1.6.x builds must continue to work through `DayPal-Hosting`. The 2.0.0 development branch points to consolidated QA. Production promotion and the 2.0.0 snapshot happen only after hosted-page QA passes.

## Compatibility decisions required before implementation

- Define how the current fourth metric slot migrates to the three-slot 2.0.0 design.
- Preserve existing metric IDs and persistence keys even if a slot is no longer visible.
- Preserve existing theme IDs. White already uses ID 4; ID 6 is a legacy Dark Blue value normalized to Blue; new Purple behavior must not silently change an installed user's saved theme.
- Decide whether Pink remains as a compatible hidden theme or remains selectable.
- Decide whether the Figma label “Use 12-hour format” changes the stored `use_24_hour` behavior or is only a UI wording inversion.
- Define browser settings-key migration before changing `daypal_settings`.
- Confirm whether the weather temperature should continue without a degree symbol.
- Confirm default settings for new installs separately from upgrade behavior.

## Planned release documents

- Product requirements
- Technical specification
- Asset inventory
- Localization plan
- Pixel-perfect QA checklist
- Release notes
- Final handoff
- QA evidence index

## Release sequence

1. Approve wireframes and resolve compatibility decisions.
2. Freeze PRD and technical specification.
3. Implement the watchface and consolidated QA configuration page.
4. Run upgrade, weather, settings, localization, asset, and pixel-perfect QA.
5. Promote the exact QA config to production and `releases/2.0.0`.
6. Switch the companion from the QA cache label to a fresh production cache label.
7. Build the final Emery PBW, complete physical-device QA, and close documentation.
