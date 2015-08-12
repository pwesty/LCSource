#ifndef SE_INCL_SHADER_COMMON_H
#define SE_INCL_SHADER_COMMON_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

void DoSpecularLayer(const INDEX iSpecularTexture,const INDEX iSpecularColor, const BOOL bOpaque);
void DoReflectionLayer(const INDEX iReflectionTexture,const INDEX iReflectionColor, const BOOL bOpaque);

/* Const vertex registers used: */
/*
Shading:
	c0       - mProjection*mView
	c4.xyz   - vLightDir
	c4.w     - 765.01f
	c5       - colLight
	c6       - colAmbient
	c7.x     - 0.0f
	c7.y     - 1.0f
	c7.z     - 2.0f
	c7.w     - 0.5f

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
NormalMap:
	c8     - viewer position          
	c9     - model*light rgb & model a
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

Reflection:
	c8       - mObjToAbs
	c11.xyz  - vViewer

Detail:
	c12.x    - X tiling
	c12.y    - Y tiling

Specular:
	c13      - specular color
	c14      - mObjToView
	c17      - vLightObj

Fog:
	c18.xyz  - vFViewerObj
	c19.xyz  - vHDirObj
	c20.x    - fFogAddZ
	c20.y    - fFogAddH
	c20.z    - fog_fMulZ
	c20.w    - fog_fMulH

Haze:
	c18.xyz  - vViewerObj
	c19.xyzw - {0,0,0,0}
	c20.x    - fHazeAdd
	c20.y    - 0.0f
	c20.z    - haze_fMul
	c20.w    - 0.0f

Ska:
	c21+
*/

/* Temp vertex registers used: */
/*
	r0    - vertex position
	r1    - vertex normal
	r4.w  - light dot normal
*/
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
/* Normal Map
	r0    - Vertex Position
	r1    - Light Direction
*/
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

/* Input registers: */
/*
	v0    - Position  (use r0)
	v1    - Normals   (use r1)
	v2    - Indices
	v3    - Weights
	v4    - Diffuse
	v5    - TexCoord0
	v6    - TexCoord1
	v7    - TexCoord2
	v8    - TexCoord3
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	v9    - tangent
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
*/


/* Const pixel registers used: */
/*
	c7    - Fog color
*/


//안태훈 수정 시작	//(Modify Sam's Old Shaders)(0.1)
#define CALCULATE_SHADING_VP \
/* Normal dot light                 */               "dp3  r4.w,   r1,      c4       \n" \
/* ClampUp output of dp3 to 1.0f    */               "min  r4.w,   r4.w,    c7.y     \n" \
/* ClampDn output of dp3 to 0.0f    */               "max  r4.w,   r4.w,    c7.x     \n" \
/* Multiply dot with light color    */               "mul  r5.xyz, c5.xyz,  r4.www   \n" \
/* Add ambient                      */               "add  r5.xyz, r5.xyz,  c6.xyz   \n" \
/* Set color alpha as surface color */               "mov  r5.w,   c7.y              \n" \
/* Clamp output color to 1.0f       */               "mul  oD0,    r5,      c7.yyyy  \n"
//안태훈 수정 끝	//(Modify Sam's Old Shaders)(0.1)


#define STANDARD_OUTPUT_VP \
/* Transform vector position in screen space */      "m4x4 oPos, r0,   c0            \n" \
/* Copy texcoord from texture unit 0         */      "mov  oT0.xy, v5.xy             \n"

