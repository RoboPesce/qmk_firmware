#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_KEYREACTIVE_ENABLED)
RGB_MATRIX_EFFECT(sakura)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

typedef struct
{ 
    // The hue for this key. We can modulate it to change the color.
    uint8_t hue;
    // The saturation for this key. This can be modulated as well to change the color. 
    // We also set it to white when the key is pressed.
    uint8_t saturation;
    // This represents the brightness of the key. We decay it over time.
    uint8_t value;
    // when the next refresh (light up) will happen for this key
    uint16_t refresh_timer; 
} KeyState;

static KeyState keystates[RGB_MATRIX_LED_COUNT];
static int16_t last_pressed_tick = 0;



bool sakura(effect_params_t* params)
{

    // Some utility functions

    inline uint32_t interval(void) 
    {
        return 500 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16) + random8_max(100);
    }

    uint16_t time = g_rgb_timer;

    inline uint16_t create_new_timer(void)
    {
        //something like this
        //return time + random() something rgb_matrix_config.speed
        return time + interval(); // for now, just do one second
    }

    // reset timers to force a refresh for all keys
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (params->init) for (uint8_t i = led_min; i < led_max; ++i) keystates[i].refresh_timer = 0;

    // For all the recently pressed keys, set their saturation to white and reset their timer
    for (int8_t i = g_last_hit_tracker.count - 1; i >= 0; --i) 
    {
        if (g_last_hit_tracker.tick[i] <= last_pressed_tick) break;

        uint8_t index = g_last_hit_tracker.index[i];
        keystates[index].saturation = 0; // minimum saturation (white) = 0
        keystates[index].refresh_timer = create_new_timer();
    }
    last_pressed_tick = g_last_hit_tracker.tick[g_last_hit_tracker.count - 1];

    for (uint8_t i = led_min; i < led_max; ++i) 
    {
        RGB_MATRIX_TEST_LED_FLAGS();

        // Decrement value (linearly or according to some function)
        keystates[i].value = qsub8(keystates[i].value, qadd8(rgb_matrix_config.speed, 1));

        // Check if timer is up; if so set value back up to max, pick a new color, and reset pressed bit
        if (keystates[i].refresh_timer <= time)
        {
           // Pink ranges
            keystates[i].hue = random8_min_max(198, 223);
            keystates[i].saturation = random8_min_max(150, 255);

            keystates[i].value = 255;
            keystates[i].refresh_timer = create_new_timer();
        }

        HSV hsv = { keystates[i].hue, keystates[i].saturation, keystates[i].value };
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // RGB_MATRIX_FRAMEBUFFER_EFFECTS