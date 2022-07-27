varying lowp vec3 v_color;
varying lowp vec2 v_texcoord;

uniform sampler2D tex;

void main() {
  gl_FragColor = vec4(v_color, 1.0) * texture2D(tex, v_texcoord);
}
