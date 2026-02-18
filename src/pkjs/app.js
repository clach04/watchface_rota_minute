var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

// from https://github.com/clach04/pebble-VoiceRelay/blob/js_indent/src/js/pebble-js-app.js
// Also see https://github.com/stopsatgreen/modernwebbook/blob/master/Code%20Examples/Chapter%2006/battery-event.html

// Battery level has changed
function on_phone_battery_level_change(battery) {
    logit("Battery Level: " + battery.level);

    var battery_level = 999;
    //   if(!isNumeric(battery.level))
    battery_level = Math.floor(battery.level * 100);

    if (!isNumeric(battery_level)) battery_level = 999;
    var dict = { "PHONE_BATTTERY_PERCENT": battery_level };
    console.log('sending dict to Pebble: ' + JSON.stringify(dict));
    Pebble.sendAppMessage(dict,
        function(e) {
            console.log('successfully sent battery level dict to pebble');
        },
        function(e) {
            console.log('failed to send battery level dict to pebble. Error: ' + JSON.stringify(e));  // NOTE possibly secret leakage if adding secrets to config
        }
    );
}

// Phone Battery Init
function phone_battery_subscribe(battery) {
/*
    // Listen for changes in charging status
    battery.addEventListener('chargingchange', function() {
        chargeStatus(battery);
    }, false);
    */
    // Listen for changes in charge level
    battery.addEventListener('levelchange', function() {
        on_phone_battery_level_change(battery);
    }, false);
    // Run both functions on page load
    on_phone_battery_level_change(battery);
    /*
    chargeStatus(battery);
    */
}


function batteryStatusFailure() {
    console.log("Phone Battery function failed to successfully resolve the promise.  Yeah, decypher that.");
}


function init_phone_battery() {
    // Test for old or new battery API
    if (navigator.battery) {
        phone_battery_subscribe(navigator.battery);
    } else if (navigator.getBattery) {
        navigator.getBattery().then(function(newBattery) {
            phone_battery_subscribe(newBattery);
        }, batteryStatusFailure);
    } else {
        console.log('No phone battery API.  Only works on newer Android releases');
    }
}


Pebble.addEventListener('ready', function(e) {
    console.log('ready entry');
    init_phone_battery();
});


// override send config, in case want to modify (change types) values or debug
Pebble.addEventListener('webviewclosed', function(e) {
    console.log('webviewclosed entry');
    if (e && !e.response) {
        return;
    }

    var dict = clay.getSettings(e.response);
    //console.log(dict.VIBRATE_ON_DISCONNECT);

    console.log('e string config data length=' + JSON.stringify(e).length);
    console.log('dict config data length=' + JSON.stringify(dict).length);
    console.log(JSON.stringify(e));  // NOTE possibly secret leakage if adding secrets to config
    console.log(JSON.stringify(dict));  // NOTE possibly secret leakage if adding secrets to config

    // Send settings values to watch side
    console.log('DEBUG pre-send');
    Pebble.sendAppMessage(dict, function(e) {
        console.log('DEBUG success send');
        console.log('Sent config data to Pebble');
    }, function(e) {
        console.log('DEBUG FAIL send');
        console.log('Failed to send data options to Pebble. Error: ' + JSON.stringify(e));  // NOTE possibly secret leakage if adding secrets to config
    });
    console.log('DEBUG post-send');
});
