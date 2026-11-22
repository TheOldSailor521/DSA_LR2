#include "order.h"

bool Order::equals(const Order &other) const {
  return date.year == other.date.year && date.month == other.date.month &&
         date.day == other.date.day && fullName == other.fullName &&
         id == other.id;
}
