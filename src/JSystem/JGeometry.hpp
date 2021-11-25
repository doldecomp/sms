#ifndef JGEOMETRY_HPP
#define JGEOMETRY_HPP value

namespace JGeometry {
	template <typename T> class TVec3 {
		public:
			TVec3();
			TVec3(const TVec3&);

			template <typename TY> TVec3(TY, TY, TY);

			TVec3& operator=(const TVec3&);
			TVec3& operator*=(const TVec3& operand);
			TVec3& operator-=(const TVec3& operand);

			void add(const TVec3& operand);
			void div(f32 divisor);
			f32 dot(const TVec3& operand) const;
			void negate();
			void scale(f32 scale);
			void scale(f32 scale, const TVec3& operand);
			void scaleAdd(f32 scale, const TVec3& operand, const TVec3& translate);
			void set(const Vec&);

			template <typename TY> void set(TY x, TY y, TY z);
			template <typename TY> void set(const TVec3<TY>&);

			void setLength(const TVec3& operand, f32 length);
			void setMax(const TVec3& other);
			void setMin(const TVec3& other);
			void sub(const TVec3& translate);
			void sub(const TVec3& base, const TVec3& translate);

			T x;
			T y;
			T z;
	};
};

#endif
