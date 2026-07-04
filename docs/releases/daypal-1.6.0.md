# DayPal 1.6.0 Release Notes

Status: In development
Branch: `daypal-1.6.0-dev`
Visual source of truth: `DayPal-Wireframes-1.6.0-Final.pdf`

## Release focus

DayPal 1.6.0 focuses on visual accuracy, theme behavior, configuration improvements, manual weather location, anonymous analytics, and release hygiene.

## Planned release scope

- Match final 1.6.0 watchface wireframes.
- Restore original metric icon shapes.
- Use correct icon colors for Default, non-reverse, and reverse themes.
- Remove runtime icon recoloring from the release approach.
- Use pre-generated PNG resources for required icon colors.
- Display weather temperature without a degree symbol.
- Display battery charge without a percent symbol.
- Support Display leading zero setting.
- Support Use 24-hour format setting.
- Support Use Celsius for temperature setting.
- Support Set location manually using ZIP/postal code or city.
- Support Reverse theme colors.
- Support configurable metric slots.
- Add Support DayPal donation panel.
- Add anonymous analytics opt-in.
- Keep production configuration protected until release approval.

## Active themes

- Default
- Blue
- Orange
- Green
- Pink
- Red
- Yellow
- Black

## Theme behavior

- Default uses black background with multi-color metric icons.
- Non-reverse color themes use selected background with black text, borders, and icons.
- Non-reverse Black uses white background with black text, borders, and icons.
- Reverse color themes use black background with selected theme-color text, borders, and icons.
- Reverse Black uses black background with white text, borders, and icons.

## Release hygiene

Before merge approval:

- `/docs` must contain only current DayPal 1.6.0 release documentation.
- Stale checkpoint docs must be removed.
- `package.json` and `appinfo.json` must reference the same valid resource set.
- No manifest may reference removed PNG assets.
- CloudPebble import/build must pass.
- Configuration page must match the final 1.6.0 wireframes.

## Out of scope

- Saved custom themes.
- User-created theme colors.
- Account sign-in.
- Collection of personal, health, or precise location data.
