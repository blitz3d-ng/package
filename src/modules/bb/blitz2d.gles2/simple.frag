varying lowp vec3 v_color;
varying lowp vec2 v_texcoord;

uniform int u_texenabled;
uniform sampler2D u_tex;

void main() {
  if( u_texenabled==1 ){
    gl_FragColor = texture2D( u_tex,v_texcoord );
  }else{
    gl_FragColor = vec4( v_color,1.0 );
  }
}
