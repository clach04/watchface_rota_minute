#pragma once

#define SETUP_TIME setup_time
#define CLEANUP_TIME cleanup_time
#define NO_TEXT_TIME_LAYER
#define DEFAULT_TIME_COLOR GColorBlack
#define DEFAULT_TIME_ARC_COLOR GColorBlue
#define DEFAULT_BACKGROUND_COLOR GColorWhite
#define CUSTOM_IN_RECV_HANDLER custom_in_recv_handler
#define BLUETOOTH_DISCONNECTED_STR "No BT"
// Consider using BT image rather than text
// CLOCK_POS is not used

//#define DEBUG
#ifdef DEBUG
#define TICK_HANDLER_INTERVAL SECOND_UNIT
#endif /* DEBUG */

#define USE_GENERIC_MAIN
#undef REMOVE_LEADING_ZERO_FROM_TIME  /* 12 hour display will not have a leading '0' or ' ' if this is defined */
#undef FONT_NAME
#undef FONT_SYSTEM_NAME  /* the default font system will be used */
#undef DEBUG_TIME
//#define USE_TIME_MACHINE  // NOTE mixing with DEBUG_TIME doesn't make sense. Requires update to package.json for dependencies

// Show step count using builtin code
#undef USE_HEALTH
//#define UPDATE_HEALTH_ON_ACTIVITY  /* If not set, only updates step count display once per minute */

//#define DRAW_BATTERY
#ifdef DRAW_BATTERY
    #if PBL_DISPLAY_HEIGHT != 228  // 200x228 Pebble Time 2 (emery)  // FIXME this is basically an EMERY check :-(
    #define DRAW_SMALL_BATTERY
    #endif  //PBL_DISPLAY_HEIGHT != 228
#endif  // DRAW_BATTERY

#define BAT_FMT_STR "%d%%"
#define MAX_BAT_STR "??%"  // When Battery is 100, the percent symbol is deliberately not shown (buffer full/truncated)


//#define QUIET_TIME_POS  GRect(0, 0, 60, 60)
#define QUIET_TIME_IMAGE RESOURCE_ID_IMAGE_QUIET_TIME
#define QUIET_TIME_IMAGE_GRECT GRect(5, 5, 20, 20)  // Example assumes a 20x20 image

// https://developer.rebble.io/guides/app-resources/system-fonts/
#define FONT_BAT_SYSTEM_NAME FONT_KEY_GOTHIC_18
#define FONT_BAT_PIXEL_HEIGHT 18
#define FONT_BAT_STR_PIXEL_WIDTH 50  // FONT_KEY_GOTHIC_18 maybe wider than really needed?

#define PERCENT_WIDTH(x) (PBL_DISPLAY_WIDTH * x / 1000)  // where x is a 1000th. E.g. x=18 == 1.8%, x=1000 == 100%
#define PERCENT_HEIGHT(x) (PBL_DISPLAY_HEIGHT * x / 1000)  // where x is a 1000th. E.g. x=18 == 1.8%, x=1000 == 100%

// See https://developer.rebble.io/guides/best-practices/building-for-every-pebble/#available-defines-and-macros for hardware specific defines
#ifdef PBL_ROUND /* 180x180 */
    #if PBL_DISPLAY_HEIGHT == 180  // 180x180 Pebble Round (chalk)
/*TODO center/move right*/
        #define CLOCK_POS GRect(0, 52, 180, 180) /* probably taller than really needed */
        #define HEALTH_POS GRect(0, 40, 180, 180)
        #define BT_POS GRect(0, 100, 180, 180) /* probably taller than really needed */

        #define DATE_ALIGN GTextAlignmentCenter
        #define DATE_POS GRect(0, PERCENT_HEIGHT(600), PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT) /* probably taller than really needed */

        #define BAT_ALIGN GTextAlignmentCenter
        #ifdef DRAW_BATTERY
            #define BAT_POS GRect(85, 10, 180, 180) /* probably taller than really needed */
        #else
            #define BAT_POS GRect(PERCENT_WIDTH(500) - (FONT_BAT_STR_PIXEL_WIDTH / 2), PERCENT_HEIGHT(680), FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)
        #endif /* DRAW_BATTERY */
    #else
        // gabbro - Pebble Round 2
        #define CLOCK_POS GRect(0, 52, PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT) /* probably taller than really needed */
        #define HEALTH_POS GRect(0, 40, PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT)
        #define BT_POS GRect(0, 100, PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT) /* probably taller than really needed */

        #define DATE_ALIGN GTextAlignmentCenter
        #define DATE_POS GRect(0, PERCENT_HEIGHT(600), PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT) /* probably taller than really needed */

        #define BAT_ALIGN GTextAlignmentCenter
        #ifdef DRAW_BATTERY
            #define BAT_POS GRect(85, 10, PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT) /* probably taller than really needed */
        #else
            #define BAT_POS GRect(0, PERCENT_HEIGHT(700), PBL_DISPLAY_WIDTH, PBL_DISPLAY_HEIGHT) /* probably taller than really needed */
        #endif /* DRAW_BATTERY */
    #endif /* PLAY_HEIGHT == 180 */

    #define PHONE_BAT_ALIGN GTextAlignmentCenter
    #define PHONE_BAT_POS GRect(PERCENT_WIDTH(500) - (FONT_BAT_STR_PIXEL_WIDTH / 2), PERCENT_HEIGHT(300), FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)

