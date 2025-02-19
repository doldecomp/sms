#ifndef JDR_CAMERA_HPP
#define JDR_CAMERA_HPP

#include <JSystem/JStage/JSGCamera.hpp>
#include <JSystem/JDrama/JDRPlacement.hpp>

namespace JDrama {

class TCamera : public TPlacement, public JStage::TCamera {
public:
	TCamera(float, float, const char*);

	virtual ~TCamera();
	virtual int getType() const { return 2; }

	virtual s32 JSGFGetType() { return 3; }

	void JSGSetFlag(u32);
	float JSGGetProjectionNear() const;
	void JSGSetProjectionNear(float);
	float JSGGetProjectionFar() const;
	void JSGSetProjectionFar(float);
};

class TPolarCamera : public TCamera {
public:
	TPolarCamera()
	    : TCamera(50.0f, 10000.0f, "<TPolarCamera>")
	{
	}
	virtual ~TPolarCamera();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	bool JSGGetProjectionType() const;
	void JSGSetProjectionType(JStage::TECameraProjection);
	float JSGGetProjectionFovy() const;
	void JSGSetProjectionFovy(float);
	float JSGGetProjectionAspect() const;
	void JSGSetProjectionAspect(float);
};

}; // namespace JDrama

#endif
