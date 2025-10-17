// order is Point, Frame, Power
// point = sizeof(Frame)
// static_assert(sizeof(Frame) == sizeof(Power))

// struct RumbleData_t {
// 	int* point;
// 	int* frame;
// 	int* power;
// };

#define MAX_RUMBLE_ID (25)

static int rumblePoint_00   = 11;
static int rumbleFrame_00[] = {
	0x00000000, 0x3D4CCCCD, 0x3DCCCCCD, 0x3E19999A, 0x3E4CCCCD, 0x3E800000,
	0x3E800000, 0x3EA8F5C3, 0x3ECCCCCD, 0x3F000000, 0x3F19999A,
};
static int rumblePower_00[] = {
	0x3F800000, 0x3F800000, 0x3F800000, 0x3F666666, 0x00000000, 0x00000000,
	0x3F19999A, 0x3F19999A, 0x3F000000, 0x3E4CCCCD, 0x00000000,
};

static int rumblePoint_01   = 24;
static int rumbleFrame_01[] = {
	0x00000000, 0x3E23D70A, 0x3E428F5C, 0x3E75C28F, 0x3E851EB8, 0x3EBD70A4,
	0x3ECCCCCD, 0x3EE66666, 0x3EEB851F, 0x3F0A3D71, 0x3F0F5C29, 0x3F1C28F6,
	0x3F1EB852, 0x3F333333, 0x3F3AE148, 0x3F47AE14, 0x3F4A3D71, 0x3F5C28F6,
	0x3F6B851F, 0x3F75C28F, 0x3F83D70A, 0x3F90A3D7, 0x3F95C28F, 0x3FB33333,
};
static int rumblePower_01[] = {
	0x3F800000, 0x3F800000, 0x00000000, 0x00000000, 0x3F733333, 0x3F59999A,
	0x00000000, 0x00000000, 0x3F4CCCCD, 0x3F3AE148, 0x00000000, 0x00000000,
	0x3F23D70A, 0x3F07AE14, 0x00000000, 0x00000000, 0x3EE147AE, 0x3EB33333,
	0x00000000, 0x3E8A3D71, 0x3CA3D70A, 0x3E23D70A, 0x3C23D70A, 0x00000000,
};

static int rumblePoint_02   = 2;
static int rumbleFrame_02[] = {
	0x00000000,
	0x3DCCCCCD,
};
static int rumblePower_02[] = {
	0x3F4CCCCD,
	0x3F19999A,
};

static int rumblePoint_03   = 2;
static int rumbleFrame_03[] = {
	0x00000000,
	0x3DCCCCCD,
};
static int rumblePower_03[] = {
	0x3F800000,
	0x3F800000,
};

static int rumblePoint_04   = 7;
static int rumbleFrame_04[] = {
	0x00000000, 0x3DCCCCCD, 0x3E19999A, 0x3E4CCCCD,
	0x3E800000, 0x3E99999A, 0x3F000000,
};
static int rumblePower_04[] = {
	0x3F800000, 0x3F800000, 0x00000000, 0x3F4CCCCD,
	0x3F4CCCCD, 0x00000000, 0x00000000,
};

static int rumblePoint_05   = 21;
static int rumbleFrame_05[] = {
	0x00000000, 0x3E3851EC, 0x3E570A3D, 0x3E8F5C29, 0x3E8F5C29, 0x3ECCCCCD,
	0x3EE66666, 0x3F028F5C, 0x3F051EB8, 0x3F23D70A, 0x3F30A3D7, 0x3F400000,
	0x3F547AE1, 0x3F63D70A, 0x3F7D70A4, 0x3F83D70A, 0x3F8F5C29, 0x3F9851EC,
	0x3FA00000, 0x3FA66666, 0x3FB33333,
};
static int rumblePower_05[] = {
	0x3F800000, 0x3F6B851F, 0x00000000, 0x00000000, 0x3F4A3D71, 0x3F2147AE,
	0x00000000, 0x3CA3D70A, 0x3EE66666, 0x3E9EB852, 0x3CA3D70A, 0x3E75C28F,
	0x3CF5C28F, 0x3E3851EC, 0x3D23D70A, 0x3E051EB8, 0x3D23D70A, 0x3DCCCCCD,
	0x3D23D70A, 0x3D75C28F, 0x00000000,
};

