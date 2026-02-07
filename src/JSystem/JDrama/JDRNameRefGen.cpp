#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRSmJ3DScn.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRSmJ3DAct.hpp>
#include <JSystem/JDrama/JDRCharacter.hpp>
#include <JSystem/JDrama/JDRLighting.hpp>
#include <JSystem/JDrama/JDRNameRefPtrList.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>

using namespace JDrama;

TNameRefGen* TNameRefGen::instance;

TNameRef* TNameRefGen::load(JSUMemoryInputStream& stream)
{
	JSUMemoryInputStream tmpStream(nullptr, 0);

	if ((mRootNameRef = TNameRef::genObject(stream, tmpStream)))
		mRootNameRef->load(tmpStream);
}

TNameRef* TNameRefGen::getNameRef(const char* name) const
{
	if (strcmp(name, "GroupObj") == 0) {
		return new TViewObjPtrListT<TViewObj, TViewObj>;
	} else if (strcmp(name, "SmJ3DScn") == 0) {
		return new TSmJ3DScn("<SmJ3DScn>", 2);
	} else if (strcmp(name, "PolarCamera") == 0) {
		return new TPolarCamera();
	} else if (strcmp(name, "SmJ3DAct") == 0) {
		return new TSmJ3DAct();
	} else if (strcmp(name, "SmplChara") == 0) {
		return new TSmplChara();
	} else if (strcmp(name, "Light") == 0) {
		return new TLight();
	} else if (strcmp(name, "IdxLight") == 0) {
		return new TIdxLight();
	} else if (strcmp(name, "LightAry") == 0) {
		return new TLightAry();
	} else if (strcmp(name, "AmbColor") == 0) {
		return new TAmbColor();
	} else if (strcmp(name, "AmbAry") == 0) {
		return new TAmbAry();
	} else if (strcmp(name, "NameRefGrp") == 0) {
		return new TNameRefPtrListT<TNameRef, TNameRef>();
	} else if (strcmp(name, "DrawBufObj") == 0) {
		return new TDrawBufObj();
	} else if (strcmp(name, "EfbCtrlTex") == 0) {
		return new TEfbCtrlTex("<EfbCtrlTex>", 0);
	} else if (strcmp(name, "Viewport") == 0) {
		return new TViewport(TRect(0, 0, 640, 480), "<Viewport>");
	} else {
		return nullptr;
	}
}
