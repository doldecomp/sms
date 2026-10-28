#include <GC2D/Menu.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <System/MarioGamePad.hpp>
#include <dolphin/gx.h>

void TMenuBase::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW) {
		J2DOrthoGraph orthoGraph(graphics->getViewport());
		orthoGraph.setup2D();
		mScreen->draw(0, 0, &orthoGraph);
		const JUTRect& rect = graphics->getScissor();
		GXSetScissor(rect.x1, rect.y1, rect.getWidth(), rect.getHeight());
	}
}

TMenuPlane::TMenuPlane(const TMarioGamePad* param_1, J2DPane* param_2,
                       u32 param_3, u32 param_4)
    : JDrama::TViewObj("<TMenuPlane>")
    , mGamePad(param_1)
    , mRootPane(param_2)
    , mFlags(0)
    , mSelectedCharColor(255, 60, 0, 255)
    , mSelectedGradColor(255, 255, 0, 255)
    , mNormalColor(255, 255, 255, 255)
    , mItemCount(0)
    , mSelectedIndex(0)
    , mItems(nullptr)
    , mConfirmInputMask(param_3)
    , mCancelInputMask(param_4)
    , mColumnStride(0)
{
	J2DTextBox* foundTextBoxes[256];

	JSUTreeIterator<J2DPane> iterator;
	for (iterator = mRootPane->getPaneTree()->getFirstChild();
	     iterator != mRootPane->getPaneTree()->getEndChild(); ++iterator) {
		if (iterator->getTypeID() == 0x13
		    && iterator->getUserInfo() != 'rset') {
			J2DTextBox* textBox        = (J2DTextBox*)iterator.getObject();
			foundTextBoxes[mItemCount] = textBox;
			if (mItemCount == 0) {
				mNormalColor = textBox->getCharColor();
				textBox->setCharColor(mSelectedCharColor);
				textBox->setGradColor(mSelectedGradColor);
			}
			++mItemCount;
		}
	}

	mItems = new J2DTextBox*[mItemCount];
	for (int i = 0; i < mItemCount; ++i)
		mItems[i] = foundTextBoxes[i];
}

void TMenuPlane::perform(u32 cue, JDrama::TGraphics*)
{
	if (mFlags & FLAG_INPUT_DISABLED)
		return;

	if (cue & CUE_MOVE) {
		if (mGamePad->checkFrameMeaning(mConfirmInputMask)) {
			mFlags |= FLAG_CONFIRMED;
			return;
		}

		if (mGamePad->checkFrameMeaning(mCancelInputMask)) {
			mFlags |= FLAG_CANCELLED;
			return;
		}

		if (mItemCount > 1
		    && mGamePad->checkFrameMeaning(
		        TMarioGamePad::MEANING_MENU_UP
		        | TMarioGamePad::MEANING_MENU_DOWN
		        | TMarioGamePad::MEANING_MENU_LEFT
		        | TMarioGamePad::MEANING_MENU_RIGHT)) {
			mItems[mSelectedIndex]->setCharColor(mNormalColor);
			mItems[mSelectedIndex]->setGradColor(mNormalColor);
			if (mGamePad->checkFrameMeaning(
			        TMarioGamePad::MEANING_MENU_LEFT
			        | TMarioGamePad::MEANING_MENU_RIGHT)) {
				if (mSelectedIndex < mColumnStride) {
					if (mItemCount > mSelectedIndex + mColumnStride) {
						mSelectedIndex += mColumnStride;
					}
				} else {
					mSelectedIndex -= mColumnStride;
				}
			}

			if (mGamePad->checkFrameMeaning(TMarioGamePad::MEANING_MENU_UP)) {
				{
					if (mSelectedIndex == 0)
						mSelectedIndex = mItemCount;
					mSelectedIndex -= 1;
				}
			} else if (mGamePad->checkFrameMeaning(
			               TMarioGamePad::MEANING_MENU_DOWN)) {
				mSelectedIndex += 1;
				if (mSelectedIndex >= mItemCount)
					mSelectedIndex = 0;
			}

			mItems[mSelectedIndex]->setCharColor(mSelectedCharColor);
			mItems[mSelectedIndex]->setGradColor(mSelectedGradColor);
		}
	}
}

void TFlashPane::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_MOVE) {
		mAlpha += mAlphaStep;
		if (mAlphaStep < 0) {
			if (mAlpha <= 0) {
				mAlpha     = 0;
				mAlphaStep = 4;
			}
		} else if (mAlpha >= 255) {
			mAlpha     = 255;
			mAlphaStep = -4;
		}

		mPane->setAlpha(mAlpha);
	}
}
