var CONFIG_URL = 'https://lyle-morris.github.io/Hosting/apps/daypal/qa/app-config.html';
var CONFIG_CACHE_LABEL = 'daypal-2.0.0-qa-1';
var DONATION_URL = 'https://ko-fi.com/lylemorris';
var SETTINGS_KEY = 'daypal_settings';
var LEGACY_SETTINGS_KEY = 'daymate_settings';
var WEATHER_CACHE_KEY = 'daypal_weather';
var LEGACY_WEATHER_CACHE_KEY = 'daymate_weather';
var WEATHER_CACHE_MAX_AGE_MS = 1000 * 60 * 60;

var DEFAULT_SETTINGS = {
  theme: 0,
  slot_1_metric: 0,
  slot_2_metric: 1,
  slot_3_metric: 2,
  slot_4_metric: 4,
  show_leading_zero: true,
  use_24_hour: false,
  use_celsius: false,
  manual_location: false,
  manual_postal_code: '',
  manual_city: '',
  reverse_theme: false,
  analytics_enabled: true
};

function mergeSettings(base, saved) {
  var result = {};
  var key;
  for (key in base) result[key] = base[key];
  if (saved) for (key in saved) result[key] = saved[key];
  return result;
}

function toBool(value, fallback) {
  if (typeof value === 'undefined' || value === null) return fallback;
  if (value === true || value === 1 || value === '1' || value === 'true') return true;
  if (value === false || value === 0 || value === '0' || value === 'false') return false;
  return fallback;
}

function toInt(value, fallback) {
  var parsed = Number(value);
  return isNaN(parsed) ? fallback : parsed;
}

function cleanString(value) {
  if (typeof value === 'undefined' || value === null) return '';
  return String(value).replace(/^\s+|\s+$/g, '');
}

function normalizeSettings(settings) {
  settings = mergeSettings(DEFAULT_SETTINGS, settings || {});
  return {
    theme: toInt(settings.theme, DEFAULT_SETTINGS.theme),
    slot_1_metric: toInt(settings.slot_1_metric, DEFAULT_SETTINGS.slot_1_metric),
    slot_2_metric: toInt(settings.slot_2_metric, DEFAULT_SETTINGS.slot_2_metric),
    slot_3_metric: toInt(settings.slot_3_metric, DEFAULT_SETTINGS.slot_3_metric),
    slot_4_metric: toInt(settings.slot_4_metric, DEFAULT_SETTINGS.slot_4_metric),
    show_leading_zero: toBool(settings.show_leading_zero, DEFAULT_SETTINGS.show_leading_zero),
    use_24_hour: toBool(settings.use_24_hour, DEFAULT_SETTINGS.use_24_hour),
    use_celsius: toBool(settings.use_celsius, DEFAULT_SETTINGS.use_celsius),
    manual_location: toBool(settings.manual_location, DEFAULT_SETTINGS.manual_location),
    manual_postal_code: cleanString(settings.manual_postal_code),
    manual_city: cleanString(settings.manual_city),
    reverse_theme: toBool(settings.reverse_theme, DEFAULT_SETTINGS.reverse_theme),
    analytics_enabled: toBool(settings.analytics_enabled, DEFAULT_SETTINGS.analytics_enabled)
  };
}

function readSettingsFromStorage(key) {
  var raw = localStorage.getItem(key);
  if (!raw) return null;
  try { return JSON.parse(raw); } catch (e) { console.log('DayPal settings parse failed for ' + key + ': ' + e); return null; }
}

function loadSettings() {
  var saved = readSettingsFromStorage(SETTINGS_KEY);
  if (saved) return normalizeSettings(saved);
  var legacy = readSettingsFromStorage(LEGACY_SETTINGS_KEY);
  if (legacy) {
    var migrated = normalizeSettings(legacy);
    saveSettings(migrated);
    return migrated;
  }
  return normalizeSettings(DEFAULT_SETTINGS);
}

function saveSettings(settings) {
  localStorage.setItem(SETTINGS_KEY, JSON.stringify(normalizeSettings(settings)));
}

function sendSettings(settings) {
  var normalized = normalizeSettings(settings);
  var payload = {
    0: normalized.theme,
    1: normalized.slot_1_metric,
    2: normalized.slot_2_metric,
    3: normalized.slot_3_metric,
    4: normalized.slot_4_metric,
    5: normalized.show_leading_zero ? 1 : 0,
    6: normalized.use_24_hour ? 1 : 0,
    7: normalized.reverse_theme ? 1 : 0
  };
  console.log('DayPal sending settings: ' + JSON.stringify(normalized));
  Pebble.sendAppMessage(payload, function() { console.log('DayPal settings sent: ' + JSON.stringify(payload)); }, function(error) { console.log('DayPal settings send failed: ' + JSON.stringify(error)); });
}

function parseConfigResponse(rawResponse) {
  if (!rawResponse) return null;
  var response = rawResponse;
  var hashIndex = response.indexOf('#');
  if (hashIndex >= 0) response = response.substring(hashIndex + 1);
  try { response = decodeURIComponent(response); } catch (e) { console.log('DayPal config response decode failed, trying raw response: ' + e); }
  if (!response || response === 'CANCELLED') return null;
  return normalizeSettings(JSON.parse(response));
}

