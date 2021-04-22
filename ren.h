#ifdef _WIN32
/* non-constant aggregate initializers */
#pragma warning(disable:4204)
/* anonymous struct/unions */
#pragma warning(disable:4201)
/* padding bytes are added */
#pragma warning(disable:4324)
#pragma warning(disable:4820)
/* unknown preprocessor macros */
#pragma warning(disable:4668)
/* assignment within conditional expression */
#pragma warning(disable:4706)
/* initialization using address of automatic variable */
#pragma warning(disable:4221)
#endif

/*
    Sokol Headers
*/
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "shaders.glsl.h"

static struct {
    /* renderer */
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;
    Mat4 cam;
} _rcx;

void render_init(void) {
    #define WORLD_SCALE (0.2f)
    float y_scale = WORLD_SCALE * (sapp_widthf() / sapp_heightf());
    _rcx.cam = scale4x4(vec2(WORLD_SCALE, y_scale));

    sg_setup(&(sg_desc){
        .context = sapp_sgcontext()
    });

    /* a vertex buffer */
    float vertices[] = {
        // positions
        -0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
    };
    _rcx.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices),
        .label = "quad-vertices"
    });

    /* an index buffer with 2 triangles */
    uint16_t indices[] = { 0, 1, 2,  0, 2, 3 };
    _rcx.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc){
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "quad-indices"
    });

    /* a shader (use separate shader sources here */
    sg_shader shd = sg_make_shader(quad_shader_desc(sg_query_backend()));

    /* a pipeline state object */
    _rcx.pip = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = shd,
        .index_type = SG_INDEXTYPE_UINT16,
        .layout = {
            .attrs = {
                [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
            }
        },
        .colors[0].blend = (sg_blend_state) {
            .enabled = true,
            .src_factor_rgb = SG_BLENDFACTOR_ONE, 
            .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, 
            .src_factor_alpha = SG_BLENDFACTOR_ONE, 
            .dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        },
        .label = "quad-pipeline"
    });

    /* default pass action */
    _rcx.pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .value={1.0f, 0.98f, 0.98f, 1.0f } }
    };
}

Vec2 mouse_pos_world(float x, float y) {
    Vec4 mouse_screen = vec4(x, y, 0.0f, 1.0f);
    /* get it in 0..2.0f */
    mouse_screen.x /= sapp_widthf() / 2.0f;
    mouse_screen.y /= sapp_heightf() / 2.0f;
    /* get it in -1.0..1.0f */
    mouse_screen.x -= 1.0f;
    mouse_screen.y -= 1.0f;

    Vec4 mouse_world = mul4x44(invert4x4(_rcx.cam), mouse_screen);
    return vec2(mouse_world.x, mouse_world.y);
}

void frame_start(void) {
    sg_begin_default_pass(&_rcx.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(_rcx.pip);
    sg_apply_bindings(&_rcx.bind);
}

typedef struct {
    float start;
    Vec2 pos, scale, pivot;
    float rot;
    Vec4 color;
    float rad;
} _rcx_Rect;
static _rcx_Rect _rcx_wip;

void draw_start(void) {
    _rcx_wip.start = true;
    _rcx_wip.color.w = 1.0f;
    _rcx_wip.scale = vec2(1.0f, 1.0f);
}
void draw_pos(float x, float y) {
    _rcx_wip.pos.x = x;
    _rcx_wip.pos.y = y;
}
void draw_scale(float x, float y) {
    _rcx_wip.scale.x = x;
    _rcx_wip.scale.y = y;
}
void draw_pivot(float x, float y) {
    _rcx_wip.pivot.x = x;
    _rcx_wip.pivot.y = y;
}
void draw_rot(float rot) {
    _rcx_wip.rot = rot;
}
void draw_dir(Vec2 dir) {
    _rcx_wip.rot = rot_vec2(dir);
}
void draw_color(float r, float g, float b, float a) {
    _rcx_wip.color.x = r;
    _rcx_wip.color.y = g;
    _rcx_wip.color.z = b;
    _rcx_wip.color.w = a;
}

void draw_rad(float rad_i) {
    _rcx_wip.rad = rad_i;
}

void draw() {
    assert(_rcx_wip.start);

    Mat4 model = _rcx.cam;
    model = mul4x4(model, translate4x4(_rcx_wip.pos));
    model = mul4x4(model, rotate_z_4x4(_rcx_wip.rot));
    model = mul4x4(model, translate4x4(_rcx_wip.pivot));
    model = mul4x4(model, scale4x4(_rcx_wip.scale));
    /* upload vertex uniforms */
    vertex_uniforms_t vs_params = {
        .model_matrix = model,
    };
    sg_apply_uniforms(SG_SHADERSTAGE_VS,
                      SLOT_vertex_uniforms,
                      &SG_RANGE(vs_params));

    /* upload fragment uniforms */
    fragment_uniforms_t fs_params = {
        .color = _rcx_wip.color,
        .radius = _rcx_wip.rad,
        .scale = _rcx_wip.scale,
    };
    sg_apply_uniforms(SG_SHADERSTAGE_FS,
                      SLOT_fragment_uniforms,
                      &SG_RANGE(fs_params));

    sg_draw(0, 6, 1);

    _rcx_wip = (_rcx_Rect) {0};
}

void draw_end(void) {
    sg_end_pass();
    sg_commit();
}
