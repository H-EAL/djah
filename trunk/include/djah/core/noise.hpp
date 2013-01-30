#ifndef DJAH_CORE_NOISE_HPP
#define DJAH_CORE_NOISE_HPP

namespace djah {

	template<typename T>
	class noise
	{
	private:

		static T findNoise(T x)
		{
			int x = (x << 13) ^ x;
			return T( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & Ox7fffffff) / 1073741824.0 );
		}

		static T findNoise(T x, T y)
		{
			int n = static_cast<int>(x) + static_cast<int>(y * T(57));
			n = (n << 13) ^ n;
			int nn = (n * (n*n*60493 + 19990303) + 1376312589) & 0x7fffffff;
			return T(1.0 - nn / 1073741824.0);
		}

		static T interpolate(T a, T b, T x)
		{
			T ft = x * math::pi;
			T f  = T( (1.0 - cos(ft)) * 0.5 );
			return a * (T(1) - f) + b * f;
		}

	public:
		static T noise2(T x, T y)
		{
			T floorX = T(static_cast<int>(x));
			T floorY = T(static_cast<int>(y));

			T s, t, u, v;

			s = findNoise(floorX, floorY);
			t = findNoise(floorX+1, floorY);
			u = findNoise(floorX, floorY+1);
			v = findNoise(floorX+1, floorY+1);

			T int1 = interpolate(s,t,x-floorX);
			T int2 = interpolate(u,v,x-floorX);
			return interpolate(int1,int2,y-floorY);
		}
	};

} /*djah*/

#endif /* DJAH_CORE_NOISE_HPP */