#include <Map/JointObj.hpp>

class J3DShape;

TJointObj::TJointObj() { }

void TJointObj::initJointObj(J3DJoint*) { }

void TJointObj::initChildren() { }

void getShapeInOneJoint(J3DJoint*, J3DShape**) { }

void TJointObj::kill() { }

void TJointObj::alive() { }

void TJointObj::sleep() { }

void TJointObj::awake() { }

void TJointObj::sit() { }

void TJointObj::stand() { }
