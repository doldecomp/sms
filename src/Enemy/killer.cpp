class TKiller {
public:
	static unsigned char mSerialBomb;
	static unsigned char mTrampleDie;
};

class TFlyEnemy {
public:
	static float mTestSp;
	static int mInvalidTime;
	static float mTestMarioSpMax;
};

unsigned char TKiller::mSerialBomb = 1;
unsigned char TKiller::mTrampleDie = 1;
float TFlyEnemy::mTestSp = 2.5f;
int TFlyEnemy::mInvalidTime = 200;
float TFlyEnemy::mTestMarioSpMax = 12.0f;
