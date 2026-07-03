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
#define CLOCK_TEXT_X_BLEED 6
#define TIME_TEXT_H 104
#define HOUR_TEXT_Y -7
#define MINUTE_TEXT_Y 75
#define DATE_TEXT_Y 186
#define DATE_TEXT_H 26
#define DAYPAL_UNAVAILABLE_HEX 0x666666
#define DAYPAL_QA_DUMMY_DATA 0
#define DAYPAL_QA_TIME_STRESS_TEST 0

#define STORAGE_KEY_THEME 100
#define STORAGE_KEY_SLOT_1_METRIC 101
#define STORAGE_KEY_SLOT_2_METRIC 102
#define STORAGE_KEY_SLOT_3_METRIC 103
#define STORAGE_KEY_SLOT_4_METRIC 104
#define STORAGE_KEY_SHOW_LEADING_ZERO 105
#define STORAGE_KEY_WEATHER_TEMP 106
#define STORAGE_KEY_WEATHER_CODE 107
#define STORAGE_KEY_WEATHER_VALID 108
#define STORAGE_KEY_USE_24_HOUR 109
#define STORAGE_KEY_REVERSE_THEME 110

#define APP_KEY_THEME 0
#define APP_KEY_SLOT_1_METRIC 1
#define APP_KEY_SLOT_2_METRIC 2
#define APP_KEY_SLOT_3_METRIC 3
#define APP_KEY_SLOT_4_METRIC 4
#define APP_KEY_SHOW_LEADING_ZERO 5
#define APP_KEY_USE_24_HOUR 6
#define APP_KEY_REVERSE_THEME 7
#define APP_KEY_WEATHER_TEMP 10
#define APP_KEY_WEATHER_CODE 11
#define APP_KEY_WEATHER_VALID 12
#define APP_KEY_REQUEST_WEATHER 20
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
  THEME_BLACK = 7,
  THEME_RED = 8,
  THEME_YELLOW = 9,
  THEME_LEGACY_KHAKI = 10,
  THEME_GRAY = 11
} ThemeType;

typedef enum {
  WEATHER_SUNNY = 0,
  WEATHER_PARTLY_CLOUDY = 1,
  WEATHER_RAINY = 2,
  WEATHER_STORM = 3,
  WEATHER_SNOW = 4,
  WEATHER_FOG = 5,
  WEATHER_CLOUDY = 6,
  WEATHER_UNKNOWN = 7
} WeatherCondition;

typedef struct {
  ThemeType theme;
  MetricType slot_metrics[4];
  bool show_leading_zero;
  bool use_24_hour;
  bool reverse_theme;
} DayPalSettings;

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
} DayPalTheme;

static Window *s_window;
static Layer *s_canvas_layer;
static GFont s_font_time;
static GFont s_font_metric;
static GFont s_font_date;

