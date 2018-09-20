#include "T2.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include <math.h>

const int kNumPrograms = 3;

enum EParams
{
  kGain = 0,
  kThres = 1,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 82,
  kGainY = 100,

  kThresX = 290,
  kThresY = 100,

  kKnobFrames = 120
};

void T2::CreatePresets() {
	MakePreset("Light Saturation No Fold", 100.0, 0.5);
	MakePreset("Some light sine folds", 200.0, 2.0);
	MakePreset("Stupid", 1000.0, 20.0);
}

T2::T2(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGain(1.), mThres(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGain)->InitDouble("Gain", 50., 0., 3000.0, 0.01, "%");
  GetParam(kGain)->SetShape(3.);

  GetParam(kThres)->InitDouble("Threshold", 1., 0.01, 20., 0.01, "%");
  GetParam(kThres)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);
  //pGraphics->AttachPanelBackground(&COLOR_GRAY);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kThresX, kThresY, kThres, &knob));

  AttachGraphics(pGraphics);

  CreatePresets();
  //MakePreset("preset 1", ... );
  //MakeDefaultPreset((char *) "-", kNumPrograms);
}

T2::~T2() {}

void T2::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

	int const chanCount = 2;
	for (int i = 0; i < chanCount; i++)
	{
		double* in = inputs[i];
		double* out = outputs[i];

		for (int s = 0; s < nFrames; s++, ++in, ++out)
		{
			*out = *in * mGain;

			*out = sin(*out * PI * mThres);
			
			/*
			if (*out >= 0) {
				*out = fmin(*out, mThres);
			}
			else {
				*out = fmax(*out, -mThres);
			}
			*out /= mThres;
			*/

		}
	}
}

void T2::Reset()
{
  TRACE;
  IMutexLock lock(this);
}

void T2::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGain:
      mGain = GetParam(kGain)->Value() / 100.;
      break;

	case kThres:
		mThres = GetParam(kThres)->Value();
		break;

    default:
      break;
  }
}