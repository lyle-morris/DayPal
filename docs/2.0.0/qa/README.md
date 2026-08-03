# DayPal 2.0.0 QA Evidence

**Status:** Not started — wireframes are still in progress.

Do not mark a section complete without evidence and a date.

## Visual QA

- [ ] Approved Figma node and revision recorded.
- [ ] Native 200 × 228 goldens exported.
- [ ] Emulator captures stored unscaled.
- [ ] Overlay/difference review completed for every approved layout and theme.
- [ ] Text optical adjustments documented and limited to approved pixel compensation.
- [ ] Physical Pebble Time 2 review completed.

## Upgrade and persistence QA

- [ ] Upgrade from the published 1.6.x build preserves existing settings.
- [ ] AppMessage IDs retain their meanings.
- [ ] Metric IDs retain their meanings.
- [ ] Theme IDs retain their meanings.
- [ ] Persistence IDs retain their meanings.
- [ ] Fourth-slot migration follows the approved rule.
- [ ] Browser settings migration is validated.

## Functional QA

- [ ] Every supported metric renders in every visible position.
- [ ] Duplicate metric selections behave independently where supported.
- [ ] Unavailable health/weather values use the approved fallback.
- [ ] Leading zero and 12/24-hour behavior pass independently.
- [ ] Current and manual location work.
- [ ] Weather refresh crosses the approved real-time schedule boundaries.
- [ ] Cached weather survives temporary fetch/location failures.
- [ ] Analytics consent behavior and privacy constraints pass.
- [ ] Localization strings and glyphs pass for the approved language set.

## Hosted configuration QA

- [ ] QA URL opens in a normal browser.
- [ ] QA URL opens through Pebble configuration.
- [ ] Settings save and return correctly.
- [ ] Reset behavior matches the approved defaults.
- [ ] Mobile layout, focus states, and validation pass.
- [ ] QA, production, and the immutable 2.0.0 snapshot are byte-identical at promotion.
- [ ] QA and production cache labels are distinct.

## Build and release QA

- [ ] Emery build succeeds without new warnings.
- [ ] Resource IDs resolve and every manifest path exists.
- [ ] Runtime/RAM behavior is validated.
- [ ] Final PBW identity and version are recorded.
- [ ] App-store screenshots, banner, and release notes are approved.
