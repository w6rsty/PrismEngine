#type vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

// smooth step
float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * t * (3.0 - 2.0 * t);
}

void main() {
    vec2 Resolution = vec2(1920, 1080);
    // covert gl_FragCoord to vec2
    vec2 coord = (gl_FragCoord.xy * 2.0 - Resolution.xy) / Resolution.y;

    vec4 texColor = texture(u_Texture, v_TexCoord);
    float length = length(coord);
    float alpha = smoothstep(1.0, 0.0, length);
    texColor.a *= alpha;

    color = texColor;
}
