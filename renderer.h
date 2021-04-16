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

#include <math.h>
#include <assert.h>

#define vec4(x, y, z, w) ((Vec4) {x,y,z,w})
typedef union {
    float nums[4];
    struct { float x, y, z, w; };
} Vec4;

#define vec2(x, y) ((Vec2) {x, y})
typedef union {
    float nums[2];
    struct { float x, y; };
} Vec2;

typedef struct {
    float nums[4][4];
} Mat4;

Mat4 scale4x4(Vec2 s) {
    return (Mat4) {
         s.x, 0.0f, 0.0f, 0.0f,
        0.0f,  s.y, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4 rotate_z_4x4(float angle) {
    return (Mat4) {
        cosf(angle), -sinf(angle), 0.0f, 0.0f,
        sinf(angle),  cosf(angle), 0.0f, 0.0f,
               0.0f,        0.0f, 1.0f, 0.0f,
               0.0f,        0.0f, 0.0f, 1.0f,
    };
}

Mat4 translate4x4(Vec2 t) {
    return (Mat4) {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
         t.x,  t.y, 0.0f, 1.0f,
    };
}

Vec4 mul4x44(Mat4 m, Vec4 v) {
    Vec4 res;
    for(int x = 0; x < 4; ++x) {
        float sum = 0;
        for(int y = 0; y < 4; ++y)
            sum += m.nums[y][x] * v.nums[y];

        res.nums[x] = sum;
    }
    return res;
}

Mat4 mul4x4(Mat4 a, Mat4 b) {
    Mat4 out = {0};
    int k, r, c;
    for (c = 0; c < 4; ++c)
        for (r = 0; r < 4; ++r) {
            out.nums[c][r] = 0.0f;
            for (k = 0; k < 4; ++k)
                out.nums[c][r] += a.nums[k][r] * b.nums[c][k];
        }
    return out;
}

Mat4 invert4x4(Mat4 a) {
    float s[6], c[6];
    s[0] = a.nums[0][0]*a.nums[1][1] - a.nums[1][0]*a.nums[0][1];
    s[1] = a.nums[0][0]*a.nums[1][2] - a.nums[1][0]*a.nums[0][2];
    s[2] = a.nums[0][0]*a.nums[1][3] - a.nums[1][0]*a.nums[0][3];
    s[3] = a.nums[0][1]*a.nums[1][2] - a.nums[1][1]*a.nums[0][2];
    s[4] = a.nums[0][1]*a.nums[1][3] - a.nums[1][1]*a.nums[0][3];
    s[5] = a.nums[0][2]*a.nums[1][3] - a.nums[1][2]*a.nums[0][3];

    c[0] = a.nums[2][0]*a.nums[3][1] - a.nums[3][0]*a.nums[2][1];
    c[1] = a.nums[2][0]*a.nums[3][2] - a.nums[3][0]*a.nums[2][2];
    c[2] = a.nums[2][0]*a.nums[3][3] - a.nums[3][0]*a.nums[2][3];
    c[3] = a.nums[2][1]*a.nums[3][2] - a.nums[3][1]*a.nums[2][2];
    c[4] = a.nums[2][1]*a.nums[3][3] - a.nums[3][1]*a.nums[2][3];
    c[5] = a.nums[2][2]*a.nums[3][3] - a.nums[3][2]*a.nums[2][3];
    
    /* Assumes it is invertible */
    float idet = 1.0f/( s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0] );
    
    Mat4 res;
    res.nums[0][0] = ( a.nums[1][1] * c[5] - a.nums[1][2] * c[4] + a.nums[1][3] * c[3]) * idet;
    res.nums[0][1] = (-a.nums[0][1] * c[5] + a.nums[0][2] * c[4] - a.nums[0][3] * c[3]) * idet;
    res.nums[0][2] = ( a.nums[3][1] * s[5] - a.nums[3][2] * s[4] + a.nums[3][3] * s[3]) * idet;
    res.nums[0][3] = (-a.nums[2][1] * s[5] + a.nums[2][2] * s[4] - a.nums[2][3] * s[3]) * idet;

    res.nums[1][0] = (-a.nums[1][0] * c[5] + a.nums[1][2] * c[2] - a.nums[1][3] * c[1]) * idet;
    res.nums[1][1] = ( a.nums[0][0] * c[5] - a.nums[0][2] * c[2] + a.nums[0][3] * c[1]) * idet;
    res.nums[1][2] = (-a.nums[3][0] * s[5] + a.nums[3][2] * s[2] - a.nums[3][3] * s[1]) * idet;
    res.nums[1][3] = ( a.nums[2][0] * s[5] - a.nums[2][2] * s[2] + a.nums[2][3] * s[1]) * idet;

    res.nums[2][0] = ( a.nums[1][0] * c[4] - a.nums[1][1] * c[2] + a.nums[1][3] * c[0]) * idet;
    res.nums[2][1] = (-a.nums[0][0] * c[4] + a.nums[0][1] * c[2] - a.nums[0][3] * c[0]) * idet;
    res.nums[2][2] = ( a.nums[3][0] * s[4] - a.nums[3][1] * s[2] + a.nums[3][3] * s[0]) * idet;
    res.nums[2][3] = (-a.nums[2][0] * s[4] + a.nums[2][1] * s[2] - a.nums[2][3] * s[0]) * idet;

    res.nums[3][0] = (-a.nums[1][0] * c[3] + a.nums[1][1] * c[1] - a.nums[1][2] * c[0]) * idet;
    res.nums[3][1] = ( a.nums[0][0] * c[3] - a.nums[0][1] * c[1] + a.nums[0][2] * c[0]) * idet;
    res.nums[3][2] = (-a.nums[3][0] * s[3] + a.nums[3][1] * s[1] - a.nums[3][2] * s[0]) * idet;
    res.nums[3][3] = ( a.nums[2][0] * s[3] - a.nums[2][1] * s[1] + a.nums[2][2] * s[0]) * idet;
    return res;
}

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
    __dbgui_setup(sapp_sample_count());

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

void draw_start(void) {
    sg_begin_default_pass(&_rcx.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(_rcx.pip);
    sg_apply_bindings(&_rcx.bind);
}

typedef struct {
    float start;
    Vec2 pos, scale, pivot;
    float rot;
    Vec4 color;
} _rcx_Rect;
static _rcx_Rect _rcx_wip_rect;

void draw_rect_start(void) {
    _rcx_wip_rect.start = true;
    _rcx_wip_rect.color.w = 1.0f;
    _rcx_wip_rect.scale = vec2(1.0f, 1.0f);
}
void draw_rect_pos(float x, float y) {
    _rcx_wip_rect.pos.x = x;
    _rcx_wip_rect.pos.y = y;
}
void draw_rect_scale(float x, float y) {
    _rcx_wip_rect.scale.x = x;
    _rcx_wip_rect.scale.y = y;
}
void draw_rect_pivot(float x, float y) {
    _rcx_wip_rect.pivot.x = x;
    _rcx_wip_rect.pivot.y = y;
}
void draw_rect_rot(float rot) {
    _rcx_wip_rect.rot = rot;
}
void draw_rect_color(float x, float y, float z, float w) {
    _rcx_wip_rect.color.x = x;
    _rcx_wip_rect.color.y = y;
    _rcx_wip_rect.color.z = z;
    _rcx_wip_rect.color.w = w;
}

void draw_rect() {
    assert(_rcx_wip_rect.start);

    Mat4 model = _rcx.cam;
    model = mul4x4(model, translate4x4(_rcx_wip_rect.pos));
    model = mul4x4(model, rotate_z_4x4(_rcx_wip_rect.rot));
    model = mul4x4(model, translate4x4(_rcx_wip_rect.pivot));
    model = mul4x4(model, scale4x4(_rcx_wip_rect.scale));
    vertex_uniforms_t vs_params = {
        .model_matrix = model,
        .color0 = _rcx_wip_rect.color,
    };
    sg_apply_uniforms(SG_SHADERSTAGE_VS,
                      SLOT_vertex_uniforms,
                      &SG_RANGE(vs_params));
    sg_draw(0, 6, 1);

    _rcx_wip_rect = (_rcx_Rect) {0};
}

void draw_end(void) {
    __dbgui_draw();
    sg_end_pass();
    sg_commit();
}

