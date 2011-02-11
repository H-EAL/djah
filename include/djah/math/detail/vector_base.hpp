#ifndef DJAH_MATH_DETAIL_VECTOR_BASE_HPP
#define DJAH_MATH_DETAIL_VECTOR_BASE_HPP

#include <ostream>
#include <cassert>
#include <iterator>

namespace djah { namespace math { namespace detail {

	template<size_t SIZE, typename T>
	class vector_base2
	{
	public:

		// Useful type definition
		typedef T value_type;
		static const size_t size_ = SIZE;

		// All data to default value
		vector_base2();
		// Init with an array
		vector_base2(const T (&array)[SIZE]);
		// Meant to be a base class
		virtual ~vector_base2() {};

		// Returns a const pointer to data array
		const T* data() const;

		// Vector member access
		const T& operator [](size_t i) const;
		T& operator [](size_t i);

		// Iterators types
		typedef       T *iterator;
		typedef const T *iterator_to_const;
		// Iterators
		iterator begin();
		iterator end();
		// Iterators to const
		iterator_to_const begin() const;
		iterator_to_const end()   const;

		// Comparison operators
		bool operator ==(const vector_base2<SIZE,T> &rhs) const
		{
			return std::equal(begin(), end(), rhs.data_);
		}
		bool operator !=(const vector_base2<SIZE,T> &rhs) const
		{
			return !(*this == rhs);
		}

		// Feed output stream with vector data
		template<size_t SIZE_, typename T_>
		friend std::ostream& operator <<(std::ostream &out, const vector_base2<SIZE_,T_> &v);


	protected:

		// Address of at a specified location
		T* at(size_t i);
		const T* at(size_t i) const;

		// Feed output stream with data
		virtual void print(std::ostream &out) const;

		// Conversion operator
		operator const T*() const;


		// Vector data
		T data_[SIZE];
	};

} /*detail*/ } /*math*/ } /*djah*/

#include "vector_base.inl"

#endif /* DJAH_MATH_DETAIL_VECTOR_BASE_HPP */