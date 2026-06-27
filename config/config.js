var THEMES = [
  ['Default', 0],
  ['Blue', 1],
  ['Pink', 2],
  ['Green', 3],
  ['White', 4],
  ['Orange', 5],
  ['Dark Blue', 6],
  ['Black', 7]
];

var METRICS = [
  ['Weather', 0],
  ['Heart Rate', 1],
  ['Battery', 2],
  ['Calories', 3],
  ['Steps', 4],
  ['None', 5]
];

var DEFAULT_SETTINGS = {
  theme: 0,
  slot_1_metric: 0,
  slot_2_metric: 1,
  slot_3_metric: 2,
  slot_4_metric: 4,
  show_leading_zero: true
};

function qs(name) {
  var match = new RegExp('[?&]' + name + '=([^&]*)').exec(window.location.search);
  return match ? decodeURIComponent(match[1]) : null;
}

function getIncomingSettings() {
  var raw = qs('settings');
  if (!raw) return DEFAULT_SETTINGS;
  try {
    return Object.assign({}, DEFAULT_SETTINGS, JSON.parse(raw));
  } catch (e) {
    return DEFAULT_SETTINGS;
  }
}

function fillSelect(id, options, selected) {
  var select = document.getElementById(id);
  options.forEach(function(option) {
    var el = document.createElement('option');
    el.textContent = option[0];
    el.value = option[1];
    if (Number(selected) === Number(option[1])) el.selected = true;
    select.appendChild(el);
  });
}

var settings = getIncomingSettings();

fillSelect('theme', THEMES, settings.theme);
fillSelect('slot_1_metric', METRICS, settings.slot_1_metric);
fillSelect('slot_2_metric', METRICS, settings.slot_2_metric);
fillSelect('slot_3_metric', METRICS, settings.slot_3_metric);
fillSelect('slot_4_metric', METRICS, settings.slot_4_metric);
document.getElementById('show_leading_zero').checked = !!settings.show_leading_zero;

document.getElementById('settings-form').addEventListener('submit', function(event) {
  event.preventDefault();
  var payload = {
    theme: Number(document.getElementById('theme').value),
    slot_1_metric: Number(document.getElementById('slot_1_metric').value),
    slot_2_metric: Number(document.getElementById('slot_2_metric').value),
    slot_3_metric: Number(document.getElementById('slot_3_metric').value),
    slot_4_metric: Number(document.getElementById('slot_4_metric').value),
    show_leading_zero: document.getElementById('show_leading_zero').checked
  };
  document.location = 'pebblejs://close#' + encodeURIComponent(JSON.stringify(payload));
});

document.getElementById('cancel').addEventListener('click', function() {
  document.location = 'pebblejs://close';
});