function weatherCodeToCondition(code) {
  if (code === 0) return 0;
  if (code === 1 || code === 2) return 1;
  if (code === 3) return 6;
  if (code === 45 || code === 48) return 5;
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return 2;
  if ((code >= 71 && code <= 77) || code === 85 || code === 86) return 4;
  if (code === 95 || code === 96 || code === 99) return 3;
  return 7;
}

function weatherUnit(settings) { return settings.use_celsius ? 'celsius' : 'fahrenheit'; }
function manualLocationQuery(settings) { if (settings.manual_postal_code) return settings.manual_postal_code; if (settings.manual_city) return settings.manual_city; return ''; }
function weatherCacheId(settings) { var normalized = normalizeSettings(settings); var mode = normalized.manual_location ? 'manual' : 'current'; var query = normalized.manual_location ? manualLocationQuery(normalized).toLowerCase() : 'device'; return mode + ':' + query + ':' + weatherUnit(normalized); }
function saveWeatherCache(temp, condition, settings) { localStorage.setItem(WEATHER_CACHE_KEY, JSON.stringify({temp: temp, condition: condition, cache_id: weatherCacheId(settings), timestamp: Date.now()})); }
function sendWeather(temp, condition) { Pebble.sendAppMessage({10: temp, 11: condition, 12: 1}, function() { console.log('DayPal weather sent: ' + temp + ', condition ' + condition); }, function(error) { console.log('DayPal weather send failed: ' + JSON.stringify(error)); }); }

function readWeatherCache() {
  var raw = localStorage.getItem(WEATHER_CACHE_KEY) || localStorage.getItem(LEGACY_WEATHER_CACHE_KEY);
  if (!raw) return null;
  try { return JSON.parse(raw); } catch (e) { console.log('DayPal weather cache parse failed: ' + e); return null; }
}

function sendWeatherFromCache(settings, allowStale) {
  var cached = readWeatherCache();
  if (!cached) return false;
  if (!allowStale && Date.now() - Number(cached.timestamp) > WEATHER_CACHE_MAX_AGE_MS) return false;
  if (cached.cache_id && cached.cache_id !== weatherCacheId(settings)) return false;
  if (!cached.cache_id && settings.use_celsius) return false;
  if (typeof cached.temp === 'undefined' || typeof cached.condition === 'undefined') return false;
  sendWeather(Number(cached.temp), Number(cached.condition));
  return true;
}

function sendCachedWeather(settings) {
  return sendWeatherFromCache(settings, false);
}

function sendLastWeatherRead(settings) {
  return sendWeatherFromCache(settings, true);
}

function sendWeatherUnavailable(settings) {
  if (sendLastWeatherRead(settings)) {
    console.log('DayPal using last successful weather read as fallback');
    return;
  }
  Pebble.sendAppMessage({12: 0});
}

function httpGetJson(url, onSuccess, onFailure) {
  var xhr = new XMLHttpRequest();
  xhr.open('GET', url, true);
  xhr.onload = function() {
    if (xhr.status < 200 || xhr.status >= 300) { console.log('DayPal HTTP failure: ' + xhr.status + ' for weather request'); onFailure(); return; }
    try { onSuccess(JSON.parse(xhr.responseText)); } catch (e) { console.log('DayPal JSON parse failed: ' + e); onFailure(); }
  };
  xhr.onerror = onFailure;
  xhr.send();
}

function fetchWeatherForCoordinates(lat, lon, settings) {
  var url = 'https://api.open-meteo.com/v1/forecast?latitude=' + encodeURIComponent(lat) + '&longitude=' + encodeURIComponent(lon) + '&current=temperature_2m,weather_code&temperature_unit=' + encodeURIComponent(weatherUnit(settings));
  httpGetJson(url, function(data) {
    if (!data.current || typeof data.current.temperature_2m === 'undefined' || typeof data.current.weather_code === 'undefined') { console.log('DayPal weather response missing current values'); sendWeatherUnavailable(settings); return; }
    var temp = Math.round(data.current.temperature_2m);
    var condition = weatherCodeToCondition(Number(data.current.weather_code));
    if (condition === 7) { console.log('DayPal weather condition unsupported: ' + data.current.weather_code); sendWeatherUnavailable(settings); return; }
    saveWeatherCache(temp, condition, settings);
    sendWeather(temp, condition);
  }, function() { sendWeatherUnavailable(settings); });
}

