# DayPal 2.0.0 Release Notes

**Status:** Draft / planned scope — **do not use as Store copy yet**

DayPal 2.0.0 is an in-progress visual and configuration refresh for Pebble Time 2. The development branch contains substantial layout, asset, theme, and app-config work, but several features described by the 2.0 design contract are not yet complete enough to claim as released behavior.

## Already present in the development branch

- Native 200 × 228 DayPal 2.0 geometry work.
- Three-slot and four-slot rendering code paths.
- 42px icon assets for three-slot presentation and 32px assets for four-slot presentation.
- Ten fixed preset themes in the checked-in implementation: Default, Orange, Blue, Purple, Yellow, Green, Red, Pink, White, and Black.
- Purple theme support.
- Weather, Heart Rate, Battery, Calories, and Steps metric support.
- Assets/UI IDs for Sleep, Activity Time, and Distance.
- Updated weather presentation with integer temperature + degree symbol.
- Consolidated DayPal 2.0 QA settings page under `lyle-morris/Hosting`.
- Current/manual-location weather with last-successful-weather fallback.

## Planned 2.0 behavior that is still blocked

The following items are **not release-complete yet** and should not be advertised as shipping until the release gate passes:

- User-controlled 3/4-slot mode. The watch currently has a compile-time three-slot QA override and no dedicated persisted layout key.
- Fresh installs defaulting to three slots. The exact three-metric default still needs to be frozen and implemented consistently.
- Preserving hidden Slot 4 across Save/reopen. The current QA page saves Slot 4 as `None` in three-slot mode.
- Live Sleep, Activity Time, and Distance data. These currently render unavailable in normal runtime.
- Language support from the 2.0 config UI. The current Language section is not wired end-to-end.
- Country-aware manual location. The Country UI is not currently serialized to the companion.
- Production GA4 analytics. Current development analytics only buffers test events locally.
- The full 15-minute weather refresh/dedupe/retry/reconnect release contract.
- Final production Hosting promotion and immutable `releases/2.0.0` snapshot.
- Final 2.0.0 manifest/package versioning and production config URL.
- Formal native visual QA evidence and physical Pebble Time 2 signoff.

## Compatibility contract

DayPal 2.0.0 must preserve existing AppMessage, persistence, metric, and theme ID meanings. New values must be appended rather than reusing old IDs.

The stable 1.6.x compatibility host at `DayPal-Hosting` must remain available for installed builds that still open that URL.

## Visual quality gate

The watchface must be validated against the latest approved Figma designs at native 200 × 228 resolution. The checked-in QA plan calls for both layouts and the complete final theme matrix to pass overlay/difference review, followed by physical Pebble Time 2 signoff.

Development conversations included positive informal reviews of the tuned 4-slot and 3-slot layouts, but those conversations do not replace the formal repository evidence required for release.

## Theme-direction note

The checked-in implementation uses ten fixed preset themes. Later product discussion explored a more granular custom-color theming model. Confirm the latest approved Figma direction before finalizing these Store notes or doing substantial theme-engine work.

## Store-facing draft — hold until QA

Once the blockers are closed, rewrite this section from the final behavior actually present in the release candidate. At minimum record:

- Final version/build identity.
- Final app commit.
- Final PBW.
- Exact supported metrics.
- Final theme model.
- Fresh-install and upgrade layout behavior.
- Analytics behavior/privacy.
- Upgrade test result.
- Physical-device signoff.
- Hosting QA/production/2.0.0 snapshot SHA verification.
- Any known limitations.
