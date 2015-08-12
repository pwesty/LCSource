
//안태훈이 추가한 파일.

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#ifndef __SHADER_CODE__
#define __SAHDER_CODE__

static char strPixelProgramPrefix14[] = 
	"ps.1.4                         \n"
	;

static char strPixelProgramPrefix11[] =
	"ps.1.1							\n"
	;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)



//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//--------------- Default BOne-Blend Shader For fog, No shade, No Calculate Normal ---//
//--- Normal Map Type, Object Space based ---//
//--- Input ---------------------------------//
//--- v0     - vertex position            ---//
//--- Output --------------------------------//
//--- r0     - vertex position            ---//
//--- 상수설정은 기본과 동일하게 간다.    ---//
static char strNoWeights_OnlyPosition[] =
	"vs.1.1                         \n"  // vertex shader version
	"mov  r0,     v0                \n"  // move vertex position to temp register r0
	;//command slot : 1

// Base vertex shader that uses one weight per vertex
static char strOneWeight_OnlyPosition[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices
	
	// First weight 
	"mov  a0.x,   r3.x                      \n"
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position bone matrix
	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
	;//command slot : 6

// Base vertex shader that uses two weight per vertex
static char strTwoWeights_OnlyPosition[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"sub  r2.w,   c7.y,   v2.z              \n"

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix

	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mad  r0.xyz, r4.xyz,  r2.www, r0.xyz   \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
	;//command slot : 13

// Base vertex shader that uses three weight per vertex
static char strThreeWeights_OnlyPosition[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"add  r2.x,   v2.z,    v2.y             \n"
	"sub  r2.w,   c7.y,   r2.x              \n"  

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mad  r0.xyz, r4.xyz,  v2.yyy, r0.xyz   \n"  // 

	// Third weight
	"mov  a0.x,   r3.z                      \n"  // move index of third matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mad  r0.xyz, r4.xyz,  r2.www, r0.xyz   \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
	;//command slot : 19

// Base vertex shader that uses three weight per vertex
static char strFourWeights_OnlyPosition[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"add  r2.x,   v2.z,    v2.y             \n"
	"add  r2.y,   r2.x,    v2.x             \n"  
	"sub  r2.w,   c7.y,    r2.y             \n"  

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mad  r0.xyz, r4.xyz,  v2.yyy, r0.xyz   \n"  // 

	// Third weight
	"mov  a0.x,   r3.z                      \n"  // move index of third matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mad  r0.xyz, r4.xyz,  v2.xxx, r0.xyz   \n"  // 

	// Fourth weight
	"mov  a0.x,   r3.w                      \n"  // move index of fourth matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mad  r0.xyz, r4.xyz,  r2.www, r0       \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
	;//command slot : 25



//--------------- Default Bone-Blend Shader --------------//
//--- Normal Map Type, Object Space based ---//
//--- Input ---------------------------------//
//--- v0     - vertex position            ---//
//--- v1.xyz - vertex normal              ---//
//--- Output --------------------------------//
//--- r0     - vertex position            ---//
//--- r1     - vertex normal              ---//
//--- 상수설정은 기본과 동일하게 간다.    ---//
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
// Empty vertex shader
static char strNoWeights[] =
	"vs.1.1                         \n"  // vertex shader version
	"mov  r0,     v0                \n"  // move vertex position to temp register r0
	"mov  r1,     v1                \n"  // move vertex normal to temp register r1
	;//command slot : 2

// Base vertex shader that uses one weight per vertex
static char strOneWeight[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices
	
	// First weight 
	"mov  a0.x,   r3.x                      \n"
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position bone matrix
	"m3x3 r1,     v1, c[a0.x + 21]          \n"  // transform vertex normal bone matrix
	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set vertex normal w as 1.0f (NOT NEEDED!)
	;//command slot : 9

// Base vertex shader that uses two weight per vertex
static char strTwoWeights[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"sub  r2.w,   c7.y,   v2.z              \n"

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r1,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix

	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength
	"mul  r1.xyz, r1.xyz,  v2.zzz           \n"  // multiply vertex normal with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mad  r0.xyz, r4.xyz,  r2.www, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  r2.www, r1.xyz   \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set vertex normal w as 1.0f (NOT NEEDED!)
	;//command slot : 21

// Base vertex shader that uses three weight per vertex
static char strThreeWeights[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"add  r2.x,   v2.z,    v2.y             \n"
	"sub  r2.w,   c7.y,   r2.x              \n"  

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r1,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength
	"mul  r1.xyz, r1.xyz,  v2.zzz           \n"  // multiply vertex normal with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mad  r0.xyz, r4.xyz,  v2.yyy, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  v2.yyy, r1.xyz   \n"  // 

	// Third weight
	"mov  a0.x,   r3.z                      \n"  // move index of third matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mad  r0.xyz, r4.xyz,  r2.www, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  r2.www, r1.xyz   \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set vertex normal w as 1.0f (NOT NEEDED!)
	;//command slot : 31

// Base vertex shader that uses three weight per vertex
static char strFourWeights[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"add  r2.x,   v2.z,    v2.y             \n"
	"add  r2.y,   r2.x,    v2.x             \n"  
	"sub  r2.w,   c7.y,    r2.y             \n"  

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r1,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength
	"mul  r1.xyz, r1.xyz,  v2.zzz           \n"  // multiply vertex normal with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mad  r0.xyz, r4.xyz,  v2.yyy, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  v2.yyy, r1.xyz   \n"  // 

	// Third weight
	"mov  a0.x,   r3.z                      \n"  // move index of third matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mad  r0.xyz, r4.xyz,  v2.xxx, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  v2.xxx, r1.xyz   \n"  // 

	// Fourth weight
	"mov  a0.x,   r3.w                      \n"  // move index of fourth matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     v1, c[a0.x + 21]          \n"  // transform vertex normal with bone matrix
	"mad  r0.xyz, r4.xyz,  r2.www, r0       \n"  // 
	"mad  r1.xyz, r5.xyz,  r2.www, r1       \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set vertex normal w as 1.0f (NOT NEEDED!)
	;//command slot : 41

static char strFogVP[] = 
	"dp3  r10.w,  r0.xyz,  c18.xyz  \n" // const FLOAT fD = vtx * _vFViewerObj;
	"dp3  r11.w,  r0.xyz,  c19.xyz  \n" // const FLOAT fH = vtx * _vHDirObj
	"add  r9.w,   r10.w,   c20.x    \n" // (fD+_fFogAddZ)
	"add  r8.w,   r11.w,   c20.y    \n" // (fH+_fFogAddH)
	// NOTE: Replace oTX with your output tex unit
	// "mul  oTX.x,  r9.w,    c20.z \n" // tex.s = (fD+_fFogAddZ) * _fog_fMulZ;
	// "mul  oTX.y,  r8.w,    c20.w \n" // tex.t = (fH+_fFogAddH) * _fog_fMulH;
	;//command slot : 4


//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//--------------- Object Space Normal Map Shader --------------//
//--- Normal Map Type, Object Space based ---//
//--- Input ---------------------------------//
//--- v0     - vertex position            ---//
//--- c4.xyz - light direction            ---//
//--- Output --------------------------------//
//--- r0     - vertex position            ---//
//--- r1     - light direction            ---//
//--- 상수설정은 기본과 동일하게 간다.    ---//

// Empty vertex shader
static char strNoWeights_NMObjectSpace[] =
	"vs.1.1									\n"  // vertex shader version
	"mov  r0,		v0						\n"  // move vertex position to temp register r0
	"mov  r1.xyz,	c4.xyz					\n"  // move light direction to temp register r1
//	"mov  r1.w,   c7.y                      \n"  // set w as 1.0f (NOT NEEDED!)
	;// command slot -> 2

// Base vertex shader that uses one weight per vertex
static char strOneWeight_NMObjectSpace[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices
	
	// First weight 
	"mov  a0.x,   r3.x                      \n"
	"m4x3 r0,     v0,     c[a0.x + 21]      \n"  // transform vertex position bone matrix
	"mov	r2.xyz,	c4.xyz				    \n"
	"m3x3 r1.xyz, r2, c[a0.x + 21]	        \n"

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set w as 1.0f (NOT NEEDED!)
	;// command slot -> 10

// Base vertex shader that uses two weight per vertex
static char strTwoWeights_NMObjectSpace[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"sub  r2.w,   c7.y,   v2.z              \n"

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0,    c[a0.x + 21]       \n"  // transform vertex position with bone matrix
	"mov	r2.xyz,	c4.xyz				    \n"
	"m3x3 r1.xyz, r2, c[a0.x + 21]          \n"

	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength
	"mul  r1.xyz, r1.xyz,  v2.zzz           \n"

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     r2, c[a0.x + 21]		    \n"

	"mad  r0.xyz, r4.xyz,  r2.www, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  r2.www, r1.xyz   \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set w as 1.0f (NOT NEEDED!)
	;// command slot -> 22

// Base vertex shader that uses three weight per vertex
static char strThreeWeights_NMObjectSpace[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"add  r2.x,   v2.z,    v2.y             \n"
	"sub  r2.w,   c7.y,   r2.x              \n"  

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mov	r2.xyz,	c4.xyz				    \n"
	"m3x3 r1,     r2, c[a0.x + 21]		    \n"

	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength
	"mul  r1.xyz, r1.xyz,  v2.zzz           \n"

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     r2, c[a0.x + 21]		    \n"

	"mad  r0.xyz, r4.xyz,  v2.yyy, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  v2.yyy, r1.xyz   \n"  // 

	// Third weight
	"mov  a0.x,   r3.z                      \n"  // move index of third matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     r2, c[a0.x + 21]          \n"

	"mad  r0.xyz, r4.xyz,  r2.www, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  r2.www, r1.xyz   \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set w as 1.0f (NOT NEEDED!)
	;// command slot -> 32

// Base vertex shader that uses three weight per vertex
static char strFourWeights_NMObjectSpace[] =
	"vs.1.1                                 \n"  // vertex shader version
	"mul  r3,     v3.zyxw, c4.wwww          \n"  // extract matrix indices

	// Calculate last weight strength
	"add  r2.x,   v2.z,    v2.y             \n"
	"add  r2.y,   r2.x,    v2.x             \n"  
	"sub  r2.w,   c7.y,    r2.y             \n"  

	// First weight
	"mov  a0.x,   r3.x                      \n"  // move index of first matrix into address register
	"m4x3 r0,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"mov	r2.xyz,	c4.xyz				    \n"  // 한명령에서 쓰일 수 있는 상수 레지스터 수의 제한때문.
	"m3x3 r1,     r2, c[a0.x + 21]          \n"

	"mul  r0.xyz, r0.xyz,  v2.zzz           \n"  // multiply vertex position with weight strength
	"mul  r1.xyz, r1.xyz,  v2.zzz           \n"  // multiply vertex normal with weight strength

	// Second weight
	"mov  a0.x,   r3.y                      \n"  // move index of second matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     r2, c[a0.x + 21]          \n"

	"mad  r0.xyz, r4.xyz,  v2.yyy, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  v2.yyy, r1.xyz   \n"  // 

	// Third weight
	"mov  a0.x,   r3.z                      \n"  // move index of third matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     r2, c[a0.x + 21]          \n"

	"mad  r0.xyz, r4.xyz,  v2.xxx, r0.xyz   \n"  // 
	"mad  r1.xyz, r5.xyz,  v2.xxx, r1.xyz   \n"  // 

	// Fourth weight
	"mov  a0.x,   r3.w                      \n"  // move index of fourth matrix into address register
	"m4x3 r4,     v0, c[a0.x + 21]          \n"  // transform vertex position with bone matrix
	"m3x3 r5,     r2, c[a0.x + 21]          \n"

	"mad  r0.xyz, r4.xyz,  r2.www, r0       \n"  // 
	"mad  r1.xyz, r5.xyz,  r2.www, r1       \n"  // 

	"mov  r0.w,   c7.y                      \n"  // set vertex position w as 1.0f
//	"mov  r1.w,   c7.y                      \n"  // set w as 1.0f (NOT NEEDED!)
	;// command slot -> 42

//--------------- Tagent Space Normal Map Shader Using Tagent --------------//
//--- Normal Map Type, Tagent Space based ---//
//--- Input ---------------------------------//
//--- v0     - vertex position            ---//
//--- v1     - vertex normal              ---//
//--- v9.xyz - vertex tangent             ---//
//--- v9.w   - vertex binormal direction  ---//
//--- c4.xyz - light direction            ---//
//--- Output --------------------------------//
//--- r0     - vertex position            ---//
//--- r1.xyz - light direction            ---//
//--- r6.xyz - tangent                    ---//
//--- r7.xyz - binormal                   ---//
//--- r8.xyz - normal                     ---//
//--- 상수설정은 기본과 동일하게 간다.    ---//
//tangent를 사용한다.
//binormal = normal X binormal

// Empty vertex shader
static char strNoWeights_NMTangentSpaceT[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mov  r0,      v0                           \n"  // move vertex position to temp register r0
	"mov  r8.xyz,  v1.xyz                       \n"  // move vertex normal to temp register
	"mov  r6.xyz,  v9.xyz                       \n"  // move vertex tangent to temp register

	///make binormal from normal & tangent
	"mul  r4.xyz,  r8.zxy,  r6.yzx              \n"  // normal X tangent 1/2
	"mad  r7.xyz,  r8.yzx,  r6.zxy,  -r4.xyz    \n"  // normal X tangent 2/2
	"mul  r7.xyz,  r7.xyz,  v9.www              \n"  // fix binormal
	"dp3  r7.w,    r7.xyz,  r7.xyz              \n"  // normalize the binormal
	"rsq  r7.w,    r7.w                         \n"
	"mul  r7.xyz,  r7.xyz,  r7.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world space -> tangent space transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 12

// Base vertex shader that uses one weight per vertex
static char strOneWeight_NMTangentSpaceT[] =
	"                                           \n"  // 
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices
	
	/// First weight 
	"mov  a0.x,    r3.x                         \n"
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r6.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	///make binormal from normal & tangent
	"mul  r4.xyz,  r8.zxy,  r6.yzx              \n"  // normal X tangent 1/2
	"mad  r7.xyz,  r8.yzx,  r6.zxy,  r4.xyz     \n"  // normal X tangent 2/2
	"mul  r7.xyz,  r7.xyz,  v9.www              \n"  // fix binormal 
	"dp3  r7.w,    r7.xyz,  r7.xyz              \n"  // normalize the binormal
	"rsq  r7.w,    r7.w                         \n"
	"mul  r7.xyz,  r7.xyz,  r7.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 21

// Base vertex shader that uses two weight per vertex
static char strTwoWeights_NMTangentSpaceT[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices

	/// Calculate last weight strength
	"sub  r2.w,    c7.y,    v2.z                \n"

	/// First weight
	"mov  a0.x,    r3.x                         \n"  // move index of first matrix into address register
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r6.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mul  r0.xyz,  r0.xyz,  v2.zzz              \n"  // multiply vertex position with weight strength
	"mul  r8.xyz,  r8.xyz,  v2.zzz              \n"  // multiply vertex normal with weight strength
	"mul  r6.xyz,  r6.xyz,  v2.zzz              \n"  // multiply vertex tangent with weight strength

	/// Second weight
	"mov  a0.x,    r3.y                         \n"  // move index of second matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  r2.www,  r0.xyz     \n"  // blend vertex position
	"mad  r8.xyz,  r11.xyz, r2.www,  r8.xyz     \n"  // blend vertex normal
	"mad  r6.xyz,  r9.xyz,  r2.www,  r6.xyz     \n"  // blend vertex tangent

	///make binormal from normal & tangent
	"mul  r4.xyz,  r8.zxy,  r6.yzx              \n"  // normal X tangent 1/2
	"mad  r7.xyz,  r8.yzx,  r6.zxy,  r4.xyz     \n"  // normal X tangent 2/2
	"mul  r7.xyz,  r7.xyz,  v9.www              \n"  // fix binormal 
	"dp3  r7.w,    r7.xyz,  r7.xyz              \n"  // normalize the binormal
	"rsq  r7.w,    r7.w                         \n"
	"mul  r7.xyz,  r7.xyz,  r7.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 38

// Base vertex shader that uses three weight per vertex
static char strThreeWeights_NMTangentSpaceT[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices

	/// Calculate last weight strength
	"add  r2.x,    v2.z,    v2.y                \n"
	"sub  r2.w,    c7.y,    r2.x                \n"  

	/// First weight
	"mov  a0.x,    r3.x                         \n"  // move index of first matrix into address register
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r6.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mul  r0.xyz,  r0.xyz,  v2.zzz              \n"  // multiply vertex position with weight strength
	"mul  r8.xyz,  r8.xyz,  v2.zzz              \n"
	"mul  r6.xyz,  r6.xyz,  v2.zzz              \n"

	/// Second weight
	"mov  a0.x,    r3.y                         \n"  // move index of second matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  v2.yyy,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.yyy,  r8.xyz     \n"  // 
	"mad  r6.xyz,  r9.xyz,  v2.yyy,  r6.xyz     \n"  // 

	/// Third weight
	"mov  a0.x,    r3.z                         \n"  // move index of third matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  r2.www,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.www,  r8.xyz     \n"  // 
	"mad  r6.xyz,  r9.xyz,  v2.www,  r6.xyz     \n"  // 
	
	///make binormal from normal & tangent
	"mul  r4.xyz,  r8.zxy,  r6.yzx              \n"  // normal X tangent 1/2
	"mad  r7.xyz,  r8.yzx,  r6.zxy,  r4.xyz     \n"  // normal X tangent 2/2
	"mul  r7.xyz,  r7.xyz,  v9.www              \n"  // fix binormal 
	"dp3  r7.w,    r7.xyz,  r7.xyz              \n"  // normalize the binormal
	"rsq  r7.w,    r7.w                         \n"
	"mul  r7.xyz,  r7.xyz,  r7.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 52

// Base vertex shader that uses three weight per vertex
static char strFourWeights_NMTangentSpaceT[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices

	/// Calculate last weight strength
	"add  r2.x,    v2.z,    v2.y                \n"
	"add  r2.y,    r2.x,    v2.x                \n"  
	"sub  r2.w,    c7.y,    r2.y                \n"  

	/// First weight
	"mov  a0.x,    r3.x                         \n"  // move index of first matrix into address register
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r6.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mul  r0.xyz,  r0.xyz,  v2.zzz              \n"  // multiply vertex position with weight strength
	"mul  r8.xyz,  r8.xyz,  v2.zzz              \n"  // multiply vertex normal with weight strength
	"mul  r6.xyz,  r6.xyz,  v2.zzz              \n"  // multiply vertex tangent with weight strength

	/// Second weight
	"mov  a0.x,    r3.y                         \n"  // move index of second matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  v2.yyy,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.yyy,  r8.xyz     \n"  // 
	"mad  r6.xyz,  r9.xyz,  v2.yyy,  r6.xyz     \n"  // 

	/// Third weight
	"mov  a0.x,    r3.z                         \n"  // move index of third matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  v2.xxx,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.xxx,  r8.xyz     \n"  // 
	"mad  r6.xyz,  r9.xyz,  v2.xxx,  r6.xyz     \n"  // 

	/// Fourth weight
	"mov  a0.x,    r3.w                         \n"  // move index of fourth matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  r2.www,  r0         \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.www,  r8.xyz     \n"  // 
	"mad  r6.xyz,  r9.xyz,  v2.www,  r6.xyz     \n"  // 

	///make binormal from normal & tangent
	"mul  r4.xyz,  r8.zxy,  r6.yzx              \n"  // normal X tangent 1/2
	"mad  r7.xyz,  r8.yzx,  r6.zxy,  r4.xyz     \n"  // normal X tangent 2/2
	"mul  r7.xyz,  r7.xyz,  v9.www              \n"  // fix binormal 
	"dp3  r7.w,    r7.xyz,  r7.xyz              \n"  // normalize the binormal
	"rsq  r7.w,    r7.w                         \n"
	"mul  r7.xyz,  r7.xyz,  r7.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 66


//--------------- Tagent Space Normal Map Shader Using Binormal --------------//
//--- Normal Map Type, Tagent Space based ---//
//--- Input ---------------------------------//
//--- v0     - vertex position            ---//
//--- v1     - vertex normal              ---//
//--- v9.xyz - vertex Binormal            ---//
//--- v9.w   - vertex tangent direction   ---//
//--- c4.xyz - light direction            ---//
//--- Output --------------------------------//
//--- r0     - vertex position            ---//
//--- r1.xyz - light direction            ---//
//--- r6.xyz - tangent                    ---//
//--- r7.xyz - binormal                   ---//
//--- r8.xyz - normal                     ---//
//--- 상수설정은 기본과 동일하게 간다.    ---//
//tangent대신에 binormal을 사용한다.
//tangent = binormal X normal

static char strNoWeights_NMTangentSpaceB[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mov  r0,      v0                           \n"  // move vertex position to temp register r0
	"mov  r8.xyz,  v1.xyz                       \n"  // move vertex normal to temp register
	"mov  r7.xyz,  v9.xyz                       \n"  // move vertex binormal to temp register

	///make binormal from normal & tangent
	"mul  r4.xyz,  r7.zxy,  r8.yzx              \n"  // binormal X normal 1/2
	"mad  r6.xyz,  r7.yzx,  r8.zxy,  -r4.xyz    \n"  // binormal X normal 2/2
	"mul  r6.xyz,  r6.xyz,  v9.www              \n"  // fix tangent
	"dp3  r6.w,    r6.xyz,  r6.xyz              \n"  // normalize the tangent
	"rsq  r6.w,    r6.w                         \n"
	"mul  r6.xyz,  r6.xyz,  r6.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world space -> tangent space transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 12

// Base vertex shader that uses one weight per vertex
static char strOneWeight_NMTangentSpaceB[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices
	
	/// First weight 
	"mov  a0.x,    r3.x                         \n"
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r7.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex binormal bone matrix

	///make binormal from normal & tangent
	"mul  r4.xyz,  r7.zxy,  r8.yzx              \n"  // binormal X normal 1/2
	"mad  r6.xyz,  r7.yzx,  r8.zxy,  r4.xyz     \n"  // binormal X normal 2/2
	"mul  r6.xyz,  r6.xyz,  v9.www              \n"  // fix tangent 
	"dp3  r6.w,    r6.xyz,  r6.xyz              \n"  // normalize the tangent
	"rsq  r6.w,    r6.w                         \n"
	"mul  r6.xyz,  r6.xyz,  r6.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 21

// Base vertex shader that uses two weight per vertex
static char strTwoWeights_NMTangentSpaceB[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices

	/// Calculate last weight strength
	"sub  r2.w,    c7.y,    v2.z                \n"

	/// First weight
	"mov  a0.x,    r3.x                         \n"  // move index of first matrix into address register
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r7.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mul  r0.xyz,  r0.xyz,  v2.zzz              \n"  // multiply vertex position with weight strength
	"mul  r8.xyz,  r8.xyz,  v2.zzz              \n"  // multiply vertex normal with weight strength
	"mul  r7.xyz,  r7.xyz,  v2.zzz              \n"  // multiply vertex tangent with weight strength

	/// Second weight
	"mov  a0.x,    r3.y                         \n"  // move index of second matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  r2.www,  r0.xyz     \n"  // blend vertex position
	"mad  r8.xyz,  r11.xyz, r2.www,  r8.xyz     \n"  // blend vertex normal
	"mad  r7.xyz,  r9.xyz,  r2.www,  r7.xyz     \n"  // blend vertex tangent

	///make binormal from normal & tangent
	"mul  r4.xyz,  r7.zxy,  r8.yzx              \n"  // binormal X normal 1/2
	"mad  r6.xyz,  r7.yzx,  r8.zxy,  r4.xyz     \n"  // binormal X normal 2/2
	"mul  r6.xyz,  r6.xyz,  v9.www              \n"  // fix tangent 
	"dp3  r6.w,    r6.xyz,  r6.xyz              \n"  // normalize the tangent
	"rsq  r6.w,    r6.w                         \n"
	"mul  r6.xyz,  r6.xyz,  r6.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 38

// Base vertex shader that uses three weight per vertex
static char strThreeWeights_NMTangentSpaceB[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices

	/// Calculate last weight strength
	"add  r2.x,    v2.z,    v2.y                \n"
	"sub  r2.w,    c7.y,    r2.x                \n"  

	/// First weight
	"mov  a0.x,    r3.x                         \n"  // move index of first matrix into address register
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r7.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mul  r0.xyz,  r0.xyz,  v2.zzz              \n"  // multiply vertex position with weight strength
	"mul  r8.xyz,  r8.xyz,  v2.zzz              \n"
	"mul  r7.xyz,  r7.xyz,  v2.zzz              \n"

	/// Second weight
	"mov  a0.x,    r3.y                         \n"  // move index of second matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  v2.yyy,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.yyy,  r8.xyz     \n"  // 
	"mad  r7.xyz,  r9.xyz,  v2.yyy,  r7.xyz     \n"  // 

	/// Third weight
	"mov  a0.x,    r3.z                         \n"  // move index of third matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  r2.www,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.www,  r8.xyz     \n"  // 
	"mad  r7.xyz,  r9.xyz,  v2.www,  r7.xyz     \n"  // 
	
	///make binormal from normal & tangent
	"mul  r4.xyz,  r7.zxy,  r8.yzx              \n"  // binormal X normal 1/2
	"mad  r6.xyz,  r7.yzx,  r8.zxy,  r4.xyz     \n"  // binormal X normal 2/2
	"mul  r6.xyz,  r6.xyz,  v9.www              \n"  // fix tangent 
	"dp3  r6.w,    r6.xyz,  r6.xyz              \n"  // normalize the tangent
	"rsq  r6.w,    r6.w                         \n"
	"mul  r6.xyz,  r6.xyz,  r6.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 52

// Base vertex shader that uses three weight per vertex
static char strFourWeights_NMTangentSpaceB[] =
	"vs.1.1                                     \n"  // vertex shader version
	"mul  r3,      v3.zyxw, c4.wwww             \n"  // extract matrix indices

	/// Calculate last weight strength
	"add  r2.x,    v2.z,    v2.y                \n"
	"add  r2.y,    r2.x,    v2.x                \n"  
	"sub  r2.w,    c7.y,    r2.y                \n"  

	/// First weight
	"mov  a0.x,    r3.x                         \n"  // move index of first matrix into address register
	"m4x3 r0,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r8.xyz,  v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r7.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mul  r0.xyz,  r0.xyz,  v2.zzz              \n"  // multiply vertex position with weight strength
	"mul  r8.xyz,  r8.xyz,  v2.zzz              \n"  // multiply vertex normal with weight strength
	"mul  r7.xyz,  r7.xyz,  v2.zzz              \n"  // multiply vertex tangent with weight strength

	/// Second weight
	"mov  a0.x,    r3.y                         \n"  // move index of second matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  v2.yyy,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.yyy,  r8.xyz     \n"  // 
	"mad  r7.xyz,  r9.xyz,  v2.yyy,  r7.xyz     \n"  // 

	/// Third weight
	"mov  a0.x,    r3.z                         \n"  // move index of third matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  v2.xxx,  r0.xyz     \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.xxx,  r8.xyz     \n"  // 
	"mad  r7.xyz,  r9.xyz,  v2.xxx,  r7.xyz     \n"  // 

	/// Fourth weight
	"mov  a0.x,    r3.w                         \n"  // move index of fourth matrix into address register
	"m4x3 r4,      v0,      c[a0.x + 21]        \n"  // transform vertex position with bone matrix
	"m3x3 r11.xyz, v1.xyz,  c[a0.x + 21]        \n"  // transform vertex normal bone matrix
	"m3x3 r9.xyz,  v9.xyz,  c[a0.x + 21]        \n"  // transform vertex tangent bone matrix

	"mad  r0.xyz,  r4.xyz,  r2.www,  r0         \n"  // 
	"mad  r8.xyz,  r11.xyz, v2.www,  r8.xyz     \n"  // 
	"mad  r7.xyz,  r9.xyz,  v2.www,  r7.xyz     \n"  // 

	///make binormal from normal & tangent
	"mul  r4.xyz,  r7.zxy,  r8.yzx              \n"  // binormal X normal 1/2
	"mad  r6.xyz,  r7.yzx,  r8.zxy,  r4.xyz     \n"  // binormal X normal 2/2
	"mul  r6.xyz,  r6.xyz,  v9.www              \n"  // fix tangent 
	"dp3  r6.w,    r6.xyz,  r6.xyz              \n"  // normalize the tangent
	"rsq  r6.w,    r6.w                         \n"
	"mul  r6.xyz,  r6.xyz,  r6.www              \n"
	"m3x3 r1.xyz,  c4.xyz,  r6                  \n"  // world -> tangent transform

//	"dp3  r2.w,    r1.xyz,  r1.xyz              \n"  //normalize
//	"rsq  r2.w,    r2.w                         \n"  //별로 필요 없을듯. 회전 행렬이니(혹은 대칭변환행렬)
//	"mul  r1.xyz,  r1.xyz,  r2.www              \n"  //일단 command slot 수에서 뺐음.

	"mov  r0.w,    c7.y                         \n"  // set vertex position w as 1.0f
//	"mov  r1.w,    c7.y                         \n"  // set w as 1.0f (NEEDED!)
	;// command slot -> 66



#endif //__SHADER_CODE__
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)