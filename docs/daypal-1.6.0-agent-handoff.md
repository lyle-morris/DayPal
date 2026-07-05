# DayPal 1.6.0 Agent Handoff

Use this document to resume DayPal 1.6.0 release work in a future ChatGPT conversation.

## Quick resume prompt

```text
Continue DayPal 1.6.0 release wrap-up using docs/daypal-1.6.0-agent-handoff.md as the source of truth. Verify current repo state before making changes.
```

## Repositories and branches

- Main app repo: `lyle-morris/DayPal`
- Hosting repo: `lyle-morris/DayPal-Hosting`
- Development branch used during 1.6.0: `daypal-1.6.0-dev`
- User reported `daypal-1.6.0-dev` has been merged to `main`
- Target platform: Pebble Time 2 / `emery`
- Release: DayPal 1.6.0

## Important production URLs

- Production config: `https://lyle-morris.github.io/DayPal-Hosting/app-config.html`
- Production analytics info page: `https://lyle-morris.github.io/DayPal-Hosting/analytics.html`
- Production location testing: `https://lyle-morris.github.io/DayPal-Hosting/location-testing.html`
- Dev config: `https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html`
- Dev analytics info page: `https://lyle-morris.github.io/DayPal-Hosting/dev/analytics.html`
- Dev location testing: `https://lyle-morris.github.io/DayPal-Hosting/dev/location-testing.html`

## Source-of-truth docs

Release docs are in `/docs` in `lyle-morris/DayPal`:

- `docs/daypal-1.6.0-requirements.md`
- `docs/daypal-1.6.0-technical-spec.md`
- `docs/daypal-1.6.0-release-workflow.md`
- `docs/daypal-1.6.0-icon-manager.md`
- `docs/daypal-1.6.0-config-handoff.md`
- `docs/daypal-1.6.0-wireframes.pdf`
- `docs/releases/daypal-1.6.0.md`
- `docs/daypal-1.6.0-agent-handoff.md`

## Release status at handoff

DayPal 1.6.0 is effectively release-ready.

Completed and validated:

- Old DayMate/DayPal config references were cleaned up.
- `src/pkjs/index.js` was changed to production config URL before production build.
- Production builds should not reference `/dev/` or `/qa/`.
- Production config page uses `app-config.html`.
- Blue dev banner was removed from production config.
- Icon Manager implementation and theme behavior were tested successfully.
- Missing icon image resources were uploaded during development.
- Location testing was successful.
- App store screenshots and 720x320 banner were created during release work.
- Short app store release notes were drafted.
- GA4 analytics was configured and validated.
- Umami analytics was retired.
- Custom Cloud Run analytics API and custom dashboard approach were shelved.
- No Cloud Run service was successfully deployed.

## Production config rule

During development:

```js
CONFIG_URL should point to /dev/app-config.html
```

Before production merge/build:

```js
CONFIG_URL must point to /app-config.html
```

Production builds must not reference:

- `/dev/`
- `/qa/`
- `DayMate-config`
- `DayPal-config`

## Icon Manager requirements

Do not use runtime bitmap tinting. Use pre-generated PNG resources.

Default theme uses multi-color metric icons:

- Weather: `#FFFF00`
- Heart rate: `#FF0000`
- Battery: `#00FF00`
- Steps: `#00AAFF`
- Calories: `#FF5500`

Non-reverse color themes use black icons/text/borders:

- Blue
- Orange
- Green
- Pink
- Yellow
- Red
- Black option

Reverse color themes use selected theme-color icons/text/borders:

- Blue: `#0055FF`
- Orange: `#FF5500`
- Green: `#00AA55`
- Pink: `#FF00AA`
- Yellow: `#FFCC55`
- Red: `#FF0055`
- Black reverse: `#FFFFFF`

Reverse Default keeps default multi-color metric behavior on black unless intentionally changed later.

Icon selection dimensions:

- Metric
- Weather condition
- Battery bucket / charging state
- Current theme
- `reverse_theme` setting

Avoid referencing `RESOURCE_ID_*` constants unless the resource is registered in both `package.json` and `appinfo.json`.

## Preserved watchface behavior

Keep these behaviors unless a future spec says otherwise:

- Weather has no degree symbol.
- Battery has no percent symbol.
- Unavailable values render as `---` in `#666666`.
- Leading zero setting remains intact.
- 24-hour format setting remains intact.
- Time format should follow the watch/config setting, not be forced globally.

## Config page status

