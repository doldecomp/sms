#ifndef JDR_VIEW_OBJ_HPP
#define JDR_VIEW_OBJ_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>

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

	CUE_ALL = 0xffffffff,
};

namespace JDrama {

/**
 * @brief A scene graph object that can perform per-frame work.
 *
 * @details A cue system is used, real-life drama theater being the analogy.
 * A director (JDrama::TDirector) has a particular "script" for the play in
 * mind and gives the actors on the scene "cues" according to the script.
 * Actors react to the cues by acting. For example, CUE_MOVE is used to make
 * actors update their positioning and other gameplay logic, CUE_CALC_ANIM
 * to make the actors calculate things like skeletal animations, and
 * CUE_DRAW is used to make actors emit GX commands.
 * JDrama defines a shared set of cues, but individual directors might
 * define additional cue types.
 */
class TViewObj : public TNameRef {
public:
	TViewObj(const char* name = "<TViewObj>")
	    : TNameRef(name)
	{
	}

	/**
	 * @brief Main per-tick function. Called by the current direct or parent
	 * objects, potentially multiple times per frame with different cues.
	 *
	 * @param cue The cues which the director wants the actor to perform.
	 * @param graphics A channel for cross-actor communication.
	 */
	virtual void perform(u32 cue, TGraphics* graphics) = 0;

	/**
	 * @brief Wrapper around TViewObj::perform that filters cues with the unkC
	 * filter field.
	 *
	 * @param cue Desired cues, will get filtered by unkC.
	 * @param graphics A channel for cross-actor communication.
	 */
	void testPerform(u32 cue, TGraphics* graphics);

public:
	/* 0xC */ TFlagT<u16> unkC;
};

}; // namespace JDrama

#endif
