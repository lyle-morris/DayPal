var CONFIG_URL = 'https://lyle-morris.github.io/DayMate-config/';
var WEATHER_CACHE_KEY = 'daymate_weather';
var WEATHER_CACHE_MAX_AGE_MS = 1000 * 60 * 60;

var DEFAULT_SETTINGS = {
  theme: 0,
  slot_1_metric: 0,
  slot_2_metric: 1,
  slot_3_metric: 2,
  slot_4_metric: 4,
  show_leading_zero: true
};

function mergeSettings(base, saved) {
  var result = {};
  var key;
  for (key in base) result[key] = base[key];
  if (saved) for (key in saved) result[key] = saved[key];
  return result;
}

function loadSettings() {
  var raw = localStorage.getItem('daymate_settings');
  if (!raw) return DEFAULT_SETTINGS;
  try {
    var saved = JSON.parse(raw);
    return mergeSettings(DEFAULT_SETTINGS, saved);
  } catch (e) {
    return DEFAULT_SETTINGS;
  }
}

function saveSettings(settings) {
  localStorage.setItem('daymate_settings', JSON.stringify(settings));
}

function normalizeSettings(settings) {
  return {
    theme: Number(settings.theme),
    slot_1_metric: Number(settings.slot_1_metric),
    slot_2_metric: Number(settings.slot_2_metric),
    slot_3_metric: Number(settings.slot_3_metric),
    slot_4_metric: Number(settings.slot_4_metric),
    show_leading_zero: !!settings.show_leading_zero
  };
}

function sendSettings(settings) {
  var normalized = normalizeSettings(settings);

  var payload = {
    0: normalized.theme,
    1: normalized.slot_1_metric,
    2: normalized.slot_2_metric,
    3: normalized.slot_3_metric,
    4: normalized.slot_4_metric,
    5: normalized.show_leading_zero ? 1 : 0
  };

  console.log('DayMate sending settings: ' + JSON.stringify(normalized));
  Pebble.sendAppMessage(payload, function() {
    console.log('DayMate settings sent: ' + JSON.stringify(payload));
  }, function(error) {
    console.log('DayMate settings send failed: ' + JSON.stringify(error));
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
    console.log('DayMate config response decode failed, trying raw response: ' + e);
  }

  if (!response || response === 'CANCELLED') return null;
  return normalizeSettings(JSON.parse(response));
}

function weatherCodeToCondition(code) {
  if (code === 0) return 0;
  if (code === 1 || code === 2 || code === 3) return 1;
  if (code === 45 || code === 48) return 5;
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return 2;
  if ((code >= 71 && code <= 77) || code === 85 || code === 86) return 4;
  if (code === 95 || code === 96 || code === 99) return 3;
  return 6;
}

function saveWeatherCache(temp, condition) {
  localStorage.setItem(WEATHER_CACHE_KEY, JSON.stringify({
    temp: temp,
    condition: condition,
    timestamp: Date.now()
  }));
}

function sendWeather(temp, condition) {
  Pebble.sendAppMessage({
    10: temp,
    11: condition,
    12: 1
  }, function() {
    console.log('DayMate weather sent: ' + temp + ', condition ' + condition);
  }, function(error) {
    console.log('DayMate weather send failed: ' + JSON.stringify(error));
  });
}

function sendCachedWeather() {
  var raw = localStorage.getItem(WEATHER_CACHE_KEY);
  if (!raw) return false;
  try {
    var cached = JSON.parse(raw);
    if (!cached || Date.now() - Number(cached.timestamp) > WEATHER_CACHE_MAX_AGE_MS) return false;
    sendWeather(Number(cached.temp), Number(cached.condition));
    return true;
  } catch (e) {
    return false;
  }
}

function sendWeatherUnavailable() {
  if (sendCachedWeather()) return;
  Pebble.sendAppMessage({
    12: 0
  });
}

function requestWeather() {
  if (!navigator.geolocation) {
    console.log('DayMate weather unavailable: no geolocation');
    sendWeatherUnavailable();
    return;
  }

  console.log('DayMate requesting weather');
  navigator.geolocation.getCurrentPosition(function(position) {
    var lat = position.coords.latitude;
    var lon = position.coords.longitude;
    var url = 'https://api.open-meteo.com/v1/forecast?latitude=' + encodeURIComponent(lat) +
      '&longitude=' + encodeURIComponent(lon) +
      '&current=temperature_2m,weather_code&temperature_unit=fahrenheit';

    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.onload = function() {
      if (xhr.status < 200 || xhr.status >= 300) {
        console.log('DayMate weather HTTP failure: ' + xhr.status);
        sendWeatherUnavailable();
        return;
      }
      try {
        var data = JSON.parse(xhr.responseText);
        if (!data.current || typeof data.current.temperature_2m === 'undefined' || typeof data.current.weather_code === 'undefined') {
          console.log('DayMate weather response missing current values');
          sendWeatherUnavailable();
          return;
        }
        var temp = Math.round(data.current.temperature_2m);
        var condition = weatherCodeToCondition(Number(data.current.weather_code));
        if (condition === 6) {
          console.log('DayMate weather condition unsupported: ' + data.current.weather_code);
          sendWeatherUnavailable();
          return;
        }
        saveWeatherCache(temp, condition);
        sendWeather(temp, condition);
      } catch (e) {
        console.log('DayMate weather parse failed: ' + e);
        sendWeatherUnavailable();
      }
    };
    xhr.onerror = sendWeatherUnavailable;
    xhr.send();
  }, function(error) {
    console.log('DayMate weather geolocation failed: ' + JSON.stringify(error));
    sendWeatherUnavailable();
  }, {
    timeout: 15000,
    maximumAge: 1000 * 60 * 15
  });
}

Pebble.addEventListener('ready', function() {
  var settings = loadSettings();
  console.log('DayMate ready with settings: ' + JSON.stringify(settings));
  sendSettings(settings);
  requestWeather();
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && (e.payload.settings_ready || e.payload['21'])) {
    console.log('DayMate settings applied on watch');
  }
  if (e.payload && (e.payload.request_weather || e.payload['20'])) {
    requestWeather();
  }
});

Pebble.addEventListener('showConfiguration', function() {
  var settings = encodeURIComponent(JSON.stringify(loadSettings()));
  console.log('DayMate opening config: ' + CONFIG_URL);
  Pebble.openURL(CONFIG_URL + '?settings=' + settings);
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) {
    console.log('DayMate config closed without response');
    return;
  }
  try {
    console.log('DayMate config response: ' + e.response);
    var settings = parseConfigResponse(e.response);
    if (!settings) {
      console.log('DayMate config response had no settings');
      return;
    }
    saveSettings(settings);
    sendSettings(settings);
    requestWeather();
  } catch (err) {
    console.log('DayMate config response ignored: ' + err);
  }
});
