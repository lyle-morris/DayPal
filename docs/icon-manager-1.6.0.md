# DayPal 1.6.0 Icon Manager

## Decision

DayPal 1.6.0 will use an Icon Manager to centralize metric, weather, and battery icon rendering.

## Asset Strategy

- Use one PNG per icon state.
- Use a white foreground on a transparent background.
- Recolor icons at runtime based on the active theme.
- Avoid separate black, white, and color PNG variants for every icon.

## Runtime Behavior

- Default / Multi-Color theme uses metric-specific colors:
  - Weather: `#FFFF00`
  - Heart Rate: `#FF0000`
  - Battery: `#00FF00`
  - Steps: `#00AAFF`
  - Calories: `#FF5500`
- Solid-color themes use the theme foreground color for icons.
- Unavailable metric states use fallback gray.
- Reverse colors should use the reversed foreground color.

## Implementation Direction

The drawing layer should not decide which icon variant to use. It should ask the Icon Manager to draw the correct icon for the metric, theme, and availability state.

Suggested API direction:

```c
static void icon_manager_draw_metric_icon(
  GContext *ctx,
  MetricType metric,
  DayPalTheme theme,
  bool available,
  GRect box
);
```

The Icon Manager should own:

- resource selection
- theme color selection
- unavailable-state color selection
- bitmap recoloring / palette replacement
- bitmap draw and cleanup

## Notes

Pebble does not behave like a modern mobile UI framework with simple CSS-style icon tinting. The safest implementation is to use white PNG source assets and recolor the bitmap palette or pixel data before drawing.
