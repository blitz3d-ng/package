precision highp float;
precision highp int;

uniform mat4 bbProjMatrix;
uniform mat4 bbViewMatrix;
uniform mat4 bbWorldMatrix;

uniform sampler2D bbTexture[8];

#define FOG_NONE   0
#define FOG_LINEAR 1

struct BBLightData {
  mat4 TForm;
  vec4 Color;
} ;

layout(std140) uniform BBLightState {
  BBLightData Light[8];

  int LightsUsed;
} LS;

// must be mindful of alignment when ordering...
struct BBTextureState {
  mat4 TForm;
  int Blend,_0,_1,_2;
} ;

layout(std140) uniform BBRenderState {
  vec4 Ambient;
  vec4 BrushColor;
  vec4 FogColor;

  BBTextureState Texture[8];

  vec2 FogRange;

  int TexturesUsed;
  int UseVertexColor;
  float BrushShininess;
  int FullBright;
  int FogMode;
} RS;

struct BBPerVertex {
  vec3 Position;
  vec4 Color;
  vec3 Normal;
  vec2 TexCoord[8];

  float FogFactor;
};

mat4 rotationMatrix(vec3 axis, float angle){
  axis = normalize(axis);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;

  return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
              oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
              oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
              0.0,                                0.0,                                0.0,                                1.0);
}

float fogFactorLinear(
  const float dist,
  const float start,
  const float end
) {
  return 1.0 - clamp((end - dist) / (end - start), 0.0, 1.0);
}



#ifdef VERTEX
/**
 * VERTEX
 **/

layout(location = 0) in vec3 bbPosition;
layout(location = 1) in vec3 bbNormal;
layout(location = 2) in vec4 bbColor;
layout(location = 3) in vec2 bbTexCoord[2];

out BBPerVertex bbVertex;

void main() {
  mat4 bbModelViewMatrix = bbViewMatrix * bbWorldMatrix;
  mat4 bbModelViewProjectionMatrix=bbProjMatrix * bbModelViewMatrix;
  mat3 bbNormalMatrix=transpose(inverse(mat3(bbModelViewMatrix)));

  for( int i=0;i<RS.TexturesUsed;i++ ){
    bbVertex.TexCoord[i] = (RS.Texture[i].TForm * vec4(bbTexCoord[0], 0.0, 1.0)).xy;
  }

  vec4 bbMaterialColor;
  if( RS.UseVertexColor>0 ){
    bbMaterialColor = bbColor;
  }else{
    bbMaterialColor = RS.BrushColor;
  }

  if( RS.FullBright==0 ){
    // bbVertex.Position = vec3( bbModelViewMatrix*vec4( bbPosition,1.0 ) );
    bbVertex.Normal = normalize( bbNormalMatrix*bbNormal );

    vec4 Diffuse=vec4( 0.0 ),Specular=vec4( 0.0 );

    for( int i=0;i<LS.LightsUsed;i++ ){
      float nDotVP,nDotHV,pf;

      vec3 LightPos=normalize( mat3( bbViewMatrix*LS.Light[i].TForm*rotationMatrix( vec3(1.0,0.0,0.0), 1.5708 ) )*vec3(0.0,1.0,0.0) );
      vec3 halfVector = normalize( LightPos+vec3( 0.0,0.0,-1.0 ));

      nDotVP = max( 0.0,dot( bbVertex.Normal,LightPos ) );
      nDotHV = max( 0.0,dot( bbVertex.Normal,vec3( halfVector )));
      pf = pow( nDotHV,100.0 )*float(nDotVP!=0.0);

      Diffuse  += LS.Light[i].Color * nDotVP;
      Specular += LS.Light[i].Color * pf;
    }

    bbVertex.Color = RS.Ambient * bbMaterialColor +
                     Diffuse    * bbMaterialColor +
                     Specular   * vec4( 1.0 );
    bbVertex.Color = clamp( bbVertex.Color, 0.0, 1.0 );
  }else{
    bbVertex.Color = bbMaterialColor;
  }

  gl_Position = bbModelViewProjectionMatrix * vec4(bbPosition, 1.0);

  switch( RS.FogMode ){
  case FOG_NONE: break;
  case FOG_LINEAR: bbVertex.FogFactor=fogFactorLinear( length( gl_Position.xyz ),RS.FogRange.x,RS.FogRange.y );break;
  }
}
#endif

#ifdef FRAGMENT
/**
 * FRAGMENT
 **/

in BBPerVertex bbVertex;
out vec4 bbFragColor;

vec4 Blend( vec4 t0,sampler2D tex,int i ){
  vec4 t1=texture( tex,bbVertex.TexCoord[i] );
  switch( RS.Texture[i].Blend ){
  case 0:return t0;
  case 2:return t0*t1;
  case 3:return t0+t1;
  }
}

void main() {
  vec4 tex=vec4( 1.0,1.0,1.0,1.0 );

  // TODO; ES doesn't allow dynamic indexing...
  if( 0<RS.TexturesUsed ) tex=Blend( tex,bbTexture[0],0 );
  if( 1<RS.TexturesUsed ) tex=Blend( tex,bbTexture[1],1 );
  if( 2<RS.TexturesUsed ) tex=Blend( tex,bbTexture[2],2 );
  if( 3<RS.TexturesUsed ) tex=Blend( tex,bbTexture[3],3 );
  if( 4<RS.TexturesUsed ) tex=Blend( tex,bbTexture[4],4 );
  if( 5<RS.TexturesUsed ) tex=Blend( tex,bbTexture[5],5 );
  if( 6<RS.TexturesUsed ) tex=Blend( tex,bbTexture[6],6 );
  if( 7<RS.TexturesUsed ) tex=Blend( tex,bbTexture[7],7 );

  bbFragColor=bbVertex.Color * tex;

  if( RS.FogMode>0 ){
    bbFragColor=mix( bbFragColor,RS.FogColor,bbVertex.FogFactor );
  }
}
#endif
