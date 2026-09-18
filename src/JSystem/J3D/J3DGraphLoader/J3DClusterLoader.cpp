#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JSupport.hpp>

// TODO (structural): these three explicit instantiations are scaffolding and
// give the symbols *global* linkage, while the map has all three weak. Retail
// got them weak by implicit instantiation from `J3DClusterLoader_v15::
// readCluster`, and the whole TU is dead-stripped apart from them; the map
// lists six UNUSED bodies to reconstruct before the linkage can be right:
//   load__24J3DClusterLoaderDataBaseFPCv          0xa0
//   __ct__20J3DClusterLoader_v15Fv                0x1c
//   __dt__16J3DClusterLoaderFv                    0x4c
//   __dt__20J3DClusterLoader_v15Fv                0x5c
//   load__20J3DClusterLoader_v15FPCv              0x9c
//   readCluster__20J3DClusterLoader_v15FPC15J3DClusterBlock  0x2bc
// `readCluster` also instantiates JSUConvertOffsetToPtr for J3DCluster,
// J3DClusterKey and J3DClusterVertex (0x18 each, all UNUSED), which is the
// evidence for what it walks.
template u16* JSUConvertOffsetToPtr<u16>(const void*, const void*);
template f32* JSUConvertOffsetToPtr<f32>(const void*, const void*);
template ResNTAB* JSUConvertOffsetToPtr<ResNTAB>(const void*, const void*);
