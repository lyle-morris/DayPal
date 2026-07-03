# DayPal 1.6.0 App-Config Handoff

## Current status

The 1.6.0 app-config implementation baseline is available in the main DayPal repository at:

```text
app-config/index.html
```

This file is intended to be copied or moved into the hosted GitHub Pages configuration repository that serves:

```text
https://lyle-morris.github.io/DayPal-config/
```

The connected GitHub search did not expose a separate `DayPal-config` or `DayMate-config` repository, so the implementation is staged inside the main DayPal repo until the hosted config repository is available.

## Required hosted config path

The PebbleKit JS companion opens:

```text
https://lyle-morris.github.io/DayPal-config/?settings=<encoded-settings>&donation_url=<encoded-ko-fi-url>
```

The hosted config page must therefore be available at the root of the GitHub Pages site, usually:

```text
index.html
```

## Donation link

The Donate button opens:

```text
https://ko-fi.com/lylemorris
```

Payment processing must remain external to DayPal.

## App-config settings contract

The page returns a JSON payload through:

```text
pebblejs://close#<encoded-json>
```

Returned object:

```json
{
  "theme": 0,
  "slot_1_metric": 0,
  "slot_2_metric": 1,
  "slot_3_metric": 2,
  "slot_4_metric": 4,
  "show_leading_zero": true,
  "use_24_hour": true,
  "use_celsius": false,
  "manual_location": false,
  "manual_postal_code": "",
  "manual_city": "",
  "reverse_theme": false,
  "analytics_enabled": true
}
```

## Metric values

```text
0 = Weather
1 = Heart rate
2 = Battery
3 = Calories
4 = Steps
5 = None
```

## Theme values

```text
0 = Default
1 = Blue
2 = Pink
3 = Green
5 = Orange
7 = Black
8 = Red
9 = Yellow
```

Legacy values may still exist in stored settings and are normalized by the watch/companion where possible.

## Manual location validation

When `manual_location = true`, either `manual_postal_code` or `manual_city` is required.

Approved error copy:

```text
Enter a ZIP/postal code or city to set the location manually.
```

The unused field must not be marked invalid when the other field contains a valid value.

## Browser notes

The config baseline uses:

- semantic HTML controls
- CSS custom properties
- Material Symbols from Google Fonts
- `:has()` for selected theme tile styling

If the Pebble app webview has trouble with `:has()`, replace the selected theme tile styling with a small JavaScript class toggle.

## Handoff steps

1. Copy `app-config/index.html` into the hosted config repository as `index.html`.
2. Confirm GitHub Pages is publishing the root `index.html`.
3. Open the config URL in a desktop browser with a sample query string.
4. Confirm the settings render correctly.
5. Confirm Donate opens Ko-fi externally.
6. Confirm Save settings closes using `pebblejs://close#...` when opened from the Pebble configuration flow.
7. Load DayPal 1.6.0 into Pebble Cloud for device/emulator testing.

## Sample browser test URL

```text
https://lyle-morris.github.io/DayPal-config/?settings=%7B%22theme%22%3A0%2C%22slot_1_metric%22%3A0%2C%22slot_2_metric%22%3A1%2C%22slot_3_metric%22%3A2%2C%22slot_4_metric%22%3A4%2C%22show_leading_zero%22%3Atrue%2C%22use_24_hour%22%3Atrue%2C%22use_celsius%22%3Afalse%2C%22manual_location%22%3Afalse%2C%22manual_postal_code%22%3A%22%22%2C%22manual_city%22%3A%22%22%2C%22reverse_theme%22%3Afalse%2C%22analytics_enabled%22%3Atrue%7D&donation_url=https%3A%2F%2Fko-fi.com%2Flylemorris
```
