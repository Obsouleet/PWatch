Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://rawgit.com/Obsouleet/Stymple/master/config/Stymple4.html';
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
	"KEY_SHOWDATE":parseInt(configData['showdate'])
	};
	

  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});