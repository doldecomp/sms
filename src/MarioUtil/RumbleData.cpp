#include <macros.h>
#include <MarioUtil/RumbleMgr.hpp>

static int rumblePoint_00 = 0x0000000B;

static float rumbleFrame_00[11]
    = { 0.0f, 0.05f, 0.1f, 0.15f, 0.2f, 0.25f, 0.25f, 0.33f, 0.4f, 0.5f, 0.6f };

static float rumblePower_00[11]
    = { 1.0f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f, 0.6f, 0.6f, 0.5f, 0.2f, 0.0f };

static int rumblePoint_01 = 0x00000018;

static float rumbleFrame_01[24] = {
	0.0f,  0.16f, 0.19f, 0.24f, 0.26f, 0.37f, 0.4f,  0.45f,
	0.46f, 0.54f, 0.56f, 0.61f, 0.62f, 0.7f,  0.73f, 0.78f,
	0.79f, 0.86f, 0.92f, 0.96f, 1.03f, 1.13f, 1.17f, 1.4f,
};

static float rumblePower_01[24] = {
	1.0f,  1.0f,  0.0f, 0.0f,  0.95f, 0.85f, 0.0f,  0.0f,
	0.8f,  0.73f, 0.0f, 0.0f,  0.64f, 0.53f, 0.0f,  0.0f,
	0.44f, 0.35f, 0.0f, 0.27f, 0.02f, 0.16f, 0.01f, 0.0f,
};

static int rumblePoint_02 = 0x00000002;

static float rumbleFrame_02[2] = { 0.0f, 0.1f };

static float rumblePower_02[2] = { 0.8f, 0.6f };

static int rumblePoint_03 = 0x00000002;

static float rumbleFrame_03[2] = { 0.0f, 0.1f };

static float rumblePower_03[2] = { 1.0f, 1.0f };

static int rumblePoint_04 = 0x00000007;

static float rumbleFrame_04[7] = { 0.0f, 0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.5f };

static float rumblePower_04[7] = { 1.0f, 1.0f, 0.0f, 0.8f, 0.8f, 0.0f, 0.0f };

static int rumblePoint_05 = 0x00000015;

static float rumbleFrame_05[21]
    = { 0.0f,  0.18f, 0.21f, 0.28f, 0.28f, 0.4f,  0.45f,
	    0.51f, 0.52f, 0.64f, 0.69f, 0.75f, 0.83f, 0.89f,
	    0.99f, 1.03f, 1.12f, 1.19f, 1.25f, 1.3f,  1.4f };

static float rumblePower_05[21]
    = { 1.0f,  0.92f, 0.0f,  0.0f,  0.79f, 0.63f, 0.0f,
	    0.02f, 0.45f, 0.31f, 0.02f, 0.24f, 0.03f, 0.18f,
	    0.04f, 0.13f, 0.04f, 0.1f,  0.04f, 0.06f, 0.0f };

static int rumblePoint_06 = 0x0000000A;

static float rumbleFrame_06[10]
    = { 0.0f, 0.09f, 0.09f, 0.1f, 0.17f, 0.17f, 0.18f, 0.26f, 0.26f, 0.5f };

static float rumblePower_06[10]
    = { 0.85f, 0.85f, 0.1f, 0.45f, 0.45f, 0.05f, 0.25f, 0.25f, 0.01f, 0.0f };

static int rumblePoint_07 = 0x00000014;

static float rumbleFrame_07[20]
    = { 0.0f,  0.11f, 0.12f, 0.13f, 0.2f,  0.21f, 0.29f, 0.31f, 0.33f, 0.41f,
	    0.42f, 0.52f, 0.53f, 0.54f, 0.62f, 0.64f, 0.72f, 0.74f, 0.75f, 1.0f };

static float rumblePower_07[20]
    = { 0.95f, 0.97f, 0.85f, 0.0f, 0.0f, 0.75f, 0.73f, 0.55f, 0.0f,  0.0f,
	    0.6f,  0.63f, 0.32f, 0.0f, 0.0f, 0.51f, 0.48f, 0.19f, 0.02f, 0.0f };

static int rumblePoint_08 = 0x00000004;

static float rumbleFrame_08[5] = { 0.0f, 0.3f, 0.5f, 0.7f, 1.0f };

