#include "stdh.h"

#include <Engine/Math/TextureMapping.h>

#include <Engine/Base/Stream.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/Plane.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Projection_DOUBLE.h>


// calculate default texture mapping control points from a plane
void CMappingVectors::FromPlane(const FLOATplane3D &plPlane)
{
  // take origin at plane reference point
  mv_vO = plPlane.ReferencePoint();

  // if the plane is mostly horizontal
  if (Abs(plPlane(2))>0.5) {
    // use cross product of +x axis and plane normal as +s axis
    mv_vU = FLOAT3D(1.0f, 0.0f, 0.0f)*(FLOAT3D&)plPlane;
  // if the plane is mostly vertical
  } else {
    // use cross product of +y axis and plane normal as +s axis
    mv_vU = FLOAT3D(0.0f, 1.0f, 0.0f)*(FLOAT3D&)plPlane;
  }
  // make +s axis normalized
  mv_vU.Normalize();

  // use cross product of plane normal and +s axis as +t axis
  mv_vV = mv_vU*(FLOAT3D &)plPlane;
}


void CMappingVectors::FromPlane_DOUBLE(const DOUBLEplane3D &plPlane)
{
  FromPlane(DOUBLEtoFLOAT(plPlane));
}


// calculate plane from default mapping vectors
void CMappingVectors::ToPlane(FLOATplane3D &plPlane) const
{
  plPlane = FLOATplane3D(mv_vV*mv_vU, mv_vO);
}


// convert to human-friendly format
void CMappingDefinition::ToUI(CMappingDefinitionUI &mdui) const
{
  // make a copy of parameters
  FLOAT fUoS = md_fUoS;
  FLOAT fUoT = md_fUoT;
  FLOAT fVoS = md_fVoS;
  FLOAT fVoT = md_fVoT;

  // find size of mapping vectors
  FLOAT fUSize = FLOAT(sqrt(fUoS*fUoS+fUoT*fUoT));
  FLOAT fVSize = FLOAT(sqrt(fVoS*fVoS+fVoT*fVoT));

  // find rotation of both vectors
  ANGLE aURot = -(ATan2(fVoT, fVoS)-90.0f);
  ANGLE aVRot = -(ATan2(fUoT, fUoS));

  // use the found values
  Snap(aURot, 0.001f);
  Snap(aVRot, 0.001f);
  mdui.mdui_aURotation= NormalizeAngle(aURot);
  mdui.mdui_aVRotation= NormalizeAngle(aVRot);
  mdui.mdui_fUStretch = 1/fUSize;
  mdui.mdui_fVStretch = 1/fVSize;
  mdui.mdui_fUOffset  = md_fUOffset;
  mdui.mdui_fVOffset  = md_fVOffset;
}


// convert from human-friendly format
void CMappingDefinition::FromUI(const CMappingDefinitionUI &mdui)
{
  // extract all values from mapping definition
  md_fUoS = +Cos(-(mdui.mdui_aVRotation));
  md_fUoT = +Sin(-(mdui.mdui_aVRotation));
  md_fVoS = +Cos(-(mdui.mdui_aURotation)+AngleDeg(90.0f));
  md_fVoT = +Sin(-(mdui.mdui_aURotation)+AngleDeg(90.0f));

  md_fUoS /= mdui.mdui_fUStretch;
  md_fUoT /= mdui.mdui_fUStretch;
  md_fVoS /= mdui.mdui_fVStretch;
  md_fVoT /= mdui.mdui_fVStretch;

  md_fUOffset = mdui.mdui_fUOffset;
  md_fVOffset = mdui.mdui_fVOffset;
}


// make mapping vectors for this mapping
void CMappingDefinition::MakeMappingVectors( const CMappingVectors &mvSrc, CMappingVectors &mvDst) const
{
  const FLOAT ood = 1.0f / (md_fUoS*md_fVoT - md_fUoT*md_fVoS);
  mvDst.mv_vO = mvSrc.mv_vO
              + mvSrc.mv_vU * (md_fUOffset*md_fVoT - md_fVOffset*md_fUoT) *ood
              + mvSrc.mv_vV * (md_fVOffset*md_fUoS - md_fUOffset*md_fVoS) *ood;
  mvDst.mv_vU = mvSrc.mv_vU*md_fUoS + mvSrc.mv_vV*md_fUoT;
  mvDst.mv_vV = mvSrc.mv_vU*md_fVoS + mvSrc.mv_vV*md_fVoT;
}


