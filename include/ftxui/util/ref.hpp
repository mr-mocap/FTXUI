#ifndef FTXUI_UTIL_REF_HPP
#define FTXUI_UTIL_REF_HPP

#include <ftxui/screen/string.hpp>
#include <string>
#include <variant>

namespace ftxui {

/// @brief An adapter. Own or reference an immutable object.
template <typename T>
class ConstRef {
 public:
  ConstRef() = delete;
  ConstRef(const T &t) : variant_(t) {}
  ConstRef(const T *t) : variant_(t) {}

  ConstRef(ConstRef<T> &&) = delete;
  ConstRef(const ConstRef<T> &) = default;
  ConstRef<T>& operator=(ConstRef<T>&) = delete;

  const T& operator*() const { return std::holds_alternative<const T>(variant_) ?  std::get<const T>(variant_)
                                                                                : *std::get<const T *>(variant_);
  }
  const T& operator()() const { return std::holds_alternative<const T>(variant_) ? std::get<const T>(variant_)
                                                                                : *std::get<const T *>(variant_);
  }
  const T* operator->() const { return std::holds_alternative<const T *>(variant_) ?  std::get<const T *>(variant_)
                                                                                   : &std::get<const T >(variant_);
  }
 private:
  std::variant<const T*, const T> variant_ = nullptr;
};

/// @brief An adapter. Own or reference an mutable object.
template <typename T>
class Ref {
 public:
  Ref() {}
  Ref(const T& t) : variant_(t) {}
  Ref(T&& t) : variant_(std::forward<T>(t)) {}
  Ref(T* t) : variant_(t) {}
  T& operator*() {
    return std::holds_alternative<T>(variant_) ?  std::get<T>(variant_)
                                               : *std::get<T *>(variant_);
  }
  T& operator()() {
    return std::holds_alternative<T>(variant_) ?  std::get<T>(variant_)
                                               : *std::get<T *>(variant_);
  }
  T* operator->() {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               :  std::get<T *>(variant_);
  }

  const T& operator*() const {
    return std::holds_alternative<T>(variant_) ?  std::get<T>(variant_)
                                               : *std::get<T *>(variant_);
  }
  const T& operator()() const {
    return std::holds_alternative<T>(variant_) ?  std::get<T>(variant_)
                                               : *std::get<T *>(variant_);
  }
  const T* operator->() const {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               :  std::get<T *>(variant_);
  }
 private:
  std::variant<T *, T> variant_;
};

/// @brief An adapter. Own or reference a constant string. For convenience, this
/// class convert multiple mutable string toward a shared representation.
class StringRef {
 public:
  StringRef(std::string* ref) : address_(ref) {}
  StringRef(std::string ref) : owned_(std::move(ref)) {}
  StringRef(const wchar_t* ref) : StringRef(to_string(std::wstring(ref))) {}
  StringRef(const char* ref) : StringRef(std::string(ref)) {}
  StringRef(const StringRef& t) : owned_(t.owned_), address_(t.address_) {}
  StringRef& operator=(const StringRef& t) {
    owned_ = t.owned_;
    address_ = t.address_;
    return *this;
  }
  std::string& operator*() { return address_ ? *address_ : owned_; }
  std::string& operator()() { return address_ ? *address_ : owned_; }
  std::string* operator->() { return address_ ? address_ : &owned_; }

 private:
  std::string owned_;
  std::string* address_ = nullptr;
};

/// @brief An adapter. Own or reference a constant string. For convenience, this
/// class convert multiple immutable string toward a shared representation.
class ConstStringRef {
 public:
  ConstStringRef(const std::string* ref) : address_(ref) {}
  ConstStringRef(const std::wstring* ref) : ConstStringRef(to_string(*ref)) {}
  ConstStringRef(std::string ref) : owned_(std::move(ref)) {}
  ConstStringRef(std::wstring ref) : ConstStringRef(to_string(ref)) {}
  ConstStringRef(const wchar_t* ref) : ConstStringRef(std::wstring(ref)) {}
  ConstStringRef(const char* ref)
      : ConstStringRef(to_wstring(std::string(ref))) {}
  ConstStringRef(const ConstStringRef& t)
      : owned_(t.owned_), address_(t.address_) {}
  ConstStringRef& operator=(const ConstStringRef& t) {
    owned_ = t.owned_;
    address_ = t.address_;
    return *this;
  }
  ConstStringRef& operator=(ConstStringRef&& t) {
    owned_ = std::move(t.owned_);
    address_ = t.address_;
    return *this;
  }
  const std::string& operator()() const {
    return address_ ? *address_ : owned_;
  }
  const std::string& operator*() const { return address_ ? *address_ : owned_; }
  const std::string* operator->() const {
    return address_ ? address_ : &owned_;
  }

 private:
  std::string owned_;
  const std::string* address_ = nullptr;
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
