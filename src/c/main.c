#include <pebble.h>
#include <string.h>

#define SCREEN_W 200
#define SCREEN_H 228
#define OUTER_PAD 8
#define METRIC_TRAY_W 42
#define GAP_W 8
#define CLOCK_PANEL_W 134
#define CLOCK_PANEL_H 212
#define CLOCK_PANEL_RADIUS 4
#define ICON_SIZE 22

#define STORAGE_KEY_THEME 100
#define STORAGE_KEY_SLOT_1_METRIC 101
#define STORAGE_KEY_SLOT_2_METRIC 102
#define STORAGE_KEY_SLOT_3_METRIC 103
#define STORAGE_KEY_SLOT_4_METRIC 104
#define STORAGE_KEY_SHOW_LEADING_ZERO 105
#define STORAGE_KEY_WEATHER_TEMP 106
#define STORAGE_KEY_WEATHER_CODE 107
#define STORAGE_KEY_WEATHER_VALID 108

typedef enum {
  METRIC_WEATHER = 0,
  METRIC_HEART_RATE = 1,
  METRIC_BATTERY = 2,
  METRIC_CALORIES = 3,
  METRIC_STEPS = 4,
  METRIC_NONE = 5
} MetricType;

typedef enum {
  THEME_DEFAULT = 0,
  THEME_BLUE = 1,
  THEME_PINK = 2,
  THEME_GREEN = 3,
  THEME_WHITE = 4,
  THEME_ORANGE = 5,
  THEME_DARK_BLUE = 6,
  THEME_BLACK = 7
} ThemeType;

typedef enum {
  WEATHER_SUNNY = 0,
  WEATHER_PARTLY_CLOUDY = 1,
  WEATHER_RAINY = 2,
  WEATHER_STORM = 3,
  WEATHER_SNOW = 4,
  WEATHER_FOG = 5,
  WEATHER_UNKNOWN = 6
} WeatherCondition;

typedef enum {
  ICON_VARIANT_DEFAULT = 0,
  ICON_VARIANT_BLACK = 1,
  ICON_VARIANT_WHITE = 2
} IconVariant;

typedef struct {
  ThemeType theme;
  MetricType slot_metrics[4];
  bool show_leading_zero;
} DayMateSettings;

typedef struct {
  GColor background;
  GColor clock_panel;
  GColor clock_text;
  GColor metric_text;
  GColor unavailable_text;
  GColor weather;
  GColor heart_rate;
  GColor battery;
  GColor calories;
  GColor steps;
  bool individual_metric_colors;
} DayMateTheme;

typedef struct {
  GBitmap *weather_sunny_black;
  GBitmap *weather_sunny_yellow;
  GBitmap *weather_sunny_white;
  GBitmap *weather_partly_cloudy_black;
  GBitmap *weather_partly_cloudy_yellow;
  GBitmap *weather_partly_cloudy_white;
  GBitmap *weather_rainy_black;
  GBitmap *weather_rainy_yellow;
  GBitmap *weather_rainy_white;
  GBitmap *weather_storm_black;
  GBitmap *weather_storm_yellow;
  GBitmap *weather_storm_white;
  GBitmap *weather_snow_black;
  GBitmap *weather_snow_yellow;
  GBitmap *weather_snow_white;
  GBitmap *weather_fog_black;
  GBitmap *weather_fog_yellow;
  GBitmap *weather_fog_white;
  GBitmap *heart_rate_black;
  GBitmap *heart_rate_green;
  GBitmap *heart_rate_white;
  GBitmap *calories_black;
  GBitmap *calories_orange;
  GBitmap *calories_white;
  GBitmap *steps_black;
  GBitmap *steps_blue;
  GBitmap *steps_white;
  GBitmap *battery_0_black;
  GBitmap *battery_0_purple;
  GBitmap *battery_0_white;
  GBitmap *battery_25_black;
  GBitmap *battery_25_purple;
  GBitmap *battery_25_white;
  GBitmap *battery_50_black;
  GBitmap *battery_50_purple;
  GBitmap *battery_50_white;
  GBitmap *battery_100_black;
  GBitmap *battery_100_purple;
  GBitmap *battery_100_white;
  GBitmap *battery_charging_0_black;
  GBitmap *battery_charging_0_purple;
  GBitmap *battery_charging_0_white;
  GBitmap *battery_charging_25_black;
  GBitmap *battery_charging_25_purple;
  GBitmap *battery_charging_25_white;
  GBitmap *battery_charging_50_black;
  GBitmap *battery_charging_50_purple;
  GBitmap *battery_charging_50_white;
  GBitmap *battery_charging_100_black;
  GBitmap *battery_charging_100_purple;
  GBitmap *battery_charging_100_white;
} DayMateImages;