static float rumblePower_08[5] = { 1.0f, 0.5f, 0.35f, 0.5f, 1.0f };

static int rumblePoint_09 = 0x00000004;

static float rumbleFrame_09[4] = { 0.0f, 0.3f, 0.5f, 0.7f };

static float rumblePower_09[4] = { 0.5f, 0.24f, 0.18f, 0.23f };

static int rumblePoint_10 = 0x00000001;

static float rumbleFrame_10[1] = { 0.0f };

static float rumblePower_10[1] = { 0.0f };

static int rumblePoint_11 = 0x00000001;

static float rumbleFrame_11[1] = { 0.0f };

static float rumblePower_11[1] = { 0.0f };

static int rumblePoint_12 = 0x00000001;

static float rumbleFrame_12[1] = { 0.0f };

static float rumblePower_12[1] = { 0.0f };

static int rumblePoint_13 = 0x00000001;

static float rumbleFrame_13[1] = { 0.0f };

static float rumblePower_13[1] = { 0.0f };

static int rumblePoint_14 = 0x00000001;

static float rumbleFrame_14[1] = { 0.0f };

static float rumblePower_14[1] = { 0.0f };

static int rumblePoint_15 = 0x00000001;

static float rumbleFrame_15[1] = { 0.0f };

static float rumblePower_15[1] = { 0.0f };

static int rumblePoint_16 = 0x00000001;

static float rumbleFrame_16[1] = { 0.0f };

static float rumblePower_16[1] = { 0.0f };

static int rumblePoint_17 = 0x00000001;

static float rumbleFrame_17[1] = { 0.0f };

static float rumblePower_17[1] = { 0.0f };

static int rumblePoint_18 = 0x00000001;

static float rumbleFrame_18[1] = { 0.0f };

static float rumblePower_18[1] = { 0.0f };

static int rumblePoint_19 = 0x00000005;

static float rumbleFrame_19[5] = { 0.0f, 0.11f, 0.2f, 0.32f, 0.4f };

static float rumblePower_19[5] = { 0.6f, 0.4f, 0.6f, 0.4f, 0.6f };

static int rumblePoint_20 = 0x00000002;

static float rumbleFrame_20[2] = { 0.0f, 1.0f / 60.0f };

static float rumblePower_20[2] = { 0.5f, 0.5f };

static int rumblePoint_21 = 0x00000002;

static float rumbleFrame_21[2] = { 0.0f, 1.0f / 60.0f };

static float rumblePower_21[2] = { 0.75f, 0.75f };

static int rumblePoint_22 = 0x00000002;

static float rumbleFrame_22[2] = { 0.0f, 2.0f };

static float rumblePower_22[2] = { 0.75f, 0.75f };

static int rumblePoint_23 = 0x0000004D;

static float rumbleFrame_23[77]
    = { 0.0f,   0.07f,  0.13f,  0.25f,  0.26f,  1.02f,  1.03f,  1.14f,  1.14f,
	    1.2f,   1.21f,  1.29f,  1.33f,  3.75f,  3.81f,  3.92f,  3.94f,  4.0f,
	    4.16f,  4.17f,  4.36f,  4.4f,   4.44f,  4.44f,  4.58f,  4.69f,  5.16f,
	    5.18f,  5.28f,  5.32f,  5.4f,   5.41f,  5.52f,  5.56f,  5.58f,  5.66f,
	    5.68f,  6.35f,  6.36f,  6.57f,  6.68f,  6.85f,  6.88f,  7.11f,  7.18f,
	    7.5f,   7.66f,  8.09f,  8.67f,  9.08f,  9.1f,   9.63f,  9.64f,  9.65f,
	    9.81f,  9.84f,  9.89f,  9.91f,  10.02f, 10.04f, 10.05f, 10.06f, 10.19f,
	    10.21f, 10.22f, 10.23f, 10.35f, 10.38f, 10.43f, 10.45f, 10.51f, 10.57f,
	    10.61f, 10.68f, 10.78f, 10.82f, 11.0f };

