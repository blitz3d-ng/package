uniform lowp vec4 bbAmbientLight;

varying lowp vec4 bbColor;

void main() {
  gl_FragColor = bbAmbientLight * bbColor;
}
