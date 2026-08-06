# DayPal 2.0.0 Analytics Specification

**Purpose:** Measure anonymous configuration usage so future DayPal design decisions are based on actual theme/layout adoption.  
**Scope:** Hosted configuration page only.  
**Existing GA4 measurement ID:** `G-T6KLQ9EYKX`.

## Privacy rule

Analytics is consented and anonymous.

Do not initialize or emit GA4 usage events unless anonymous analytics is enabled. If a user disables analytics, stop emitting usage events. Do not manufacture a stable user identifier for this feature.

Never send:

- Country, ZIP/postal code, city, coordinates, or other location data.
- Weather condition or temperature values.
- Heart-rate values.
- Steps, calories, sleep, or battery readings.
- Pebble/device identifiers.
- Account identifiers.
- Free-form strings entered by the user.

Metric **type names** such as `weather`, `steps`, or `sleep` are allowed because they describe configuration, not readings.

## Events

Use only these configuration events for the initial 2.0.0 release.

### `settings_loaded`

Emit after saved settings have been parsed and the page is ready **only when the saved analytics preference already permits analytics**.

Purpose: observe the installed configuration users return with.

### `settings_saved`

Emit after a successful Save action when the resulting analytics preference permits analytics.

Purpose: observe deliberate configuration choices.

If Save disables analytics, do not emit the disabled configuration as a GA4 event.

## Event parameters

Both events use the same schema:

| Parameter | Example | Rule |
|---|---|---|
| `app_name` | `daypal` | Constant |
| `app_version` | `2.0.0` | Release version |
| `theme_id` | `12` | Numeric theme ID |
| `theme_name` | `purple` | Normalized stable name |
| `reverse_theme` | `false` | Compatibility visibility only; 2.0 UI does not expose it |
| `layout_mode` | `3_slot` | `3_slot` or `4_slot` |
| `visible_slot_count` | `3` | 3 or 4 |
| `slot_1_metric` | `weather` | Metric type, never value |
| `slot_2_metric` | `heart_rate` | Metric type |
| `slot_3_metric` | `sleep` | Metric type |
| `slot_4_metric` | `steps` | Saved slot 4 even when hidden |
| `layout_signature` | `3_slot:weather|heart_rate|sleep` | Deterministic normalized signature |

For three-slot mode, `slot_4_metric` may report the saved hidden selection so migration/return-to-four-slot behavior can be studied; `layout_signature` must contain only the visible slots.

Allowed metric names:

- `weather`
- `heart_rate`
- `battery`
- `calories`
- `steps`
- `none`
- `sleep`

Allowed selectable theme names:

- `default`
- `orange`
- `blue`
- `purple`
- `yellow`
- `green`
- `red`
- `pink`
- `white`
- `black`

Legacy theme values should be normalized to a stable compatibility label rather than relabeled as a new 2.0 theme.

## Signature construction

Construct `layout_signature` from normalized values, not UI labels.

Examples:

- `3_slot:weather|heart_rate|sleep`
- `4_slot:weather|heart_rate|battery|steps`

Never include location, actual metric readings, language free text, or device data in the signature.

## GA4 reporting setup

To make the custom parameters usable in standard GA4 exploration/reporting, register the required event-scoped custom dimensions in the GA4 property.

Recommended custom dimensions:

- theme_id
- theme_name
- layout_mode
- visible_slot_count
- slot_1_metric
- slot_2_metric
- slot_3_metric
- slot_4_metric
- layout_signature
- app_name
- app_version

`reverse_theme` is optional as a report dimension because it exists only for legacy/compatibility visibility in DayPal 2.0.0.

## Questions the reports should answer

- Theme adoption percentage.
- Three-slot vs four-slot adoption.
- Most common metric in each position.
- Most common visible layout signatures.
- Whether upgraded four-slot users voluntarily move to three slots.

Do not use GA4 to infer or reconstruct health/location behavior.

## Cross-app consistency

Essential Redux should later use the same event and parameter names with:

`app_name=essential_redux`

Keep normalized theme/metric values app-specific where the products differ, but keep the taxonomy structurally identical so cross-app dashboards do not require separate schemas.

## QA

- [ ] Analytics disabled: no GA4 usage event is emitted.
- [ ] Previously consented settings load: one `settings_loaded` event with normalized configuration only.
- [ ] Consented Save: one `settings_saved` event after successful Save.
- [ ] Save that disables analytics: no post-disable event.
- [ ] Three-slot signature contains exactly three visible metric names.
- [ ] Four-slot signature contains exactly four visible metric names.
- [ ] Slot 4 saved value does not disappear when hidden.
- [ ] Network payload contains no location, metric reading, device ID, or free-form string.
- [ ] Custom dimensions are registered before relying on GA4 reports.
