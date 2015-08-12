#ifndef SE_INCL_VIDEODECODER_H
#define SE_INCL_VIDEODECODER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

class CVideoDecoderX {
public:
  // Create a decoder that streams from file
  CVideoDecoderX(void);
  ~CVideoDecoderX(void);

  // Playing wmv
#define VDX_LOADINTOMEMORY    (1UL<<0)
#define VDX_USEOVERLAY        (1UL<<1)
  BOOL Open(const CTFileName &fnm, ULONG ulFlags);
  // Clear decoder data
  void Close(void);
  // Decode and render one frame to destination box
  void Update(const PIXaabbox2D &boxDest);
  // change flags (for example to disable overlay)
  void ChangeFlags(ULONG ulNewFlags);

  // pause/continue playing
  void Pause(void);
  void Continue(void);

  // Restart wmv
  void Restart(void);
  // Is stream open
  BOOL IsOpen(void);
  // Is end of stream
  BOOL IsAtEnd(void);


public:
  void *vd_pvInternal;
};

#endif
