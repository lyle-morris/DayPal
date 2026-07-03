var CONFIG_URL = 'https://lyle-morris.github.io/DayPal-config/';
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
  use_24_hour: true,
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
  try {
    return JSON.parse(raw);
  } catch (e) {
    console.log('DayPal settings parse failed for ' + key + ': ' + e);
    return null;
  }
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
  Pebble.sendAppMessage(payload, function() {
    console.log('DayPal settings sent: ' + JSON.stringify(payload));
  }, function(error) {
    console.log('DayPal settings send failed: ' + JSON.stringify(error));
  });
}

function parseConfigResponse(rawResponse) {
  if (!rawResponse) return null;

  var response = rawResponse;
  var hashIndex = response.indexOf('#');
  if (hashIndex >= 0) response = response.substring(hashIndex + 1);

  try {
    response = decodeURIComponent(response);
  } catch (e) {
    console.log('DayPal config response decode failed, trying raw response: ' + e);
  }

  if (!response || response === 'CANCELLED') return null;
  return normalizeSettings(JSON.parse(response));
}

function weatherCodeToCondition(code) {
  if (code === 0) return 0; // Sunny
  if (code === 1 || code === 2) return 1; // Partly cloudy
  if (code === 3) return 6; // Cloudy / overcast
  if (code === 45 || code === 48) return 5; // Fog
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return 2; // Rain
  if ((code >= 71 && code <= 77) || code === 85 || code === 86) return 4; // Snow
  if (code === 95 || code === 96 || code === 99) return 3; // Storm
  return 7; // Unknown / unsupported
}

function weatherUnit(settings) {
  return settings.use_celsius ? 'celsius' : 'fahrenheit';
}

function manualLocationQuery(settings) {
  if (settings.manual_postal_code) return settings.manual_postal_code;
  if (settings.manual_city) return settings.manual_city;
  return '';
}

function weatherCacheId(settings) {
  var normalized = normalizeSettings(settings);
  var mode = normalized.manual_location ? 'manual' : 'current';
  var query = normalized.manual_location ? manualLocationQuery(normalized).toLowerCase() : 'device';
  return mode + ':' + query + ':' + weatherUnit(normalized);
}

function saveWeatherCache(temp, condition, settings) {
  localStorage.setItem(WEATHER_CACHE_KEY, JSON.stringify({
    temp: temp,
    condition: condition,
    cache_id: weatherCacheId(settings),
    timestamp: Date.now()
  }));
}

function sendWeather(temp, condition) {
  Pebble.sendAppMessage({
    10: temp,
    11: condition,
    12: 1
  }, function() {
    console.log('DayPal weather sent: ' + temp + ', condition ' + condition);
  }, function(error) {
    console.log('DayPal weather send failed: ' + JSON.stringify(error));
  });
}

function sendCachedWeather(settings) {
  var raw = localStorage.getItem(WEATHER_CACHE_KEY);
  if (!raw) raw = localStorage.getItem(LEGACY_WEATHER_CACHE_KEY);
  if (!raw) return false;
  try {
    var cached = JSON.parse(raw);
    if (!cached || Date.now() - Number(cached.timestamp) > WEATHER_CACHE_MAX_AGE_MS) return false;
    if (cached.cache_id && cached.cache_id !== weatherCacheId(settings)) return false;
    if (!cached.cache_id && settings.use_celsius) return false;
    sendWeather(Number(cached.temp), Number(cached.condition));
    return true;
  } catch (e) {
    return false;
  }
}

function sendWeatherUnavailable(settings) {
  if (sendCachedWeather(settings)) return;
  Pebble.sendAppMessage({
    12: 0
  });
}

function httpGetJson(url, onSuccess, onFailure) {
  var xhr = new XMLHttpRequest();
  xhr.open('GET', url, true);
  xhr.onload = function() {
    if (xhr.status < 200 || xhr.status >= 300) {
      console.log('DayPal HTTP failure: ' + xhr.status + ' for ' + url);
      onFailure();
      return;
    }
    try {
      onSuccess(JSON.parse(xhr.responseText));
    } catch (e) {
      console.log('DayPal JSON parse failed: ' + e);
      onFailure();
    }
  };
  xhr.onerror = onFailure;
  xhr.send();
}

