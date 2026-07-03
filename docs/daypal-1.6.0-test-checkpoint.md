# DayPal 1.6.0 Test Checkpoint Checklist

## Current checkpoint

This checkpoint verifies the first 1.6.0 implementation slice:

- App version updated to 1.6.0
- PebbleKit JS settings model expanded
- DayMate storage references migrated to DayPal
- Hosted configuration page deployed to `lyle-morris/DayMate-config`
- Use 24-hour format setting added
- Reverse theme colors setting added
- Celsius weather request support added
- Manual location weather support added
- Ko-fi donation URL passed to app-config
- App-config baseline added under `app-config/index.html`

## Post-checkpoint implementation slice

After the Pebble Cloud test checkpoint, the `daypal-1.6.0-dev` branch includes these watchface polish updates:

- Clock text drawing now uses a small horizontal bleed and `GTextOverflowModeFill` to avoid clipping the minute trailing zero.
- Hour and minute vertical offsets are adjusted together for better visual centering.
- Weather temperature renders as a degree-only value, for example `100°`.
- Unavailable metric text uses the explicit fallback color `#666666`.
- First-run 24-hour mode now falls back to the watch setting when no saved DayPal setting exists.

## Not included yet

- Runtime PNG recoloring / final Icon Manager implementation
- Icon resource cleanup
- Real analytics service integration
- Final repository rename from `DayMate-config` to `DayPal-config`
- Screenshot resource for Pebble app listing metadata
- Rain asset rename/update to `pulsecast_weather_rain_40_0165FC.png`

## Before Pebble Cloud upload

1. Confirm this URL loads in a browser:

```text
https://lyle-morris.github.io/DayMate-config/
```

2. Confirm `package.json` is on version `1.6.0`.
3. Confirm branch is `daypal-1.6.0-dev`.
4. Confirm `src/pkjs/index.js` points to `https://lyle-morris.github.io/DayMate-config/` for this test checkpoint.

## Pebble Cloud smoke test

Upload/build the `daypal-1.6.0-dev` branch.

Expected build result:

- Build succeeds
- No missing resource IDs
- No C compile errors
- No JavaScript syntax errors

## Emulator/watch smoke test

1. Install DayPal.
2. Confirm watchface loads.
3. Confirm default layout:
   - Slot 1: Weather
   - Slot 2: Heart rate
   - Slot 3: Battery
   - Slot 4: Steps
4. Open settings from the Pebble app.
5. Confirm app-config page loads.
6. Toggle Use 24-hour format off and save.
7. Confirm watch uses 12-hour time.
8. Toggle Display leading zero off and save.
9. Confirm single-digit hour displays without leading zero.
10. Toggle Reverse theme colors on with Blue selected and save.
11. Confirm watch uses black background with blue text/icons.
12. Toggle Use Celsius for temperature and save.
13. Confirm weather refreshes using Celsius numeric value.
14. Toggle Set location manually on.
15. Enter `30068` in ZIP/postal code and save.
16. Confirm weather resolves and displays.
17. Repeat manual location with City only.
18. Confirm leaving ZIP/postal code and City blank shows the validation message.
19. Confirm Donate opens Ko-fi externally from the config page.
20. Confirm Share anonymous usage analytics can be unchecked and saved.
21. Confirm weather text renders as `100°`-style degree-only text, not `100°F` or `100°C`.
22. Confirm unavailable Weather, Heart Rate, Calories, or Steps text renders as `#666666` fallback text.
23. Confirm minute values ending in zero, such as `10`, `20`, `30`, `40`, and `50`, do not clip the trailing zero.
24. Confirm the default 24-hour behavior follows the watch setting on a fresh install before changing DayPal settings.

## Screenshots/logs to capture

Please capture:

- Build result / build log
- Default watchface screenshot
- Blue reversed screenshot
- Config page screenshot
- Manual-location error screenshot
- Manual-location success screenshot
- Minute trailing-zero screenshot, ideally `10:50` or another minute ending in `0`
- Unavailable metric fallback screenshot

## Known risks

- The hosted config repo is still named `DayMate-config`; the test branch points to that working URL for now.
- Manual ZIP/postal geocoding uses Open-Meteo geocoding search. Some postal codes may resolve better with country context in a future enhancement.
- Icon Manager runtime recoloring is not complete in this checkpoint; existing icon variants remain in place for safety.
- Pebble app screenshot metadata and the renamed blue rain asset still require binary asset updates.