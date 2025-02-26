#ifndef M3DUTIL_SAMPLE_CTRL_MODEL_HPP
#define M3DUTIL_SAMPLE_CTRL_MODEL_HPP

class J3DModelData;
class J3DJoint;

class SampleCtrlModelData {
public:
	SampleCtrlModelData(J3DModelData*);
	void makeHierarchy(J3DJoint*);
	virtual ~SampleCtrlModelData() { }
};

#endif
