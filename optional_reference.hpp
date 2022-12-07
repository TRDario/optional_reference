/// @file
/// @author Dario Cvitanović
/// 
/// @section License
///
/// Copyright (c) 2022 Dario Cvitanović
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
/// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software,
/// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
///
/// The above copyright noticeand this permission notice shall be included in all copies or
/// substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
/// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
/// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
/// 
/// @section Description
/// 
/// This file contains an implementation of and several helper functions related to class optional_reference,
/// a simple wrapper around a raw pointer with an interface similar to C++17's std::optional.
/// The purpose is to be an idiomatic and safe alternative to classical non-owning raw pointers without
/// needing to resort to clunky alternatives such as std::optional<std::reference_wrapper>.

#if __cplusplus < 201402L
static_assert(false, "optional_reference requires C++14 or higher.");
#endif

#ifndef DL_OPTIONAL_REFERENCE_HPP
#define DL_OPTIONAL_REFERENCE_HPP

#include <stdexcept>
#include <memory>
#ifndef NDEBUG
#include <cassert>
#endif


namespace dl {

  /// Empty class type used to indicate an optional_reference with an uninitialized state.
  struct nullref_t {};

  /// Constant of type nullref_t used to indicate an optional_reference with an uninitialized state.
  inline constexpr nullref_t nullref;


  /// Exception type thrown by optional_reference::ref when accessing an empty optional_reference.
  class bad_optional_reference_access : public std::exception {

  public:
    bad_optional_reference_access() noexcept {}

    const char* what() const noexcept {
      return "bad optional reference access";
    }

  };


  /// Raw pointer wrapper with std::optional-like semantics and safety against null dereferencing.
  template <class T>
  class optional_reference {

  public:
    /// Constructs an object that does not contain a reference.
    constexpr optional_reference() noexcept
      : m_ptr(nullptr) {}

    /// Constructs an object that does not contain a reference.
    constexpr optional_reference(nullref_t) noexcept
      : m_ptr(nullptr) {}

    /// @brief Constructs an object containing a reference.
    /// @param reference - the reference to be stored in the object.
    constexpr optional_reference(T& reference) noexcept
      : m_ptr(std::addressof(reference)) {}


    /// Returns true if *this contains a reference, false if *this does not contain a reference.
    constexpr explicit operator bool() const noexcept {
      return m_ptr;
    }

    /// Returns true if *this contains a reference, false if *this does not contain a reference.
    constexpr bool has_ref() const noexcept {
      return m_ptr;
    }


    /// @brief Returns the contained reference.
    /// @details Unlike optional_reference::ref, this operator is unchecked and will
    /// unapologetically dereference a null pointer without throwing an exception.
    constexpr T& operator*() const noexcept {
      #ifndef NDEBUG
      assert(m_ptr);
      #endif
      return *m_ptr;
    }

    /// @brief Returns the contained reference.
    /// @details Unlike operator*, This function performs a check for null and will throw
    /// a bad_optional_reference_access exception if *this is empty.
    constexpr T& ref() const {
      if (!m_ptr) throw bad_optional_reference_access();
      return *m_ptr;
    }

    /// @brief Returns the contained reference as a pointer.
    /// @details Note that this function is unchecked and can return nullptr if *this is empty.
    constexpr T* operator->() const noexcept {
      #ifndef NDEBUG
      assert(m_ptr);
      #endif
      return m_ptr;
    }


    /// If *this contains a reference, resets it to being empty.
    constexpr void reset() const noexcept {
      m_ptr = nullptr;
    }

  private:
    T* m_ptr;

  };


  /// @brief Returns an optional_reference object holding the passed reference.
  /// @param reference - the reference to be stored inside the object.
  template <class T>
  constexpr optional_reference<T> opt_ref(T& reference) noexcept {
    return optional_reference<T>(reference);
  }

  /// @brief Returns an optional_reference object holding an immutable version of the passed reference.
  /// @param reference - the reference to be stored inside the object.
  template <class T>
  constexpr optional_reference<const T> opt_cref(const T& reference) noexcept {
    return optional_reference<const T>(reference);
  }

} // namespace dl

#endif // !DL_OPTIONAL_REFERENCE_HPP