static DayPalSettings s_settings = {
  .theme = THEME_DEFAULT,
  .slot_metrics = {METRIC_WEATHER, METRIC_HEART_RATE, METRIC_BATTERY, METRIC_STEPS},
  .show_leading_zero = true,
  .use_24_hour = false,
  .reverse_theme = false
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

static GColor unavailable_color(void) {
  return GColorFromHEX(DAYPAL_UNAVAILABLE_HEX);
}

static ThemeType normalize_theme(ThemeType theme) {
  if (theme == THEME_LEGACY_KHAKI) return THEME_YELLOW;
  if (theme == THEME_DARK_BLUE) return THEME_BLUE;
  return theme;
}

static bool has_metric_configured(MetricType metric) {
  for (int i = 0; i < 4; i++) {
    if (s_settings.slot_metrics[i] == metric) return true;
  }
  return false;
}

static DayPalTheme black_text_theme(GColor background) {
  return (DayPalTheme){background, GColorBlack, GColorBlack, GColorBlack, unavailable_color(), GColorBlack, GColorBlack, GColorBlack, GColorBlack, GColorBlack, false};
}

static DayPalTheme white_text_theme(GColor background) {
  return (DayPalTheme){background, GColorWhite, GColorWhite, GColorWhite, unavailable_color(), GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorWhite, false};
}

static DayPalTheme foreground_on_black_theme(GColor foreground) {
  return (DayPalTheme){GColorBlack, foreground, foreground, foreground, unavailable_color(), foreground, foreground, foreground, foreground, foreground, false};
}

static GColor theme_color(ThemeType theme) {
  switch (normalize_theme(theme)) {
    case THEME_BLUE: return GColorFromHEX(0x0055FF);
    case THEME_PINK: return GColorFromHEX(0xFF00AA);
    case THEME_GREEN: return GColorFromHEX(0x00AA55);
    case THEME_ORANGE: return GColorFromHEX(0xFF5500);
    case THEME_RED: return GColorFromHEX(0xFF0055);
    case THEME_YELLOW: return GColorFromHEX(0xFFCC55);
    case THEME_GRAY: return GColorFromHEX(0xAAAAAA);
    case THEME_WHITE: return GColorWhite;
    case THEME_BLACK: return GColorWhite;
    case THEME_DEFAULT:
    default: return GColorBlack;
  }
}

static DayPalTheme default_theme(void) {
  return (DayPalTheme){GColorBlack, GColorFromHEX(0x555555), GColorWhite, GColorWhite, unavailable_color(), GColorFromHEX(0xFFFF00), GColorFromHEX(0xFF0000), GColorFromHEX(0x00FF00), GColorFromHEX(0xFF5500), GColorFromHEX(0x00AAFF), true};
}

static DayPalTheme get_theme(void) {
  ThemeType theme = normalize_theme(s_settings.theme);

  if (theme == THEME_DEFAULT) {
    return default_theme();
  }

  if (s_settings.reverse_theme) {
    if (theme == THEME_BLACK || theme == THEME_WHITE) {
      return white_text_theme(GColorBlack);
    }
    return foreground_on_black_theme(theme_color(theme));
  }

  switch (theme) {
    case THEME_BLUE:
    case THEME_PINK:
    case THEME_GREEN:
    case THEME_ORANGE:
    case THEME_RED:
    case THEME_YELLOW:
    case THEME_GRAY:
      return black_text_theme(theme_color(theme));
    case THEME_BLACK:
    case THEME_WHITE:
      return black_text_theme(GColorWhite);
    default:
      return default_theme();
  }
}

static bool metric_available(MetricType metric) {
#if DAYPAL_QA_DUMMY_DATA
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

static GColor color_for_metric(DayPalTheme theme, MetricType metric, bool available) {
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
#if DAYPAL_QA_DUMMY_DATA
  switch (metric) {
    case METRIC_WEATHER: snprintf(buffer, size, "100°"); break;
    case METRIC_HEART_RATE: snprintf(buffer, size, "110"); break;
    case METRIC_BATTERY: snprintf(buffer, size, "100%%"); break;
    case METRIC_CALORIES: snprintf(buffer, size, "1520"); break;
    case METRIC_STEPS: snprintf(buffer, size, "8542"); break;
    default: if (size > 0) buffer[0] = '\0'; break;
  }
#else
  switch (metric) {
    case METRIC_WEATHER: if (s_weather_available) snprintf(buffer, size, "%d°", s_weather_temp); else write_text(buffer, size, "---"); break;
    case METRIC_HEART_RATE: if (s_heart_available) snprintf(buffer, size, "%d", s_heart_rate); else write_text(buffer, size, "---"); break;
    case METRIC_BATTERY: snprintf(buffer, size, "%d%%", s_battery.charge_percent); break;
    case METRIC_CALORIES: format_compact(s_calories, s_calories_available, buffer, size); break;
    case METRIC_STEPS: format_compact(s_steps, s_steps_available, buffer, size); break;
    default: if (size > 0) buffer[0] = '\0'; break;
  }
#endif
}

static void set_icon_color(GContext *ctx, GColor color) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_fill_color(ctx, color);
  graphics_context_set_stroke_width(ctx, 2);
}

static void draw_weather_icon(GContext *ctx, GRect icon_box) {
  int cx = icon_box.origin.x + 16;
  int cy = icon_box.origin.y + 15;

  switch (s_weather_condition) {
    case WEATHER_RAINY:
      graphics_draw_circle(ctx, GPoint(cx - 5, cy), 6);
      graphics_draw_circle(ctx, GPoint(cx + 4, cy - 2), 8);
      graphics_draw_line(ctx, GPoint(cx - 13, cy + 7), GPoint(cx + 14, cy + 7));
      graphics_draw_line(ctx, GPoint(cx - 8, cy + 12), GPoint(cx - 11, cy + 18));
      graphics_draw_line(ctx, GPoint(cx, cy + 12), GPoint(cx - 3, cy + 18));
      graphics_draw_line(ctx, GPoint(cx + 8, cy + 12), GPoint(cx + 5, cy + 18));
      break;
    case WEATHER_STORM:
      graphics_draw_circle(ctx, GPoint(cx - 5, cy), 6);
      graphics_draw_circle(ctx, GPoint(cx + 4, cy - 2), 8);
      graphics_draw_line(ctx, GPoint(cx - 13, cy + 7), GPoint(cx + 14, cy + 7));
      graphics_draw_line(ctx, GPoint(cx + 1, cy + 10), GPoint(cx - 4, cy + 18));
      graphics_draw_line(ctx, GPoint(cx - 4, cy + 18), GPoint(cx + 4, cy + 16));
      graphics_draw_line(ctx, GPoint(cx + 4, cy + 16), GPoint(cx - 1, cy + 25));
      break;
    case WEATHER_SNOW:
      graphics_draw_line(ctx, GPoint(cx, cy - 1), GPoint(cx, cy + 23));
      graphics_draw_line(ctx, GPoint(cx - 10, cy + 5), GPoint(cx + 10, cy + 17));
      graphics_draw_line(ctx, GPoint(cx - 10, cy + 17), GPoint(cx + 10, cy + 5));
      break;
    case WEATHER_CLOUDY:
    case WEATHER_FOG:
    case WEATHER_PARTLY_CLOUDY:
      graphics_draw_circle(ctx, GPoint(cx - 5, cy + 2), 6);
      graphics_draw_circle(ctx, GPoint(cx + 4, cy), 8);
      graphics_draw_line(ctx, GPoint(cx - 13, cy + 9), GPoint(cx + 14, cy + 9));
      if (s_weather_condition == WEATHER_FOG) {
        graphics_draw_line(ctx, GPoint(cx - 12, cy + 16), GPoint(cx + 12, cy + 16));
        graphics_draw_line(ctx, GPoint(cx - 8, cy + 22), GPoint(cx + 8, cy + 22));
      }
      break;
    case WEATHER_SUNNY:
    case WEATHER_UNKNOWN:
    default:
      graphics_draw_circle(ctx, GPoint(cx, cy + 2), 8);
      graphics_draw_line(ctx, GPoint(cx, cy - 12), GPoint(cx, cy - 7));
      graphics_draw_line(ctx, GPoint(cx, cy + 16), GPoint(cx, cy + 22));
      graphics_draw_line(ctx, GPoint(cx - 14, cy + 2), GPoint(cx - 9, cy + 2));
      graphics_draw_line(ctx, GPoint(cx + 9, cy + 2), GPoint(cx + 14, cy + 2));
      graphics_draw_line(ctx, GPoint(cx - 10, cy - 8), GPoint(cx - 6, cy - 4));
      graphics_draw_line(ctx, GPoint(cx + 6, cy + 8), GPoint(cx + 10, cy + 12));
      graphics_draw_line(ctx, GPoint(cx + 10, cy - 8), GPoint(cx + 6, cy - 4));
      graphics_draw_line(ctx, GPoint(cx - 6, cy + 8), GPoint(cx - 10, cy + 12));
      break;
  }
}

static void draw_heart_icon(GContext *ctx, GRect icon_box) {
  int x = icon_box.origin.x;
  int y = icon_box.origin.y;
  graphics_draw_circle(ctx, GPoint(x + 11, y + 11), 6);
  graphics_draw_circle(ctx, GPoint(x + 21, y + 11), 6);
  graphics_draw_line(ctx, GPoint(x + 5, y + 13), GPoint(x + 16, y + 26));
  graphics_draw_line(ctx, GPoint(x + 27, y + 13), GPoint(x + 16, y + 26));
  graphics_draw_line(ctx, GPoint(x + 7, y + 17), GPoint(x + 25, y + 17));
}

static void draw_battery_icon(GContext *ctx, GRect icon_box) {
  int x = icon_box.origin.x + 3;
  int y = icon_box.origin.y + 8;
  graphics_draw_rect(ctx, GRect(x, y, 23, 14));
  graphics_fill_rect(ctx, GRect(x + 24, y + 4, 3, 6), 0, GCornerNone);
  int fill_w = 0;
#if DAYPAL_QA_DUMMY_DATA
  fill_w = 19;
#else
  int charge_percent = s_battery.charge_percent;
  if (charge_percent > 100) charge_percent = 100;
  fill_w = charge_percent * 19 / 100;
#endif
  if (fill_w > 19) fill_w = 19;
  if (fill_w > 0) graphics_fill_rect(ctx, GRect(x + 2, y + 2, fill_w, 10), 0, GCornerNone);
}

static void draw_calories_icon(GContext *ctx, GRect icon_box) {
  int x = icon_box.origin.x;
  int y = icon_box.origin.y;
  graphics_draw_line(ctx, GPoint(x + 16, y + 4), GPoint(x + 8, y + 19));
  graphics_draw_line(ctx, GPoint(x + 16, y + 4), GPoint(x + 24, y + 19));
  graphics_draw_line(ctx, GPoint(x + 8, y + 19), GPoint(x + 16, y + 28));
  graphics_draw_line(ctx, GPoint(x + 24, y + 19), GPoint(x + 16, y + 28));
  graphics_draw_line(ctx, GPoint(x + 16, y + 10), GPoint(x + 13, y + 21));
  graphics_draw_line(ctx, GPoint(x + 16, y + 10), GPoint(x + 20, y + 21));
}

static void draw_steps_icon(GContext *ctx, GRect icon_box) {
  int x = icon_box.origin.x;
  int y = icon_box.origin.y;
  graphics_draw_circle(ctx, GPoint(x + 11, y + 9), 4);
  graphics_draw_circle(ctx, GPoint(x + 21, y + 20), 4);
  graphics_draw_rect(ctx, GRect(x + 7, y + 14, 8, 13));
  graphics_draw_rect(ctx, GRect(x + 17, y + 4, 8, 13));
}

static void draw_metric_icon(GContext *ctx, MetricType metric, GColor color, GRect icon_box) {
  set_icon_color(ctx, color);
  switch (metric) {
    case METRIC_WEATHER: draw_weather_icon(ctx, icon_box); break;
    case METRIC_HEART_RATE: draw_heart_icon(ctx, icon_box); break;
    case METRIC_BATTERY: draw_battery_icon(ctx, icon_box); break;
    case METRIC_CALORIES: draw_calories_icon(ctx, icon_box); break;
    case METRIC_STEPS: draw_steps_icon(ctx, icon_box); break;
    default: break;
  }
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
#if DAYPAL_QA_TIME_STRESS_TEST
  snprintf(hour, hour_size, "88");
  snprintf(minute, minute_size, "88");
  write_text(date, date_size, "Jun 26, Fri");
#else
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  int hour_value = tick_time->tm_hour;
  if (!s_settings.use_24_hour) {
    hour_value = hour_value % 12;
    if (hour_value == 0) hour_value = 12;
  }
  if (s_settings.show_leading_zero) snprintf(hour, hour_size, "%02d", hour_value);
  else snprintf(hour, hour_size, "%d", hour_value);
  snprintf(minute, minute_size, "%02d", tick_time->tm_min);
  strftime(date, date_size, "%b %d, %a", tick_time);
#endif
}

static void draw_metric(GContext *ctx, DayPalTheme theme, MetricType metric, GRect box) {
  bool available = metric_available(metric);
  GColor metric_color = color_for_metric(theme, metric, available);
  draw_metric_icon(ctx, metric, metric_color, GRect(METRIC_ICON_X, box.origin.y, ICON_SIZE, ICON_SIZE));
  graphics_context_set_text_color(ctx, metric_color);
  char value[12];
  value_for_metric(metric, value, sizeof(value));
  graphics_draw_text(ctx, value, s_font_metric, GRect(0, box.origin.y + METRIC_VALUE_Y_OFFSET, METRIC_TRAY_W, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void draw_clock_text(GContext *ctx, const char *text, GFont font, GRect box) {
  graphics_draw_text(ctx, text, font, box, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

static void draw_clock(GContext *ctx, DayPalTheme theme, int clock_x, int clock_w) {
  char hour[4], minute[4], date[18];
  format_time(hour, sizeof(hour), minute, sizeof(minute), date, sizeof(date));
  int hour_x = clock_x;
  if (!s_settings.show_leading_zero && strlen(hour) == 1) {
    hour_x += SINGLE_DIGIT_HOUR_X_OFFSET;
  }

  int text_x = clock_x - CLOCK_TEXT_X_BLEED;
  int text_w = clock_w + (CLOCK_TEXT_X_BLEED * 2);
  int hour_text_x = hour_x - CLOCK_TEXT_X_BLEED;

  graphics_context_set_text_color(ctx, theme.clock_text);
  draw_clock_text(ctx, hour, s_font_time, GRect(hour_text_x, HOUR_TEXT_Y, text_w, TIME_TEXT_H));
  draw_clock_text(ctx, minute, s_font_time, GRect(text_x, MINUTE_TEXT_Y, text_w, TIME_TEXT_H));
  graphics_draw_text(ctx, date, s_font_date, GRect(clock_x, DATE_TEXT_Y, clock_w, DATE_TEXT_H), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  DayPalTheme theme = get_theme();
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
  s_settings.theme = normalize_theme(s_settings.theme);
  persist_write_int(STORAGE_KEY_THEME, s_settings.theme);
  persist_write_int(STORAGE_KEY_SLOT_1_METRIC, s_settings.slot_metrics[0]);
  persist_write_int(STORAGE_KEY_SLOT_2_METRIC, s_settings.slot_metrics[1]);
  persist_write_int(STORAGE_KEY_SLOT_3_METRIC, s_settings.slot_metrics[2]);
  persist_write_int(STORAGE_KEY_SLOT_4_METRIC, s_settings.slot_metrics[3]);
  persist_write_bool(STORAGE_KEY_SHOW_LEADING_ZERO, s_settings.show_leading_zero);
  persist_write_bool(STORAGE_KEY_USE_24_HOUR, s_settings.use_24_hour);
  persist_write_bool(STORAGE_KEY_REVERSE_THEME, s_settings.reverse_theme);
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
  s_settings.theme = normalize_theme((ThemeType)read_int_or_default(STORAGE_KEY_THEME, THEME_DEFAULT));
  s_settings.slot_metrics[0] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_1_METRIC, METRIC_WEATHER);
  s_settings.slot_metrics[1] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_2_METRIC, METRIC_HEART_RATE);
  s_settings.slot_metrics[2] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_3_METRIC, METRIC_BATTERY);
  s_settings.slot_metrics[3] = (MetricType)read_int_or_default(STORAGE_KEY_SLOT_4_METRIC, METRIC_STEPS);
  s_settings.show_leading_zero = persist_exists(STORAGE_KEY_SHOW_LEADING_ZERO) ? persist_read_bool(STORAGE_KEY_SHOW_LEADING_ZERO) : true;
  s_settings.use_24_hour = persist_exists(STORAGE_KEY_USE_24_HOUR) ? persist_read_bool(STORAGE_KEY_USE_24_HOUR) : false;
  s_settings.reverse_theme = persist_exists(STORAGE_KEY_REVERSE_THEME) ? persist_read_bool(STORAGE_KEY_REVERSE_THEME) : false;
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
  if ((t = dict_find(iter, APP_KEY_THEME))) { s_settings.theme = normalize_theme((ThemeType)t->value->int32); settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_SLOT_1_METRIC))) { s_settings.slot_metrics[0] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_SLOT_2_METRIC))) { s_settings.slot_metrics[1] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_SLOT_3_METRIC))) { s_settings.slot_metrics[2] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_SLOT_4_METRIC))) { s_settings.slot_metrics[3] = (MetricType)t->value->int32; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_SHOW_LEADING_ZERO))) { s_settings.show_leading_zero = t->value->int32 == 1; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_USE_24_HOUR))) { s_settings.use_24_hour = t->value->int32 == 1; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_REVERSE_THEME))) { s_settings.reverse_theme = t->value->int32 == 1; settings_changed = true; }
  if ((t = dict_find(iter, APP_KEY_WEATHER_TEMP))) { s_weather_temp = t->value->int32; weather_changed = true; }
  if ((t = dict_find(iter, APP_KEY_WEATHER_CODE))) { s_weather_condition = (WeatherCondition)t->value->int32; weather_changed = true; }
  if ((t = dict_find(iter, APP_KEY_WEATHER_VALID))) { s_weather_available = t->value->int32 == 1; weather_changed = true; }
  layer_mark_dirty(s_canvas_layer);
  if (weather_changed) save_weather();
  if (settings_changed) {
    save_settings();
    send_settings_ready();
  }
}

static void request_weather(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter) {
    dict_write_uint8(iter, APP_KEY_REQUEST_WEATHER, 1);
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