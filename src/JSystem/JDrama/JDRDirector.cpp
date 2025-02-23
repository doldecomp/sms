#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRActor.hpp>

using namespace JDrama;

TNameRef* TDirector::searchF(u16 key, const char* name)
{
	TNameRef* res = TNameRef::searchF(key, name);
	if (res)
		return res;

	if (unk10) {
		TNameRef* r = unk10->searchF(key, name);
		if (r)
			return r;
	}

	if (unk14) {
		TNameRef* r = unk14->searchF(key, name);
		if (r)
			return r;
	}

	return nullptr;
}

void TDirector::direct()
{
	TGraphics graphics;
	unk10->testPerform(3, &graphics);
	unk10->testPerform(8, &graphics);
}

JStage::TObject* TDirector::JSGFindObject(const char* name,
                                          JStage::TEObject type) const
{
	TNameRef* candidate = ((TDirector*)this)->search(name);

	if (candidate) {
		switch (candidate->getType()) {
		case 2:
			return static_cast<TCamera*>(candidate);

		case 1:
			return static_cast<TActor*>(candidate);
		}
	}

	return nullptr;
}
