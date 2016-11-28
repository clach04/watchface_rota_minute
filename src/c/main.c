/*
** Rota Minute Watchface - based on watchface_framework
**
**    https://github.com/clach04/watchface_framework
**
*/

#include <pebble.h>

#include "watchface.h"

#define digits_color time_color
#ifdef PBL_BW
    #define arc_color digits_color
#else
    static GColor       arc_color;
#endif /* PBL_BW */
static int config_arc_color;
static Layer *time_layer=NULL;
static bool draw_hour_as_text=true;

// FIXME test BT image. Bluetooth text works.

bool custom_in_recv_handler(DictionaryIterator *iterator, void *context)
{
    Tuple *t=NULL;
    bool wrote_config=false;

    /* NOTE if new entries are added, increase MAX_MESSAGE_SIZE_OUT macro */

    APP_LOG(APP_LOG_LEVEL_DEBUG, "custom_in_recv_handler() called");
    t = dict_find(iterator, MESSAGE_KEY_MINUTES_COLOR);
    if (t)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got MESSAGE_KEY_MINUTES_COLOR");
        config_arc_color = (int)t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Persisting arc color: 0x%06x", config_arc_color);
        persist_write_int(MESSAGE_KEY_MINUTES_COLOR, config_arc_color);
        wrote_config = true;
        arc_color = GColorFromHEX(config_arc_color);
        // force paint?
        // layer_mark_dirty(time_layer);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "MESSAGE_KEY_MINUTES_COLOR DONE");
    }

    // TODO consider wrapping below in a macro (or maybe function) to avoid code duplication
    // one for bool, another for int
    t = dict_find(iterator, MESSAGE_KEY_HOUR_AS_TEXT);
    if (t)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got MESSAGE_KEY_HOUR_AS_TEXT");
        draw_hour_as_text = (bool)t->value->int32;  /* this doesn't feel correct... */
        APP_LOG(APP_LOG_LEVEL_INFO, "Persisting draw_hour_as_text: %d", (int) draw_hour_as_text);
        persist_write_bool(MESSAGE_KEY_HOUR_AS_TEXT, draw_hour_as_text);
        wrote_config = true;
        // force paint?
        // layer_mark_dirty(time_layer);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "MINUTES COLOR DONE");
    }

    return wrote_config;
}

static void digits_display_update_proc(Layer *layer, GContext* ctx, GRect bounds, char *digits_str)
{
    GPoint center = grect_center_point(&bounds);

#ifdef DEBUG
       APP_LOG(APP_LOG_LEVEL_DEBUG, "%s() entry", __func__);
#endif /* DEBUG */

    graphics_context_set_text_color(ctx, digits_color);

    GSize text_size = graphics_text_layout_get_content_size(digits_str, time_font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);
    #define digits_height text_size.h
    #define digits_width text_size.w
    GRect hour_rect = GRect(center.x - (digits_width / 2), center.y - (digits_height / 2) - 7, digits_width, digits_height);
#ifdef DEBUG
       APP_LOG(APP_LOG_LEVEL_DEBUG, "text_size %d %d", text_size.w, text_size.h);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "center.x: %d, center.y %d", center.x, center.y);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "%d %d  - %d %d", hour_rect.origin.x, hour_rect.origin.y, hour_rect.size.w, hour_rect.size.h);
#endif /* DEBUG */

    graphics_draw_text(ctx, digits_str, time_font,
                       hour_rect,
                       GTextOverflowModeWordWrap,
                       GTextAlignmentCenter,
                       NULL);
    /*
       TODO Consider using bitmap font
       https://developer.pebble.com/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect
       and  graphics_context_set_compositing_mode()
     */
}

static void draw_arc_display_update_proc(Layer *layer, GContext* ctx, GRect bounds, int angle)
{
    // https://developer.pebble.com/docs/c/Graphics/Graphics_Context/
    //graphics_context_set_antialiased(ctx, true);
    //graphics_context_set_antialiased(ctx, false);
    graphics_context_set_stroke_color(ctx, arc_color);
    graphics_context_set_fill_color(ctx, arc_color);
    graphics_fill_radial(
        ctx,
        bounds,
        GOvalScaleModeFitCircle,
        7, /* inset, stroke width */
        DEG_TO_TRIGANGLE(0), /* angle_start */
        (TRIG_MAX_ANGLE / 360) * angle /* angle_end */
        );
}

static void update_time_update_proc(Layer *layer, GContext* ctx)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char digits_array[3]="12";


    unsigned int angle=0;
    GRect bounds = layer_get_unobstructed_bounds(layer);

    //APP_LOG(APP_LOG_LEVEL_DEBUG, "text_size t->tm_hour=%d t->tm_min=%d", t->tm_hour, t->tm_min);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "angle before: %d", angle);
    if (draw_hour_as_text)
    {
        if (clock_is_24h_style() == true)
        {
            // 24h hour format
            strftime(digits_array, sizeof(digits_array), "%H", t);
        }
        else
        {
            // 12 hour format
            unsigned short display_hour = t->tm_hour % 12;
            display_hour = display_hour ? display_hour : 12;
            snprintf(digits_array, sizeof(digits_array), "%d", display_hour);
        }
        // Angle of minutes is like a minute hand on a traditional analog watch
        angle = t->tm_min * 6;  // 360 / 60
    }
    else
    {
        // determine 12 hour format for hour
        unsigned short display_hour = t->tm_hour % 12;
        display_hour = display_hour ? display_hour : 12;

        // Angle of hour is like an hour hand on a traditional analog watch
        angle = display_hour * 30;  // 360 / 12
        strftime(digits_array, sizeof(digits_array), "%M", t);
    }
#ifdef DEBUG
    angle = t->tm_sec * 6; // Seconds for debug reasons
#endif
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "angle after: %d", angle);

    digits_display_update_proc(layer, ctx, bounds, digits_array);
#ifndef NO_DATE
    /* Update the date only when the day changes */
    if (last_day != t->tm_mday)
    {
        update_date(t);
    }
#endif /* NO_DATE */
    draw_arc_display_update_proc(layer, ctx, bounds, angle);
}

void update_time(struct tm *tick_time)
{
    layer_mark_dirty(time_layer);
}

void cleanup_time()
{
    layer_destroy(time_layer);
}

void setup_time(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    if (persist_exists(MESSAGE_KEY_MINUTES_COLOR))
    {
        config_arc_color = persist_read_int(MESSAGE_KEY_MINUTES_COLOR);
        APP_LOG(APP_LOG_LEVEL_INFO, "Read arc color: %x", config_arc_color);
        arc_color = GColorFromHEX(config_arc_color);
    }
    else
    {
        arc_color = COLOR_FALLBACK(DEFAULT_TIME_ARC_COLOR, GColorBlack);
    }

    if (persist_exists(MESSAGE_KEY_HOUR_AS_TEXT))
    {
        draw_hour_as_text = persist_read_bool(MESSAGE_KEY_HOUR_AS_TEXT);
        APP_LOG(APP_LOG_LEVEL_INFO, "Read draw_hour_as_text: %d", (int) draw_hour_as_text);
    }
    // TODO wipe_config() needs to handle MESSAGE_KEY_MINUTES_COLOR and MESSAGE_KEY_HOUR_AS_TEXT

    //time_font = fonts_get_system_font(FONT_SYSTEM_NAME);

    window_set_background_color(window, background_color);

    time_layer = layer_create(bounds);
    layer_set_update_proc(time_layer, update_time_update_proc);
    layer_add_child(window_layer, time_layer);
}
