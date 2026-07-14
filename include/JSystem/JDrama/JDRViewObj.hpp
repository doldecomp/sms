#ifndef JDR_VIEW_OBJ_HPP
#define JDR_VIEW_OBJ_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-list.hpp>

enum {
	/// Gameplay logic
	CUE_MOVE = 0x1,
	/// Animations
	CUE_CALC_ANIM = 0x2,
	/// View-dependent calculations
	CUE_CALC_VIEW = 0x4,
	/// GX commands
	CUE_DRAW = 0x8,
	/// Commit the projection matrix to GX
	CUE_SET_PROJECTION = 0x10,
	/// Commit lights to GX
	CUE_LIGHT = 0x20,
	// TODO: uncertain
	CUE_DRAW_INIT = 0x80,
	// TODO: uncertain
	CUE_DRAW_STAGE_END = 0x100,
	/// "Enter" models into draw buffers
	CUE_ENTRY = 0x200,
	/// Set draw buffers to global state
	CUE_SET_DRAW_BUFFER = 0x400,
	// TODO: uncertain
	CUE_MOVEMENT_GATE_A = 0x1000,
	// TODO: uncertain
	CUE_MOVEMENT_GATE_B = 0x2000,
};

namespace JDrama {

class TViewObj : public TNameRef {
public:
	TViewObj(const char* name = "<TViewObj>")
	    : TNameRef(name)
	{
	}

	void testPerform(u32 cue, TGraphics* graphics);

	virtual void perform(u32 cue, TGraphics* graphics) = 0;

public:
	/* 0xC */ TFlagT<u16> unkC;
};

}; // namespace JDrama

#endif
