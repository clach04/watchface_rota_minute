var meta = require('../../package.json');

module.exports = [
    { 
        "type": "heading", 
        "defaultValue": "Preferences" ,
        "size": 3
    }, 
    { 
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Colors"
            },
            {
                "type": "text",
                "defaultValue": "Only available on compatible platforms."
            },
            {
                "type": "color",
                "allowGray": true,
                "label": "Background",
                "messageKey": "BACKGROUND_COLOR",
                "defaultValue": "FFFFFF"
            },
            {
                "type": "color",
                "label": "Foreground",
                "messageKey": "TIME_COLOR",
                "defaultValue": "000000"
            },
            {
                "type": "color",
                "allowGray": true,
                "label": "Arc",
                "messageKey": "MINUTES_COLOR",
                "defaultValue": "0000FF"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Features"
            },
            {
                "type": "toggle",
                "label": "Vibrate on Bluetooth disconnect",
                "messageKey": "VIBRATE_ON_DISCONNECT",
                "defaultValue": false
            },
            {
                "type": "text",
                "defaultValue": "Will NOT vibrate during Quiet Time or if health monitoring detects sleeping."
            },

            {
                "type": "toggle",
                "label": "Display hour as text",
                "messageKey": "HOUR_AS_TEXT",
                "defaultValue": true
            },
            {
                "type": "slider",
                "label": "Arc width",
                "messageKey": "ARC_WIDTH",
                "defaultValue": 3 * 7,
                "min": 7,
                "max": 5 * 7,
                "step": 1
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    },
    {
        "type": "text",
        "defaultValue": "v" + meta.version
    }
];
