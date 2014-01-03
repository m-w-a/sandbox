//----------------------------------------------------------------------------//
//  Copyright Mostafa 2013. 
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//----------------------------------------------------------------------------//

#include <cstddef>
#include <boost/type_traits/remove_all_extents.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/utility.hpp>

namespace detail
{

//---------------
//flat_array_view
//---------------

template <typename T>
struct flat_array_view
{
  enum { total_extent = 1 };

  static T * begin(T & first_elem)
  {
    return boost::addressof(first_elem);
  }
};

template <typename T, std::size_t N>
struct flat_array_view<T[N]>
{
  enum { total_extent = N * flat_array_view<T>::total_extent };

  static typename boost::remove_all_extents<T>::type * begin(T (&arr)[N])
  {
    return flat_array_view<T>::begin(*arr);
  }
};

}

// Declared and purposefully not defined.
template <typename T>
class flat_array_view_range;

template <typename T, std::size_t N>
class flat_array_view_range<T[N]>
{
  typedef typename boost::remove_all_extents<T>::type value_type;
  typedef value_type 
    flat_array_view_type[detail::flat_array_view<T[N]>::total_extent];
public:
  typedef boost::sub_range<flat_array_view_type> type;
};

template <typename T, std::size_t N>
typename flat_array_view_range<T[N]>::type
  flat_array_view(T (&arr)[N])
{
  typename boost::remove_all_extents<T>::type * const begin = 
    detail::flat_array_view<T[N]>::begin(arr);
  
  std::size_t const total_extent = 
    detail::flat_array_view<T[N]>::total_extent;
  
  // Where is make_sub_range?
  return 
    typename flat_array_view_range<T[N]>::type(begin, begin + total_extent);
}


#include <iostream>
#include <boost/range.hpp>

int main()
{
  typedef int Arr[2][3];
  
  Arr arr1 = { {0, 1, 2}, {3, 4, 5} };
  Arr const arr2 = { {0, 1, 2}, {3, 4, 5} };
  
  std::cout 
    << "Test boost::equal: "
    << boost::equal(flat_array_view(arr1), flat_array_view(arr2))
    << '\n';
    
  std::cout << "Range size: " << flat_array_view(arr1).size() << '\n';
  std::cout << "Range 5th elem: " << flat_array_view(arr1)[4] << '\n';

  return 0;
}