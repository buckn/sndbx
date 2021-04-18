#pragma sokol @ctype mat4 Mat4
#pragma sokol @ctype vec4 Vec4

/* quad vertex shader */
@vs vs

in vec4 position;

uniform vertex_uniforms {
    mat4 model_matrix;
};

void main() {
    gl_Position = model_matrix * position;
}
@end

/* quad fragment shader */
@fs fs
out vec4 frag_color;

uniform fragment_uniforms {
    vec4 color;
    float radius;
};

void main() {
    frag_color = color;
}
@end

/* quad shader program */
@program quad vs fs