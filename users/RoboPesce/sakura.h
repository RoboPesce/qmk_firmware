#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_KEYREACTIVE_ENABLED)
RGB_MATRIX_EFFECT(sakura)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool sakura(effect_params_t* params)
{
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    /*
    // Clear the buffer and turn off all lights
    if (params->init) 
    {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
    }
    */

    for (uint8_t i = led_min; i < led_max; i++) 
    {
        rgb_matrix_set_color(i, random8_min_max(230, 255), random8_min_max(185, 205), random8_min_max(210, 255));
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // RGB_MATRIX_FRAMEBUFFER_EFFECTS