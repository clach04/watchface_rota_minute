/*
** Watchface Framework - Color or Mono
**
**    https://github.com/clach04/watchface_framework
**
** This file may not be needed. This is only needed if additional C code is required.
*/

#include <pebble.h>

#include "watchface.h"

#define hour_color time_color
#ifdef PBL_BW
    #define min_color hour_color
#else
    static GColor       min_color;
#endif /* PBL_BW */
int          config_min_color; // FIXME read on startup.
static Layer  *time_layer=NULL;

// FIXME location of date. either lower? or top/bottom right with newlines
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
        config_min_color = (int)t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Persisting minutes color: 0x%06x", config_min_color);
        persist_write_int(MESSAGE_KEY_MINUTES_COLOR, config_min_color);
        wrote_config = true;
        min_color = GColorFromHEX(config_min_color);
        // force paint?
        // layer_mark_dirty(time_layer);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "MINUTES COLOR DONE");
    }

    return wrote_config;
}

void hour_display_update_proc(Layer *layer, GContext* ctx)
{
    time_t    now=time(NULL);
    struct tm *tick_time=localtime(&now);
    char      hour_str[3]="12";

    GRect  bounds = layer_get_unobstructed_bounds(layer);
    GPoint center = grect_center_point(&bounds);

#ifdef DEBUG
       APP_LOG(APP_LOG_LEVEL_DEBUG, "%s() entry", __func__);
#endif /* DEBUG */

    graphics_context_set_text_color(ctx, hour_color);
    if (clock_is_24h_style() == true)
    {
        // 24h hour format
        strftime(hour_str, sizeof(hour_str), "%H", tick_time);
    }
    else
    {
        // 12 hour format
        unsigned short display_hour = tick_time->tm_hour % 12;
        display_hour = display_hour ? display_hour : 12;
        snprintf(hour_str, sizeof(hour_str), "%d", display_hour);
    }

    GSize text_size = graphics_text_layout_get_content_size(hour_str, time_font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);
    #define hour_h text_size.h
    #define hour_w text_size.w
    GRect hour_rect = GRect(center.x - (hour_w / 2), center.y - (hour_h / 2) - 7, hour_w, hour_h);
#ifdef DEBUG
       APP_LOG(APP_LOG_LEVEL_DEBUG, "text_size %d %d", text_size.w, text_size.h);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "center.x: %d, center.y %d", center.x, center.y);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "%d %d  - %d %d", hour_rect.origin.x, hour_rect.origin.y, hour_rect.size.w, hour_rect.size.h);
#endif /* DEBUG */

    graphics_draw_text(ctx, hour_str, time_font,
                       hour_rect,
                       GTextOverflowModeWordWrap,
                       GTextAlignmentCenter,
                       NULL);
    /*
       TODO Consider using bitmap font
       https://developer.pebble.com/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect
       and  graphics_context_set_compositing_mode()
     */
#ifndef NO_DATE
    /* Update the date only when the day changes */
    if (last_day != tick_time->tm_mday)
    {
        update_date(tick_time);
    }
#endif /* NO_DATE */
}

void minute_display_update_proc(Layer *layer, GContext* ctx)
{
    time_t    now = time(NULL);
    struct tm *t = localtime(&now);

#ifdef DEBUG
    unsigned int angle = t->tm_sec * 6; // Seconds for debug reasons
#else
    unsigned int angle = t->tm_min * 6;
#endif
    GRect        bounds = layer_get_unobstructed_bounds(layer);

    hour_display_update_proc(layer, ctx);

    // https://developer.pebble.com/docs/c/Graphics/Graphics_Context/
    //graphics_context_set_antialiased(ctx, true);
    //graphics_context_set_antialiased(ctx, false);
    graphics_context_set_stroke_color(ctx, min_color);
    graphics_context_set_fill_color(ctx, min_color);
    graphics_fill_radial(
        ctx,
        bounds,
        GOvalScaleModeFitCircle,
        7, /* inset, stroke width */
        DEG_TO_TRIGANGLE(0), /* angle_start */
        (TRIG_MAX_ANGLE / 360) * angle /* angle_end */
        );
}

void update_time()
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
        config_min_color = persist_read_int(MESSAGE_KEY_MINUTES_COLOR);
        APP_LOG(APP_LOG_LEVEL_INFO, "Read time color: %x", config_min_color);
        min_color = GColorFromHEX(config_min_color);
    }
    else
    {
        min_color = COLOR_FALLBACK(DEFAULT_TIME_MIN_COLOR, GColorBlack);
    }

    //time_font = fonts_get_system_font(FONT_SYSTEM_NAME);

    window_set_background_color(window, background_color);

    time_layer = layer_create(bounds);
    layer_set_update_proc(time_layer, minute_display_update_proc);
    layer_add_child(window_layer, time_layer);
}
