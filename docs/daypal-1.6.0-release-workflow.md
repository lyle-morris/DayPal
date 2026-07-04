# DayPal 1.6.0 Release Workflow

Status: Release-cycle reference
Branch: `daypal-1.6.0-dev`
Applies to: DayPal watchface repo and `lyle-morris/DayPal-Hosting`

## 1. Purpose

This document defines the DayPal 1.6.0 release workflow so future release cycles can follow a consistent development, QA, hosting, and production-promotion process.

The key principle is separation of concerns:

- DayPal watchface repo owns watchface code, Pebble manifests, watch assets, and release documentation.
- DayPal-Hosting owns hosted configuration, analytics dashboard, and location/weather QA pages.
- Development URLs must not ship in production watchface builds.

## 2. Repositories

### 2.1 DayPal watchface repo

Repository:

```text
lyle-morris/DayPal
```

Responsibilities:

- Pebble C watchface code.
- PebbleKit JS companion code.
- `package.json` and `appinfo.json` manifests.
- Watchface icon resources.
- Release documentation.
- CloudPebble import/build package.

### 2.2 DayPal-Hosting repo

Repository:

```text
lyle-morris/DayPal-Hosting
```

Responsibilities:

- Hosted app configuration page.
- Hosted analytics dashboard.
- Hosted location/weather QA tool.
- Dev and production hosting paths.

## 3. Branch model

### 3.1 Watchface repo branches

```text
main                  Production watchface release branch
daypal-1.6.0-dev      Active 1.6.0 development branch
```

Future releases should follow the same pattern:

```text
daypal-<version>-dev
```

Example:

```text
daypal-1.7.0-dev
```

### 3.2 Hosting repo structure

DayPal-Hosting uses path-based dev and production deployment.

Development files:

```text
DayPal-Hosting/dev/app-config.html
DayPal-Hosting/dev/analytics.html
DayPal-Hosting/dev/location-testing.html
```

Production files:

```text
DayPal-Hosting/app-config.html
DayPal-Hosting/analytics.html
DayPal-Hosting/location-testing.html
```

## 4. URL model

### 4.1 Development URLs

The watchface development branch should point to the hosted development config page:

```text
https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html
```

Development support URLs:

```text
https://lyle-morris.github.io/DayPal-Hosting/dev/analytics.html
https://lyle-morris.github.io/DayPal-Hosting/dev/location-testing.html
```

### 4.2 Production URLs

Production watchface builds must point to the production app-config page:

```text
https://lyle-morris.github.io/DayPal-Hosting/app-config.html
```

Production support URLs:

```text
https://lyle-morris.github.io/DayPal-Hosting/analytics.html
https://lyle-morris.github.io/DayPal-Hosting/location-testing.html
```

## 5. Configuration URL rule

The PebbleKit JS companion controls which hosted configuration page opens from the Pebble mobile app.

File:

```text
src/pkjs/index.js
```

Development value:

```js
var CONFIG_URL = 'https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html';
```

Production value:

```js
var CONFIG_URL = 'https://lyle-morris.github.io/DayPal-Hosting/app-config.html';
```

Release rule:

- Development branches may reference `/dev/app-config.html`.
- Production release branches must not reference `/dev/`, `/qa/`, or any temporary hosting path.
- Before merging into `main`, update `CONFIG_URL` to the production app-config URL.

## 6. Development workflow

### 6.1 Start a release

1. Create or reuse a release dev branch in the DayPal repo.
2. Create or update release source-of-truth docs in `/docs`.
3. Point `src/pkjs/index.js` to the DayPal-Hosting development app-config URL.
4. Keep production hosted files unchanged until release approval.

### 6.2 Build release features

During feature development:

- Update watchface code in `src/c/main.c`.
- Update PebbleKit JS in `src/pkjs/index.js`.
- Update manifests when resources or AppMessage keys change.
- Update release docs when requirements or implementation decisions change.
- Keep `package.json` and `appinfo.json` aligned.
- Keep DayPal-Hosting dev files aligned with the active release.

### 6.3 Update hosted dev files

When app-config, analytics, or location testing changes:

1. Update the relevant source file.
2. Publish or copy it to DayPal-Hosting `/dev/`.
3. Validate the hosted development URL loads.
4. Validate the Pebble app opens the dev app-config URL.

## 7. Manifest workflow

DayPal supports CloudPebble, so both manifests must be maintained.

Files:

```text
package.json
appinfo.json
```

Rules:

- `package.json` and `appinfo.json` must reference the same active media resources.
- Neither manifest may reference missing files.
- Any new icon, font, or resource used by C code must be registered in both manifests.
- Any removed resource must be removed from both manifests.
- CloudPebble may use `appinfo.json`, so it cannot be treated as secondary or stale.

