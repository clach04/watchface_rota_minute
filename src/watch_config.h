#pragma once

#define SETUP_TIME setup_time
#define CLEANUP_TIME cleanup_time
#define NO_TEXT_TIME_LAYER
#define DEFAULT_TIME_COLOR GColorBlack
#define DEFAULT_TIME_MIN_COLOR GColorBlue
#define DEFAULT_BACKGROUND_COLOR GColorWhite
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

#if defined(PBL_HEALTH)
    #define USE_HEALTH
    //#define UPDATE_HEALTH_ON_ACTIVITY  /* If not set, only updates step count display once per minute */
#endif /* PBL_HEALTH */

#define DRAW_BATTERY
#define DRAW_SMALL_BATTERY

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
    #define BT_POS GRect(0, 120, 144, 168) /* probably taller than really needed */
    #define DATE_POS GRect(0, 148, 125, 168) /* bottom of "g" in "Aug" starting to be cut off . Top RHS may be be better (see Capinion/stormtrooper/spawn */
    #ifdef DRAW_BATTERY
        #define BAT_POS GRect(5, 150, 144, 168)
    #else
        #define BAT_POS GRect(0, 140, 144, 168) /* probably taller than really needed */
    #endif /* DRAW_BATTERY */
#endif /* end of Round or rectangle */

/* for screen shots and font testing
#define DEBUG_TIME
#define DEBUG_TIME_SCREENSHOT
 */
