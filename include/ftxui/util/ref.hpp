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
  ConstRef(const T &t) : variant_(t) {}
  ConstRef(const T *t) : variant_(t) {}

  ConstRef(ConstRef<T> &&) = default;
  ConstRef(const ConstRef<T> &) = default;

  // Make a "reseatable" reference
  ConstRef<T>& operator=(const ConstRef<T>&) = default;
  ConstRef<T>& operator=(ConstRef<T> &&) = default;

  const T& operator*() const { return getReference(); }
  const T& operator()() const { return getReference(); }
  const T* operator->() const { return getPointer(); }
 private:
  std::variant<T, const T *> variant_ = T{};

  const T &getReference() const { return *getPointer(); }
  const T *getPointer() const
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

  T& operator*() { return getReference(); }
  T& operator()() { return getReference(); }
  T* operator->() { return getPointer(); }

  const T& operator*() const { return getReference(); }
  const T& operator()() const { return getReference(); }
  const T* operator->() const { return getPointer(); }
 private:
  std::variant<T, T *> variant_ = T{};

  const T &getReference() const { return *getPointer(); }
        T &getReference()       { return *getPointer(); }

  const T *getPointer() const
  {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               :  std::get<T *>(variant_);
  }
  T *getPointer()
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
  ConstStringRef() = delete;
  ConstStringRef(const std::string &s) : variant_(s) {}
  ConstStringRef(std::string *sp) : variant_( std::string_view{ *sp } ) {}
  ConstStringRef(std::string_view sv)  : variant_(sv) {}
  ConstStringRef(const char *string_in_code) : variant_( std::string_view{string_in_code} ) {}

  ConstStringRef(ConstStringRef &&) = default;
  ConstStringRef(const ConstStringRef &) = default;

  // Make a "reseatable" reference
  ConstStringRef &operator=(ConstStringRef &&) = default;
  ConstStringRef &operator=(const ConstStringRef &) = default;

  std::string operator*()  const { return std::string{ getStringView() }; }
  std::string operator()() const { return std::string{ getStringView() }; }
 private:
  std::variant<std::string_view, std::string> variant_ = std::string_view{};

  const std::string_view getStringView() const
  {
    return std::holds_alternative<std::string>(variant_) ? std::get<std::string>(variant_)
                                                         : std::get<std::string_view>(variant_);
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