function fetchWeatherForCoordinates(lat, lon, settings) {
  var url = 'https://api.open-meteo.com/v1/forecast?latitude=' + encodeURIComponent(lat) +
    '&longitude=' + encodeURIComponent(lon) +
    '&current=temperature_2m,weather_code&temperature_unit=' + encodeURIComponent(weatherUnit(settings));

  httpGetJson(url, function(data) {
    if (!data.current || typeof data.current.temperature_2m === 'undefined' || typeof data.current.weather_code === 'undefined') {
      console.log('DayPal weather response missing current values');
      sendWeatherUnavailable(settings);
      return;
    }
    var temp = Math.round(data.current.temperature_2m);
    var condition = weatherCodeToCondition(Number(data.current.weather_code));
    if (condition === 7) {
      console.log('DayPal weather condition unsupported: ' + data.current.weather_code);
      sendWeatherUnavailable(settings);
      return;
    }
    saveWeatherCache(temp, condition, settings);
    sendWeather(temp, condition);
  }, function() {
    sendWeatherUnavailable(settings);
  });
}

function requestManualWeather(settings) {
  var query = manualLocationQuery(settings);
  if (!query) {
    console.log('DayPal manual weather unavailable: missing location query');
    sendWeatherUnavailable(settings);
    return;
  }

  var geocodeUrl = 'https://geocoding-api.open-meteo.com/v1/search?name=' + encodeURIComponent(query) +
    '&count=1&language=en&format=json';

  console.log('DayPal resolving manual weather location: ' + query);
  httpGetJson(geocodeUrl, function(data) {
    if (!data.results || !data.results.length) {
      console.log('DayPal manual weather location not found');
      sendWeatherUnavailable(settings);
      return;
    }
    fetchWeatherForCoordinates(data.results[0].latitude, data.results[0].longitude, settings);
  }, function() {
    sendWeatherUnavailable(settings);
  });
}

function requestCurrentLocationWeather(settings) {
  if (!navigator.geolocation) {
    console.log('DayPal weather unavailable: no geolocation');
    sendWeatherUnavailable(settings);
    return;
  }

  console.log('DayPal requesting current-location weather');
  navigator.geolocation.getCurrentPosition(function(position) {
    fetchWeatherForCoordinates(position.coords.latitude, position.coords.longitude, settings);
  }, function(error) {
    console.log('DayPal weather geolocation failed: ' + JSON.stringify(error));
    sendWeatherUnavailable(settings);
  }, {
    timeout: 15000,
    maximumAge: 1000 * 60 * 15
  });
}

function requestWeather() {
  var settings = loadSettings();
  if (settings.manual_location) {
    requestManualWeather(settings);
  } else {
    requestCurrentLocationWeather(settings);
  }
}

function trackAnalyticsEvent(name, data) {
  var settings = loadSettings();
  if (!settings.analytics_enabled) return;
  console.log('DayPal analytics placeholder: ' + name + ' ' + JSON.stringify(data || {}));
}

Pebble.addEventListener('ready', function() {
  var settings = loadSettings();
  console.log('DayPal ready with settings: ' + JSON.stringify(settings));
  sendSettings(settings);
  requestWeather();
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && (e.payload.settings_ready || e.payload['21'])) {
    console.log('DayPal settings applied on watch');
  }
  if (e.payload && (e.payload.request_weather || e.payload['20'])) {
    requestWeather();
  }
});

Pebble.addEventListener('showConfiguration', function() {
  var settings = encodeURIComponent(JSON.stringify(loadSettings()));
  console.log('DayPal opening config: ' + CONFIG_URL);
  Pebble.openURL(CONFIG_URL + '?settings=' + settings + '&donation_url=' + encodeURIComponent(DONATION_URL));
  trackAnalyticsEvent('config_opened');
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) {
    console.log('DayPal config closed without response');
    return;
  }
  try {
    console.log('DayPal config response: ' + e.response);
    var settings = parseConfigResponse(e.response);
    if (!settings) {
      console.log('DayPal config response had no settings');
      return;
    }
    saveSettings(settings);
    sendSettings(settings);
    requestWeather();
    trackAnalyticsEvent('settings_saved', {
      theme: settings.theme,
      reverse_theme: settings.reverse_theme ? 1 : 0,
      use_24_hour: settings.use_24_hour ? 1 : 0,
      use_celsius: settings.use_celsius ? 1 : 0,
      manual_location: settings.manual_location ? 1 : 0
    });
  } catch (err) {
    console.log('DayPal config response ignored: ' + err);
  }
});
