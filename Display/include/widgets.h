#include <lvgl.h>
#include "controller.h"

// Theme colors
lv_color_t C_BACKGROUND_1       = lv_color_hex(0x152122);
lv_color_t C_BACKGROUND_2       = lv_color_hex(0x00272B);
lv_color_t C_BACKGROUND_3       = lv_color_hex(0x00282C);
lv_color_t C_BACKGROUND_4       = lv_color_hex(0x003E44);
lv_color_t C_ACCENT     = lv_color_hex(0x344041);
lv_color_t C_ACCENT_2   = lv_color_hex(0x27979D);
lv_color_t C_BANNER     = lv_color_hex(0x000000);
lv_color_t C_WHITE      = lv_color_hex(0xffffff);

// Banner
lv_obj_t *banner;
lv_obj_t *banner_label;
// lv_obj_t *banner_image;

// Controls
lv_obj_t *controls_background;
lv_obj_t *left_colors_label;
lv_obj_t *left_colors_wheel;
lv_obj_t *right_colors_label;
lv_obj_t *right_colors_wheel;
lv_obj_t *linked_button_frame;
lv_obj_t *linked_button_label;
lv_obj_t *linked_button_icon;
lv_obj_t *brightness_label;
lv_obj_t *brightness_slider;
lv_obj_t *brightness_value_label;

// Effects
lv_obj_t *effects_background;
lv_obj_t *effects_label;
lv_obj_t *effects_button_solid_background;
lv_obj_t *effects_button_solid_icon;
lv_obj_t *effects_button_solid_label;
lv_obj_t *effects_button_breathe_background;
lv_obj_t *effects_button_breathe_icon;
lv_obj_t *effects_button_breathe_label;
lv_obj_t *effects_button_rainbow_background;
lv_obj_t *effects_button_rainbow_icon;
lv_obj_t *effects_button_rainbow_label;

// Styles
lv_style_t style_synced_enabled;
lv_style_t style_synced_disabled;

// Widget States
unsigned int colors_synced = 0;

uint8_t* get_hsv(lv_obj_t* cw);

// Callback function to handle the hue selection
static void colorwheel_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        // Update lighting
        lv_obj_t* cw = lv_event_get_target(e);
        int triggered_by_left = (cw == left_colors_wheel);
        lv_obj_t* opposite_cw = triggered_by_left ? right_colors_wheel : left_colors_wheel;

        uint8_t* hsv = get_hsv(cw);
        if (triggered_by_left)
        {
            update_lighting(1, colors_synced, hsv[0], hsv[1], hsv[2] * lv_slider_get_value(brightness_slider) / 255, 1);
        }
        else
        {
            update_lighting(colors_synced, 1, hsv[0], hsv[1], hsv[2] * lv_slider_get_value(brightness_slider) / 255, 1);
        }

        // Update display
        if (colors_synced) lv_colorwheel_set_hsv(opposite_cw, lv_colorwheel_get_hsv(cw));

        lv_obj_set_style_bg_grad_color(brightness_slider, lv_colorwheel_get_rgb(right_colors_wheel), LV_PART_INDICATOR);
    }
}

void brightness_slider_event_cb(lv_event_t * e)
{

    update_brightness(1, 0, get_hsv(left_colors_wheel)[2] * lv_slider_get_value(brightness_slider) / 255);
    update_brightness(0, 1, get_hsv(right_colors_wheel)[2] * lv_slider_get_value(brightness_slider) / 255);
}

void sync_btn_cb(lv_event_t* e) 
{ 
    colors_synced = !colors_synced;
    if (colors_synced) 
    {
        lv_colorwheel_set_hsv(right_colors_wheel, lv_colorwheel_get_hsv(left_colors_wheel));
        uint8_t* hsv = get_hsv(left_colors_wheel);
        update_lighting(0, 1, hsv[0], hsv[1], hsv[2], 1);
    }

}

