
/// @brief Optional reference class and related functions.
/// 
/// @section License
/// Copyright (c) 2022 Dario Cvitanović
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
/// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
/// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all copies or
/// substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
/// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
/// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef DL_OPTIONAL_REFERENCE_HPP
#define DL_OPTIONAL_REFERENCE_HPP

#include <cassert>
#include <memory>
#include <stdexcept>


namespace dl {

  /// Empty class type used to indicate an optional_reference with an uninitialized state.
  class nullref_t {};

  /// Constant of type nullref_t used to indicate an optional_reference with an uninitialized state.
  inline constexpr nullref_t nullref;


  /// Exception type thrown by optional_reference::ref when accessing an empty optional_reference.
  class bad_optional_reference_access : public std::exception {

  public:
    /// Constructor.
    bad_optional_reference_access() noexcept {}

    /// Brief explanation of the error.
    const char* what() const noexcept {
      return "bad optional reference access";
    }

  }; // class bad_optional_reference_access


  /// Raw pointer wrapper with std::optional-like semantics and safety against null dereferencing.
  template <class T>
  class optional_reference {

  public:
    /// Constructs an object that does not contain a reference.
    [[nodiscard]] constexpr optional_reference() noexcept
      : m_ptr(nullptr) {}

    /// Constructs an object that does not contain a reference.
    [[nodiscard]] constexpr optional_reference(nullref_t) noexcept
      : m_ptr(nullptr) {}

    /// Constructs an object containing a reference.
    [[nodiscard]] constexpr optional_reference(T& reference) noexcept
      : m_ptr(std::addressof(reference)) {}

    /// Constructs an object from a raw pointer.
    [[nodiscard]] constexpr optional_reference(T* reference) noexcept
      : m_ptr(reference) {}


    /// Const conversion operator.
    [[nodiscard]] constexpr operator optional_reference<const T>() const noexcept {
      return optional_reference<const T>(this);
    }


    /// Returns true if *this contains a reference, false otherwise.
    [[nodiscard]] constexpr operator bool() const noexcept {
      return m_ptr;
    }

    /// Returns true if *this contains a reference, false otherwise.
    [[nodiscard]] constexpr bool has_ref() const noexcept {
      return m_ptr;
    }


    /// @brief Returns the contained reference.
    /// @details Unlike optional_reference::ref, this operator is unchecked and will
    /// unapologetically dereference a null pointer without throwing an exception.
    [[nodiscard]] constexpr T& operator*() const noexcept {
      assert(m_ptr);
      return *m_ptr;
    }

    /// @brief Returns the contained reference.
    /// @exception bad_optional_reference_access - If *this is empty.
    [[nodiscard]] constexpr T& ref() const {
      if (!m_ptr) throw bad_optional_reference_access();
      return *m_ptr;
    }

    /// @brief Returns the contained reference with pointer syxtax.
    /// @details Note that this function is unchecked and can return nullptr if *this is empty.
    [[nodiscard]] constexpr T* operator->() const noexcept {
      assert(m_ptr);
      return m_ptr;
    }

    /// Returns a pointer to the referenced value or nullptr.
    [[nodiscard]] constexpr T* ptr() const noexcept {
      return m_ptr;
    }


    /// If *this contains a reference, resets it to being empty.
    constexpr void reset() noexcept {
      m_ptr = nullptr;
    }

  private:
    T* m_ptr;

  }; // template class optional_reference


  /// Returns an optional_reference object holding the passed reference.
  template <class T>
  [[nodiscard]] constexpr optional_reference<T> opt_ref(T& reference) noexcept {
    return optional_reference<T>(reference);
  }

  /// Returns an optional_reference object holding an immutable version of the passed reference.
  template <class T>
  [[nodiscard]] constexpr optional_reference<const T> opt_cref(const T& reference) noexcept {
    return optional_reference<const T>(reference);
  }

} // namespace dl

#endif // !DL_OPTIONAL_REFERENCE_HPP
