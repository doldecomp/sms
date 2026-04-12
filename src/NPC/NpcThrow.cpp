#include <NPC/NpcThrow.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JMath.hpp>
#include <Strategic/HitActor.hpp>
#include <Camera/cameralib.hpp>
#include <Player/MarioAccess.hpp>

void TNpcThrow::throwMario(THitActor* param_1)
{
	JGeometry::TVec3<f32> local_14;
	if (unk4 >= 90.0f) {
		local_14.set(0.0f, 1.0f, 0.0f);
	} else if (unk4 <= 0.0f) {
		local_14.set(0.0f, 0.0f, -1.0f);
	} else {
		s16 angle = CLBDegToShortAngle(unk4);
		local_14.set(0.0f, JMASSin(angle), -JMASCos(angle));
	}

	f32 yaw  = param_1->mRotation.y;
	s16 sYaw = CLBDegToShortAngle(yaw);

	f32 x = local_14.x;

	local_14.x = x * JMASCos(sYaw) + local_14.z * JMASSin(sYaw);
	local_14.z = -x * JMASSin(sYaw) + local_14.z * JMASCos(sYaw);

	SMS_SendMessageToMario(param_1, HIT_MESSAGE_UNK7);
	SMS_ThrowMario(local_14, unk0);
}
