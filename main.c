#include "renderer.h"
#include "mat.h"

static struct {
    /* ... */

    /* input */
    bool keys_down[SAPP_MAX_KEYCODES];

    /* player */
    Vec2 pos, vel;
} CliState;

void init(void) {
    /* I mean this is what it is by default but
       never hurts to be explicit */
    game.player_rotation = 0.0f;

    render_init();
}

void frame(void) {
    frame_start();

    //player
    draw_start();
    draw_scale(1.0f, 1.0f);
    draw_pivot(0.2f, 0.0f);
    draw_rot(game.player_rotation);
    draw_rad(0.1f);
    draw();
    
    //static rect
    draw_start();
    draw_scale(1.0f, 1.0f);
    draw_pivot(0.2f, 0.0f);
    draw_rot(game.player_rotation);
    draw_rad(0.1f);
    draw_color(128, 128, 128, 128)
    draw();

    draw_end();
}

void cleanup(void) {
    sg_shutdown();
}

void event(const sapp_event *ev) {
    switch (ev->type) {
    case (SAPP_EVENTTYPE_KEY_DOWN):;
        state.keys_down[ev->key_code] = true;
        #ifndef NDEBUG
        if (ev->key_code == SAPP_KEYCODE_ESCAPE)
            sapp_request_quit();
        #endif
        break;
    case SAPP_EVENTTYPE_KEY_UP:;
        state.keys_down[ev->key_code] = false;
        break;
    case SAPP_EVENTTYPE_MOUSE_UP:;
    case SAPP_EVENTTYPE_MOUSE_DOWN:;
    case SAPP_EVENTTYPE_MOUSE_MOVE:;
        Vec2 mp = mouse_pos_world(ev->mouse_x, ev->mouse_y);
        game.player_rotation = atan2f(mp.y, mp.x);
        break;
    default: ;
    };
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    return (sapp_desc){
        .sample_count = 32,
        .high_dpi = true,
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 1920,
        .height = 1080,
        .window_title = "Quad (sokol-app)",
    };
}
