#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Conductor.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

TAreaCylinder::TAreaCylinder(const char* name)
    : JDrama::TViewObj(name)
{
	mPos.zero();
	mRadius = 0.0f;
}

void TAreaCylinder::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	JGeometry::TVec3<f32> v;

	stream >> mPos.x >> mPos.y >> mPos.z;
	stream >> v.x >> v.y >> v.z;
	stream >> mRadius >> mHeight;
	stream >> v.z;

	mRadius *= 50.0f;
	mHeight *= 50.0f;

	stream.readString();

	int count = stream.readS32();
	for (int i = 0; i < count; ++i) {
		f32 tmp;
		stream >> tmp;
		stream.readString();
	}

	const char* name = stream.readString();
	TAreaCylinderManager* mgr
	    = (TAreaCylinderManager*)gpConductor->search(name);

	if (!mgr)
		mgr = new TAreaCylinderManager(name);

	mgr->registerCylinder(this);

	s32 rate;
	stream >> rate;
	mProbability = (f32)rate / 100.0f;
}

void TAreaCylinder::perform(u32, JDrama::TGraphics*) { }

TAreaCylinderManager::TAreaCylinderManager(const char* name)
    : JDrama::TViewObj(name)
{
	gpConductor->registerAreaCylinderManager(this);
}

void TAreaCylinderManager::registerCylinder(TAreaCylinder* cyl)
{
	mList.push_back(cyl);
}

BOOL TAreaCylinderManager::contain(const JGeometry::TVec3<f32>& pos)
{
	for (JGadget::TList<TAreaCylinder*>::iterator it = mList.begin(),
	                                              e  = mList.end();
	     it != e; ++it)
		if ((*it)->contain(pos))
			return TRUE;

	return FALSE;
}

TAreaCylinder*
TAreaCylinderManager::getCylinderContains(const JGeometry::TVec3<f32>& pos)
{
	for (JGadget::TList<TAreaCylinder*>::iterator it = mList.begin(),
	                                              e  = mList.end();
	     it != e; ++it)
		if ((*it)->contain(pos))
			return *it;

	return nullptr;
}

void TAreaCylinderManager::perform(u32, JDrama::TGraphics*) { }
