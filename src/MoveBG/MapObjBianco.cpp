typedef s16 S16Color[4];

static f32 sRadius = 2380.0f;
static f32 sSubZ   = 150.0f;
static f32 sSpeed  = 0.05f;

class TMapObjRootPakkun {
public:
	static f32 mTremblePower;
	static f32 mTrembleAccel;
	static f32 mTrembleBrake;
	static u32 mTrembleTime;
};

class TBiancoWatermillVertical {
public:
	static f32 mRotAccel;
	static f32 mRotSpeedDownRate;
	static f32 mRotSpeedMax;
	static f32 mBridgeRotRate;
};

class TBiancoMiniWindmill {
public:
	static f32 mRotWaterAccel;
	static f32 mFriction;
	static f32 mRotSpeedMax;
};

class TLeafBoatRotten {
public:
	static f32 mAlphaDownSpeed;
	static f32 mCollisionRemoveAlpha;
	static S16Color mRottenColor;
};

f32 TMapObjRootPakkun::mTremblePower = 15.0f;
f32 TMapObjRootPakkun::mTrembleAccel = 0.95f;
f32 TMapObjRootPakkun::mTrembleBrake = 0.98f;
u32 TMapObjRootPakkun::mTrembleTime  = 360;

f32 TBiancoWatermillVertical::mRotAccel          = 0.15f;
f32 TBiancoWatermillVertical::mRotSpeedDownRate = 0.005f;
f32 TBiancoWatermillVertical::mRotSpeedMax       = 3.0f;
f32 TBiancoWatermillVertical::mBridgeRotRate     = 0.03f;

f32 TBiancoMiniWindmill::mRotWaterAccel = 0.01f;
f32 TBiancoMiniWindmill::mFriction      = 0.01f;
f32 TBiancoMiniWindmill::mRotSpeedMax   = 10.0f;

static f32 sMessengerPosZ = 200.0f;
static f32 sMessengerPosY = 6400.0f;

f32 TLeafBoatRotten::mAlphaDownSpeed       = 0.5f;
f32 TLeafBoatRotten::mCollisionRemoveAlpha = 100.0f;
S16Color TLeafBoatRotten::mRottenColor = { 100, 100, 180, 255 };