static Window *s_window;
static Layer *s_canvas_layer;
static GFont s_font_time;
static GFont s_font_metric;
static GFont s_font_date;
static DayMateImages s_images;

static DayMateSettings s_settings = {
  .theme = THEME_DEFAULT,
  .slot_metrics = {METRIC_WEATHER, METRIC_HEART_RATE, METRIC_BATTERY, METRIC_STEPS},
  .show_leading_zero = true
};

static BatteryChargeState s_battery;
static bool s_weather_available = false;
static int s_weather_temp = 95;
static WeatherCondition s_weather_condition = WEATHER_PARTLY_CLOUDY;
static bool s_steps_available = false;
static int s_steps = 8542;
static bool s_heart_available = false;
static int s_heart_rate = 110;
static bool s_calories_available = false;
static int s_calories = 1520;

static DayMateTheme get_theme(void) {
  switch (s_settings.theme) {
    case THEME_BLUE:
      return (DayMateTheme){GColorBlue, GColorBlueMoon, GColorWhite, GColorWhite, GColorDarkGray, GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_PINK:
      return (DayMateTheme){GColorMagenta, GColorJazzberryJam, GColorWhite, GColorWhite, GColorDarkGray, GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_GREEN:
      return (DayMateTheme){GColorKellyGreen, GColorDarkGreen, GColorWhite, GColorWhite, GColorDarkGray, GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_WHITE:
      return (DayMateTheme){GColorWhite, GColorLightGray, GColorBlack, GColorBlack, GColorDarkGray, GColorBlack, GColorBlack, GColorBlack, GColorBlack, GColorBlack, false};
    case THEME_ORANGE:
      return (DayMateTheme){GColorOrange, GColorDarkCandyAppleRed, GColorWhite, GColorWhite, GColorDarkGray, GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_DARK_BLUE:
      return (DayMateTheme){GColorDukeBlue, GColorOxfordBlue, GColorWhite, GColorWhite, GColorDarkGray, GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_BLACK:
      return (DayMateTheme){GColorBlack, GColorDarkGray, GColorWhite, GColorWhite, GColorDarkGray, GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_DEFAULT:
    default:
      return (DayMateTheme){GColorBlack, GColorDarkGray, GColorWhite, GColorWhite, GColorDarkGray, GColorYellow, GColorGreen, GColorVividViolet, GColorOrange, GColorCyan, true};
  }
}

static GColor color_for_metric(DayMateTheme theme, MetricType metric, bool available) {
  if (!available) return theme.unavailable_text;
  if (!theme.individual_metric_colors) return theme.metric_text;
  switch (metric) {
    case METRIC_WEATHER: return theme.weather;
    case METRIC_HEART_RATE: return theme.heart_rate;
    case METRIC_BATTERY: return theme.battery;
    case METRIC_CALORIES: return theme.calories;
    case METRIC_STEPS: return theme.steps;
    default: return theme.metric_text;
  }
}

static IconVariant icon_variant_for_theme(void) {
  if (s_settings.theme == THEME_WHITE) return ICON_VARIANT_BLACK;
  if (s_settings.theme == THEME_DEFAULT) return ICON_VARIANT_DEFAULT;
  return ICON_VARIANT_WHITE;
}

static GBitmap *variant_bitmap(GBitmap *black, GBitmap *color, GBitmap *white, IconVariant variant) {
  switch (variant) {
    case ICON_VARIANT_BLACK: return black;
    case ICON_VARIANT_WHITE: return white;
    case ICON_VARIANT_DEFAULT:
    default: return color;
  }
}

static GBitmap *weather_bitmap(IconVariant variant) {
  switch (s_weather_condition) {
    case WEATHER_SUNNY:
      return variant_bitmap(s_images.weather_sunny_black, s_images.weather_sunny_yellow, s_images.weather_sunny_white, variant);
    case WEATHER_RAINY:
      return variant_bitmap(s_images.weather_rainy_black, s_images.weather_rainy_yellow, s_images.weather_rainy_white, variant);
    case WEATHER_STORM:
      return variant_bitmap(s_images.weather_storm_black, s_images.weather_storm_yellow, s_images.weather_storm_white, variant);
    case WEATHER_SNOW:
      return variant_bitmap(s_images.weather_snow_black, s_images.weather_snow_yellow, s_images.weather_snow_white, variant);
    case WEATHER_FOG:
      return variant_bitmap(s_images.weather_fog_black, s_images.weather_fog_yellow, s_images.weather_fog_white, variant);
    case WEATHER_PARTLY_CLOUDY:
    default:
      return variant_bitmap(s_images.weather_partly_cloudy_black, s_images.weather_partly_cloudy_yellow, s_images.weather_partly_cloudy_white, variant);
  }
}

static int battery_bucket(void) {
  if (s_battery.charge_percent <= 0) return 0;
  if (s_battery.charge_percent < 50) return 25;
  if (s_battery.charge_percent < 100) return 50;
  return 100;
}

static GBitmap *battery_bitmap(IconVariant variant) {
  bool charging = s_battery.is_charging;
  int bucket = battery_bucket();

  if (charging) {
    if (bucket == 0) return variant_bitmap(s_images.battery_charging_0_black, s_images.battery_charging_0_purple, s_images.battery_charging_0_white, variant);
    if (bucket == 25) return variant_bitmap(s_images.battery_charging_25_black, s_images.battery_charging_25_purple, s_images.battery_charging_25_white, variant);
    if (bucket == 50) return variant_bitmap(s_images.battery_charging_50_black, s_images.battery_charging_50_purple, s_images.battery_charging_50_white, variant);
    return variant_bitmap(s_images.battery_charging_100_black, s_images.battery_charging_100_purple, s_images.battery_charging_100_white, variant);
  }

  if (bucket == 0) return variant_bitmap(s_images.battery_0_black, s_images.battery_0_purple, s_images.battery_0_white, variant);
  if (bucket == 25) return variant_bitmap(s_images.battery_25_black, s_images.battery_25_purple, s_images.battery_25_white, variant);
  if (bucket == 50) return variant_bitmap(s_images.battery_50_black, s_images.battery_50_purple, s_images.battery_50_white, variant);
  return variant_bitmap(s_images.battery_100_black, s_images.battery_100_purple, s_images.battery_100_white, variant);
}

static GBitmap *bitmap_for_metric(MetricType metric) {
  IconVariant variant = icon_variant_for_theme();
  switch (metric) {
    case METRIC_WEATHER:
      return weather_bitmap(variant);
    case METRIC_HEART_RATE:
      return variant_bitmap(s_images.heart_rate_black, s_images.heart_rate_green, s_images.heart_rate_white, variant);
    case METRIC_BATTERY:
      return battery_bitmap(variant);
    case METRIC_CALORIES:
      return variant_bitmap(s_images.calories_black, s_images.calories_orange, s_images.calories_white, variant);
    case METRIC_STEPS:
      return variant_bitmap(s_images.steps_black, s_images.steps_blue, s_images.steps_white, variant);
    default:
      return NULL;
  }
}

static bool metric_available(MetricType metric) {
  switch (metric) {
    case METRIC_WEATHER: return s_weather_available;
    case METRIC_HEART_RATE: return s_heart_available;
    case METRIC_BATTERY: return true;
    case METRIC_CALORIES: return s_calories_available;
    case METRIC_STEPS: return s_steps_available;
    default: return false;
  }
}

static void write_text(char *buffer, size_t size, const char *text) {
  if (size == 0) return;
  snprintf(buffer, size, "%s", text);
}

static void format_compact(int value, bool available, char *buffer, size_t size) {
  if (size == 0) return;

  if (!available || value < 0) {
    write_text(buffer, size, "---");
  } else if (value < 10000) {
    snprintf(buffer, size, "%d", value);
  } else {
    int thousands = value / 1000;
    if (thousands > 999) thousands = 999;
    snprintf(buffer, size, "%d", thousands);
    size_t len = strlen(buffer);
    if (len < size - 1) {
      buffer[len] = 'k';
      buffer[len + 1] = '\0';
    }
  }
}

static void value_for_metric(MetricType metric, char *buffer, size_t size) {
  switch (metric) {
    case METRIC_WEATHER:
      if (s_weather_available) snprintf(buffer, size, "%d", s_weather_temp);
      else write_text(buffer, size, "---");
      break;
    case METRIC_HEART_RATE:
      if (s_heart_available) snprintf(buffer, size, "%d", s_heart_rate);
      else write_text(buffer, size, "---");
      break;
    case METRIC_BATTERY:
      snprintf(buffer, size, "%d", s_battery.charge_percent);
      break;
    case METRIC_CALORIES:
      format_compact(s_calories, s_calories_available, buffer, size);
      break;
    case METRIC_STEPS:
      format_compact(s_steps, s_steps_available, buffer, size);
      break;
    default:
      if (size > 0) buffer[0] = '\0';
      break;
  }
}

static int get_visible_metrics(MetricType visible[4]) {
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (s_settings.slot_metrics[i] != METRIC_NONE) {
      visible[count++] = s_settings.slot_metrics[i];
    }
  }
  return count;
}

static void format_time(char *hour, size_t hour_size, char *minute, size_t minute_size, char *date, size_t date_size) {
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  int hour_value = tick_time->tm_hour;
  if (!clock_is_24h_style()) {
    hour_value = hour_value % 12;
    if (hour_value == 0) hour_value = 12;
  }
  if (s_settings.show_leading_zero) snprintf(hour, hour_size, "%02d", hour_value);
  else snprintf(hour, hour_size, "%d", hour_value);
  snprintf(minute, minute_size, "%02d", tick_time->tm_min);
  strftime(date, date_size, "%b %d, %a", tick_time);
}

static void draw_metric(GContext *ctx, DayMateTheme theme, MetricType metric, GRect box) {
  bool available = metric_available(metric);
  GColor metric_color = color_for_metric(theme, metric, available);
  GBitmap *bitmap = bitmap_for_metric(metric);

  if (bitmap) {
    int icon_x = box.origin.x + (box.size.w - ICON_SIZE) / 2;
    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    graphics_draw_bitmap_in_rect(ctx, bitmap, GRect(icon_x, box.origin.y, ICON_SIZE, ICON_SIZE));
  }

  graphics_context_set_text_color(ctx, metric_color);
  char value[12];
  value_for_metric(metric, value, sizeof(value));
  graphics_draw_text(ctx, value, s_font_metric, GRect(box.origin.x, box.origin.y + 22, box.size.w, 22), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void draw_clock(GContext *ctx, DayMateTheme theme, GRect panel) {
  char hour[4], minute[4], date[18];
  format_time(hour, sizeof(hour), minute, sizeof(minute), date, sizeof(date));

  graphics_context_set_fill_color(ctx, theme.clock_panel);
  graphics_fill_rect(ctx, panel, CLOCK_PANEL_RADIUS, GCornersAll);

  graphics_context_set_text_color(ctx, theme.clock_text);
  graphics_draw_text(ctx, hour, s_font_time, GRect(panel.origin.x, panel.origin.y + 9, panel.size.w, 76), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, minute, s_font_time, GRect(panel.origin.x, panel.origin.y + 79, panel.size.w, 76), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, date, s_font_date, GRect(panel.origin.x, panel.origin.y + 172, panel.size.w, 24), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  DayMateTheme theme = get_theme();
  graphics_context_set_fill_color(ctx, theme.background);
  graphics_fill_rect(ctx, GRect(0, 0, SCREEN_W, SCREEN_H), 0, GCornerNone);

  MetricType visible[4];
  int count = get_visible_metrics(visible);

  if (count == 0) {
    draw_clock(ctx, theme, GRect(OUTER_PAD, OUTER_PAD, SCREEN_W - (OUTER_PAD * 2), SCREEN_H - (OUTER_PAD * 2)));
    return;
  }

  draw_clock(ctx, theme, GRect(OUTER_PAD + METRIC_TRAY_W + GAP_W, OUTER_PAD, CLOCK_PANEL_W, CLOCK_PANEL_H));

  int tray_x = OUTER_PAD;
  int slot_y[4] = {14, 58, 102, 146};

  if (count == 1) {
    draw_metric(ctx, theme, visible[0], GRect(tray_x, 92, METRIC_TRAY_W, 42));
  } else {
    for (int i = 0; i < count; i++) {
      int y = count == 4 ? slot_y[i] : 14 + ((CLOCK_PANEL_H - 42) * i) / (count - 1);
      draw_metric(ctx, theme, visible[i], GRect(tray_x, y, METRIC_TRAY_W, 42));
    }
  }
}

static void save_settings(void) {
  persist_write_int(STORAGE_KEY_THEME, s_settings.theme);
  persist_write_int(STORAGE_KEY_SLOT_1_METRIC, s_settings.slot_metrics[0]);
  persist_write_int(STORAGE_KEY_SLOT_2_METRIC, s_settings.slot_metrics[1]);
  persist_write_int(STORAGE_KEY_SLOT_3_METRIC, s_settings.slot_metrics[2]);
  persist_write_int(STORAGE_KEY_SLOT_4_METRIC, s_settings.slot_metrics[3]);
  persist_write_bool(STORAGE_KEY_SHOW_LEADING_ZERO, s_settings.show_leading_zero);
}

static int read_int_or_default(int key, int fallback) {
  return persist_exists(key) ? persist_read_int(key) : fallback;
}

static void load_settings(void) {
  s_settings.theme = (ThemeType)read_int_or_default(STORAGE_KEY_THEME, THEME_DEFAULT);
  s_settings.slot_metrics[0] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_1_METRIC, METRIC_WEATHER);
  s_settings.slot_metrics[1] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_2_METRIC, METRIC_HEART_RATE);
  s_settings.slot_metrics[2] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_3_METRIC, METRIC_BATTERY);
  s_settings.slot_metrics[3] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_4_METRIC, METRIC_STEPS);
  s_settings.show_leading_zero = persist_exists(STORAGE_KEY_SHOW_LEADING_ZERO) ? persist_read_bool(STORAGE_KEY_SHOW_LEADING_ZERO) : true;
}

static void inbox_received(DictionaryIterator *iter, void *context) {
  Tuple *t;
  if ((t = dict_find(iter, 0))) s_settings.theme = (ThemeType)t->value->int32;
  if ((t = dict_find(iter, 1))) s_settings.slot_metrics[0] = (MetricType)t->value->int32;
  if ((t = dict_find(iter, 2))) s_settings.slot_metrics[1] = (MetricType)t->value->int32;
  if ((t = dict_find(iter, 3))) s_settings.slot_metrics[2] = (MetricType)t->value->int32;
  if ((t = dict_find(iter, 4))) s_settings.slot_metrics[3] = (MetricType)t->value->int32;
  if ((t = dict_find(iter, 5))) s_settings.show_leading_zero = t->value->int32 == 1;
  if ((t = dict_find(iter, 10))) s_weather_temp = t->value->int32;
  if ((t = dict_find(iter, 11))) s_weather_condition = (WeatherCondition)t->value->int32;
  if ((t = dict_find(iter, 12))) s_weather_available = t->value->int32 == 1;
  save_settings();
  layer_mark_dirty(s_canvas_layer);
}

static void request_weather(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter) {
    dict_write_uint8(iter, 20, 1);
    app_message_outbox_send();
  }
}

static void battery_handler(BatteryChargeState state) {
  s_battery = state;
  layer_mark_dirty(s_canvas_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(s_canvas_layer);
}

static GBitmap *load_bitmap(uint32_t resource_id) {
  return gbitmap_create_with_resource(resource_id);
}

static void load_images(void) {
  s_images.weather_sunny_black = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_SUNNY_BLACK);
  s_images.weather_sunny_yellow = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_SUNNY_YELLOW);
  s_images.weather_sunny_white = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_SUNNY_WHITE);
  s_images.weather_partly_cloudy_black = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_BLACK);
  s_images.weather_partly_cloudy_yellow = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_YELLOW);
  s_images.weather_partly_cloudy_white = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_WHITE);
  s_images.weather_rainy_black = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_RAINY_BLACK);
  s_images.weather_rainy_yellow = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_RAINY_YELLOW);
  s_images.weather_rainy_white = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_RAINY_WHITE);
  s_images.weather_storm_black = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_STORM_BLACK);
  s_images.weather_storm_yellow = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_STORM_YELLOW);
  s_images.weather_storm_white = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_STORM_WHITE);
  s_images.weather_snow_black = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_SNOW_BLACK);
  s_images.weather_snow_yellow = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_SNOW_YELLOW);
  s_images.weather_snow_white = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_SNOW_WHITE);
  s_images.weather_fog_black = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_FOG_BLACK);
  s_images.weather_fog_yellow = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_FOG_YELLOW);
  s_images.weather_fog_white = load_bitmap(RESOURCE_ID_IMAGE_WEATHER_FOG_WHITE);
  s_images.heart_rate_black = load_bitmap(RESOURCE_ID_IMAGE_HEART_RATE_BLACK);
  s_images.heart_rate_green = load_bitmap(RESOURCE_ID_IMAGE_HEART_RATE_GREEN);
  s_images.heart_rate_white = load_bitmap(RESOURCE_ID_IMAGE_HEART_RATE_WHITE);
  s_images.calories_black = load_bitmap(RESOURCE_ID_IMAGE_CALORIES_BLACK);
  s_images.calories_orange = load_bitmap(RESOURCE_ID_IMAGE_CALORIES_ORANGE);
  s_images.calories_white = load_bitmap(RESOURCE_ID_IMAGE_CALORIES_WHITE);
  s_images.steps_black = load_bitmap(RESOURCE_ID_IMAGE_STEPS_BLACK);
  s_images.steps_blue = load_bitmap(RESOURCE_ID_IMAGE_STEPS_BLUE);
  s_images.steps_white = load_bitmap(RESOURCE_ID_IMAGE_STEPS_WHITE);
  s_images.battery_0_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_0_BLACK);
  s_images.battery_0_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_0_PURPLE);
  s_images.battery_0_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_0_WHITE);
  s_images.battery_25_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_25_BLACK);
  s_images.battery_25_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_25_PURPLE);
  s_images.battery_25_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_25_WHITE);
  s_images.battery_50_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_50_BLACK);
  s_images.battery_50_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_50_PURPLE);
  s_images.battery_50_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_50_WHITE);
  s_images.battery_100_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_100_BLACK);
  s_images.battery_100_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_100_PURPLE);
  s_images.battery_100_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_100_WHITE);
  s_images.battery_charging_0_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_0_BLACK);
  s_images.battery_charging_0_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_0_PURPLE);
  s_images.battery_charging_0_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_0_WHITE);
  s_images.battery_charging_25_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_25_BLACK);
  s_images.battery_charging_25_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_25_PURPLE);
  s_images.battery_charging_25_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_25_WHITE);
  s_images.battery_charging_50_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_50_BLACK);
  s_images.battery_charging_50_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_50_PURPLE);
  s_images.battery_charging_50_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_50_WHITE);
  s_images.battery_charging_100_black = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_100_BLACK);
  s_images.battery_charging_100_purple = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_100_PURPLE);
  s_images.battery_charging_100_white = load_bitmap(RESOURCE_ID_IMAGE_BATTERY_CHARGING_100_WHITE);
}

