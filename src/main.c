#include "pebble.h"


static Window *s_main_window;
static Layer  *s_minute_display_layer, *s_hour_display_layer;

static GPath *s_minute_segment_path, *s_hour_segment_path;

static void minute_display_update_proc(Layer *layer, GContext* ctx)
{
    time_t    now = time(NULL);
    struct tm *t = localtime(&now);

#ifdef DEBUG
    unsigned int angle = t->tm_sec * 6; // Seconds for debug reasons
#else
    unsigned int angle = t->tm_min * 6;
#endif
    GRect        bounds = layer_get_bounds(layer);


    // https://developer.pebble.com/docs/c/Graphics/Graphics_Context/
    //graphics_context_set_antialiased(ctx, true);
    //graphics_context_set_antialiased(ctx, false);
    graphics_context_set_stroke_color(ctx, GColorRed);
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_fill_radial(
        ctx,
        bounds,
        GOvalScaleModeFitCircle,
        7, /* inset, stroke width */
        DEG_TO_TRIGANGLE(0), /* angle_start */
        (TRIG_MAX_ANGLE / 360) * angle /* angle_end */
        );
}

static void hour_display_update_proc(Layer *layer, GContext* ctx)
{
    time_t    now=time(NULL);
    struct tm *tick_time=localtime(&now);
    char      hour_str[3]="12";

    GRect  bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);
    GFont  font=fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);

    graphics_context_set_text_color(ctx, GColorWhite);
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
        //sprintf(hour_str, "%d", display_hour);
        snprintf(hour_str, sizeof(hour_str), "%d", display_hour);
        /*
        strftime(hour_str, sizeof(hour_str), "%I", tick_time);
        if (hour_str[0] == '0' || hour_str[0] == ' ')
        {
            memmove(&hour_str[0], &hour_str[1], sizeof(hour_str) - 1); // remove leading character (really byte)
        }
        */
    }

    GSize text_size = graphics_text_layout_get_content_size(hour_str, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);
    #define hour_h text_size.h
    #define hour_w text_size.w
    GRect hour_rect = GRect(center.x - (hour_w / 2), center.y - (hour_h / 2) - 7, hour_w, hour_h);
    /*
       APP_LOG(APP_LOG_LEVEL_DEBUG, "text_size %d %d", text_size.w, text_size.h);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "%s() entry", __func__);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "center.x: %d, center.y %d", center.x, center.y);
       APP_LOG(APP_LOG_LEVEL_DEBUG, "%d %d  - %d %d", hour_rect.origin.x, hour_rect.origin.y, hour_rect.size.w, hour_rect.size.h);
     */

    graphics_draw_text(ctx, hour_str, font,
                       hour_rect,
                       //bounds,
                       GTextOverflowModeWordWrap,
                       GTextAlignmentCenter,
                       NULL);
    /*
       TODO Consider using bitmap font
       https://developer.pebble.com/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect
       and  graphics_context_set_compositing_mode()
     */
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed)
{
    layer_mark_dirty(s_hour_display_layer);
    layer_mark_dirty(s_minute_display_layer);
}

static void main_window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    window_set_background_color(s_main_window, GColorBlack);

    s_minute_display_layer = layer_create(bounds);
    layer_set_update_proc(s_minute_display_layer, minute_display_update_proc);

    s_hour_display_layer = layer_create(bounds);
    layer_set_update_proc(s_hour_display_layer, hour_display_update_proc);
    layer_add_child(window_layer, s_hour_display_layer);
    layer_add_child(window_layer, s_minute_display_layer);
}

static void main_window_unload(Window *window)
{
    gpath_destroy(s_minute_segment_path);
    gpath_destroy(s_hour_segment_path);

    layer_destroy(s_minute_display_layer);
    layer_destroy(s_hour_display_layer);
}

static void init()
{
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers)
    {
        .load = main_window_load,
        .unload = main_window_unload,
    });
    window_stack_push(s_main_window, true);

#ifdef DEBUG
    tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick); // for debug reasons
#else
    tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
#endif
}

static void deinit()
{
    tick_timer_service_unsubscribe();
    window_destroy(s_main_window);
}

int main()
{
    init();
    app_event_loop();
    deinit();
}
