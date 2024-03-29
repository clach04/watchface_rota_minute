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

#define DRAW_BATTERY
#define DRAW_SMALL_BATTERY

//#define QUIET_TIME_IMAGE RESOURCE_ID_IMAGE_QUIET_TIME
//#define QUIET_TIME_IMAGE_GRECT GRect(20, 20, 20, 20)  // Example assumes a 20x20 image

#ifdef PBL_ROUND /* 180x180 */
/*TODO center/move right*/
    #define CLOCK_POS GRect(0, 52, 180, 180) /* probably taller than really needed */
    #define HEALTH_POS GRect(0, 40, 180, 180)
    #define BT_POS GRect(0, 100, 180, 180) /* probably taller than really needed */

    #define DATE_ALIGN GTextAlignmentCenter
    #define DATE_POS GRect(0, 120, 180, 180) /* probably taller than really needed */

    #define BAT_ALIGN GTextAlignmentCenter
    #ifdef DRAW_BATTERY
        #define BAT_POS GRect(85, 10, 180, 180) /* probably taller than really needed */
    #else
        #define BAT_POS GRect(0, 140, 180, 180) /* probably taller than really needed */
    #endif /* DRAW_BATTERY */

#else /* PBL_RECT 144x168*/
    #define CLOCK_POS GRect(0, 52, 144, 168) /* probably taller than really needed */
    #define HEALTH_POS GRect(0, 40, 144, 168)
    //#define BT_POS GRect(0, 100, 144, 168) /* probably taller than really needed */
    #define BT_POS GRect(0, 20, 144, 168) /* probably taller than really needed */
    //#define DATE_POS GRect(0, 148, 125, 168) /* bottom of "g" in "Aug" starting to be cut off . Top RHS may be be better (see Capinion/stormtrooper/spawn */
//#define DATE_FMT_STR "%a\n%b\n%d"
//#define MAX_DATE_STR "Thu\n00\nAug" /* if custom version of DATE_FMT_STR is set, MAX_DATE_STR  needs to be updated too */
//#define DATE_FMT_STR "%b\n%d"
//#define MAX_DATE_STR "00\nAug" /* if custom version of DATE_FMT_STR is set, MAX_DATE_STR  needs to be updated too */
#define DATE_FMT_STR "%a %b\n%d"
#define MAX_DATE_STR "Thu Aug\n00"

#define DATE_POS GRect(0, -5, 140, 168) /* probably taller than really needed */

    #ifdef DRAW_BATTERY
        #define BAT_POS GRect(5, 150, 144, 168)
    #else
        #define BAT_POS GRect(0, 140, 144, 168) /* probably taller than really needed */
    #endif /* DRAW_BATTERY */
#endif /* end of Round or rectangle */

/* for screen shots and font testing
#define DEBUG_TIME
#define DEBUG_TIME_SCREENSHOT
// ensure quiet time and bluetooth disconnection info is shown
#ifndef quiet_time_is_active  // so not aplite
#define quiet_time_is_active() true  // DEBUG!
#endif
#define bluetooth_connection_service_peek() false  // DEBUG!
*/
