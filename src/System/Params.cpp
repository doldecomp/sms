#include <System/Params.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

static const char SceneParamsDir[] = "/map/params";

JKRFileLoader* TParams::mArc      = nullptr;
JKRFileLoader* TParams::mSceneArc = nullptr;

bool TParams::load(const char* filename)
{
	bool found = false;
	if (filename[0] == '/') {
		filename++;
	}

	void* resource = nullptr;
	if (mSceneArc != nullptr && mSceneArc->becomeCurrent(SceneParamsDir)) {
		resource = mSceneArc->getResource(filename);
	}

	if (resource != nullptr) {
		s32 size = mSceneArc->getResSize(resource);
		JSUMemoryInputStream stream(resource, size);
		load(stream);
		found = true;
	} else {
		if (mArc != nullptr) {
			mArc->becomeCurrent("/");
			void* resource2 = mArc->getResource(filename);
			if (resource2 != nullptr) {
				s32 size = mArc->getResSize(resource2);
				JSUMemoryInputStream stream(resource2, size);
				load(stream);
				found = true;
			}
		}
	}

	return found;
}

void TParams::finalize()
{
	mArc      = nullptr;
	mSceneArc = nullptr;
}

void TParams::init()
{
	mArc      = JKRFileLoader::getVolume("params");
	mSceneArc = JKRFileLoader::getVolume("scene");
}

void TParams::load(JSUMemoryInputStream& stream)
{
	if (mHead != nullptr) {
		s32 length = stream.readS32();
		for (int i = 0; i < length; i++) {
			u16 keyCode = stream.read16b();
			char buffer[0x50];
			stream.readString(buffer, 0x50);

			TBaseParam* param;
			for (param = mHead; param != nullptr; param = param->next) {
				if (keyCode == param->keyCode && !strcmp(buffer, param->name)) {
					param->load(stream);
					break;
				}
			}
			if (param == nullptr) {
				s32 end = stream.read32b();
				stream.skip(end);
			}
		}
	}
}
