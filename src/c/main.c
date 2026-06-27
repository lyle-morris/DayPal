#include <pebble.h>
#include <string.h>

#define SCREEN_W 200
#define SCREEN_H 228
#define METRIC_TRAY_W 58
#define DIVIDER_X 58
#define DIVIDER_W 1
#define CLOCK_X 59
#define CLOCK_W 141
#define CLOCK_FULL_X 0
#define CLOCK_FULL_W 200
#define SINGLE_DIGIT_HOUR_X_OFFSET 28
#define ICON_SIZE 32
#define METRIC_ROW_H 52
#define METRIC_ICON_X 13
#define METRIC_VALUE_Y_OFFSET 30
#define DAYMATE_QA_DUMMY_DATA 0
#define DAYMATE_QA_TIME_STRESS_TEST 0

#define STORAGE_KEY_THEME 100
#define STORAGE_KEY_SLOT_1_METRIC 101
#define STORAGE_KEY_SLOT_2_METRIC 102
#define STORAGE_KEY_SLOT_3_METRIC 103
#define STORAGE_KEY_SLOT_4_METRIC 104
#define STORAGE_KEY_SHOW_LEADING_ZERO 105
#define STORAGE_KEY_WEATHER_TEMP 106
#define STORAGE_KEY_WEATHER_CODE 107
#define STORAGE_KEY_WEATHER_VALID 108

#define APP_KEY_SETTINGS_READY 21

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
  GColor divider;
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

static Window *s_window;
static Layer *s_canvas_layer;
static GFont s_font_time;
static GFont s_font_metric;
static GFont s_font_date;

static DayMateSettings s_settings = {
  .theme = THEME_DEFAULT,
  .slot_metrics = {METRIC_WEATHER, METRIC_HEART_RATE, METRIC_BATTERY, METRIC_STEPS},
  .show_leading_zero = true
};

static BatteryChargeState s_battery;
static bool s_weather_available = false;
static int s_weather_temp = 95;
static WeatherCondition s_weather_condition = WEATHER_SUNNY;
static bool s_steps_available = false;
static int s_steps = 8542;
static bool s_heart_available = false;
static int s_heart_rate = 110;
static bool s_calories_available = false;
static int s_calories = 1520;

static bool has_metric_configured(MetricType metric) {
  for (int i = 0; i < 4; i++) {
    if (s_settings.slot_metrics[i] == metric) return true;
  }
  return false;
}

static DayMateTheme get_theme(void) {
  switch (s_settings.theme) {
    case THEME_BLUE:
      return (DayMateTheme){GColorFromHEX(0x0055FF), GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0xFFFFFF), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_PINK:
      return (DayMateTheme){GColorFromHEX(0xFF00AA), GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0xFFFFFF), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_GREEN:
      return (DayMateTheme){GColorFromHEX(0x00AA55), GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0xFFFFFF), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_WHITE:
      return (DayMateTheme){GColorWhite, GColorBlack, GColorBlack, GColorBlack, GColorDarkGray, GColorBlack, GColorBlack, GColorBlack, GColorBlack, GColorBlack, false};
    case THEME_ORANGE:
      return (DayMateTheme){GColorFromHEX(0xFF5500), GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0xFFFFFF), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_DARK_BLUE:
      return (DayMateTheme){GColorFromHEX(0x001A55), GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0xFFFFFF), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_BLACK:
      return (DayMateTheme){GColorBlack, GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0xFFFFFF), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
    case THEME_DEFAULT:
    default:
      return (DayMateTheme){GColorBlack, GColorWhite, GColorWhite, GColorWhite, GColorFromHEX(0x777777), GColorFromHEX(0xFFFF00), GColorFromHEX(0x00FF00), GColorFromHEX(0xAA55FF), GColorFromHEX(0xFF5500), GColorFromHEX(0x00AAFF), true};
  }
}

