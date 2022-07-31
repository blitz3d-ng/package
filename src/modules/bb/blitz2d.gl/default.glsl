precision highp float;

uniform vec2 u_res;
uniform vec4 u_xywh;
uniform vec3 u_color;
uniform vec2 u_texscale;
uniform int u_texenabled;
uniform sampler2D u_tex;

struct BBPerVertex{
  vec3 color;
  vec2 texcoord;
};

#ifdef VERTEX
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texcoord;

out BBPerVertex v;

void main() {
  vec2 xy=u_xywh.xy,wh=u_xywh.zw;

  vec2 v_position = (a_position * wh) + xy;

  v_position *= 2.0;

  v_position.y = u_res.y - v_position.y;
  v_position /= u_res;

  v_position = v_position * 2.0 - 1.0;

  v.color = u_color;
  v.texcoord = a_texcoord*u_texscale;

  gl_Position = vec4(v_position, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT

in BBPerVertex v;
out vec4 bbFragColor;

void main() {
  if( u_texenabled==1 ){
    bbFragColor = texture( u_tex,v.texcoord ) * vec4( v.color,1.0 );
  }else{
    bbFragColor = vec4( v.color,1.0 );
  }
}
#endif
