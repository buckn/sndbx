#pragma sokol @ctype mat4 Mat4
#pragma sokol @ctype vec4 Vec4

/* quad vertex shader */
@vs vs

in vec4 position;
out vec4 color;

uniform vertex_uniforms {
    mat4 model_matrix;
    vec4 color0;
};

void main() {
    gl_Position = model_matrix * position;
    color = color0;
}
@end

/* quad fragment shader */
@fs fs
in vec4 color;
out vec4 frag_color;

void main() {
    frag_color = color;
}
@end

/* quad shader program */
@program quad vs fs