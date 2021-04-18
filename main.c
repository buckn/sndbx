#include "renderer.h"

static struct {
    float player_rotation;
} game;

void init(void) {
    /* I mean this is what it is by default but
       never hurts to be explicit */
    game.player_rotation = 0.0f;

    render_init();
}

void frame(void) {
    draw_start();

    draw_rect_start();
    draw_rect_scale(1.0f, 0.6f);
    draw_rect_pivot(0.2f, 0.0f);
    draw_rect_rot(game.player_rotation);
    draw_rect();

    draw_end();
}

void cleanup(void) {
    sg_shutdown();
}

void event(const sapp_event *ev) {
    switch (ev->type) {
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
        .sample_count = 8,
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
