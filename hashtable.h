#pragma once

#include "order.h"

#include <iostream>

class HashTable {
  struct Cell {
    Order order;
    int status = 0;
  };

  static constexpr std::size_t minimumCapacity = 5;
  static constexpr std::size_t step = 2;
  std::size_t arrayCapacity;
  Cell *cells;
  std::size_t count = 0;

  static std::size_t normalizedCapacity(std::size_t size);

  static std::size_t hashFor(const Order &order, std::size_t capacity);

  std::size_t next(std::size_t index) const;

  std::size_t resolveCollision(const Order &order,
                               bool forInsertion = false,
                               std::size_t *steps = nullptr) const;

  void insertUnchecked(const Order &order);

  void resize(std::size_t capacity);

public:
  HashTable();

  explicit HashTable(std::size_t initialCapacity);

  ~HashTable();

  std::size_t size() const;
  std::size_t capacity() const;

  std::size_t hash(const Order &order) const;

  const Order *search(const Order &order, std::size_t *steps = nullptr) const;

  const Order *getOrder(std::size_t index) const;

  bool add(const Order &order);

  bool remove(const Order &order);

  void clear();

  void print(std::ostream &out = std::cout) const;
};
