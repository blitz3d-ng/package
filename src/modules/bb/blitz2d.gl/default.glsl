precision highp float;

uniform sampler2D u_tex;

// must be mindful of alignment when ordering...
layout(std140) uniform BBRenderState {
  uniform vec4 xywh;
  uniform vec2 res;
  uniform vec2 texscale;
  uniform vec3 color;
  uniform int texenabled;
  uniform float scale;
} RS;

struct BBPerVertex{
  vec3 color;
  vec2 texcoord;
};

#ifdef VERTEX
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texcoord;

out BBPerVertex v;

void main() {
  vec2 xy=RS.xywh.xy,wh=RS.xywh.zw;

  vec2 v_position = (a_position * wh) + xy;

  v_position *= RS.scale;

  v_position.y = RS.res.y - v_position.y;
  v_position /= RS.res;

  v_position = v_position * 2.0 - 1.0;

  v.color = RS.color;
  v.texcoord = a_texcoord*RS.texscale;

  // gl_PointSize = 1.0; // TODO: figure out if there's a performance penalty when not used...
  gl_Position = vec4(v_position, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT

in BBPerVertex v;
out vec4 bbFragColor;

void main() {
  if( RS.texenabled==1 ){
    bbFragColor = texture( u_tex,v.texcoord ) * vec4( v.color,1.0 );
  }else{
    bbFragColor = vec4( v.color,1.0 );
  }
}
#endif