static int rumblePoint_06   = 10;
static int rumbleFrame_06[] = {
	0x00000000, 0x3DB851EC, 0x3DB851EC, 0x3DCCCCCD, 0x3E2E147B,
	0x3E2E147B, 0x3E3851EC, 0x3E851EB8, 0x3E851EB8, 0x3F000000,
};
static int rumblePower_06[] = {
	0x3F59999A, 0x3F59999A, 0x3DCCCCCD, 0x3EE66666, 0x3EE66666,
	0x3D4CCCCD, 0x3E800000, 0x3E800000, 0x3C23D70A, 0x00000000,
};

static int rumblePoint_07   = 20;
static int rumbleFrame_07[] = {
	0x00000000, 0x3DE147AE, 0x3DF5C28F, 0x3E051EB8, 0x3E4CCCCD,
	0x3E570A3D, 0x3E947AE1, 0x3E9EB852, 0x3EA8F5C3, 0x3ED1EB85,
	0x3ED70A3D, 0x3F051EB8, 0x3F07AE14, 0x3F0A3D71, 0x3F1EB852,
	0x3F23D70A, 0x3F3851EC, 0x3F3D70A4, 0x3F400000, 0x3F800000,
};
static int rumblePower_07[] = {
	0x3F733333, 0x3F7851EC, 0x3F59999A, 0x00000000, 0x00000000,
	0x3F400000, 0x3F3AE148, 0x3F0CCCCD, 0x00000000, 0x00000000,
	0x3F19999A, 0x3F2147AE, 0x3EA3D70A, 0x00000000, 0x00000000,
	0x3F028F5C, 0x3EF5C28F, 0x3E428F5C, 0x3CA3D70A, 0x00000000,
};

static int rumblePoint_08   = 4;
static int rumbleFrame_08[] = {
	0x00000000, 0x3E99999A, 0x3F000000, 0x3F333333, 0x3F800000,
};
static int rumblePower_08[] = {
	0x3F800000, 0x3F000000, 0x3EB33333, 0x3F000000, 0x3F800000,
};

static int rumblePoint_09   = 4;
static int rumbleFrame_09[] = {
	0x00000000,
	0x3E99999A,
	0x3F000000,
	0x3F333333,
};
static int rumblePower_09[] = {
	0x3F000000,
	0x3E75C28F,
	0x3E3851EC,
	0x3E6B851F,
};

static int rumblePoint_10   = 1;
static int rumbleFrame_10[] = {
	0x00000000,
};
static int rumblePower_10[] = {
	0x00000000,
};

static int rumblePoint_11   = 1;
static int rumbleFrame_11[] = {
	0x00000000,
};
static int rumblePower_11[] = {
	0x00000000,
};

static int rumblePoint_12   = 1;
static int rumbleFrame_12[] = {
	0x00000000,
};
static int rumblePower_12[] = {
	0x00000000,
};

static int rumblePoint_13   = 1;
static int rumbleFrame_13[] = {
	0x00000000,
};
static int rumblePower_13[] = {
	0x00000000,
};

static int rumblePoint_14   = 1;
static int rumbleFrame_14[] = {
	0x00000000,
};
static int rumblePower_14[] = {
	0x00000000,
};

static int rumblePoint_15   = 1;
static int rumbleFrame_15[] = {
	0x00000000,
};
static int rumblePower_15[] = {
	0x00000000,
};

static int rumblePoint_16   = 1;
static int rumbleFrame_16[] = {
	0x00000000,
};
static int rumblePower_16[] = {
	0x00000000,
};

static int rumblePoint_17   = 1;
static int rumbleFrame_17[] = {
	0x00000000,
};
static int rumblePower_17[] = {
	0x00000000,
};

static int rumblePoint_18   = 1;
static int rumbleFrame_18[] = {
	0x00000000,
};
static int rumblePower_18[] = {
	0x00000000,
};

static int rumblePoint_19   = 5;
static int rumbleFrame_19[] = {
	0x00000000, 0x3DE147AE, 0x3E4CCCCD, 0x3EA3D70A, 0x3ECCCCCD,
};
static int rumblePower_19[] = {
	0x3F19999A, 0x3ECCCCCD, 0x3F19999A, 0x3ECCCCCD, 0x3F19999A,
};

