#ifndef SE_INCL_STATISTICS_INTERNAL_H
#define SE_INCL_STATISTICS_INTERNAL_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#if !ENGINE_EXPORTS
  #error engine-internal file included out of engine!
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/Timer.h>
#include <Engine/Templates/StaticArray.h>

class CStatEntry {
public:
  INDEX se_iOrder;   // order in output
  virtual CTString Report(void) = 0;
};

/*
 * Statistics counter.
 */
class CStatCounter : public CStatEntry {
public:
  CTString sc_strFormat;  // printing format (must contain one %f or %g or %e)
  FLOAT sc_iCount;        // the counter itself
  FLOAT sc_fFactor;       // printout factor
  __forceinline void Clear(void) {};
  virtual CTString Report(void);
};

/*
 * Statistics timer.
 */
class CStatTimer : public CStatEntry {
public:
  CTString st_strFormat;  // printing format (must contain one %f or %g or %e)
  CTimerValue st_tvStarted; // time when the timer was started last time
  CTimerValue st_tvElapsed; // total elapsed time of the timer
  FLOAT st_fFactor;       // printout factor
  __forceinline void Clear(void) {};
  virtual CTString Report(void);
};

/*
 * Statistics label.
 */
class CStatLabel : public CStatEntry {
public:
  CTString sl_strFormat;  // printing format
  __forceinline void Clear(void) {};
  virtual CTString Report(void);
};

/*
 * Class for gathering and reporting statistics information.
 */
class CStatForm {
public:
// implementation:
  CStaticArray<class CStatCounter> sf_ascCounters;   // profiling counters
  CStaticArray<class CStatTimer>   sf_astTimers;     // profiling timers
  CStaticArray<class CStatLabel>   sf_aslLabels;     // profiling labels

// interface:
  enum StatLabelIndex
  {
    SLI_COUNT
  };
  enum StatTimerIndex
  {
    STI_WORLDTRANSFORM,
    STI_WORLDVISIBILITY,
    STI_WORLDRENDERING,
    STI_MODELSETUP,
    STI_MODELRENDERING,
    STI_PARTICLERENDERING,
    STI_FLARESRENDERING,
//강동민 수정 시작			03.04
	STI_SHADOWRENDERING,
//강동민 수정 끝			03.04
//강동민 수정 시작 Water 구현		04.22
	STI_WATERRENDERING,
//강동민 수정 끝 Water 구현			04.22
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
    STI_EFFECT,			//Effect Process and Render
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
    STI_EFFECTRENDER,	//Effect Texture Render
    STI_SOUNDUPDATE,
    STI_SOUNDMIXING,
    STI_TIMER,
    STI_MAINLOOP,
    STI_RAYCAST,

    STI_SHADOWUPDATE,
    STI_BINDTEXTURE, 

    STI_GFXAPI,
    STI_SWAPBUFFERS,

    STI_COUNT
  };
  enum StatCounterIndex
  {
    SCI_SCENE_TRIANGLES,
    SCI_SCENE_TRIANGLEPASSES,
    SCI_SECTORS,
    SCI_POLYGONS,
    SCI_DETAILPOLYGONS,
    SCI_POLYGONEDGES,
    SCI_EDGETRANSITIONS,

//강동민 수정 시작 테스트 클라이언트 작업	06.29
	SCI_REFLECTION_TRI,
//강동민 수정 끝 테스트 클라이언트 작업		06.29

    SCI_SOUNDSPLAYING,
    SCI_SOUNDSPENDING,

    SCI_CACHEDSHADOWS,
    SCI_FLATSHADOWS,
    SCI_CACHEDSHADOWBYTES,
    SCI_DYNAMICSHADOWS,
    SCI_DYNAMICSHADOWBYTES,
    SCI_SHADOWBINDS,
    SCI_SHADOWBINDBYTES,

    SCI_TEXTUREBINDS,
    SCI_TEXTUREBINDBYTES,
    SCI_TEXTUREUPLOADS,
    SCI_TEXTUREUPLOADBYTES,

    SCI_PARTICLES,
    SCI_OLDMODELS_QUEUED,
    SCI_OLDMODELS_RENDERED,
    SCI_SKAMODELS_QUEUED,
    SCI_SKAMODELS_RENDERED,
    SCI_SKAMESHES_RENDERED,
    SCI_SKELETONS,
    SCI_BONES,
    SCI_MODELSHADOWS,
    SCI_MDLTRIANGLES_USEDMIP,
    SCI_MDLTRIANGLES_FIRSTMIP,
    SCI_SKATRIANGLES_USEDMIP,
    SCI_SKATRIANGLES_FIRSTMIP,
    SCI_GFXVERTICES,
//강동민 수정 시작 테스트 클라이언트 작업	06.29
	SCI_REFLECTION_VERTICE,
	SCI_REFLECTION_SKATRI,
//강동민 수정 끝 테스트 클라이언트 작업		06.29

//안태훈 수정 시작	//(For Performance)(0.2)
	SCI_DPCOUNT,
//안태훈 수정 끝	//(For Performance)(0.2)
    SCI_COUNT
  };

  CStatForm(void);
  void Clear(void);

  /* Increment counter by one. */
  __forceinline void IncrementCounter(INDEX iCounter) {
    sf_ascCounters[iCounter].sc_iCount++;
  };

  /* Increment counter by given count. */
  __forceinline void IncrementCounter(INDEX iCounter, INDEX iCount) {
    sf_ascCounters[iCounter].sc_iCount += iCount;
  };

  /* Start a timer. */
  __forceinline void StartTimer(INDEX iTimer) {
    extern INDEX _iStatsMode;
    if( _iStatsMode<2) return;
    CStatTimer &st = sf_astTimers[iTimer];
    st.st_tvStarted = _pTimer->GetHighPrecisionTimer();
  };
  /* Stop a timer. */
  __forceinline void StopTimer(INDEX iTimer) {
    extern INDEX _iStatsMode;
    if( _iStatsMode<2) return;
    CStatTimer &st = sf_astTimers[iTimer];
    st.st_tvElapsed += _pTimer->GetHighPrecisionTimer()-st.st_tvStarted;
    st.st_tvStarted.tv_llValue = -1;
  };

  /* Check whether the timer is counting. */
  __forceinline BOOL CheckTimer(INDEX iTimer) { 
    CStatTimer &st = sf_astTimers[iTimer];
    return (st.st_tvStarted.tv_llValue != -1);
  };

  // initialize component
  void InitCounter(INDEX iCounter, INDEX iOrder, const char *strFormat, FLOAT fFactor);
  void InitTimer(INDEX iTimer, INDEX iOrder, const char *strFormat, FLOAT fFactor);
  void InitLabel(INDEX iLabel, INDEX iOrder, const char *strFormat);

  // reset all values
  void Reset(void);
  // make a new report
  void Report(CTString &strReport);
};

// one globaly used stats report
ENGINE_API extern CStatForm _sfStats;


#endif  /* include-once check. */