#else /* PBL_RECT */
    //#define DATE_FMT_STR "%a\n%b\n%d"
    //#define MAX_DATE_STR "Thu\n00\nAug" /* if custom version of DATE_FMT_STR is set, MAX_DATE_STR  needs to be updated too */
    //#define DATE_FMT_STR "%b\n%d"
    //#define MAX_DATE_STR "00\nAug" /* if custom version of DATE_FMT_STR is set, MAX_DATE_STR  needs to be updated too */
    #define DATE_FMT_STR "%a %b\n%d"
    #define MAX_DATE_STR "Thu Aug\n00"

    #if PBL_DISPLAY_HEIGHT == 228  // 200x228 Pebble Time 2 (emery)
        // FIXME size check
        // FIXME hard code date and battery location - see framework sample
        // TODO add TODO notes to dynamically determine location? as is done for circle and time text?
        #define HEALTH_POS GRect(0, 40, 200, 228)
        #define DATE_POS GRect(-5, 0, 200, 228) /* probably taller than really needed */
        #ifdef DRAW_BATTERY
            #define BAT_POS GRect(5, 210, 200, 228)
        #endif /* DRAW_BATTERY */

        #define BT_POS GRect(0, 130, 200, 228) /* probably taller than really needed */

        #undef FONT_BAT_SYSTEM_NAME
        #undef FONT_BAT_PIXEL_HEIGHT
        //#define FONT_BAT_SYSTEM_NAME FONT_KEY_GOTHIC_24
        #define FONT_BAT_SYSTEM_NAME FONT_KEY_GOTHIC_24_BOLD
        #define FONT_BAT_PIXEL_HEIGHT 24
        //#define FONT_BAT_STR_PIXEL_WIDTH 50  // FONT_KEY_GOTHIC_24 Just enough for 3 characters, Charging shows as "Cha" - TODO revisit this


    #else  //  144x168  Original pebbles; Pebble Classic (aplite), Pebble Time (basalt), Pebble 2 (diorite), Pebble 2 Duo (flint)
        #define CLOCK_POS GRect(0, 52, 144, 168) /* probably taller than really needed */
        #define HEALTH_POS GRect(0, 40, 144, 168)
        #define BT_POS GRect(0, 100, 144, 168) /* probably taller than really needed */
        //#define BT_POS GRect(0, 20, 144, 168) /* probably taller than really needed */
        //#define DATE_POS GRect(0, 148, 125, 168) /* bottom of "g" in "Aug" starting to be cut off . Top RHS may be be better (see Capinion/stormtrooper/spawn */

        #define DATE_POS GRect(0, -5, 140, 168) /* probably taller than really needed */

        #ifdef DRAW_BATTERY
            #define BAT_POS GRect(5, 150, 144, 168)
        #endif /* DRAW_BATTERY */
    #endif  // end of original rectangle size

#ifndef DRAW_BATTERY
    #define BAT_POS GRect(PERCENT_WIDTH(22), (PBL_DISPLAY_HEIGHT - FONT_BAT_PIXEL_HEIGHT - PERCENT_HEIGHT(18)), FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)
#endif /* DRAW_BATTERY */

    #define PHONE_BAT_ALIGN GTextAlignmentRight
    #define PHONE_BAT_POS GRect(PBL_DISPLAY_WIDTH - FONT_BAT_STR_PIXEL_WIDTH - PERCENT_WIDTH(22), (PBL_DISPLAY_HEIGHT - FONT_BAT_PIXEL_HEIGHT - PERCENT_HEIGHT(18)), FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)

    //#define PHONE_BAT_POS GRect(PBL_DISPLAY_WIDTH - FONT_BAT_STR_PIXEL_WIDTH, PBL_DISPLAY_HEIGHT - FONT_BAT_PIXEL_HEIGHT - (PBL_DISPLAY_HEIGHT / 1000 * 100 /* 10%  1.8% border spacing */ ), FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)
    //#define PHONE_BAT_POS GRect(PBL_DISPLAY_WIDTH - FONT_BAT_STR_PIXEL_WIDTH, (PBL_DISPLAY_HEIGHT - FONT_BAT_PIXEL_HEIGHT - (PBL_DISPLAY_HEIGHT / 1000 * 300)), FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)
    //#define  PHONE_BAT_POS GRect(PBL_DISPLAY_WIDTH - FONT_BAT_STR_PIXEL_WIDTH, 140, FONT_BAT_STR_PIXEL_WIDTH, FONT_BAT_PIXEL_HEIGHT)   // this works as expected, above does not - very confusing

#endif /* end of Round or rectangle */


/* for screen shots and font testing
#define DEBUG  // If set will update each second and use seconds as minutes for checking updates (not the best for screenshots)
#ifdef DEBUG
#define TICK_HANDLER_INTERVAL SECOND_UNIT
#endif // DEBUG

#define DEBUG_TIME
#define DEBUG_TIME_SCREENSHOT
// ensure quiet time and bluetooth disconnection info is shown
#ifndef quiet_time_is_active  // so not aplite
#define quiet_time_is_active() true  // DEBUG!
#else
#undef quiet_time_is_active  // See if we can force this to always be true
#define quiet_time_is_active() true  // DEBUG!
#endif
#define bluetooth_connection_service_peek() false  // DEBUG!
*/
