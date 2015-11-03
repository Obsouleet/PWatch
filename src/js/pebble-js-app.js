Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://cdn.rawgit.com/Obsouleet/PWatch/master/config/index.html';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

//  var backgroundColor = configData['background_color'];

  var dict = {
	"KEY_COLOR_NR":parseInt(configData['colour']),
	"KEY_INVERTED":parseInt(configData['inverted']),
	"KEY_TWELVEHR":parseInt(configData['twelvehr']),
	"KEY_SHOWDATE":parseInt(configData['showdate']),
		
	"KEY_COL00":parseInt(configData['col00']),
	"KEY_COL01":parseInt(configData['col01']),
	"KEY_COL02":parseInt(configData['col02']),
		
	"KEY_COL10":parseInt(configData['col10']),
	"KEY_COL11":parseInt(configData['col11']),
	"KEY_COL12":parseInt(configData['col12']),
		
	"KEY_COL20":parseInt(configData['col20']),
	"KEY_COL21":parseInt(configData['col21']),
	"KEY_COL22":parseInt(configData['col22']),

	"KEY_COL30":parseInt(configData['col30']),
	"KEY_COL31":parseInt(configData['col31']),
	"KEY_COL32":parseInt(configData['col32'])

	};

  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});