static void destroy_bitmap(GBitmap *bitmap) {
  if (bitmap) gbitmap_destroy(bitmap);
}

static void unload_images(void) {
  destroy_bitmap(s_images.weather_sunny_black);
  destroy_bitmap(s_images.weather_sunny_yellow);
  destroy_bitmap(s_images.weather_sunny_white);
  destroy_bitmap(s_images.weather_partly_cloudy_black);
  destroy_bitmap(s_images.weather_partly_cloudy_yellow);
  destroy_bitmap(s_images.weather_partly_cloudy_white);
  destroy_bitmap(s_images.weather_rainy_black);
  destroy_bitmap(s_images.weather_rainy_yellow);
  destroy_bitmap(s_images.weather_rainy_white);
  destroy_bitmap(s_images.weather_storm_black);
  destroy_bitmap(s_images.weather_storm_yellow);
  destroy_bitmap(s_images.weather_storm_white);
  destroy_bitmap(s_images.weather_snow_black);
  destroy_bitmap(s_images.weather_snow_yellow);
  destroy_bitmap(s_images.weather_snow_white);
  destroy_bitmap(s_images.weather_fog_black);
  destroy_bitmap(s_images.weather_fog_yellow);
  destroy_bitmap(s_images.weather_fog_white);
  destroy_bitmap(s_images.heart_rate_black);
  destroy_bitmap(s_images.heart_rate_green);
  destroy_bitmap(s_images.heart_rate_white);
  destroy_bitmap(s_images.calories_black);
  destroy_bitmap(s_images.calories_orange);
  destroy_bitmap(s_images.calories_white);
  destroy_bitmap(s_images.steps_black);
  destroy_bitmap(s_images.steps_blue);
  destroy_bitmap(s_images.steps_white);
  destroy_bitmap(s_images.battery_0_black);
  destroy_bitmap(s_images.battery_0_purple);
  destroy_bitmap(s_images.battery_0_white);
  destroy_bitmap(s_images.battery_25_black);
  destroy_bitmap(s_images.battery_25_purple);
  destroy_bitmap(s_images.battery_25_white);
  destroy_bitmap(s_images.battery_50_black);
  destroy_bitmap(s_images.battery_50_purple);
  destroy_bitmap(s_images.battery_50_white);
  destroy_bitmap(s_images.battery_100_black);
  destroy_bitmap(s_images.battery_100_purple);
  destroy_bitmap(s_images.battery_100_white);
  destroy_bitmap(s_images.battery_charging_0_black);
  destroy_bitmap(s_images.battery_charging_0_purple);
  destroy_bitmap(s_images.battery_charging_0_white);
  destroy_bitmap(s_images.battery_charging_25_black);
  destroy_bitmap(s_images.battery_charging_25_purple);
  destroy_bitmap(s_images.battery_charging_25_white);
  destroy_bitmap(s_images.battery_charging_50_black);
  destroy_bitmap(s_images.battery_charging_50_purple);
  destroy_bitmap(s_images.battery_charging_50_white);
  destroy_bitmap(s_images.battery_charging_100_black);
  destroy_bitmap(s_images.battery_charging_100_purple);
  destroy_bitmap(s_images.battery_charging_100_white);
}

static void main_window_load(Window *window) {
  s_canvas_layer = layer_create(GRect(0, 0, SCREEN_W, SCREEN_H));
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);
}

static void main_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
}

static void init(void) {
  load_settings();
  s_battery = battery_state_service_peek();
  s_font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BLACK_78));
  s_font_metric = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_16));
  s_font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_16));
  load_images();
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){.load = main_window_load, .unload = main_window_unload});
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
  app_message_register_inbox_received(inbox_received);
  app_message_open(512, 512);
  request_weather();
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  unload_images();
  fonts_unload_custom_font(s_font_time);
  fonts_unload_custom_font(s_font_metric);
  fonts_unload_custom_font(s_font_date);
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