static int rumblePoint_20   = 2;
static int rumbleFrame_20[] = {
	0x00000000,
	0x3C888889,
};
static int rumblePower_20[] = {
	0x3F000000,
	0x3F000000,
};

static int rumblePoint_21   = 2;
static int rumbleFrame_21[] = {
	0x00000000,
	0x3C888889,
};
static int rumblePower_21[] = {
	0x3F400000,
	0x3F400000,
};

static int rumblePoint_22   = 2;
static int rumbleFrame_22[] = {
	0x00000000,
	0x40000000,
};
static int rumblePower_22[] = {
	0x3F400000,
	0x3F400000,
};

static int rumblePoint_23   = 77;
static int rumbleFrame_23[] = {
	0x00000000, 0x3D8F5C29, 0x3E051EB8, 0x3E800000, 0x3E851EB8, 0x3F828F5C,
	0x3F83D70A, 0x3F91EB85, 0x3F91EB85, 0x3F99999A, 0x3F9AE148, 0x3FA51EB8,
	0x3FAA3D71, 0x40700000, 0x4073D70A, 0x407AE148, 0x407C28F6, 0x40800000,
	0x40851EB8, 0x408570A4, 0x408B851F, 0x408CCCCD, 0x408E147B, 0x408E147B,
	0x40928F5C, 0x4096147B, 0x40A51EB8, 0x40A5C28F, 0x40A8F5C3, 0x40AA3D71,
	0x40ACCCCD, 0x40AD1EB8, 0x40B0A3D7, 0x40B1EB85, 0x40B28F5C, 0x40B51EB8,
	0x40B5C28F, 0x40CB3333, 0x40CB851F, 0x40D23D71, 0x40D5C28F, 0x40DB3333,
	0x40DC28F6, 0x40E3851F, 0x40E5C28F, 0x40F00000, 0x40F51EB8, 0x410170A4,
	0x410AB852, 0x411147AE, 0x4111999A, 0x411A147B, 0x411A3D71, 0x411A6666,
	0x411CF5C3, 0x411D70A4, 0x411E3D71, 0x411E8F5C, 0x412051EC, 0x4120A3D7,
	0x4120CCCD, 0x4120F5C3, 0x41230A3D, 0x41235C29, 0x4123851F, 0x4123AE14,
	0x4125999A, 0x4126147B, 0x4126E148, 0x41273333, 0x412828F6, 0x41291EB8,
	0x4129C28F, 0x412AE148, 0x412C7AE1, 0x412D1EB8, 0x41300000,
};
static int rumblePower_23[] = {
	0x3F4CCCCD, 0x3F800000, 0x3F733333, 0x3F4CCCCD, 0x00000000, 0x00000000,
	0x3F733333, 0x3F733333, 0x3ED1EB85, 0x3EE147AE, 0x3F3851EC, 0x3F147AE1,
	0x00000000, 0x00000000, 0x3F4F5C29, 0x3F666666, 0x3F30A3D7, 0x00000000,
	0x00000000, 0x3F7AE148, 0x3F733333, 0x3F07AE14, 0x3F000000, 0x3F5EB852,
	0x3F333333, 0x00000000, 0x00000000, 0x3F5EB852, 0x3F570A3D, 0x3EA3D70A,
	0x3E8A3D71, 0x3F30A3D7, 0x3F0CCCCD, 0x00000000, 0x3EAE147B, 0x3E8A3D71,
	0x00000000, 0x00000000, 0x3F47AE14, 0x3F35C28F, 0x00000000, 0x00000000,
	0x3F47AE14, 0x3F2E147B, 0x00000000, 0x00000000, 0x3ED70A3D, 0x3F3851EC,
	0x3F5EB852, 0x3F800000, 0x00000000, 0x00000000, 0x3F800000, 0x3F800000,
	0x3F800000, 0x00000000, 0x00000000, 0x3F733333, 0x3F59999A, 0x00000000,
	0x00000000, 0x3F4CCCCD, 0x3F3AE148, 0x00000000, 0x00000000, 0x3F23D70A,
	0x3F07AE14, 0x00000000, 0x00000000, 0x3EE147AE, 0x3EB33333, 0x00000000,
	0x3E8A3D71, 0x3CA3D70A, 0x3E23D70A, 0x3C23D70A, 0x00000000,
};