static float rumblePower_23[77]
    = { 0.8f,  1.0f,  0.95f, 0.8f,  0.0f,  0.0f,  0.95f, 0.95f, 0.41f, 0.44f,
	    0.72f, 0.58f, 0.0f,  0.0f,  0.81f, 0.9f,  0.69f, 0.0f,  0.0f,  0.98f,
	    0.95f, 0.53f, 0.5f,  0.87f, 0.7f,  0.0f,  0.0f,  0.87f, 0.84f, 0.32f,
	    0.27f, 0.69f, 0.55f, 0.0f,  0.34f, 0.27f, 0.0f,  0.0f,  0.78f, 0.71f,
	    0.0f,  0.0f,  0.78f, 0.68f, 0.0f,  0.0f,  0.42f, 0.72f, 0.87f, 1.0f,
	    0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.95f, 0.85f, 0.0f,
	    0.0f,  0.8f,  0.73f, 0.0f,  0.0f,  0.64f, 0.53f, 0.0f,  0.0f,  0.44f,
	    0.35f, 0.0f,  0.27f, 0.02f, 0.16f, 0.01f, 0.0f };

static int rumblePoint_24 = 0x00000025;

static float rumbleFrame_24[37]
    = { 0.0f,  0.07f, 0.13f, 0.25f, 0.26f, 1.02f, 1.03f, 1.14f, 1.14f, 1.2f,
	    1.21f, 1.29f, 1.33f, 3.75f, 3.81f, 3.92f, 3.94f, 4.0f,  4.16f, 4.17f,
	    4.36f, 4.4f,  4.44f, 4.44f, 4.58f, 4.69f, 5.16f, 5.18f, 5.28f, 5.32f,
	    5.4f,  5.41f, 5.52f, 5.56f, 5.58f, 5.66f, 5.68f };

static float rumblePower_24[37]
    = { 0.8f,  1.0f,  0.95f, 0.8f,  0.0f,  0.0f,  0.95f, 0.95f, 0.41f, 0.44f,
	    0.72f, 0.58f, 0.0f,  0.0f,  0.81f, 0.9f,  0.69f, 0.0f,  0.0f,  0.98f,
	    0.95f, 0.53f, 0.5f,  0.87f, 0.7f,  0.0f,  0.0f,  0.87f, 0.84f, 0.32f,
	    0.27f, 0.69f, 0.55f, 0.0f,  0.34f, 0.27f, 0.0f };

RumbleChannelDataTbl channelDataTbl[25] = {
	{ &rumblePoint_00, rumbleFrame_00, rumblePower_00 },
	{ &rumblePoint_01, rumbleFrame_01, rumblePower_01 },
	{ &rumblePoint_02, rumbleFrame_02, rumblePower_02 },
	{ &rumblePoint_03, rumbleFrame_03, rumblePower_03 },
	{ &rumblePoint_04, rumbleFrame_04, rumblePower_04 },
	{ &rumblePoint_05, rumbleFrame_05, rumblePower_05 },
	{ &rumblePoint_06, rumbleFrame_06, rumblePower_06 },
	{ &rumblePoint_07, rumbleFrame_07, rumblePower_07 },
	{ &rumblePoint_08, rumbleFrame_08, rumblePower_08 },
	{ &rumblePoint_09, rumbleFrame_09, rumblePower_09 },
	{ &rumblePoint_10, rumbleFrame_10, rumblePower_10 },
	{ &rumblePoint_11, rumbleFrame_11, rumblePower_11 },
	{ &rumblePoint_12, rumbleFrame_12, rumblePower_12 },
	{ &rumblePoint_13, rumbleFrame_13, rumblePower_13 },
	{ &rumblePoint_14, rumbleFrame_14, rumblePower_14 },
	{ &rumblePoint_15, rumbleFrame_15, rumblePower_15 },
	{ &rumblePoint_16, rumbleFrame_16, rumblePower_16 },
	{ &rumblePoint_17, rumbleFrame_17, rumblePower_17 },
	{ &rumblePoint_18, rumbleFrame_18, rumblePower_18 },
	{ &rumblePoint_19, rumbleFrame_19, rumblePower_19 },
	{ &rumblePoint_20, rumbleFrame_20, rumblePower_20 },
	{ &rumblePoint_21, rumbleFrame_21, rumblePower_21 },
	{ &rumblePoint_22, rumbleFrame_22, rumblePower_22 },
	{ &rumblePoint_23, rumbleFrame_23, rumblePower_23 },
	{ &rumblePoint_24, rumbleFrame_24, rumblePower_24 },
};

int channelNum = ARRAY_COUNT(channelDataTbl);
