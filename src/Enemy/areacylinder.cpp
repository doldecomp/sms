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
	// Declared as one block at the top, C style: MWCC reserves a frame slot
	// for each scalar here even though two of them are never touched. Only
	// their count (2), size (4 bytes each), position (between `rot` and
	// `count`, at 0x74 and 0x70 of a 0xa0 frame) and class are recoverable
	// from the binary -- a dead *pointer* local reserves nothing, so these
	// two are scalars. The record's dropped scale.z and the dropped entry of
	// the key table below are the plausible originals.
	JGeometry::TVec3<f32> rot;
	f32 unusedScaleZ;
	s32 unusedKeyValue;
	s32 count;

	JDrama::TNameRef::load(stream);

	// Position, rotation and scale, as in every scene-binary record. The
	// rotation is dropped, the scale's x and y become the cylinder's radius
	// and height (scale 1.0 is 50 world units) and its z is read over the
	// dead rotation z.
	stream >> mPos.x >> mPos.y >> mPos.z;
	stream >> rot.x >> rot.y >> rot.z;
	stream >> mRadius >> mHeight;
	stream >> rot.z;

	mRadius *= 50.0f;
	mHeight *= 50.0f;

	stream.readString();

	stream >> count;

	// The ROM holds the bound in a callee-saved register across the body's
	// calls, which an address-taken local cannot be: it was copied out of
	// `count` first.
	s32 num = count;
	for (s32 i = 0; i < num; ++i) {
		stream.readF32();
		stream.readString();
	}

	const char* name = stream.readString();
	TAreaCylinderManager* mgr
	    = (TAreaCylinderManager*)gpConductor->search(name);

	if (!mgr)
		mgr = new TAreaCylinderManager(name);

	mgr->registerCylinder(this);

	mProbability = (f32)stream.readS32() / 100.0f;
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