#define CALCULATE_REFLECTION_VP \
/* Get normal in absolute space */\
/* fN = mObjToAbs%nor */                             "m3x3 r8,     r1,      c8         \n"\
/* Reflect viewer around normal */\
/* const FLOAT fNV  = fNx*vViewer(1) + fNy*vViewer(2) + fNz*vViewer(3); */\
/* const FLOAT fRVx = vViewer(1) - 2*fNx*fNV;  */\
/* const FLOAT fRVy = vViewer(2) - 2*fNy*fNV; */\
/* const FLOAT fRVz = vViewer(3) - 2*fNz*fNV; */\
/* const FLOAT fNV  = ....                    */     "dp3  r11.w,  r8.xyz,  c11.xyz    \n"\
/* fN*fNV */                                         "mul  r10.xyz,r8.xyz,  r11.www    \n"\
/* fN*fNV*2 */                                       "mul  r10.xyz,r10.xyz, c7.zzz     \n"\
/* vViewer - (fN*fNV*2) */                           "sub  r9.xyz, c11.xyz, r10.xyz    \n"\
/* const FLOAT f1oFM = 0.5f / sqrt(2+2*fRVy); */\
/* 2+2*fRVy */                                       "mad  r8.w,   r9.y, c7.z,   c7.z  \n"\
/* 1.0f / sqrt(2+2*fRVy); */                         "rsq  r8.x,   r8.w                \n"\
/* 0.5f * (1.0f / sqrt(2+2*fRVy)); */                "mul  r8.y,   r8.x, c7.w          \n"\
/* ptcUVMap[ivx].s = fRVx*f1oFM +0.5f; */\
/* ptcUVMap[ivx].t = fRVz*f1oFM +0.5f; */\
/* NOTE: Replace oTX with your output tex unit */    "// mad  oT1.xy, r9.xz,r8.yy,  c7.ww \n"

#define CALCULATE_SPECULAR_VP \
/* Output specular color */ \
/* Result of dot is 1.0f or 0.0f */                  "slt  r3.w,   c7.x,    r4.w     \n"\
/* Mul specular color with specular aplha */         "mul  oD1,    c13,     r3.wwww  \n"\
/* */\
/* Reflect light vec around normal in object space */\
/* const FLOAT fNL = nor.nx*vLightObj(1) + nor.ny*vLightObj(2) +nor.nz*vLightObj(3);*/\
													 "dp3  r11.w,  v1.xyz,  c17.xyz  \n"\
/* const FLOAT fR = vLightObj - 2*nor*fNL; */\
/* nor*fNL */                                        "mul  r10.xyz,v1.xyz,  r11.www  \n"\
/* 2*nor*fNL */                                      "mul  r9.xyz, r10.xyz, c7.zzz   \n"\
/* vLightObj - 2*nor*fNL */                          "sub  r8.xyz, c17.xyz, r9.xyz   \n"\
/* transform the reflected vector to viewer space */ "m3x3 r7,     r8,      c14      \n"\
/* */\
/* map reflected vector to texture */\
/* const FLOAT f1oFM = 0.5f / sqrt(2+2*fRVz); */\
/* 2+2*fRVz */                                       "mad  r6.w,   r7.z, c7.z,  c7.z \n"\
/* (1.0f / sqrt(2+2*fRVz)) */                        "rsq  r5.w,   r6.w              \n"\
/* 0.5f * (1.0f / sqrt(2+2*fRVz)) */                 "mul  r4.w,   c7.w, r5.w        \n"\
/* ptcUVMap[ivx].s = fRVx*f1oFM +0.5f; */\
/* ptcUVMap[ivx].t = fRVy*f1oFM +0.5f; */\
/* NOTE: Replace oTX with your output tex unit */    "// mad  oTX.xy, r7.xy,r4.ww, c7.ww \n"

#define CACLULATE_FOG_VP \
/* const FLOAT fD = vtx * _vFViewerObj */            "dp3  r10.w,  r0.xyz,  c18.xyz  \n"\
/* const FLOAT fH = vtx * _vHDirObj */               "dp3  r11.w,  r0.xyz,  c19.xyz  \n"\
/* (fD+_fFogAddZ) */                                 "add  r9.w,   r10.w,   c20.x    \n"\
/* (fH+_fFogAddH) */                                 "add  r8.w,   r11.w,   c20.y    \n"\
/* NOTE: Replace oTX with your output tex unit */ \
/* tex.s = (fD+_fFogAddZ) * _fog_fMulZ */            "// mul  oTX.x,  r9.w,    c20.z \n"\
/* tex.t = (fH+_fFogAddH) * _fog_fMulH;*/            "// mul  oTX.y,  r8.w,    c20.w \n"

#define D3D_ENABLE_SPECULAR()
#define D3D_DISABLE_SPECULAR()
#define D3D_DISABLE_SPECULAR_PS

#endif  /* include-once check. */
