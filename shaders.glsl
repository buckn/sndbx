#pragma sokol @ctype mat4 Mat4
#pragma sokol @ctype vec4 Vec4
#pragma sokol @ctype vec2 Vec2

/* quad vertex shader */
@vs vs

in vec4 position;
out vec2 square_pos;

uniform vertex_uniforms {
    mat4 model_matrix;
};

void main() {
    gl_Position = model_matrix * position;
    square_pos = position.xy;
}
@end

/* quad fragment shader */
@fs fs
in vec2 square_pos;
out vec4 frag_color;

uniform fragment_uniforms {
    vec4 color;
    vec2 scale;
    float radius;
};

void main() {
    frag_color = color;

    // border thickness
    float thc = 0.01f * (scale.x + scale.y) / 2.0;

    vec2 cir_center = vec2(sign(square_pos.x), sign(square_pos.y)) * (0.5 - radius);
    float alpha = 1.0;
    if (abs(square_pos.x) > (0.5f - radius) && abs(square_pos.y) > (0.5f - radius))
        alpha = smoothstep(radius, radius - thc, length(square_pos - cir_center));
    alpha = min(alpha, smoothstep(0.5f, 0.5f - thc, abs(square_pos.x)));
    alpha = min(alpha, smoothstep(0.5f, 0.5f - thc, abs(square_pos.y)));
    frag_color.w = alpha;
}
@end

/* quad shader program */
@program quad vs fs