function isDigit(ch) { return ch >= '0' && ch <= '9'; }
function isLetter(ch) { return ch && ch.toLowerCase() !== ch.toUpperCase(); }
function isUSPostalCode(query) { var q = cleanString(query); return q.length >= 5 && isDigit(q.charAt(0)) && isDigit(q.charAt(1)) && isDigit(q.charAt(2)) && isDigit(q.charAt(3)) && isDigit(q.charAt(4)); }
function isCanadianPostalCode(query) { var q = cleanString(query).toUpperCase().replace(/ /g, ''); return q.length >= 3 && isLetter(q.charAt(0)) && isDigit(q.charAt(1)) && isLetter(q.charAt(2)); }
function zippopotamCountryForQuery(query) { if (isCanadianPostalCode(query)) return 'ca'; if (isUSPostalCode(query)) return 'us'; return null; }
function normalizePostalFallbackQuery(query, country) { var normalized = cleanString(query).toUpperCase().replace(/ /g, ''); if (country === 'ca') return normalized.substring(0, 3); return normalized.split('-')[0]; }

function requestPostalFallbackWeather(query, settings) {
  var country = zippopotamCountryForQuery(query);
  if (!country) { sendWeatherUnavailable(settings); return; }
  var normalizedPostal = normalizePostalFallbackQuery(query, country);
  var url = 'https://api.zippopotam.us/' + country + '/' + encodeURIComponent(normalizedPostal);
  console.log('DayPal resolving postal fallback');
  httpGetJson(url, function(data) {
    if (!data.places || !data.places.length) { console.log('DayPal postal fallback had no places'); sendWeatherUnavailable(settings); return; }
    fetchWeatherForCoordinates(data.places[0].latitude, data.places[0].longitude, settings);
  }, function() { sendWeatherUnavailable(settings); });
}

function requestManualWeather(settings) {
  var query = manualLocationQuery(settings);
  if (!query) { console.log('DayPal manual weather unavailable: missing location query'); sendWeatherUnavailable(settings); return; }
  var geocodeUrl = 'https://geocoding-api.open-meteo.com/v1/search?name=' + encodeURIComponent(query) + '&count=1&language=en&format=json';
  console.log('DayPal resolving manual weather location');
  httpGetJson(geocodeUrl, function(data) {
    if (!data.results || !data.results.length) { console.log('DayPal geocode not found; trying postal fallback if supported'); requestPostalFallbackWeather(query, settings); return; }
    fetchWeatherForCoordinates(data.results[0].latitude, data.results[0].longitude, settings);
  }, function() { requestPostalFallbackWeather(query, settings); });
}

function requestCurrentLocationWeather(settings) {
  if (!navigator.geolocation) { console.log('DayPal weather unavailable: no geolocation'); sendWeatherUnavailable(settings); return; }
  console.log('DayPal requesting current-location weather');
  navigator.geolocation.getCurrentPosition(function(position) {
    fetchWeatherForCoordinates(position.coords.latitude, position.coords.longitude, settings);
  }, function(error) {
    console.log('DayPal weather geolocation failed: ' + JSON.stringify(error));
    sendWeatherUnavailable(settings);
  }, {timeout: 15000, maximumAge: 1000 * 60 * 15});
}

function requestWeather() { var settings = loadSettings(); if (settings.manual_location) requestManualWeather(settings); else requestCurrentLocationWeather(settings); }

function trackAnalyticsEvent(name, data) {
  var settings = loadSettings();
  if (!settings.analytics_enabled) return;
  var events = [];
  try { events = JSON.parse(localStorage.getItem('daypal_analytics_events') || '[]'); } catch (e) { events = []; }
  events.push({name: name, data: data || {}, timestamp: new Date().toISOString()});
  localStorage.setItem('daypal_analytics_events', JSON.stringify(events.slice(-200)));
  console.log('DayPal analytics test event: ' + name + ' ' + JSON.stringify(data || {}));
}

Pebble.addEventListener('ready', function() { var settings = loadSettings(); console.log('DayPal ready with settings: ' + JSON.stringify(settings)); sendSettings(settings); requestWeather(); });
Pebble.addEventListener('appmessage', function(e) { if (e.payload && (e.payload.settings_ready || e.payload['21'])) console.log('DayPal settings applied on watch'); if (e.payload && (e.payload.request_weather || e.payload['20'])) requestWeather(); });
Pebble.addEventListener('showConfiguration', function() { var settings = encodeURIComponent(JSON.stringify(loadSettings())); console.log('DayPal opening config: ' + CONFIG_URL); Pebble.openURL(CONFIG_URL + '?v=' + encodeURIComponent(CONFIG_CACHE_LABEL) + '&settings=' + settings + '&donation_url=' + encodeURIComponent(DONATION_URL)); trackAnalyticsEvent('config_opened'); });
Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) { console.log('DayPal config closed without response'); return; }
  try {
    console.log('DayPal config response: ' + e.response);
    var settings = parseConfigResponse(e.response);
    if (!settings) { console.log('DayPal config response had no settings'); return; }
    saveSettings(settings);
    sendSettings(settings);
    requestWeather();
    trackAnalyticsEvent('settings_saved', {theme: settings.theme, reverse_theme: settings.reverse_theme ? 1 : 0, use_24_hour: settings.use_24_hour ? 1 : 0, use_celsius: settings.use_celsius ? 1 : 0, manual_location: settings.manual_location ? 1 : 0});
  } catch (err) { console.log('DayPal config response ignored: ' + err); }
});