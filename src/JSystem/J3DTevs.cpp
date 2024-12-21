#include <JSystem/J3DTevs.hpp>


typedef void IDK;
IDK loadCullMode(u8);
IDK loadColorChanNum(u8);
IDK loadTexGenNum(u32);
IDK loadTexNo(u32, const u16&);
IDK loadTevKColorSel(u32, u8);
IDK loadTevKAlphaSel(u32, u8);
IDK loadTevStageNum(u8);
IDK loadIndTexStageNum(u8);
IDK loadDither(u8);

struct J3DNBTScale;
IDK loadNBTScale(J3DNBTScale&);
IDK makeTexCoordTable();
IDK makeAlphaCmpTable();
IDK makeZModeTable();
IDK makeTevSwapTable();