void styles_init()
{
    lv_style_init(&style_synced_enabled);
    lv_style_set_bg_color(&style_synced_enabled, C_ACCENT);
    lv_style_set_shadow_color(&style_synced_enabled, C_ACCENT_2);
    lv_style_set_shadow_width(&style_synced_enabled, 10);
    lv_style_set_shadow_spread(&style_synced_enabled, 5);

    lv_style_init(&style_synced_disabled);
    lv_style_set_bg_color(&style_synced_disabled, C_ACCENT);
}

void widgets_init(lv_obj_t* screen)
{
    styles_init();

    lv_obj_set_style_bg_color(screen, C_BACKGROUND_2, LV_PART_MAIN);

    controls_background = lv_obj_create(screen);
    lv_obj_set_style_bg_color(controls_background, C_BACKGROUND_2, LV_PART_MAIN);
    lv_obj_set_style_border_opa(controls_background, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(controls_background, 0, LV_PART_MAIN);
    lv_obj_set_size(controls_background, 800, 320);
    lv_obj_align(controls_background, LV_ALIGN_TOP_LEFT, 0, 0);

    banner = lv_obj_create(screen);
    lv_obj_set_style_bg_color(banner, C_BANNER, LV_PART_MAIN);
    lv_obj_set_style_border_opa(banner, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(banner, 0, LV_PART_MAIN);
    lv_obj_set_size(banner, 800, 32);
    lv_obj_set_scrollbar_mode(banner, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(banner, LV_ALIGN_TOP_LEFT, 0, 0);

    banner_label = lv_label_create(screen);
    lv_label_set_text(banner_label, "HYUNDAI SONATA - FOOTWELL");
    lv_obj_set_style_text_color(banner_label, C_ACCENT_2, LV_PART_MAIN);
    lv_obj_set_style_text_font(banner_label, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_align_to(banner_label, banner, LV_ALIGN_LEFT_MID, 0, 0);

    right_colors_wheel = lv_colorwheel_create(screen, false);
    lv_obj_set_size(right_colors_wheel, 217, 217);
    lv_obj_set_style_arc_width(right_colors_wheel, 40, LV_PART_MAIN);
    lv_obj_align_to(right_colors_wheel, banner, LV_ALIGN_OUT_BOTTOM_RIGHT, -40, 64);
    lv_obj_add_event_cb(right_colors_wheel, colorwheel_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_style_pad_all(right_colors_wheel, 0, LV_PART_KNOB);
    lv_obj_set_style_bg_opa(right_colors_wheel, 0, LV_PART_KNOB);
    lv_obj_set_style_border_width(right_colors_wheel, 3, LV_PART_KNOB);
    lv_obj_set_style_border_color(right_colors_wheel, lv_color_hex(0xffffff), LV_PART_KNOB);

    right_colors_label = lv_label_create(screen);
    lv_label_set_text(right_colors_label, "RIGHT (Passenger)");
    lv_obj_set_style_text_color(right_colors_label, C_WHITE, LV_PART_MAIN);
    lv_obj_set_style_text_font(right_colors_label, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_align_to(right_colors_label, right_colors_wheel, LV_ALIGN_OUT_TOP_MID, 0, -16);

    left_colors_wheel = lv_colorwheel_create(screen, false);
    lv_obj_set_size(left_colors_wheel, 217, 217);
    lv_obj_set_style_arc_width(left_colors_wheel, 40, LV_PART_MAIN);
    lv_obj_align_to(left_colors_wheel, banner, LV_ALIGN_OUT_BOTTOM_LEFT, 40, 64);
    lv_obj_add_event_cb(left_colors_wheel, colorwheel_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_style_pad_all(left_colors_wheel, 0, LV_PART_KNOB);
    lv_obj_set_style_bg_opa(left_colors_wheel, 0, LV_PART_KNOB);
    lv_obj_set_style_border_width(left_colors_wheel, 3, LV_PART_KNOB);
    lv_obj_set_style_border_color(left_colors_wheel, lv_color_hex(0xffffff), LV_PART_KNOB);

    left_colors_label = lv_label_create(screen);
    lv_label_set_text(left_colors_label, "LEFT (Driver)");
    lv_obj_set_style_text_color(left_colors_label, C_WHITE, LV_PART_MAIN);
    lv_obj_set_style_text_font(left_colors_label, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_align_to(left_colors_label, left_colors_wheel, LV_ALIGN_OUT_TOP_MID, 0, -16);

    linked_button_frame = lv_btn_create(controls_background);
    lv_obj_set_size(linked_button_frame, 164, 92);
    lv_obj_set_style_radius(linked_button_frame, 20, LV_PART_MAIN);
    lv_obj_align_to(linked_button_frame, controls_background, LV_ALIGN_TOP_MID, 0, 64);
    lv_obj_add_flag(linked_button_frame, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_style(linked_button_frame, &style_synced_enabled, LV_STATE_CHECKED);
    lv_obj_add_style(linked_button_frame, &style_synced_disabled, LV_PART_MAIN);
    lv_obj_add_event_cb(linked_button_frame, sync_btn_cb, LV_EVENT_CLICKED, NULL);

    linked_button_label = lv_label_create(linked_button_frame);
    lv_label_set_text(linked_button_label, "SYNCED");
    lv_obj_set_style_text_font(linked_button_label, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_align_to(linked_button_label, linked_button_frame, LV_ALIGN_BOTTOM_MID, 0, 0);
    // linked_button_icon

    brightness_label = lv_label_create(controls_background);
    lv_label_set_text(brightness_label, "MASTER BRIGHTNESS");
    lv_obj_set_style_text_color(brightness_label, lv_color_hex(0xdddddd), LV_PART_MAIN);
    lv_obj_align_to(brightness_label, controls_background, LV_ALIGN_TOP_MID, 0, 212);

    brightness_slider = lv_slider_create(controls_background);
    lv_slider_set_range(brightness_slider, 0, 255);
    lv_obj_set_style_bg_color(brightness_slider, lv_color_hex(0x111111), LV_PART_MAIN);
    lv_obj_set_style_height(brightness_slider, 24, LV_PART_MAIN);
    lv_obj_set_style_width(brightness_slider, 256, LV_PART_MAIN);

    lv_obj_set_style_bg_color(brightness_slider, lv_color_hex(0x000000), LV_PART_INDICATOR);
    lv_obj_set_style_bg_grad_color(brightness_slider, lv_colorwheel_get_rgb(right_colors_wheel), LV_PART_INDICATOR);
    lv_obj_set_style_bg_grad_dir(brightness_slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);

    lv_obj_set_style_radius(brightness_slider, 4, LV_PART_KNOB);
    lv_obj_set_style_bg_color(brightness_slider, lv_color_hex(0xffffff), LV_PART_KNOB);
    lv_obj_set_style_pad_ver(brightness_slider, 6, LV_PART_KNOB);
    lv_obj_set_style_pad_hor(brightness_slider, -2, LV_PART_KNOB);

    lv_obj_align_to(brightness_slider, controls_background, LV_ALIGN_TOP_MID, 0, 236);

    lv_obj_add_event_cb(brightness_slider, brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    effects_background = lv_obj_create(screen);
    lv_obj_set_style_bg_color(effects_background, C_BACKGROUND_1, LV_PART_MAIN);
    lv_obj_set_style_border_opa(effects_background, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(effects_background, 0, LV_PART_MAIN);
    lv_obj_set_size(effects_background, 800, 160);
    lv_obj_align_to(effects_background, controls_background, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
}

uint8_t* get_hsv(lv_obj_t* cw)
{
    static uint8_t result[3];
    lv_color_hsv_t hsv = lv_colorwheel_get_hsv(cw);
    result[0] = hsv.h * 0.708333;
    result[1] = hsv.s * 2.55;
    result[2] = hsv.v * 2.55;

    return result;
}