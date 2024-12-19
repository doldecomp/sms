#ifndef JGEOMETRY_HPP
#define JGEOMETRY_HPP

#include <dolphin/types.h>
#include <dolphin/vec.h>

namespace JGeometry {

	template<typename T>
	struct TUtil;

	template<>
	struct TUtil<float> {
			static float inv_sqrt(float);
	};

	template <typename T> class TVec3 {
		public:
    	TVec3() { x = 0; y = 0; z = 0; }
			TVec3(const TVec3& other) { x = other.x; y = other.y; z = other.z; }

			TVec3(T x_, T y_, T z_) { x = x_; y = y_; z = z_; }

			template <typename TY> TVec3(TY x_, TY y_, TY z_) { x = x_; y = y_; z = z_; }

			TVec3& operator=(const TVec3& other) { x = other.x; y = other.y; z = other.z; }
			TVec3& operator*=(const TVec3& operand);
			TVec3& operator-=(const TVec3& operand);

			void add(const TVec3& operand);
			void div(f32 divisor);
			T dot(const TVec3<T>& other) const
			{
					return x*other.x + y*other.y + z*other.z;
			}
			void negate();
			void scale(f32 scale);
			void scale(f32 scale, const TVec3& operand);
			void scaleAdd(f32 scale, const TVec3& operand, const TVec3& translate);
			void set(const Vec&);

			template <typename TY> void set(TY x, TY y, TY z);
			template <typename TY> void set(const TVec3<TY>&);

			void setLength(const TVec3<T>& v, f32 length)
			{
					const f32 lsq = v.dot(v);
					if (lsq <= 0.0000038146973f) {
							x = y = z = 0;
					}
					else {
							float invLen = length * JGeometry::TUtil<f32>::inv_sqrt(lsq);
							x = v.x*invLen;
							y = v.y*invLen;
							z = v.z*invLen;
					}
			}

			void cross(const TVec3<T>& fst, const TVec3<T>& snd)
			{
					x = fst.y*snd.z-fst.z*snd.y;
					y = fst.z*snd.x-fst.x*snd.z;
					z = fst.x*snd.y-fst.y*snd.x;
			}

			void setMax(const TVec3& other);
			void setMin(const TVec3& other);
			void sub(const TVec3& translate);
			void sub(const TVec3& base, const TVec3& translate);

			T x;
			T y;
			T z;
	};


	template<typename T>
	struct SMatrix34C {
			T mMtx[3][4];
	};

	// NOTE: this HAS to be an explicit specialization for function prologue & epilogue to match,
	// and this HAS to have 8 byte alignment (but the attribute seems to be unnecessary, mwcc aligns anyways)
	template<>
	struct SMatrix34C<f32> {
			f32 mMtx[3][4] __attribute__((aligned(8)));
	};

	template<typename T>
	struct TMatrix34 : public T {
	};
}

typedef JGeometry::SMatrix34C<f32> TSMtxf;
typedef JGeometry::TMatrix34<TSMtxf> TMtx34f;

#endif
