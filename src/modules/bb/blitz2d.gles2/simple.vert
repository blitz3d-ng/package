attribute highp vec3 a_position;
uniform vec4 u_xywh;
uniform vec3 u_color;

varying lowp vec3 v_color;

void main() {
  gl_Position = vec4(a_position, 1.0);
  // gl_Position.x = (gl_Position.x / viewport.x) * 2.0 - 1.0;
  // gl_Position.y = (gl_Position.y / viewport.y) * 2.0 - 1.0;
  gl_Position.x = ((gl_Position.x * u_xywh.z + u_xywh.x) / 1242.0) * 2.0 - 1.0;
  gl_Position.y = ((2208.0 - (gl_Position.y * u_xywh.w + u_xywh.y)) / 2208.0) * 2.0 - 1.0;
  v_color = u_color;
}
