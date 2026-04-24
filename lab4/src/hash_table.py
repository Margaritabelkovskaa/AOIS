from record import Record
from node import Node


class HashTable:
    """Хеш-таблица с разрешением коллизий связным списком"""

    # Русский алфавит для кодирования
    RUS_UPPER = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
    RUS_LOWER = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"

    def __init__(self, size: int = 20, start_address: int = 0):
        self.size = size
        self.start_address = start_address
        self.table = [None] * size
        self.all_records = []
        self.next_id = 1

    def _get_code(self, c: str) -> int:
        """Получить код русской буквы (А=0, Б=1, ..., Я=32)"""
        if c in self.RUS_UPPER:
            return self.RUS_UPPER.index(c)
        if c in self.RUS_LOWER:
            return self.RUS_LOWER.index(c)
        return 0

    def _key_to_number(self, key: str) -> int:
        """Перевод ключевого слова в числовое значение V"""
        if len(key) < 2:
            return 0
        first = self._get_code(key[0])
        second = self._get_code(key[1])
        return first * 33 + second

    def _hash_func(self, v: int) -> int:
        """Хеш-функция: h = V % size + start_address"""
        return (v % self.size) + self.start_address

    def _gen_id(self) -> str:
        """Генерация ID для новой записи"""
        new_id = f"ID{self.next_id}"
        self.next_id += 1
        return new_id

    def search(self, keyword: str):
        """Поиск записи по ключевому слову"""
        if len(keyword) < 2:
            return None

        v = self._key_to_number(keyword)
        h = self._hash_func(v)

        current = self.table[h]
        while current:
            if current.record.is_active() and current.record.keyword == keyword:
                return current.record
            current = current.next
        return None

    def insert(self, keyword: str, data: str):
        """Вставка записи (автоматическая генерация ID)"""
        key_id = self._gen_id()
        return self.insert_with_id(key_id, keyword, data)

    def insert_with_id(self, key_id: str, keyword: str, data: str):
        """Вставка записи с указанным ID"""
        if len(keyword) < 2:
            print("Ключ слишком короткий")
            return False

        if self.search(keyword):
            print(f"Ошибка: '{keyword}' уже существует")
            return False

        v = self._key_to_number(keyword)
        h = self._hash_func(v)

        new_record = Record(key_id, keyword, data)
        new_node = Node(new_record, len(self.all_records))

        if self.table[h] is not None:
            # Коллизия
            new_record.C = 1
            current = self.table[h]
            while current.next:
                current = current.next
            current.next = new_node
            current.record.T = 0
            current.record.Po = len(self.all_records)
        else:
            self.table[h] = new_node

        self.all_records.append(new_record)
        print(f"Добавлено: {key_id} -> '{keyword}' (V={v}, h={h})")
        return True

    def update(self, keyword: str, new_data: str) -> bool:
        """Обновление данных записи"""
        record = self.search(keyword)
        if record:
            record.data = new_data
            print("Обновлено")
            return True
        print("Не найдено")
        return False

    def delete(self, keyword: str) -> bool:
        """Удаление записи (логическое)"""
        if len(keyword) < 2:
            return False

        v = self._key_to_number(keyword)
        h = self._hash_func(v)

        prev = None
        current = self.table[h]

        while current:
            if current.record.is_active() and current.record.keyword == keyword:
                current.record.mark_deleted()

                if prev is None and current.next is None:
                    self.table[h] = None
                elif prev is None:
                    self.table[h] = current.next
                    if self.table[h]:
                        self.table[h].record.T = 1
                else:
                    prev.next = current.next
                    if prev.next is None:
                        prev.record.T = 1

                print("Удалено")
                return True

            prev = current
            current = current.next

        print("Не найдено")
        return False

    def fill_factor(self) -> float:
        """Коэффициент заполнения"""
        active_count = sum(1 for r in self.all_records if r.is_active())
        return active_count / self.size

    def get_occupied_count(self) -> int:
        """Количество активных записей"""
        return sum(1 for r in self.all_records if r.is_active())

    def print_table(self):
        """Вывод хеш-таблицы"""
        print("\n" + "=" * 110)
        print("ХЕШ-ТАБЛИЦА (МАТЕМАТИКА)")
        print("=" * 110)
        print("  №  ID            C U T L D Po Данные")
        print("-" * 110)

        for i in range(self.size):
            current = self.table[i]
            if current:
                # Первый элемент в цепочке
                r = current.record
                print(f"{i:4} {r.keyword:13} {r.C} {1 if r.is_active() else 0} {r.T} {r.L} {r.D} "
                      f"{current.next.record.Po if current.next else -1:3} "
                      f"{r.data[:50] + '...' if len(r.data) > 50 else r.data}")

                # Остальные в цепочке
                current = current.next
                while current:
                    r2 = current.record
                    print(f"     {'  -> ' + r2.keyword:13} {r2.C} {1 if r2.is_active() else 0} {r2.T} {r2.L} {r2.D} "
                          f"{current.next.record.Po if current.next else -1:3} "
                          f"{r2.data[:50] + '...' if len(r2.data) > 50 else r2.data}")
                    current = current.next
            else:
                print(f"{i:4} {'':13} 0 0 0 0 0  -   ")

        print("-" * 110)
        print(f"Коэффициент заполнения: {self.fill_factor():.2f}")
        print("=" * 110)

    def print_info(self):
        """Вывод информации о строках (V и h для каждого ключа)"""
        print("\n" + "=" * 95)
        print("ИНФОРМАЦИЯ О СТРОКАХ")
        print("=" * 95)
        print("№ строки  ID      Ключевое слово        V    h   Статус")
        print("-" * 95)

        # Группируем записи по хешу
        by_hash = [[] for _ in range(self.size)]
        for r in self.all_records:
            if r.is_active() and r.keyword:
                v = self._key_to_number(r.keyword)
                h = self._hash_func(v)
                by_hash[h].append((r, v))

        for i in range(self.size):
            if by_hash[i]:
                for idx, (r, v) in enumerate(by_hash[i]):
                    if idx == 0:
                        print(f"{i:9} {r.key_id:7} {r.keyword:20} {v:4} {i:3} Активна")
                    else:
                        print(f"{'':9} {r.key_id:7} {'  └─ ' + r.keyword:20} {v:4} {i:3} Коллизия {idx}")
            else:
                print(f"{i:9} {'None':7} {'None':20} {'None':4} {'None':3} Пусто")

        print("=" * 95)
        print(f"\n   Всего строк: {self.size}")
        print(f"   Занято: {self.get_occupied_count()}")
        print(f"   Пустых: {self.size - self.get_occupied_count()}")
        print(f"   Коэффициент: {self.fill_factor():.2f}")

        collision_count = sum(1 for r in self.all_records if r.C == 1 and r.is_active())
        print(f"   Коллизий: {collision_count}")
        print("=" * 95)