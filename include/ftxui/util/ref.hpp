#ifndef FTXUI_UTIL_REF_HPP
#define FTXUI_UTIL_REF_HPP

#include <ftxui/screen/string.hpp>
#include <string>
#include <string_view>
#include <variant>

namespace ftxui {

/// @brief An adapter. Own or reference an immutable object.
template <typename T>
class ConstRef {
 public:
  ConstRef() = delete;
  ConstRef(const ConstRef<T> &) = default;
  ConstRef(const T &t) : variant_(t) {}
  ConstRef(const T *t) : variant_(t) {}

  // Make a "reseatable" reference
  ConstRef<T>& operator=(const ConstRef<T>&) = default;

  const T& operator*()  const { return *Address(); }
  const T& operator()() const { return *Address(); }
  const T* operator->() const { return  Address(); }
 private:
  std::variant<T, const T *> variant_ = T{};

  const T *Address() const
  {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               :  std::get<const T *>(variant_);
  }
};

/// @brief An adapter. Own or reference an mutable object.
template <typename T>
class Ref {
 public:
  Ref() = default;
  Ref(const T& t) : variant_(t) {}
  Ref(T&& t) : variant_(std::forward<T>(t)) {}
  Ref(T* t) : variant_(t) {}

  T& operator*()  { return *Address(); }
  T& operator()() { return *Address(); }
  T* operator->() { return  Address(); }

  const T& operator*()  const { return *Address(); }
  const T& operator()() const { return *Address(); }
  const T* operator->() const { return  Address(); }
 private:
  std::variant<T, T *> variant_ = T{};

  T *Address()
  {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               :  std::get<T *>(variant_);
  }
};

/// @brief An adapter. Own or reference a constant string. For convenience, this
/// class convert multiple mutable string toward a shared representation.
class StringRef : public Ref<std::string> {
 public:
  using Ref<std::string>::Ref;

  StringRef(const wchar_t* ref) : StringRef(to_string(std::wstring(ref))) {}
  StringRef(const char* ref) : StringRef(std::string(ref)) {}
};

/// @brief An adapter. Own or reference a constant string. For convenience, this
/// class convert multiple immutable string toward a shared representation.
class ConstStringRef {
 public:
  ConstStringRef() = default;
  ConstStringRef(const ConstStringRef &) = default;
  ConstStringRef(const std::string &s) : variant_(s) {}
  ConstStringRef(std::string *sp) : variant_(sp) {}
  ConstStringRef(std::string_view sv)  : variant_(sv) {}
  ConstStringRef(const char *string_in_code) : variant_( std::string_view{string_in_code} ) {}

  // Make a "reseatable" reference
  ConstStringRef &operator=(const ConstStringRef &) = default;

  std::string operator*()  const { return std::string{ StringView() }; }
  std::string operator()() const { return std::string{ StringView() }; }
 private:
  std::variant<std::string_view, std::string *, std::string> variant_ = std::string_view{};

  std::string_view StringView() const
  {
    if ( std::holds_alternative<std::string_view>(variant_) )
      return std::get<std::string_view>(variant_);
    else if ( std::holds_alternative<std::string *>(variant_) )
      return *std::get<std::string *>(variant_);
    else if ( std::holds_alternative<std::string>(variant_) )
      return std::get<std::string>(variant_);
  }
};

/// @brief An adapter. Reference a list of strings.
class ConstStringListRef {
 public:
  ConstStringListRef() = default;
  ConstStringListRef(const std::vector<std::string>* ref) : ref_(ref) {}
  ConstStringListRef(const std::vector<std::wstring>* ref) : ref_wide_(ref) {}

  size_t size() const {
    if (ref_) {
      return ref_->size();
    }
    if (ref_wide_) {
      return ref_wide_->size();
    }
    return 0;
  }

  std::string operator[](size_t i) const {
    if (ref_) {
      return (*ref_)[i];
    }
    if (ref_wide_) {
      return to_string((*ref_wide_)[i]);
    }
    return "";
  }

 private:
  const std::vector<std::string>* ref_ = nullptr;
  const std::vector<std::wstring>* ref_wide_ = nullptr;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_UTIL_REF_HPP */

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
