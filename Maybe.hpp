#include <type_traits>

namespace maybe
{

  struct Nothing {};

  template < typename T >
  struct Maybe
  {
    template < typename U, typename Enable = void >
    struct ValueType
    {
      typedef U * const type;
    };

    template < typename U >
    struct ValueType < U, typename std::enable_if < std::is_reference < U >::value >::type >
    {
      typedef typename std::remove_reference < T >::type * const type;
    };

    typedef typename ValueType < T >::type value_type;

    value_type m_v;

    Maybe(Nothing const &) : m_v(0)
    {
    }

    struct Just
    {
      value_type m_v;
      Just() = delete;
      explicit Just(T &v) : m_v(&v)
      {
      }
    };

    Maybe(Just const &just) : m_v(just.m_v)
    {
    }
  };

  Nothing nothing()
  {
    return Nothing();
  }

  template < typename T >
  Maybe < T > just(T &v)
  {
    return typename Maybe < T >::Just(v);
  }

  template < typename T >
  Maybe < T const > just(T const &v)
  {
    return typename Maybe < T const >::Just(v);
  }

  template < typename T, typename R, typename A >
  Maybe < R > operator | (Maybe < T > const &t, R (*f)(A const &))
  {
    if (t.m_v)
      return just < R >(f(*t.m_v));
    else
      return nothing();
  }

  template < typename T, typename R, typename A >
  Maybe < R > operator | (Maybe < T > const &t, Maybe < R > (*f)(A const &))
  {
    if (t.m_v)
      return f(*t.m_v);
    else
      return nothing();
  }

  template < typename T, typename R, typename A >
  Maybe < R > operator | (Maybe < T > const &t, R (*f)(A &))
  {
    if (t.m_v)
      return just < R >(f(*t.m_v));
    else
      return nothing();
  }

  template < typename T, typename R, typename A >
  Maybe < R > operator | (Maybe < T > const &t, Maybe < R > (*f)(A &))
  {
    if (t.m_v)
      return f(*t.m_v);
    else
      return nothing();
  }

  template < typename T, typename R, typename... A >
  Maybe < R > operator | (Maybe < T const > const &t, R (T::*f)(A const &...) const)
  {
    if (t.m_v)
      return just < R >(((*t.m_v).*f)());
    else
      return nothing();
  }

  template < typename T, typename R, typename... A >
  Maybe < R > operator | (Maybe < T const > const &t, Maybe < R > (T::*f)(A const &...) const)
  {
    if (t.m_v)
      return just < R >((t.m_v->*f)());
    else
      return nothing();
  }

  template < typename T, typename R, typename... A >
  Maybe < R > operator | (Maybe < T const > const &t, R (T::*f)(A const &...))
  {
    if (t.m_v)
      return just < R >(((*t.m_v).*f)());
    else
      return nothing();
  }

  template < typename T, typename R, typename... A >
  Maybe < R > operator | (Maybe < T const > const &t, Maybe < R > (T::*f)(A const &...))
  {
    if (t.m_v)
      return just < R >((t.m_v->*f)());
    else
      return nothing();
  }

  template < typename T, typename A >
  void operator | (Maybe < T > const &t, void (*f)(A const &))
  {
    if (t.m_v)
      f(*t.m_v);
  }

}