static int rumblePoint_24   = 37;
static int rumbleFrame_24[] = {
	0x00000000, 0x3D8F5C29, 0x3E051EB8, 0x3E800000, 0x3E851EB8, 0x3F828F5C,
	0x3F83D70A, 0x3F91EB85, 0x3F91EB85, 0x3F99999A, 0x3F9AE148, 0x3FA51EB8,
	0x3FAA3D71, 0x40700000, 0x4073D70A, 0x407AE148, 0x407C28F6, 0x40800000,
	0x40851EB8, 0x408570A4, 0x408B851F, 0x408CCCCD, 0x408E147B, 0x408E147B,
	0x40928F5C, 0x4096147B, 0x40A51EB8, 0x40A5C28F, 0x40A8F5C3, 0x40AA3D71,
	0x40ACCCCD, 0x40AD1EB8, 0x40B0A3D7, 0x40B1EB85, 0x40B28F5C, 0x40B51EB8,
	0x40B5C28F,
};
static int rumblePower_24[] = {
	0x3F4CCCCD, 0x3F800000, 0x3F733333, 0x3F4CCCCD, 0x00000000, 0x00000000,
	0x3F733333, 0x3F733333, 0x3ED1EB85, 0x3EE147AE, 0x3F3851EC, 0x3F147AE1,
	0x00000000, 0x00000000, 0x3F4F5C29, 0x3F666666, 0x3F30A3D7, 0x00000000,
	0x00000000, 0x3F7AE148, 0x3F733333, 0x3F07AE14, 0x3F000000, 0x3F5EB852,
	0x3F333333, 0x00000000, 0x00000000, 0x3F5EB852, 0x3F570A3D, 0x3EA3D70A,
	0x3E8A3D71, 0x3F30A3D7, 0x3F0CCCCD, 0x00000000, 0x3EAE147B, 0x3E8A3D71,
	0x00000000,
};

const int* channelDataTbl[] = {
	&rumblePoint_00, rumbleFrame_00,  rumblePower_00,  &rumblePoint_01,
	rumbleFrame_01,  rumblePower_01,  &rumblePoint_02, rumbleFrame_02,
	rumblePower_02,  &rumblePoint_03, rumbleFrame_03,  rumblePower_03,
	&rumblePoint_04, rumbleFrame_04,  rumblePower_04,  &rumblePoint_05,
	rumbleFrame_05,  rumblePower_05,  &rumblePoint_06, rumbleFrame_06,
	rumblePower_06,  &rumblePoint_07, rumbleFrame_07,  rumblePower_07,
	&rumblePoint_08, rumbleFrame_08,  rumblePower_08,  &rumblePoint_09,
	rumbleFrame_09,  rumblePower_09,  &rumblePoint_10, rumbleFrame_10,
	rumblePower_10,  &rumblePoint_11, rumbleFrame_11,  rumblePower_11,
	&rumblePoint_12, rumbleFrame_12,  rumblePower_12,  &rumblePoint_13,
	rumbleFrame_13,  rumblePower_13,  &rumblePoint_14, rumbleFrame_14,
	rumblePower_14,  &rumblePoint_15, rumbleFrame_15,  rumblePower_15,
	&rumblePoint_16, rumbleFrame_16,  rumblePower_16,  &rumblePoint_17,
	rumbleFrame_17,  rumblePower_17,  &rumblePoint_18, rumbleFrame_18,
	rumblePower_18,  &rumblePoint_19, rumbleFrame_19,  rumblePower_19,
	&rumblePoint_20, rumbleFrame_20,  rumblePower_20,  &rumblePoint_21,
	rumbleFrame_21,  rumblePower_21,  &rumblePoint_22, rumbleFrame_22,
	rumblePower_22,  &rumblePoint_23, rumbleFrame_23,  rumblePower_23,
	&rumblePoint_24, rumbleFrame_24,  rumblePower_24,
};

int channelNum = MAX_RUMBLE_ID;
