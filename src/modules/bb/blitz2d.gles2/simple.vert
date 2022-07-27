attribute highp vec3 a_position;
attribute highp vec2 a_texcoord;

uniform vec2 u_res;
uniform vec4 u_xywh;
uniform vec3 u_color;

varying lowp vec2 v_texcoord;
varying lowp vec3 v_color;

void main() {
  gl_Position = vec4(a_position, 1.0);

  gl_Position.x = ((gl_Position.x * u_xywh.z + u_xywh.x) / u_res.x) * 2.0 - 1.0;
  gl_Position.y = ((u_res.y - (gl_Position.y * u_xywh.w + u_xywh.y)) / u_res.y) * 2.0 - 1.0;

  v_texcoord = a_texcoord;
  v_color = u_color;
}
