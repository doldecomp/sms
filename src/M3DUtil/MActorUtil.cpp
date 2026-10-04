#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

void SMS_DumpMActor(MActor*) { }

MActor* SMS_MakeMActorFromSDLModelData(SDLModelData* model_data,
                                       MActorAnmData* anm_data, u32 flags)
{
	JUT_ASSERT(model_data);
	JUT_ASSERT(anm_data);

	SDLModel* model = new SDLModel(model_data, flags, 1);
	MActor* actor   = new MActor(anm_data);
	actor->setModel(model, 0);
	return actor;
}

SDLModelData* SMS_MakeSDLModelData(const char* model_path, u32 flags)
{
	JUT_ASSERT(model_path);

	void* res = JKRGetResource(model_path);
	JUT_ASSERT(res);

	J3DModelData* j3ddata = J3DModelLoaderDataBase::load(res, flags);
	SDLModelData* sdlData = new SDLModelData(j3ddata);

	return sdlData;
}

MActor** SMS_MakeMActorsWithAnmData(const char* model_path,
                                    MActorAnmData* anm_data, int count,
                                    u32 model_flags, u32 loader_flags)
{
	JUT_ASSERT(model_path);
	JUT_ASSERT(anm_data);

	SDLModelData* sdlData = SMS_MakeSDLModelData(model_path, loader_flags);

	MActor** actors = new MActor*[count];
	for (int i = 0; i < count; ++i)
		actors[i]
		    = SMS_MakeMActorFromSDLModelData(sdlData, anm_data, model_flags);

	return actors;
}

MActor* SMS_MakeMActorWithAnmData(const char* model_path,
                                  MActorAnmData* anm_data, u32 model_flags,
                                  u32 loader_flags)
{
	JUT_ASSERT(model_path);
	JUT_ASSERT(anm_data);

	MActor** actors = SMS_MakeMActorsWithAnmData(model_path, anm_data, 1,
	                                             model_flags, loader_flags);
	return actors[0];
}

MActor** SMS_MakeMActors(const char* anm_folder, const char* model_path,
                         int count, u32 model_flags, u32 loader_flags)
{
	JUT_ASSERT(anm_folder);
	JUT_ASSERT(model_path);

	MActorAnmData* anm = new MActorAnmData;
	anm->init(anm_folder, nullptr);
	return SMS_MakeMActorsWithAnmData(model_path, anm, count, model_flags,
	                                  loader_flags);
}

MActor* SMS_MakeMActor(const char* anm_folder, const char* model_path,
                       u32 model_flags, u32 loader_flags)
{
	JUT_ASSERT(anm_folder);
	JUT_ASSERT(model_path);

	MActor** actors
	    = SMS_MakeMActors(anm_folder, model_path, 1, model_flags, loader_flags);
	return actors[0];
}
