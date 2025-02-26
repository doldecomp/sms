#ifndef M3DUTIL_SAMPLE_CTRL_NODE_HPP
#define M3DUTIL_SAMPLE_CTRL_NODE_HPP

class J3DShape;
class J3DMaterial;
class J3DJoint;

class SampleCtrlShape {
public:
	SampleCtrlShape(J3DShape*);
	virtual ~SampleCtrlShape() { }
};

class SampleCtrlMaterial {
public:
	SampleCtrlMaterial(J3DMaterial*);
	virtual ~SampleCtrlMaterial() { }
};

class SampleCtrlJoint {
public:
	SampleCtrlJoint(J3DJoint*);
	virtual ~SampleCtrlJoint() { }
};

#endif
