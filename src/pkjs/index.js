var CONFIG_URL = 'config/index.html';

var KEYS = {
  theme: 0,
  slot_1_metric: 1,
  slot_2_metric: 2,
  slot_3_metric: 3,
  slot_4_metric: 4,
  show_leading_zero: 5,
  weather_temp: 10,
  weather_code: 11,
  weather_valid: 12,
  request_weather: 20,
  settings_ready: 21
};

var DEFAULT_SETTINGS = {
  theme: 0,
  slot_1_metric: 0,
  slot_2_metric: 1,
  slot_3_metric: 2,
  slot_4_metric: 4,
  show_leading_zero: true
};

function loadSettings() {
  var raw = localStorage.getItem('daymate_settings');
  if (!raw) return DEFAULT_SETTINGS;
  try {
    var saved = JSON.parse(raw);
    return Object.assign({}, DEFAULT_SETTINGS, saved);
  } catch (e) {
    return DEFAULT_SETTINGS;
  }
}

function saveSettings(settings) {
  localStorage.setItem('daymate_settings', JSON.stringify(settings));
}

function sendSettings(settings) {
  Pebble.sendAppMessage({
    0: Number(settings.theme),
    1: Number(settings.slot_1_metric),
    2: Number(settings.slot_2_metric),
    3: Number(settings.slot_3_metric),
    4: Number(settings.slot_4_metric),
    5: settings.show_leading_zero ? 1 : 0
  });
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

function sendWeatherUnavailable() {
  Pebble.sendAppMessage({
    12: 0
  });
}

function requestWeather() {
  if (!navigator.geolocation) {
    sendWeatherUnavailable();
    return;
  }

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
        sendWeatherUnavailable();
        return;
      }
      try {
        var data = JSON.parse(xhr.responseText);
        if (!data.current || typeof data.current.temperature_2m === 'undefined' || typeof data.current.weather_code === 'undefined') {
          sendWeatherUnavailable();
          return;
        }
        var temp = Math.round(data.current.temperature_2m);
        var condition = weatherCodeToCondition(Number(data.current.weather_code));
        if (condition === 6) {
          sendWeatherUnavailable();
          return;
        }
        Pebble.sendAppMessage({
          10: temp,
          11: condition,
          12: 1
        });
      } catch (e) {
        sendWeatherUnavailable();
      }
    };
    xhr.onerror = sendWeatherUnavailable;
    xhr.send();
  }, sendWeatherUnavailable, {
    timeout: 15000,
    maximumAge: 1000 * 60 * 15
  });
}

Pebble.addEventListener('ready', function() {
  var settings = loadSettings();
  sendSettings(settings);
  requestWeather();
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && e.payload.request_weather) {
    requestWeather();
  }
});

Pebble.addEventListener('showConfiguration', function() {
  var settings = encodeURIComponent(JSON.stringify(loadSettings()));
  Pebble.openURL(CONFIG_URL + '?settings=' + settings);
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) return;
  try {
    var settings = JSON.parse(decodeURIComponent(e.response));
    saveSettings(settings);
    sendSettings(settings);
    requestWeather();
  } catch (err) {
    // Ignore cancelled or invalid configuration responses.
  }
});
