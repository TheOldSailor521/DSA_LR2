#include "hashtable.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

std::size_t loadOrdersFromFile(const std::string &filename, HashTable &table,
                               std::size_t maxLines = 1000000) {
  std::ifstream file(filename);
  std::size_t duplicates = 0;
  std::size_t lineNumber = 0;
  std::string line;
  while (lineNumber < maxLines && std::getline(file, line)) {
    ++lineNumber;
    std::istringstream row(line);
    row >> std::ws;
    if (row.eof())
      continue;

    Order order;
    order.lineNumber = lineNumber;
    char separator;
    row >> order.date.day >> separator >> order.date.month >> separator >>
        order.date.year >> separator;
    std::getline(row, order.fullName, ';');
    row >> order.id;

    if (!table.add(order))
      ++duplicates;
  }
  if (!file && !file.eof())
    throw std::runtime_error("Cannot read input file: " + filename);
  return duplicates;
}

void saveOrdersToFile(const std::string &filename, const HashTable &table) {
  std::ofstream file(filename);
  for (std::size_t index = 0; index < table.capacity(); ++index) {
    const Order *order = table.getOrder(index);
    if (order)
      file << std::setfill('0') << std::setw(2) << order->date.day << '-'
           << std::setw(2) << order->date.month << '-' << std::setw(4)
           << order->date.year << ';' << order->fullName << ';' << order->id
           << '\n';
  }
  file.close();
  if (!file)
    throw std::runtime_error("Cannot write output file: " + filename);
}

int main() {
  int n;
  std::cout << "Введите число строк n: ";
  if (!(std::cin >> n) || n < 0)
    return 1;

  HashTable table;
  const std::size_t duplicates = loadOrdersFromFile("input.txt", table, n);
  std::cout << "Загружено записей: " << table.size()
            << ", дубликатов: " << duplicates << '\n';

  Order key;
  char separator;
  std::cout << "Введите дату (dd-mm-yyyy): ";
  if (!(std::cin >> key.date.day >> separator >> key.date.month >> separator >>
        key.date.year))
    return 1;
  std::cout << "Введите ФИО: ";
  std::getline(std::cin >> std::ws, key.fullName);
  std::cout << "Введите ID: ";
  if (!(std::cin >> key.id))
    return 1;

  std::size_t searchSteps;
  const Order *found = table.search(key, &searchSteps);
  if (found)
    std::cout << "Найдена запись, строка файла: " << found->lineNumber << '\n';
  else
    std::cout << "Запись не найдена\n";

  std::cout << "Количество шагов поиска: " << searchSteps << '\n';

  std::cout << "Удаление по введённому ключу: "
            << (table.remove(key) ? "запись удалена" : "запись не найдена")
            << '\n';
  std::cout << "Повторное удаление по тому же ключу: "
            << (table.remove(key) ? "запись удалена" : "запись не найдена")
            << '\n';

  table.print(std::cout);

  saveOrdersToFile("output.txt", table);

  return 0;
}
