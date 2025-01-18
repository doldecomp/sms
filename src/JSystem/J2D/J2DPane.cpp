#include <JSystem/J2D/J2DPane.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <JSystem/JUtility/JUTResource.hpp>
#include <dolphin/gx/GXGeometry.h>
#include <math.h>

J2DPane::J2DPane()
    : mBounds()
    , mGlobalBounds()
    , mClipRect()
    , mPaneTree(this)
{
	mInfoTag     = 16;
	mKind        = 'PAN1';
	mVisible     = true;
	mUserInfoTag = 0;
	mBounds.set(0, 0, 0, 0);

	mRotateOffsetY = mRotateOffsetX = 0;

	mRotAxis           = ROTATE_Z;
	mRotation          = 0.0f;
	mCullMode          = GX_CULL_NONE;
	mAlpha             = 255;
	mIsInfluencedAlpha = true;
	mColorAlpha        = 255;
	mConnected         = false;
}

J2DPane::J2DPane(J2DPane* p_pane, u16 tag, bool visible, u32 user_tag,
                 const JUTRect& bounds)
    : mBounds()
    , mGlobalBounds()
    , mClipRect()
    , mPaneTree(this)
{
	mInfoTag     = tag;
	mKind        = 'PAN1';
	mVisible     = visible;
	mUserInfoTag = user_tag;
	mBounds      = bounds;
	if (p_pane != nullptr) {
		p_pane->mPaneTree.appendChild(&mPaneTree);
	}

	mRotateOffsetY = mRotateOffsetX = 0;

	mRotAxis           = ROTATE_Z;
	mRotation          = 0.0f;
	mCullMode          = GX_CULL_NONE;
	mAlpha             = 255;
	mIsInfluencedAlpha = true;
	mColorAlpha        = 255;
	mConnected         = false;
}

J2DPane::J2DPane(u16 tag, u32 user_tag, const JUTRect& bounds)
    : mBounds()
    , mGlobalBounds()
    , mClipRect()
    , mPaneTree(this)
{
	mInfoTag     = tag;
	mKind        = 'PAN1';
	mVisible     = true;
	mUserInfoTag = user_tag;
	mBounds      = bounds;

	mRotateOffsetY = mRotateOffsetX = 0;

	mRotAxis           = ROTATE_Z;
	mRotation          = 0.0f;
	mCullMode          = GX_CULL_NONE;
	mAlpha             = 255;
	mIsInfluencedAlpha = true;
	mColorAlpha        = 255;
	mConnected         = false;
}

J2DPane::J2DPane(J2DPane* p_parent, JSURandomInputStream* p_stream, bool isEx)
    : mPaneTree(this)
{
	if (isEx) {
		p_stream->read(&mKind, sizeof(mKind));
		p_stream->skip(4);
		u8 fields;
		p_stream->read(&fields, sizeof(fields));
		p_stream->read(&mVisible, sizeof(mVisible));
		p_stream->skip(2);
		mUserInfoTag = p_stream->readU32();
		mBounds.x1   = p_stream->readS16();
		mBounds.y1   = p_stream->readS16();
		mBounds.x2   = mBounds.x1 + p_stream->readS16();
		mBounds.y2   = mBounds.y1 + p_stream->readS16();
		fields -= 6;
		if (fields != 0) {
			mRotation = p_stream->readU16();
			--fields;
		} else {
			mRotation = 0.0f;
		}

		if (fields != 0) {
			setBasePosition((J2DBasePosition)p_stream->readU8());
			--fields;
		} else {
			setBasePosition(J2DBasePosition_0);
		}

		if (fields != 0) {
			mAlpha = p_stream->readU8();
			--fields;
		} else {
			mAlpha = 0xff;
		}

		mIsInfluencedAlpha = true;
		if (fields != 0) {
			mIsInfluencedAlpha = p_stream->readU8();
			--fields;
		}

		p_stream->align(4);
	} else {
		p_stream->read(&mInfoTag, sizeof(mInfoTag));
		p_stream->read(&mVisible, sizeof(mVisible));
		p_stream->skip(1);
		mUserInfoTag = p_stream->readU32();
		mBounds.x1   = p_stream->readS16();
		mBounds.y1   = p_stream->readS16();
		mBounds.x2   = mBounds.x1 + p_stream->readS16();
		mBounds.y2   = mBounds.y1 + p_stream->readS16();
		setBasePosition(J2DBasePosition_0);
		mRotation          = 0.0f;
		mAlpha             = 0xff;
		mIsInfluencedAlpha = true;
	}

	if (p_parent != nullptr) {
		p_parent->mPaneTree.appendChild(&mPaneTree);
	}

	mCullMode   = GX_CULL_NONE;
	mColorAlpha = 0xff;
	mConnected  = false;
}

