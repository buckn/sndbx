#pragma sokol @ctype mat4 Mat4
#pragma sokol @ctype vec4 Vec4

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
    float radius;
};

void main() {
	frag_color = color;
    frag_color.w = smoothstep(radius, radius - 0.01f, length(square_pos));
}
@end

/* quad shader program */
@program quad vs fs