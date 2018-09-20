#ifndef __T2__
#define __T2__

#include "IPlug_include_in_plug_hdr.h"

class T2 : public IPlug
{
public:
  T2(IPlugInstanceInfo instanceInfo);
  ~T2();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:

  double mGain;
  double mThres;
  void CreatePresets();
};

#endif