// transform default mapping vectors to mapping vectors for this mapping
void CMappingDefinition::TransformMappingVectors( const CMappingVectors &mvSrc, CMappingVectors &mvDst) const
{
  mvDst.mv_vU = mvSrc.mv_vU*md_fUoS + mvSrc.mv_vV*md_fUoT;
  mvDst.mv_vV = mvSrc.mv_vU*md_fVoS + mvSrc.mv_vV*md_fVoT;

  const FLOAT sou2 = mvDst.mv_vU % mvDst.mv_vU;
  const FLOAT sov2 = mvDst.mv_vV % mvDst.mv_vU;
  const FLOAT tou2 = mvDst.mv_vU % mvDst.mv_vV;
  const FLOAT tov2 = mvDst.mv_vV % mvDst.mv_vV;

  const FLOAT ood2 = 1.0f / (sou2*tov2-sov2*tou2);
  const FLOAT uos2 = +tov2*ood2;  const FLOAT uot2 = -sov2*ood2;
  const FLOAT vot2 = +sou2*ood2;  const FLOAT vos2 = -tou2*ood2;

  mvDst.mv_vO = mvSrc.mv_vO
              + mvDst.mv_vU * (md_fUOffset*uos2 + md_fVOffset*vos2)     // !!!! uot2 ???? offsets ????
              + mvDst.mv_vV * (md_fUOffset*vos2 + md_fVOffset*vot2);
}


// convert to mapping vectors
void CMappingDefinition::ToMappingVectors( const CMappingVectors &mvDefault, CMappingVectors &mvVectors) const
{
  const FLOAT ood = 1.0f /(md_fUoS*md_fVoT - md_fUoT*md_fVoS);
  const FLOAT sou = +md_fVoT*ood;  const FLOAT sov = -md_fUoT*ood;
  const FLOAT tov = +md_fUoS*ood;  const FLOAT tou = -md_fVoS*ood;
  mvVectors.mv_vO = mvDefault.mv_vO
                  + mvDefault.mv_vU * (md_fUOffset*sou + md_fVOffset*sov)
                  + mvDefault.mv_vV * (md_fUOffset*tou + md_fVOffset*tov);
  mvVectors.mv_vU = mvDefault.mv_vU*sou + mvDefault.mv_vV*tou;
  mvVectors.mv_vV = mvDefault.mv_vU*sov + mvDefault.mv_vV*tov;
}


// convert from mapping vectors
void CMappingDefinition::FromMappingVectors( const CMappingVectors &mvDefault, const CMappingVectors &mvVectors)
{
  const FLOAT sou = mvVectors.mv_vU % mvDefault.mv_vU;
  const FLOAT sov = mvVectors.mv_vV % mvDefault.mv_vU;
  const FLOAT tou = mvVectors.mv_vU % mvDefault.mv_vV;
  const FLOAT tov = mvVectors.mv_vV % mvDefault.mv_vV;

  const FLOAT ood = 1.0f / (sou*tov - sov*tou);
  md_fUoS = +tov*ood;  md_fUoT = -sov*ood;
  md_fVoT = +sou*ood;  md_fVoS = -tou*ood;

  const FLOAT3D vOffset = mvVectors.mv_vO - mvDefault.mv_vO;
  const FLOAT s = vOffset % mvDefault.mv_vU;
  const FLOAT t = vOffset % mvDefault.mv_vV;

  md_fUOffset = md_fUoS*s + md_fUoT*t;
  md_fVOffset = md_fVoS*s + md_fVoT*t;
}


// convert from old version of texture mapping defintion
void CMappingDefinition::ReadOld_t(CTStream &strm) // throw char *
{
  // old texture mapping orientation and offsets structure
  // - obsolete - used only for loading old worlds
  class CTextureMapping_old {
  public:
    ULONG tm_ulFlags;       // flags
    ANGLE tm_aRotation;     // angle of texture rotation
    FLOAT tm_fOffsetU;      // texture offsets (in meters)
    FLOAT tm_fOffsetV;
  } tmo;
  strm.Read_t(&tmo, sizeof(tmo));

  FLOAT fSin = Sin(tmo.tm_aRotation);
  FLOAT fCos = Cos(tmo.tm_aRotation);

  md_fUOffset = -tmo.tm_fOffsetU;
  md_fVOffset = -tmo.tm_fOffsetV;
  md_fUoS = +fCos;
  md_fUoT = -fSin;
  md_fVoS = +fSin;
  md_fVoT = +fCos;
}


/* Find texture coordinates for an object-space point. */
void CMappingDefinition::GetTextureCoordinates(
  const CMappingVectors &mvDefault, const FLOAT3D &vSpace, MEX2D &vTexture) const
{
  const FLOAT3D vOffset = vSpace-mvDefault.mv_vO;
  const FLOAT s = mvDefault.mv_vU % vOffset;
  const FLOAT t = mvDefault.mv_vV % vOffset;
  const FLOAT u = s*md_fUoS + t*md_fUoT;
  const FLOAT v = s*md_fVoS + t*md_fVoT;
  vTexture(1) = FloatToInt( (u+md_fUOffset)*1024.0f);
  vTexture(2) = FloatToInt( (v+md_fVOffset)*1024.0f);
}


/* Find object-space coordinates for a texture point. */
void CMappingDefinition::GetSpaceCoordinates(
  const CMappingVectors &mvDefault, const MEX2D &vTexture, FLOAT3D &vSpace) const
{
  const FLOAT ood = 1.0f / (md_fUoS*md_fVoT - md_fUoT*md_fVoS);
  const FLOAT sou = +md_fVoT*ood;  const FLOAT sov = -md_fUoT*ood;
  const FLOAT tov = +md_fUoS*ood;  const FLOAT tou = -md_fVoS*ood;

  const FLOAT u = (vTexture(1)/1024.0f) + md_fUOffset;
  const FLOAT v = (vTexture(2)/1024.0f) + md_fVOffset;
  const FLOAT s = u*sou + v*sov;
  const FLOAT t = u*tou + v*tov;
  vSpace = mvDefault.mv_vO + mvDefault.mv_vU*s + mvDefault.mv_vV*t;
}