## 8. Icon/resource workflow

DayPal 1.6.0 uses pre-generated PNG resources for icons.

Rules:

- Do not runtime-tint or recolor Pebble bitmap assets.
- Generate required icon colors as PNG files before referencing them in C code.
- Register new resources in both manifests before using their `RESOURCE_ID_*` constants.
- Verify black, white, reverse-theme, and default multi-color icon assets exist.

Implementation order:

1. Upload or generate PNG assets.
2. Register assets in `package.json`.
3. Mirror registrations in `appinfo.json`.
4. Update `src/c/main.c` to reference the new `RESOURCE_ID_*` constants.
5. Build in CloudPebble.

## 9. QA workflow

### 9.1 Watchface QA

Validate these items before release approval:

- CloudPebble import succeeds.
- CloudPebble build succeeds.
- Default theme uses multi-color metric icons.
- Non-reverse themes use black text, borders, and icons.
- Reverse themes use selected theme-color text, borders, and icons.
- Reverse Black uses white text, borders, and icons.
- Weather displays without a degree symbol.
- Battery displays without a percent symbol.
- Unavailable values display as `---` in `#666666`.
- Leading zero setting works.
- 12-hour and 24-hour settings work.
- Metric slot customization works.
- Empty slots do not break layout.

### 9.2 Hosted config QA

Validate these development URLs:

```text
https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html
https://lyle-morris.github.io/DayPal-Hosting/dev/analytics.html
https://lyle-morris.github.io/DayPal-Hosting/dev/location-testing.html
```

Validate app-config behavior:

- Existing settings load from query string.
- Save settings returns `pebblejs://close#<encoded-json>`.
- Reset layout restores default slot order and default theme.
- Manual location shows ZIP/postal code and City fields.
- Manual location validation error appears when both fields are empty.
- Analytics opt-in/out can be saved.
- Donation link opens externally.

### 9.3 Weather/location QA

Validate:

- Current-location weather works.
- Manual city lookup works.
- Manual ZIP/postal code lookup works.
- Supported international postal/city lookup works where the weather provider supports it.
- Celsius setting changes returned temperature units.
- Unsupported weather response fails gracefully.

## 10. Production promotion workflow

Production promotion should happen only after watchface and hosted dev QA pass.

### 10.1 Promote DayPal-Hosting files

Copy or promote:

```text
DayPal-Hosting/dev/app-config.html        -> DayPal-Hosting/app-config.html
DayPal-Hosting/dev/analytics.html         -> DayPal-Hosting/analytics.html
DayPal-Hosting/dev/location-testing.html  -> DayPal-Hosting/location-testing.html
```

Then validate the production hosted URLs:

```text
https://lyle-morris.github.io/DayPal-Hosting/app-config.html
https://lyle-morris.github.io/DayPal-Hosting/analytics.html
https://lyle-morris.github.io/DayPal-Hosting/location-testing.html
```

### 10.2 Update production watchface URL

In the DayPal release branch, update:

```text
src/pkjs/index.js
```

From:

```js
var CONFIG_URL = 'https://lyle-morris.github.io/DayPal-Hosting/dev/app-config.html';
```

To:

```js
var CONFIG_URL = 'https://lyle-morris.github.io/DayPal-Hosting/app-config.html';
```

### 10.3 Final build validation

After switching to the production config URL:

1. Pull/import the release branch into CloudPebble.
2. Build the project.
3. Open configuration from the Pebble mobile app.
4. Confirm it opens the production app-config URL.
5. Save settings and verify they apply on the watch.
6. Verify weather request still works.
7. Verify no dev-only copy or QA notes appear in the production config page.

## 11. Merge workflow

After production promotion and final build validation:

1. Confirm docs are current.
2. Confirm no stale release docs remain in `/docs`.
3. Confirm no references to old release-only paths remain.
4. Confirm `src/pkjs/index.js` points to production app-config.
5. Confirm `package.json` and `appinfo.json` are aligned.
6. Merge the release dev branch into `main`.
7. Tag the release if desired.

## 12. Pre-merge audit checklist

Search the release branch for stale references:

```text
1.5.0
1.0.0
DayMate-config
DayPal-config
/qa/
/dev/app-config.html
index.html as hosted production config
```

Expected outcome before production merge:

- Historical release references may remain only in intentional release-history sections.
- `DayMate-config` and `DayPal-config` should not appear as active hosting references.
- `/qa/` should not appear in active release files.
- `/dev/app-config.html` should not appear in production watchface code.
- Hosted production config should be `app-config.html`, not `index.html`.

## 13. Future release notes

For future releases, copy this workflow and update:

- Release branch name.
- Release version.
- Hosted development path if needed.
- Production target URL if hosting changes.
- Source-of-truth document names.
- CloudPebble validation notes.