Production and dev config pages use GA4 analytics and the DayPal settings UI.

Config page privacy model:

- Analytics loads only when `Share anonymous usage analytics` is enabled.
- No ZIP/postal code is sent.
- No city is sent.
- No GPS coordinates are sent.
- No weather values are sent.
- No heart-rate data is sent.
- No battery percentage/value is sent.
- No personally identifying information is intentionally sent by DayPal.

Known implementation note:

- If analytics is disabled after GA4 has already loaded during the same config-page session, DayPal custom events stop, but the already-loaded GA library remains until the page closes. For the Pebble config flow this is short-lived and was not considered a release blocker.

## Analytics status

GA4 is the analytics solution for DayPal 1.6.0.

- GA4 Measurement ID: `G-T6KLQ9EYKX`
- GA4 Property ID: `544281382`
- GA4 Realtime validation was successful.
- Validated GA4 events included:
  - `page_view`
  - `settings_saved`
  - `first_visit`
  - `session_start`
  - `user_engagement`
  - `scroll`

DayPal custom events retained:

- `settings_saved`
- `layout_reset`
- `donation_clicked`

Umami:

- Umami was tested first and received events, but dashboard behavior was confusing.
- Umami was retired in favor of GA4.
- Remove/avoid any lingering `cloud.umami.is`, Umami website IDs, or Umami-specific code.

Custom dashboard / Cloud Run:

- A Cloud Run analytics API was briefly drafted but shelved.
- Cloud Run deployment failed due to Cloud Shell/account environment issues.
- No Cloud Run service was successfully deployed.
- No Cloud Run billing should be active from this project work.
- `analytics.html` and `dev/analytics.html` in the hosting repo should remain simple informational pages pointing users/admins to GA4 conceptually, not requiring a backend endpoint.

## Hosting repo status

`lyle-morris/DayPal-Hosting` contains production and dev hosted pages.

Expected production files:

- `app-config.html`
- `analytics.html`
- `location-testing.html`

Expected dev files:

- `dev/app-config.html`
- `dev/analytics.html`
- `dev/location-testing.html`

Production `analytics.html` should not expose a private dashboard or admin-only data. It should remain simple because GA4 is the real analytics dashboard.

## Final release checklist

Before declaring 1.6.0 fully complete:

1. Confirm `src/pkjs/index.js` in the release branch/main points to `https://lyle-morris.github.io/DayPal-Hosting/app-config.html`.
2. Confirm production app config opens from Pebble app / CloudPebble build.
3. Confirm settings save and return to Pebble app.
4. Confirm GA4 Realtime receives `page_view` from production config.
5. Confirm GA4 Realtime receives `settings_saved` after saving settings.
6. Confirm location/weather still works after final production config changes.
7. Confirm no production references to `/dev/`, `/qa/`, `DayMate-config`, or `DayPal-config`.
8. Confirm screenshots/banner/release notes are ready for the app store.
9. Publish DayPal 1.6.0.

## Suggested future audit commands/searches

Search `lyle-morris/DayPal` and `lyle-morris/DayPal-Hosting` for:

```text
DayMate-config
DayPal-config
cloud.umami
58191cd3
/dev/app-config.html
/qa/
G-T6KLQ9EYKX
settings_saved
layout_reset
donation_clicked
```

Review these files if anything changes:

- `src/pkjs/index.js`
- `appinfo.json`
- `package.json`
- `src/c/main.c`
- `app-config/index.html`
- `docs/daypal-1.6.0-release-workflow.md`
- `docs/daypal-1.6.0-icon-manager.md`
- `docs/releases/daypal-1.6.0.md`
- Hosting repo `app-config.html`
- Hosting repo `analytics.html`
- Hosting repo `location-testing.html`

## Notes for future agents

Always verify current GitHub state before editing. The user has been working across two repos and has sometimes merged branches manually. Treat this handoff as context, not as a substitute for checking the files.

When editing the hosting repo, be careful not to expose analytics/admin links from the public config page. The user specifically wanted analytics links hidden from normal config users.

When working on app-store assets, required sizes included:

- Watchface screenshots: `200x288px`
- Product banner: `720x320px`

The product banner went through iterations to remove text/icons and keep rainbow dots in the background with the Pebble Time 2/watchface image.

## Minimal future prompt

```text
Continue DayPal 1.6.0 from docs/daypal-1.6.0-agent-handoff.md. Verify current GitHub state, then complete the final release checklist.
```