static bool metric_available(MetricType metric) {
#if DAYMATE_QA_DUMMY_DATA
  return metric != METRIC_NONE;
#else
  switch (metric) {
    case METRIC_WEATHER: return s_weather_available;
    case METRIC_HEART_RATE: return s_heart_available;
    case METRIC_BATTERY: return true;
    case METRIC_CALORIES: return s_calories_available;
    case METRIC_STEPS: return s_steps_available;
    default: return false;
  }
#endif
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

static uint32_t choose_variant(uint32_t black, uint32_t color, uint32_t white) {
  switch (icon_variant_for_theme()) {
    case ICON_VARIANT_BLACK: return black;
    case ICON_VARIANT_WHITE: return white;
    case ICON_VARIANT_DEFAULT:
    default: return color;
  }
}

static uint32_t weather_resource_id(void) {
#if DAYMATE_QA_DUMMY_DATA
  return choose_variant(RESOURCE_ID_IMAGE_WEATHER_SUNNY_BLACK, RESOURCE_ID_IMAGE_WEATHER_SUNNY_YELLOW, RESOURCE_ID_IMAGE_WEATHER_SUNNY_WHITE);
#else
  switch (s_weather_condition) {
    case WEATHER_SUNNY:
      return choose_variant(RESOURCE_ID_IMAGE_WEATHER_SUNNY_BLACK, RESOURCE_ID_IMAGE_WEATHER_SUNNY_YELLOW, RESOURCE_ID_IMAGE_WEATHER_SUNNY_WHITE);
    case WEATHER_RAINY:
      return choose_variant(RESOURCE_ID_IMAGE_WEATHER_RAINY_BLACK, RESOURCE_ID_IMAGE_WEATHER_RAINY_YELLOW, RESOURCE_ID_IMAGE_WEATHER_RAINY_WHITE);
    case WEATHER_STORM:
      return choose_variant(RESOURCE_ID_IMAGE_WEATHER_STORM_BLACK, RESOURCE_ID_IMAGE_WEATHER_STORM_YELLOW, RESOURCE_ID_IMAGE_WEATHER_STORM_WHITE);
    case WEATHER_SNOW:
      return choose_variant(RESOURCE_ID_IMAGE_WEATHER_SNOW_BLACK, RESOURCE_ID_IMAGE_WEATHER_SNOW_YELLOW, RESOURCE_ID_IMAGE_WEATHER_SNOW_WHITE);
    case WEATHER_FOG:
      return choose_variant(RESOURCE_ID_IMAGE_WEATHER_FOG_BLACK, RESOURCE_ID_IMAGE_WEATHER_FOG_YELLOW, RESOURCE_ID_IMAGE_WEATHER_FOG_WHITE);
    case WEATHER_PARTLY_CLOUDY:
    default:
      return choose_variant(RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_BLACK, RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_YELLOW, RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_WHITE);
  }
#endif
}

static int battery_bucket(void) {
#if DAYMATE_QA_DUMMY_DATA
  return 50;
#else
  if (s_battery.charge_percent <= 0) return 0;
  if (s_battery.charge_percent < 50) return 25;
  if (s_battery.charge_percent < 100) return 50;
  return 100;
#endif
}

static uint32_t battery_resource_id(void) {
  bool charging = s_battery.is_charging;
  int bucket = battery_bucket();
#if DAYMATE_QA_DUMMY_DATA
  charging = false;
#endif
  if (charging) {
    if (bucket == 0) return choose_variant(RESOURCE_ID_IMAGE_BATTERY_CHARGING_0_BLACK, RESOURCE_ID_IMAGE_BATTERY_CHARGING_0_PURPLE, RESOURCE_ID_IMAGE_BATTERY_CHARGING_0_WHITE);
    if (bucket == 25) return choose_variant(RESOURCE_ID_IMAGE_BATTERY_CHARGING_25_BLACK, RESOURCE_ID_IMAGE_BATTERY_CHARGING_25_PURPLE, RESOURCE_ID_IMAGE_BATTERY_CHARGING_25_WHITE);
    if (bucket == 50) return choose_variant(RESOURCE_ID_IMAGE_BATTERY_CHARGING_50_BLACK, RESOURCE_ID_IMAGE_BATTERY_CHARGING_50_PURPLE, RESOURCE_ID_IMAGE_BATTERY_CHARGING_50_WHITE);
    return choose_variant(RESOURCE_ID_IMAGE_BATTERY_CHARGING_100_BLACK, RESOURCE_ID_IMAGE_BATTERY_CHARGING_100_PURPLE, RESOURCE_ID_IMAGE_BATTERY_CHARGING_100_WHITE);
  }
  if (bucket == 0) return choose_variant(RESOURCE_ID_IMAGE_BATTERY_0_BLACK, RESOURCE_ID_IMAGE_BATTERY_0_PURPLE, RESOURCE_ID_IMAGE_BATTERY_0_WHITE);
  if (bucket == 25) return choose_variant(RESOURCE_ID_IMAGE_BATTERY_25_BLACK, RESOURCE_ID_IMAGE_BATTERY_25_PURPLE, RESOURCE_ID_IMAGE_BATTERY_25_WHITE);
  if (bucket == 50) return choose_variant(RESOURCE_ID_IMAGE_BATTERY_50_BLACK, RESOURCE_ID_IMAGE_BATTERY_50_PURPLE, RESOURCE_ID_IMAGE_BATTERY_50_WHITE);
  return choose_variant(RESOURCE_ID_IMAGE_BATTERY_100_BLACK, RESOURCE_ID_IMAGE_BATTERY_100_PURPLE, RESOURCE_ID_IMAGE_BATTERY_100_WHITE);
}

static uint32_t resource_id_for_metric(MetricType metric) {
  switch (metric) {
    case METRIC_WEATHER: return weather_resource_id();
    case METRIC_HEART_RATE: return choose_variant(RESOURCE_ID_IMAGE_HEART_RATE_BLACK, RESOURCE_ID_IMAGE_HEART_RATE_GREEN, RESOURCE_ID_IMAGE_HEART_RATE_WHITE);
    case METRIC_BATTERY: return battery_resource_id();
    case METRIC_CALORIES: return choose_variant(RESOURCE_ID_IMAGE_CALORIES_BLACK, RESOURCE_ID_IMAGE_CALORIES_ORANGE, RESOURCE_ID_IMAGE_CALORIES_WHITE);
    case METRIC_STEPS: return choose_variant(RESOURCE_ID_IMAGE_STEPS_BLACK, RESOURCE_ID_IMAGE_STEPS_BLUE, RESOURCE_ID_IMAGE_STEPS_WHITE);
    default: return 0;
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
#if DAYMATE_QA_DUMMY_DATA
  switch (metric) {
    case METRIC_WEATHER: snprintf(buffer, size, "95"); break;
    case METRIC_HEART_RATE: snprintf(buffer, size, "110"); break;
    case METRIC_BATTERY: snprintf(buffer, size, "50"); break;
    case METRIC_CALORIES: snprintf(buffer, size, "1520"); break;
    case METRIC_STEPS: snprintf(buffer, size, "8542"); break;
    default: if (size > 0) buffer[0] = '\0'; break;
  }
#else
  switch (metric) {
    case METRIC_WEATHER: if (s_weather_available) snprintf(buffer, size, "%d", s_weather_temp); else write_text(buffer, size, "---"); break;
    case METRIC_HEART_RATE: if (s_heart_available) snprintf(buffer, size, "%d", s_heart_rate); else write_text(buffer, size, "---"); break;
    case METRIC_BATTERY: snprintf(buffer, size, "%d", s_battery.charge_percent); break;
    case METRIC_CALORIES: format_compact(s_calories, s_calories_available, buffer, size); break;
    case METRIC_STEPS: format_compact(s_steps, s_steps_available, buffer, size); break;
    default: if (size > 0) buffer[0] = '\0'; break;
  }
#endif
}

static time_t day_start_time(void) {
  time_t now = time(NULL);
  struct tm *today = localtime(&now);
  today->tm_hour = 0;
  today->tm_min = 0;
  today->tm_sec = 0;
  return mktime(today);
}

static bool health_metric_available(HealthMetric metric, time_t start, time_t end) {
  return health_service_metric_accessible(metric, start, end) & HealthServiceAccessibilityMaskAvailable;
}

static void update_health_metrics(void) {
  time_t now = time(NULL);
  time_t start = day_start_time();

  s_steps_available = health_metric_available(HealthMetricStepCount, start, now);
  if (s_steps_available) s_steps = (int)health_service_sum(HealthMetricStepCount, start, now);

  s_calories_available = health_metric_available(HealthMetricActiveKCalories, start, now);
  if (s_calories_available) s_calories = (int)health_service_sum(HealthMetricActiveKCalories, start, now);

  s_heart_available = health_metric_available(HealthMetricHeartRateBPM, now - 30 * 60, now);
  if (s_heart_available) {
    HealthValue heart_rate = health_service_peek_current_value(HealthMetricHeartRateBPM);
    if (heart_rate > 0) s_heart_rate = (int)heart_rate;
    else s_heart_available = false;
  }
}

static int get_visible_metrics(MetricType visible[4]) {
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (s_settings.slot_metrics[i] != METRIC_NONE) visible[count++] = s_settings.slot_metrics[i];
  }
  return count;
}

static void format_time(char *hour, size_t hour_size, char *minute, size_t minute_size, char *date, size_t date_size) {
#if DAYMATE_QA_TIME_STRESS_TEST
  snprintf(hour, hour_size, "88");
  snprintf(minute, minute_size, "88");
  write_text(date, date_size, "Jun 26, Fri");
#else
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
#endif
}

static void draw_metric(GContext *ctx, DayMateTheme theme, MetricType metric, GRect box) {
  bool available = metric_available(metric);
  GColor metric_color = color_for_metric(theme, metric, available);
  uint32_t resource_id = resource_id_for_metric(metric);
  if (resource_id) {
    GBitmap *bitmap = gbitmap_create_with_resource(resource_id);
    if (bitmap) {
      graphics_context_set_compositing_mode(ctx, GCompOpSet);
      graphics_draw_bitmap_in_rect(ctx, bitmap, GRect(METRIC_ICON_X, box.origin.y, ICON_SIZE, ICON_SIZE));
      gbitmap_destroy(bitmap);
    }
  }
  graphics_context_set_text_color(ctx, metric_color);
  char value[12];
  value_for_metric(metric, value, sizeof(value));
  graphics_draw_text(ctx, value, s_font_metric, GRect(0, box.origin.y + METRIC_VALUE_Y_OFFSET, METRIC_TRAY_W, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void draw_clock_text(GContext *ctx, const char *text, GFont font, GRect box) {
  graphics_draw_text(ctx, text, font, box, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void draw_clock(GContext *ctx, DayMateTheme theme, int clock_x, int clock_w) {
  char hour[4], minute[4], date[18];
  format_time(hour, sizeof(hour), minute, sizeof(minute), date, sizeof(date));
  int hour_x = clock_x;
  if (!s_settings.show_leading_zero && strlen(hour) == 1) {
    hour_x += SINGLE_DIGIT_HOUR_X_OFFSET;
  }
  graphics_context_set_text_color(ctx, theme.clock_text);
  draw_clock_text(ctx, hour, s_font_time, GRect(hour_x, -11, clock_w, 104));
  draw_clock_text(ctx, minute, s_font_time, GRect(clock_x, 71, clock_w, 104));
  draw_clock_text(ctx, date, s_font_date, GRect(clock_x, 186, clock_w, 26));
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  DayMateTheme theme = get_theme();
  graphics_context_set_fill_color(ctx, theme.background);
  graphics_fill_rect(ctx, GRect(0, 0, SCREEN_W, SCREEN_H), 0, GCornerNone);
  MetricType visible[4];
  int count = get_visible_metrics(visible);
  if (count == 0) {
    draw_clock(ctx, theme, CLOCK_FULL_X, CLOCK_FULL_W);
    return;
  }
  graphics_context_set_fill_color(ctx, theme.divider);
  graphics_fill_rect(ctx, GRect(DIVIDER_X, 0, DIVIDER_W, SCREEN_H), 0, GCornerNone);
  int slot_y[4] = {7, 61, 115, 169};
  for (int i = 0; i < 4; i++) {
    MetricType metric = s_settings.slot_metrics[i];
    if (metric != METRIC_NONE) {
      draw_metric(ctx, theme, metric, GRect(0, slot_y[i], METRIC_TRAY_W, METRIC_ROW_H));
    }
  }
  draw_clock(ctx, theme, CLOCK_X, CLOCK_W);
}

static void save_settings(void) {
  persist_write_int(STORAGE_KEY_THEME, s_settings.theme);
  persist_write_int(STORAGE_KEY_SLOT_1_METRIC, s_settings.slot_metrics[0]);
  persist_write_int(STORAGE_KEY_SLOT_2_METRIC, s_settings.slot_metrics[1]);
  persist_write_int(STORAGE_KEY_SLOT_3_METRIC, s_settings.slot_metrics[2]);
  persist_write_int(STORAGE_KEY_SLOT_4_METRIC, s_settings.slot_metrics[3]);
  persist_write_bool(STORAGE_KEY_SHOW_LEADING_ZERO, s_settings.show_leading_zero);
}

static void save_weather(void) {
  persist_write_int(STORAGE_KEY_WEATHER_TEMP, s_weather_temp);
  persist_write_int(STORAGE_KEY_WEATHER_CODE, s_weather_condition);
  persist_write_bool(STORAGE_KEY_WEATHER_VALID, s_weather_available);
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

static void load_weather(void) {
  s_weather_temp = read_int_or_default(STORAGE_KEY_WEATHER_TEMP, s_weather_temp);
  s_weather_condition = (WeatherCondition)read_int_or_default(STORAGE_KEY_WEATHER_CODE, s_weather_condition);
  s_weather_available = persist_exists(STORAGE_KEY_WEATHER_VALID) ? persist_read_bool(STORAGE_KEY_WEATHER_VALID) : false;
}

static void send_settings_ready(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter) {
    dict_write_uint8(iter, APP_KEY_SETTINGS_READY, 1);
    app_message_outbox_send();
  }
}

static void inbox_received(DictionaryIterator *iter, void *context) {
  Tuple *t;
  bool settings_changed = false;
  bool weather_changed = false;
  if ((t = dict_find(iter, 0))) { s_settings.theme = (ThemeType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, 1))) { s_settings.slot_metrics[0] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, 2))) { s_settings.slot_metrics[1] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, 3))) { s_settings.slot_metrics[2] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, 4))) { s_settings.slot_metrics[3] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, 5))) { s_settings.show_leading_zero = t->value->int32 == 1; settings_changed = true; }
  if ((t = dict_find(iter, 10))) { s_weather_temp = t->value->int32; weather_changed = true; }
  if ((t = dict_find(iter, 11))) { s_weather_condition = (WeatherCondition)t->value->int32; weather_changed = true; }
  if ((t = dict_find(iter, 12))) { s_weather_available = t->value->int32 == 1; weather_changed = true; }
  layer_mark_dirty(s_canvas_layer);
  if (weather_changed) {
    save_weather();
  }
  if (settings_changed) {
    save_settings();
    send_settings_ready();
  }
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
  if (tick_time->tm_min % 30 == 0 && has_metric_configured(METRIC_WEATHER)) {
    request_weather();
  }
  update_health_metrics();
  layer_mark_dirty(s_canvas_layer);
}

static void health_handler(HealthEventType event, void *context) {
  update_health_metrics();
  layer_mark_dirty(s_canvas_layer);
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
  load_weather();
  update_health_metrics();
  s_battery = battery_state_service_peek();
  s_font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BLACK_102));
  s_font_metric = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_16));
  s_font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_20));
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){.load = main_window_load, .unload = main_window_unload});
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
  health_service_events_subscribe(health_handler, NULL);
  app_message_register_inbox_received(inbox_received);
  app_message_open(512, 512);
  request_weather();
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  health_service_events_unsubscribe();
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
