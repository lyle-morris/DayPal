# DayPal 1.5.0 Build Notes

Source of truth: updated 1.5.0 wireframe PDF.

## Theme palette

| Theme | Background | Text/Icon | Border | Notes |
|---|---:|---:|---:|---|
| Default / Multi-Color | `#000000` | `#FFFFFF` | `#555555` | Metric colors remain individual |
| Blue | `#0055FF` | `#000000` | `#000000` | Black icons/text |
| Orange | `#FF5500` | `#000000` | `#000000` | Black icons/text |
| Green | `#00AA55` | `#000000` | `#000000` | Black icons/text |
| Pink | `#FF00AA` | `#000000` | `#000000` | Black icons/text |
| Yellow | `#FFCC55` | `#000000` | `#000000` | Replaces Khaki naming |
| Red | `#FF0055` | `#000000` | `#000000` | Black icons/text |
| Black | `#000000` | `#FFFFFF` | `#FFFFFF` | White icons/text |
| Gray | `#AAAAAA` | `#000000` | `#000000` | Black icons/text |
| White | `#FFFFFF` | `#000000` | `#000000` | Black icons/text |

## Config page requirements

- Remove theme dropdown.
- Use color tiles for selection.
- Show selected tile with a centered checkmark overlay.
- Rename Khaki to Yellow.
- Make Save settings a fixed footer.
- Keep Reset layout inside Metric slots.
- Reset layout defaults:
  - Slot 1: Weather
  - Slot 2: Heart rate
  - Slot 3: Battery
  - Slot 4: Steps

## Watchface requirements

- Default remains the multicolor theme.
- All non-default color themes use one foreground color for all metric text/icons.
- Battery 75 state represents 52-80% and has a matching charging state.
- Weather supports Sunny, Partly Cloudy, Cloudy, Rain, Storm, Snow, and Fog.
- Legacy theme value `10` should normalize to Yellow value `9` if encountered.
