#include "hashtable.h"

#include <limits>
#include <stdexcept>

std::size_t HashTable::normalizedCapacity(const std::size_t size) {
  if (size < minimumCapacity)
    return minimumCapacity;

  return size % 2 == 0 ? size + 1 : size;
}

std::size_t HashTable::hashFor(const Order &order, const std::size_t capacity) {
  const std::string key = std::to_string(order.date.year) + "/" +
                          std::to_string(order.date.month) + "/" +
                          std::to_string(order.date.day) + ":" +
                          std::to_string(order.fullName.size()) + ":" +
                          order.fullName + ":" + std::to_string(order.id);

  std::size_t sum = 0;
  for (const unsigned char character : key)
    sum += character;
  return sum % capacity;
}

std::size_t HashTable::next(const std::size_t index) const {
  return index >= arrayCapacity - step ? index - (arrayCapacity - step)
                                       : index + step;
}

std::size_t HashTable::resolveCollision(const Order &order,
                                        const bool forInsertion,
                                        std::size_t *steps) const {
  if (steps)
    *steps = 0;
  if (arrayCapacity == 0)
    return arrayCapacity;
  std::size_t index = hash(order);
  for (std::size_t visited = 0; visited < arrayCapacity; ++visited) {
    if (steps)
      ++*steps;
    if (cells[index].status != 1) {
      if (forInsertion)
        return index;
      if (cells[index].status == 0)
        break;
    } else if (cells[index].order.equals(order))
      return index;
    index = next(index);
  }
  return arrayCapacity;
}

void HashTable::insertUnchecked(const Order &order) {
  const std::size_t index = resolveCollision(order, true);
  cells[index].order = order;
  cells[index].status = 1;
  ++count;
}

void HashTable::resize(std::size_t capacity) {
  HashTable replacement(capacity);
  for (std::size_t index = 0; index < arrayCapacity; ++index)
    if (cells[index].status == 1)
      replacement.insertUnchecked(cells[index].order);
  std::swap(cells, replacement.cells);
  std::swap(arrayCapacity, replacement.arrayCapacity);
}

HashTable::HashTable() : HashTable(minimumCapacity) {}

HashTable::HashTable(std::size_t initialCapacity)
    : arrayCapacity(normalizedCapacity(initialCapacity)),
      cells(new Cell[arrayCapacity]) {}

HashTable::~HashTable() { clear(); }

std::size_t HashTable::size() const { return count; }

std::size_t HashTable::capacity() const { return arrayCapacity; }

std::size_t HashTable::hash(const Order &order) const {
  return hashFor(order, arrayCapacity == 0 ? minimumCapacity : arrayCapacity);
}

const Order *HashTable::search(const Order &order, std::size_t *steps) const {
  const std::size_t index = resolveCollision(order, false, steps);
  return index == arrayCapacity ? nullptr : &cells[index].order;
}

const Order *HashTable::getOrder(std::size_t index) const {
  return index < arrayCapacity && cells[index].status == 1
             ? &cells[index].order
             : nullptr;
}

bool HashTable::add(const Order &order) {
  if (search(order))
    return false;
  if (arrayCapacity == 0)
    resize(minimumCapacity);
  if (count + 1 > arrayCapacity - arrayCapacity / 3) {
    if (arrayCapacity > (std::numeric_limits<std::size_t>::max() - 1) / 2)
      throw std::length_error("HashTable capacity overflow");
    resize(arrayCapacity * 2 + 1);
  }
  insertUnchecked(order);
  return true;
}

bool HashTable::remove(const Order &order) {
  const std::size_t index = resolveCollision(order);
  if (index == arrayCapacity)
    return false;
  cells[index].status = 2;
  --count;
  if (arrayCapacity > minimumCapacity && count <= arrayCapacity / 5)
    resize(arrayCapacity / 2);
  return true;
}

void HashTable::clear() {
  delete[] cells;
  cells = nullptr;
  arrayCapacity = 0;
  count = 0;
}

void HashTable::print(std::ostream &out) const {
  for (std::size_t index = 0; index < arrayCapacity; ++index) {
    const Cell &cell = cells[index];
    out << index << " [" << cell.status << "]";
    if (cell.status == 1)
      out << ' ' << cell.order.date.year << '-' << cell.order.date.month << '-'
          << cell.order.date.day << " | " << cell.order.fullName << " | "
          << cell.order.id;
    out << '\n';
  }
}
