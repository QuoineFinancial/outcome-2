/* monad_policy.ipp
Configures basic_monad as an option<T>, result<T> and outcome<T>.
(C) 2015-2017 Niall Douglas http://www.nedprod.com/
File Created: July 2015


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_OUTCOME_MONAD_NAME
#error BOOST_OUTCOME_MONAD_NAME needs to be defined
#endif
#define BOOST_OUTCOME_GLUE2(a, b) a##b
#define BOOST_OUTCOME_GLUE(a, b) BOOST_OUTCOME_GLUE2(a, b)
#ifndef BOOST_OUTCOME_MONAD_POLICY_NAME
#define BOOST_OUTCOME_MONAD_POLICY_NAME BOOST_OUTCOME_GLUE(BOOST_OUTCOME_MONAD_NAME, _policy)
#endif
#ifndef BOOST_OUTCOME_MONAD_POLICY_BASE_NAME
#define BOOST_OUTCOME_MONAD_POLICY_BASE_NAME BOOST_OUTCOME_GLUE(BOOST_OUTCOME_MONAD_NAME, _policy_base)
#endif

namespace policy
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4800)  // forcing value to bool
#pragma warning(disable : 4702)  // unreachable code
#endif
  //! \brief outcome<T>, result<T> and option<T> personalisation of basic_monad
  template <class monad_storage, class value_type, class error_type = void, class exception_type = void> struct BOOST_OUTCOME_MONAD_POLICY_BASE_NAME : public monad_storage
  {
  protected:
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME() = delete;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME(const BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &) = delete;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME(BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &&) = delete;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &operator=(const BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &) = default;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &operator=(BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &&) = default;
    struct passthru_t {};
    template <class... Args>
    constexpr BOOST_OUTCOME_MONAD_POLICY_BASE_NAME(passthru_t, Args &&... args)
      : monad_storage(std::forward<Args>(args)...)
    {
    }
    // Must handle error situation ec. Can return false to cancel the calling operation.
    static BOOST_OUTCOME_CONSTEXPR bool _throw_error(monad_errc ec) { return BOOST_OUTCOME_THROW_MONAD_ERROR(ec, monad_error(ec)); }
    // Common preamble to the below
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void _pre_get_value() const
    {
      if(!monad_storage::is_ready())
        _throw_error(monad_errc::no_state);
#if defined(BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE) || defined(BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE)
      if(monad_storage::has_error() || monad_storage::has_exception())
      {
#ifdef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
        if(monad_storage::has_error())
          BOOST_OUTCOME_THROW_SYSTEM_ERROR(monad_storage::_storage.error, stl11::system_error(monad_storage::_storage.error));
#endif
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
        if(monad_storage::has_exception())
          BOOST_OUTCOME_RETHROW_EXCEPTION(monad_storage::_storage.exception);
#endif
      }
#endif
    }
    // If storage is packed into a byte, it cannot be referenced
    using lvalue_type = typename std::conditional<monad_storage::value_storage_type::is_referenceable, value_type &, value_type>::type;
    using const_lvalue_type = typename std::conditional<monad_storage::value_storage_type::is_referenceable, const value_type &, value_type>::type;
    using rvalue_type = typename std::conditional<monad_storage::value_storage_type::is_referenceable, value_type &&, value_type>::type;
    using const_rvalue_type = typename std::conditional<monad_storage::value_storage_type::is_referenceable, const value_type &&, value_type>::type;

  public:
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const auto *operator-> () const
    {
      _pre_get_value();
      return &monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE auto *operator-> ()
    {
      _pre_get_value();
      return &monad_storage::_storage.value;
    }

    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE lvalue_type operator*() &
    {
      _pre_get_value();
      return monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE lvalue_type get() &
    {
      _pre_get_value();
      return monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE lvalue_type value() &
    {
      _pre_get_value();
      return monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const_lvalue_type operator*() const &
    {
      _pre_get_value();
      return monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const_lvalue_type get() const &
    {
      _pre_get_value();
      return monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const_lvalue_type value() const &
    {
      _pre_get_value();
      return monad_storage::_storage.value;
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE rvalue_type operator*() &&
    {
      _pre_get_value();
      return move_if<monad_storage::value_storage_type::is_referenceable, value_type>()(monad_storage::_storage.value);
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE rvalue_type get() &&
    {
      _pre_get_value();
      return move_if<monad_storage::value_storage_type::is_referenceable, value_type>()(monad_storage::_storage.value);
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE rvalue_type value() &&
    {
      _pre_get_value();
      return move_if<monad_storage::value_storage_type::is_referenceable, value_type>()(monad_storage::_storage.value);
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const_rvalue_type operator*() const &&
    {
      _pre_get_value();
      return move_if<monad_storage::value_storage_type::is_referenceable, value_type>()(monad_storage::_storage.value);
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const_rvalue_type get() const &&
    {
      _pre_get_value();
      return move_if<monad_storage::value_storage_type::is_referenceable, value_type>()(monad_storage::_storage.value);
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE const_rvalue_type value() const &&
    {
      _pre_get_value();
      return move_if<monad_storage::value_storage_type::is_referenceable, value_type>()(monad_storage::_storage.value);
    }
#ifdef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE error_type get_error() const
    {
      if(!monad_storage::is_ready())
      {
        if(!_throw_error(monad_errc::no_state))
          return error_type();
      }
      if(monad_storage::has_error())
        return monad_storage::_storage.error;
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
      if(monad_storage::has_exception())
        return error_type((int) monad_errc::exception_present, monad_category());
#endif
      return error_type();
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE error_type error() const { return get_error(); }
#endif
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE exception_type get_exception() const
    {
      if(!monad_storage::is_ready())
      {
        if(!_throw_error(monad_errc::no_state))
          return exception_type();
      }
      if(!monad_storage::has_error() && !monad_storage::has_exception())
        return exception_type();
      if(monad_storage::has_error())
        return std::make_exception_ptr(stl11::system_error(monad_storage::_storage.error));
      if(monad_storage::has_exception())
        return monad_storage::_storage.exception;
      return exception_type();
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE exception_type exception() const { return get_exception(); }
#endif
  };
  template <class monad_storage, class error_type, class exception_type> struct BOOST_OUTCOME_MONAD_POLICY_BASE_NAME<monad_storage, void, error_type, exception_type> : public monad_storage
  {
  protected:
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME() = delete;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME(const BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &) = delete;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME(BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &&) = delete;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &operator=(const BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &) = default;
    BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &operator=(BOOST_OUTCOME_MONAD_POLICY_BASE_NAME &&) = default;
    struct passthru_t {};
    template <class... Args>
    constexpr BOOST_OUTCOME_MONAD_POLICY_BASE_NAME(passthru_t, Args &&... args)
      : monad_storage(std::forward<Args>(args)...)
    {
    }
    // Must handle error situation ec. Can return false to cancel the calling operation.
    static BOOST_OUTCOME_CONSTEXPR bool _throw_error(monad_errc ec) { return BOOST_OUTCOME_THROW_MONAD_ERROR(ec, monad_error(ec)); }
    // Common preamble to the below
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void _pre_get_value() const
    {
      if(!monad_storage::is_ready())
        _throw_error(monad_errc::no_state);
#if defined(BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE) || defined(BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE)
      if(monad_storage::has_error() || monad_storage::has_exception())
      {
#ifdef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
        if(monad_storage::has_error())
          BOOST_OUTCOME_THROW_SYSTEM_ERROR(monad_storage::_storage.error, stl11::system_error(monad_storage::_storage.error));
#endif
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
        if(monad_storage::has_exception())
          BOOST_OUTCOME_RETHROW_EXCEPTION(monad_storage::_storage.exception);
#endif
      }
#endif
    }

  public:
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void operator*() & { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void get() & { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void value() & { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void operator*() const & { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void get() const & { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void value() const & { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void operator*() && { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void get() && { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void value() && { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void operator*() const && { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void get() const && { _pre_get_value(); }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE void value() const && { _pre_get_value(); }
#ifdef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE error_type get_error() const
    {
      if(!monad_storage::is_ready())
      {
        if(!_throw_error(monad_errc::no_state))
          return error_type();
      }
      if(monad_storage::has_error())
        return monad_storage::_storage.error;
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
      if(monad_storage::has_exception())
        return error_type((int) monad_errc::exception_present, monad_category());
#endif
      return error_type();
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE error_type error() const { return get_error(); }
#endif
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE exception_type get_exception() const
    {
      if(!monad_storage::is_ready())
      {
        if(!_throw_error(monad_errc::no_state))
          return exception_type();
      }
      if(!monad_storage::has_error() && !monad_storage::has_exception())
        return exception_type();
      if(monad_storage::has_error())
        return std::make_exception_ptr(stl11::system_error(monad_storage::_storage.error));
      if(monad_storage::has_exception())
        return monad_storage::_storage.exception;
      return exception_type();
    }
    BOOST_OUTCOME_CONSTEXPR BOOSTLITE_FORCEINLINE exception_type exception() const { return get_exception(); }
#endif
  };

  //! \brief An implementation policy for basic_monad implementing outcome<T>, result<T> and option<T>
  template <typename R> struct BOOST_OUTCOME_MONAD_POLICY_NAME
  {
    // The final resulting implementation type
    typedef basic_monad<BOOST_OUTCOME_MONAD_POLICY_NAME> implementation_type;
    // The value type to use. Can be void to disable.
    typedef R value_type;
// The error code type to use. Can be void to disable.
#ifdef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
    typedef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE error_type;
#else
    typedef void error_type;
#endif
// The exception pointer type to use. Can be void to disable.
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
    typedef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE exception_type;
#else
    typedef void exception_type;
#endif

    // Ought the monad be default constructible?
    static constexpr bool is_default_constructible = true;
    // Is default construction non-throwing?
    static constexpr bool is_nothrow_default_constructible = true;

    // The base class to use to store state
    typedef BOOST_OUTCOME_MONAD_POLICY_BASE_NAME<basic_monad_storage<BOOST_OUTCOME_MONAD_POLICY_NAME>, value_type, error_type, exception_type> base;

    // The type which basic_monad::rebind<U> should return
    template <typename U> using rebind = basic_monad<BOOST_OUTCOME_MONAD_POLICY_NAME<U>>;
    // The type which rebinding myself produces
    template <typename U> using rebind_policy = BOOST_OUTCOME_MONAD_POLICY_NAME<U>;
  };
  template <> struct BOOST_OUTCOME_MONAD_POLICY_NAME<void>
  {
    // The final resulting implementation type
    typedef basic_monad<BOOST_OUTCOME_MONAD_POLICY_NAME> implementation_type;
    // The value type to use. Can be void to disable.
    typedef void value_type;
// The error code type to use. Can be void to disable.
#ifdef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
    typedef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE error_type;
#else
    typedef void error_type;
#endif
// The exception pointer type to use. Can be void to disable.
#ifdef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
    typedef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE exception_type;
#else
    typedef void exception_type;
#endif

    // Ought the monad be default constructible?
    static constexpr bool is_default_constructible = true;
    // Is default construction non-throwing?
    static constexpr bool is_nothrow_default_constructible = true;

    // The base class to use to store state
    typedef BOOST_OUTCOME_MONAD_POLICY_BASE_NAME<basic_monad_storage<BOOST_OUTCOME_MONAD_POLICY_NAME>, value_type, error_type, exception_type> base;

    // The type which basic_monad::rebind<U> should return
    template <typename U> using rebind = basic_monad<BOOST_OUTCOME_MONAD_POLICY_NAME<U>>;
    // The type which rebinding myself produces
    template <typename U> using rebind_policy = BOOST_OUTCOME_MONAD_POLICY_NAME<U>;
  };
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#undef BOOST_OUTCOME_GLUE
#undef BOOST_OUTCOME_GLUE2
#undef BOOST_OUTCOME_PROMISE_NAME
#undef BOOST_OUTCOME_MONAD_NAME
#undef BOOST_OUTCOME_MONAD_POLICY_NAME
#undef BOOST_OUTCOME_MONAD_POLICY_ERROR_TYPE
#undef BOOST_OUTCOME_MONAD_POLICY_EXCEPTION_TYPE