J2DPane::~J2DPane()
{
	JSUTreeIterator<J2DPane> iterator;
	for (iterator = mPaneTree.getFirstChild();
	     iterator != mPaneTree.getEndChild();) {
		J2DPane* child = (iterator++).getObject();
		delete child;
	}
}

void J2DPane::draw(int x, int y, J2DGrafContext const* p_grafCtx,
                   bool isOrthoGraf)
{
	J2DGrafContext tmpGraf(*p_grafCtx);

	if (tmpGraf.unk4 != 1)
		isOrthoGraf = false;

	JSUTree<J2DPane>* parentTree = mPaneTree.getParent();
	J2DPane* parent              = nullptr;
	if (parentTree != nullptr) {
		parent = parentTree->getObject();
	}

	if (mVisible && !mBounds.isEmpty()) {
		mGlobalBounds = mBounds;
		mClipRect     = mBounds;

		makeMatrix(mBounds.x1 + x, mBounds.y1 + y);
		if (parent != nullptr) {
			mGlobalBounds.add(parent->mGlobalBounds.x1,
			                  parent->mGlobalBounds.y1);
			MTXConcat(parent->mGlobalMtx, mPositionMtx, mGlobalMtx);

			mClipRect.add(parent->mGlobalBounds.x1, parent->mGlobalBounds.y1);
			mClipRect.intersect(parent->mClipRect);

			mColorAlpha = mAlpha;
			if (mIsInfluencedAlpha) {
				mColorAlpha = (mAlpha * parent->mColorAlpha) / 255;
			}
		} else {
			mGlobalBounds.add(x, y);
			MTXCopy(mPositionMtx, mGlobalMtx);
			mClipRect.add(x, y);
			mColorAlpha = mAlpha;
		}

		if (isOrthoGraf) {
			((J2DOrthoGraph*)p_grafCtx)
			    ->scissorBounds(&mScissorBounds, &mClipRect);
		}

		if (!mClipRect.isEmpty() || !isOrthoGraf) {
			tmpGraf.place(mGlobalBounds);
			if (isOrthoGraf) {
				tmpGraf.scissor(mScissorBounds);
				tmpGraf.setScissor();
			}
			GXSetCullMode(mCullMode);
			drawSelf(x, y, &tmpGraf.mPosMtx);

			for (JSUTreeIterator<J2DPane> iter = mPaneTree.getFirstChild();
			     iter != mPaneTree.getEndChild(); ++iter) {
				iter.getObject()->draw(0, 0, p_grafCtx, isOrthoGraf);
			}
		}
	}
}

void J2DPane::clip(const JUTRect& bounds)
{
	JUTRect box(bounds);
	box.add(mGlobalBounds.x1, mGlobalBounds.y1);
	mClipRect.intersect(box);
}

J2DPane* J2DPane::search(u32 tag)
{
	if (tag == mUserInfoTag) {
		return this;
	}

	for (JSUTreeIterator<J2DPane> iter = mPaneTree.getFirstChild();
	     iter != mPaneTree.getEndChild(); ++iter) {
		if (J2DPane* result = iter.getObject()->search(tag)) {
			return result;
		}
	}
	return nullptr;
}

void J2DPane::makeMatrix(int x, int y)
{
	Mtx transBack, rotate, trans;
	if (mRotation != 0.0f) {
		MTXTrans(transBack, -mRotateOffsetX, -mRotateOffsetY, 0.0f);
		float r = mRotAxis == 'z' ? -mRotation : mRotation;
		MTXRotRad(rotate, mRotAxis, DEG_TO_RAD(r));
		MTXTrans(trans, mRotateOffsetX + x, mRotateOffsetY + y, 0.0f);
		MTXConcat(rotate, transBack, mPositionMtx);
		MTXConcat(trans, mPositionMtx, mPositionMtx);
	} else {
		MTXTrans(mPositionMtx, x, y, 0.0f);
	}
}

void J2DPane::setCullBack(GXCullMode mode)
{
	mCullMode = mode;

	for (JSUTreeIterator<J2DPane> iter = mPaneTree.getFirstChild();
	     iter != mPaneTree.getEndChild(); ++iter) {
		iter.getObject()->setCullBack(mode);
	}
}

void J2DPane::setBasePosition(J2DBasePosition position)
{
	mBasePosition = position;

	if (position % 3 == 0) {
		mRotateOffsetX = 0;
	} else if (position % 3 == 1) {
		mRotateOffsetX = getWidth() / 2;
	} else {
		mRotateOffsetX = getWidth();
	}

	if (position / 3 == 0) {
		mRotateOffsetY = 0;
	} else if (position / 3 == 1) {
		mRotateOffsetY = getHeight() / 2;
	} else {
		mRotateOffsetY = getHeight();
	}

	mRotAxis = ROTATE_Z;
}
