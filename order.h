#pragma once

#include <string>

struct Date {
  int year{};
  int month{};
  int day{};
};

struct Order {
  std::size_t lineNumber{};
  Date date{};
  std::string fullName;
  int id{};

  bool equals(const Order &other) const;
};
