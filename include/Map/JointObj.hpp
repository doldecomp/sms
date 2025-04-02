#ifndef JOINT_OBJ_HPP
#define JOINT_OBJ_HPP

class J3DJoint;

class TJointObj {
public:
	TJointObj();

	void initChildren();

	virtual void initJointObj(J3DJoint*);
	virtual void alive();
	virtual void awake();
	virtual void stand();
	virtual void kill();
	virtual void sleep();
	virtual void sit();
	virtual void newJointObj() const;
};

#endif
