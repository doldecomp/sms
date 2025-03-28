#include <JSystem/JParticle/JPAEmitterLoader.hpp>
#include <types.h>

JPAEmitterData* JPAEmitterLoaderDataBase::load(const u8* param_1,
                                               JKRHeap* param_2,
                                               JPATextureResource* param_3)
{
	JPABinaryHeader* header = (JPABinaryHeader*)param_1;
	if (header->unk0 == 'JEFF' && header->unk4 == 'jpa1') {
		JPAEmitterLoader_v10 loader(param_2, param_1, header);
		return loader.load(param_3);
	} else {
		return nullptr;
	}
}

JPAEmitterLoader_v10::JPAEmitterLoader_v10(JKRHeap* param_1, const u8* param_2,
                                           const JPABinaryHeader* param_3)
    : JPAEmitterLoader(param_1, param_2, param_3)
{
}

JPAEmitterData* JPAEmitterLoader_v10::load(JPATextureResource*) { }