// project another mapping on this one
void CMappingDefinition::ProjectMapping( 
  const FLOATplane3D &plOriginal, const CMappingDefinition &mdOriginal, const FLOATplane3D &pl)
{
  // make original mapping vectors
  CMappingVectors mvDefaultOrg;
  mvDefaultOrg.FromPlane(plOriginal);
  CMappingVectors mvOriginal;
  mdOriginal.ToMappingVectors(mvDefaultOrg, mvOriginal);

  // transform them to this plane
  CMappingVectors mv;
  mv.mv_vO = pl.DeprojectPoint    (plOriginal, mvOriginal.mv_vO);
  mv.mv_vU = pl.DeprojectDirection(plOriginal, mvOriginal.mv_vU);
  mv.mv_vV = pl.DeprojectDirection(plOriginal, mvOriginal.mv_vV);
  FLOAT3D vOTest = plOriginal.ProjectPoint(mv.mv_vO);
  FLOAT3D vUTest = plOriginal.ProjectDirection(mv.mv_vU);
  FLOAT3D vVTest = plOriginal.ProjectDirection(mv.mv_vV);

  // make mapping on this plane
  CMappingVectors mvDefault;
  mvDefault.FromPlane(pl);
  FromMappingVectors(mvDefault, mv);
}


// translate mapping in 3D
void CMappingDefinition::Translate(const CMappingVectors &mvDefault, const FLOAT3D &vTranslation)
{
  FLOATplane3D plPlane;
  mvDefault.ToPlane(plPlane);
  // make mapping vectors
  CMappingVectors mv;
  ToMappingVectors(mvDefault, mv);
  // translate mapping origin
  mv.mv_vO+=plPlane.ProjectDirection(vTranslation);
  // convert back from new mapping vectors
  FromMappingVectors(mvDefault, mv);
}


// rotate mapping in 3D
void CMappingDefinition::Rotate(const CMappingVectors &mvDefault, 
  const FLOAT3D &vRotationOrigin, ANGLE aRotation)
{
  FLOATplane3D plPlane;
  mvDefault.ToPlane(plPlane);
  // project rotation origin to the plane
  FLOAT3D vProjectedRotationOrigin = plPlane.ProjectPoint(vRotationOrigin);
  // get texture coordinates of the origin before rotation
  MEX2D vmexRotationOrigin0;
  GetTextureCoordinates(mvDefault, vProjectedRotationOrigin, vmexRotationOrigin0);
  // rotate the mapping
  CMappingDefinitionUI mdui;
  ToUI(mdui);
  mdui.mdui_aURotation+=aRotation;
  mdui.mdui_aVRotation+=aRotation;
  FromUI(mdui);
  // get texture coordinates of the origin after rotation
  MEX2D vmexRotationOrigin1;
  GetTextureCoordinates(mvDefault, vProjectedRotationOrigin, vmexRotationOrigin1);
  // adjust texture offsets so that origin stays on same place in texture
  md_fUOffset += (vmexRotationOrigin1(1)-vmexRotationOrigin0(1))/1024.0f;
  md_fVOffset += (vmexRotationOrigin1(2)-vmexRotationOrigin0(2))/1024.0f;
}


// center mapping to given point in 3D
void CMappingDefinition::Center(const CMappingVectors &mvDefault, 
  const FLOAT3D &vNewOrigin)
{
  FLOATplane3D plPlane;
  mvDefault.ToPlane(plPlane);
  // make mapping vectors
  CMappingVectors mv;
  ToMappingVectors(mvDefault, mv);
  // set new mapping origin
  mv.mv_vO=plPlane.ProjectPoint(vNewOrigin);
  // convert back from new mapping vectors
  FromMappingVectors(mvDefault, mv);
}


// transform mapping from one placement to another
void CMappingDefinition::Transform(const FLOATplane3D &plSourcePlane,
    const CPlacement3D &plSource, const CPlacement3D &plTarget)
{
  CSimpleProjection3D_DOUBLE prProjection;
  prProjection.ObjectPlacementL() = plSource;
  prProjection.ViewerPlacementL() = plTarget;
  prProjection.Prepare();
  CMappingDefinition mdTarget;
  prProjection.ProjectMapping(*this, FLOATtoDOUBLE(plSourcePlane), mdTarget);
  *this = mdTarget;
}

// stream operations
CTStream &operator>>(CTStream &strm, CMappingDefinition &md)
{
  strm.Read_t(&md, sizeof(md));
  return strm;
}


CTStream &operator<<(CTStream &strm, const CMappingDefinition &md)
{
  strm.Write_t(&md, sizeof(md));
  return strm;
}

