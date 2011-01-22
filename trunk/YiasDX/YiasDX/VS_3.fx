sampler2D g_samSrcColor;

texture alpha2Tex;
texture stage2Tex;

texture alpha3Tex;
texture stage3Tex;

texture selTex;
texture baseAlpha;

texture baseTex;


sampler Sampler1_1 = sampler_state { texture = <alpha2Tex> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};
sampler Sampler1_2 = sampler_state { texture = <stage2Tex> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};

sampler Sampler2_1 = sampler_state { texture = <alpha3Tex> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};
sampler Sampler2_2 = sampler_state { texture = <stage3Tex> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};

sampler Sampler0 = sampler_state { texture = <baseTex> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};

sampler selSamp = sampler_state {
  texture = <selTex>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = wrap;
  AddressV = wrap;
};

sampler baseSamp = sampler_state {
  texture = <baseAlpha>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = wrap;
  AddressV = wrap;
};

float radius;
float3 mouse;
float3 click;
bool drawSel;
bool texEnable;
float brushType;
float sqrt2;
bool drawHL;
float hlDist;

float4x4 viewProj;

struct PixelInput
{
  float4 pos : POSITION0;
  float2 Tex : TEXCOORD0;
  float2 Tex2 : TEXCOORD1;
  float3 Tex3 : TEXCOORD2;
};

struct VertexInput
{
  float4 pos : POSITION0;
  float2 tex1 : TEXCOORD0;
  float2 tex2 : TEXCOORD1;
};

PixelInput VShader(VertexInput input)
{
  PixelInput ret = (PixelInput)0;
  ret.pos = mul(float4(input.pos.xyz, 1.0f), viewProj);
  ret.Tex = input.tex1;
  ret.Tex2 = input.tex2;
  ret.Tex3 = input.pos.xyz;
  return ret;
}

float4 MyShader(PixelInput pInput) : COLOR0
{
    float4 Color;
    float4 aColor;
    float4 stage;
    float4 aColor2;
    float4 stage2;
    float tmp;
    stage = tex2D(Sampler1_2, pInput.Tex.xy);
    aColor = tex2D(Sampler1_1, pInput.Tex2.xy);
    stage2 = tex2D(Sampler2_2, pInput.Tex.xy);
    Color = tex2D(Sampler0, pInput.Tex.xy);
    
    float4 oneminus1 = 1.0 - aColor.b;
    float4 oneminus2 = 1.0 - aColor.g;

    float4 r1 = aColor.b * stage + oneminus1 * Color;
    float4 ret = aColor.g * stage2 + oneminus2 * r1;
    
    float dx = pInput.Tex3.x - mouse.x;
    float dz = pInput.Tex3.z - mouse.z;
    
    float tot = sqrt(dx * dx + dz * dz);
    if((abs(dx) < radius && abs(dz) < radius && brushType == 1) || (tot <= radius && brushType == 0) || (tot <= radius && !texEnable))
    {
      float2 texco = float2(dx / (radius * 2) + 0.5f, dz / (radius * 2) + 0.5f);
      float4 white;
      if(brushType == 0 || !texEnable)
       white = tex2D(selSamp, texco.xy);
      else
        white.a = 0.5;
      white = float4(1 - ret.r, 1 - ret.g, 1 - ret.b, white.a);
      
      if(white.a > 0.2f)
        return white;
      ret = white.a * white + (1.0f - white.a) * ret;
      if(abs(pInput.Tex3.x) % 33.333333125 < 0.1 || abs(pInput.Tex3.z) % 33.333333125 < 0.1)
      {
        ret = float4(1, 0, 0, 1);
      }
      
     if(abs(pInput.Tex3.y) % hlDist < 0.5 && drawHL)
     {
       ret.a = 1;
       ret.r = ret.g = ret.b = 0;
     }
      return ret;
    }
    
    if(drawSel)
    {
      dx = pInput.Tex3.x - click.x;
      dz = pInput.Tex3.z - click.z;
      tot = sqrt(dx * dx + dz * dz);
      if(tot <= radius)
      {
        float4 white = float4(1.0f, 1.0f, 1.0f, 1.0f);
        white.a = 0.5f;
        ret = white.a * white + (1.0f - white.a) * ret;
      }
    }
    if(abs(pInput.Tex3.x) % 33.333333125 < 0.1 || abs(pInput.Tex3.z) % 33.333333125 < 0.1)
      {
        ret = float4(1, 0, 0, 1);
      }
    float h = abs(pInput.Tex3.y);
     h = h % hlDist;
     h = abs(h);
     if(h < 0.5 && drawHL)
     {
       ret.a = 1;
       ret.r = ret.g = ret.b = 0;
     }
    return ret;
}


technique EntryPoint
{
    pass p1
    {
        VertexShader = compile vs_3_0 VShader();
        PixelShader = compile ps_3_0 MyShader();
    }

}
