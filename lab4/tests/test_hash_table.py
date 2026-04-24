import unittest
from hash_table import HashTable


class TestHashTableConstructor(unittest.TestCase):

    def test_constructor_default(self):
        ht = HashTable()
        self.assertEqual(ht.size, 20)
        self.assertEqual(ht.start_address, 0)
        self.assertEqual(len(ht.table), 20)

    def test_constructor_custom(self):
        ht = HashTable(10, 5)
        self.assertEqual(ht.size, 10)
        self.assertEqual(ht.start_address, 5)
        self.assertEqual(len(ht.table), 10)


class TestHashTableHashFunctions(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_get_code_upper(self):
        self.assertEqual(self.ht._get_code('А'), 0)
        self.assertEqual(self.ht._get_code('Б'), 1)
        self.assertEqual(self.ht._get_code('Я'), 32)

    def test_get_code_lower(self):
        self.assertEqual(self.ht._get_code('а'), 0)
        self.assertEqual(self.ht._get_code('б'), 1)
        self.assertEqual(self.ht._get_code('я'), 32)

    def test_get_code_unknown(self):
        self.assertEqual(self.ht._get_code('Z'), 0)
        self.assertEqual(self.ht._get_code('?'), 0)

    def test_key_to_number_normal(self):
        self.assertEqual(self.ht._key_to_number("Аксиома"), 11)
        self.assertEqual(self.ht._key_to_number("Теорема"), 632)

    def test_key_to_number_short(self):
        self.assertEqual(self.ht._key_to_number("А"), 0)
        self.assertEqual(self.ht._key_to_number(""), 0)

    def test_hash_func(self):
        self.assertEqual(self.ht._hash_func(11), 11)
        self.assertEqual(self.ht._hash_func(632), 12)
        self.assertEqual(self.ht._hash_func(401), 1)

    def test_hash_func_with_offset(self):
        ht = HashTable(20, 5)
        self.assertEqual(ht._hash_func(11), 16)
        self.assertEqual(ht._hash_func(632), 17)

    def test_gen_id_sequential(self):
        self.assertEqual(self.ht._gen_id(), "ID1")
        self.assertEqual(self.ht._gen_id(), "ID2")
        self.assertEqual(self.ht._gen_id(), "ID3")


class TestHashTableInsert(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_insert_success(self):
        result = self.ht.insert("Аксиома", "Определение")
        self.assertTrue(result)
        self.assertEqual(self.ht.get_occupied_count(), 1)

    def test_insert_with_id(self):
        result = self.ht.insert_with_id("ID999", "Теорема", "Определение")
        self.assertTrue(result)
        record = self.ht.search("Теорема")
        self.assertEqual(record.key_id, "ID999")

    def test_insert_duplicate(self):
        self.ht.insert("Лемма", "Первое")
        result = self.ht.insert("Лемма", "Второе")
        self.assertFalse(result)
        self.assertEqual(self.ht.get_occupied_count(), 1)

    def test_insert_short_key(self):
        result = self.ht.insert("А", "Одна буква")
        self.assertFalse(result)
        self.assertEqual(self.ht.get_occupied_count(), 0)

    def test_insert_empty_key(self):
        result = self.ht.insert("", "Пусто")
        self.assertFalse(result)

    def test_insert_collision_flag(self):
        self.ht.insert("Аксиома", "Первый")
        self.ht.insert("Интеграл", "Второй")
        for r in self.ht.all_records:
            if r.keyword == "Интеграл":
                self.assertEqual(r.C, 1)

    def test_insert_terminal_flag(self):
        self.ht.insert("Аксиома", "Первый")
        self.ht.insert("Интеграл", "Второй")
        for r in self.ht.all_records:
            if r.keyword == "Аксиома":
                self.assertEqual(r.T, 0)
            if r.keyword == "Интеграл":
                self.assertEqual(r.T, 1)


class TestHashTableSearch(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_search_existing(self):
        self.ht.insert("Синус", "sin")
        record = self.ht.search("Синус")
        self.assertIsNotNone(record)
        self.assertEqual(record.data, "sin")

    def test_search_non_existing(self):
        record = self.ht.search("НетТакого")
        self.assertIsNone(record)

    def test_search_deleted(self):
        self.ht.insert("Косинус", "cos")
        self.ht.delete("Косинус")
        record = self.ht.search("Косинус")
        self.assertIsNone(record)

    def test_search_short_key(self):
        record = self.ht.search("А")
        self.assertIsNone(record)

    def test_search_empty_key(self):
        record = self.ht.search("")
        self.assertIsNone(record)

    def test_search_in_collision_chain(self):
        self.ht.insert("Аксиома", "Первый")
        self.ht.insert("Интеграл", "Второй")
        self.ht.insert("Вектор", "Третий")
        record = self.ht.search("Вектор")
        self.assertIsNotNone(record)
        self.assertEqual(record.data, "Третий")


class TestHashTableUpdate(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_update_existing(self):
        self.ht.insert("Тангенс", "tg")
        result = self.ht.update("Тангенс", "tg = sin/cos")
        self.assertTrue(result)
        record = self.ht.search("Тангенс")
        self.assertEqual(record.data, "tg = sin/cos")

    def test_update_non_existing(self):
        result = self.ht.update("НетТакого", "Данные")
        self.assertFalse(result)

    def test_update_deleted(self):
        self.ht.insert("Гипотенуза", "c")
        self.ht.delete("Гипотенуза")
        result = self.ht.update("Гипотенуза", "new")
        self.assertFalse(result)


class TestHashTableDelete(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_delete_existing(self):
        self.ht.insert("Матрица", "Таблица")
        self.assertEqual(self.ht.get_occupied_count(), 1)
        result = self.ht.delete("Матрица")
        self.assertTrue(result)
        self.assertEqual(self.ht.get_occupied_count(), 0)

    def test_delete_non_existing(self):
        result = self.ht.delete("НетТакого")
        self.assertFalse(result)

    def test_delete_head_of_chain(self):
        self.ht.insert("Аксиома", "Первый")
        self.ht.insert("Интеграл", "Второй")
        self.ht.delete("Аксиома")
        self.assertIsNone(self.ht.search("Аксиома"))
        self.assertIsNotNone(self.ht.search("Интеграл"))

    def test_delete_last_in_chain(self):
        self.ht.insert("Аксиома", "Первый")
        self.ht.insert("Интеграл", "Второй")
        self.ht.delete("Интеграл")
        self.assertIsNotNone(self.ht.search("Аксиома"))
        self.assertIsNone(self.ht.search("Интеграл"))
        for r in self.ht.all_records:
            if r.keyword == "Аксиома":
                self.assertEqual(r.T, 1)

    def test_delete_middle_of_chain(self):
        self.ht.insert("Аксиома", "Первый")
        self.ht.insert("Интеграл", "Второй")
        self.ht.insert("Вектор", "Третий")
        self.ht.delete("Интеграл")
        self.assertIsNotNone(self.ht.search("Аксиома"))
        self.assertIsNone(self.ht.search("Интеграл"))
        self.assertIsNotNone(self.ht.search("Вектор"))

    def test_delete_single_chain(self):
        self.ht.insert("Радиан", "Единица")
        self.ht.delete("Радиан")
        h = self.ht._hash_func(self.ht._key_to_number("Радиан"))
        self.assertIsNone(self.ht.table[h])

    def test_delete_short_key(self):
        result = self.ht.delete("А")
        self.assertFalse(result)


class TestHashTableFillFactor(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_fill_factor_zero(self):
        self.assertEqual(self.ht.fill_factor(), 0.0)

    def test_fill_factor_after_insert(self):
        self.ht.insert("Термин1", "Данные")
        self.assertEqual(self.ht.fill_factor(), 1.0 / 20.0)
        self.ht.insert("Термин2", "Данные")
        self.assertEqual(self.ht.fill_factor(), 2.0 / 20.0)

    def test_fill_factor_after_delete(self):
        self.ht.insert("Термин1", "Данные")
        self.ht.insert("Термин2", "Данные")
        self.ht.delete("Термин1")
        self.assertEqual(self.ht.fill_factor(), 1.0 / 20.0)


class TestHashTableOccupiedCount(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_occupied_count(self):
        self.assertEqual(self.ht.get_occupied_count(), 0)
        self.ht.insert("A", "B")
        self.assertEqual(self.ht.get_occupied_count(), 0)
        self.ht.insert("Аксиома", "B")
        self.assertEqual(self.ht.get_occupied_count(), 1)


class TestHashTablePrint(unittest.TestCase):

    def setUp(self):
        self.ht = HashTable(20, 0)

    def test_print_table_does_not_crash(self):
        self.ht.insert("Аксиома", "Определение")
        self.ht.print_table()

    def test_print_info_does_not_crash(self):
        self.ht.insert("Аксиома", "Определение")
        self.ht.print_info()

    def test_print_empty_table(self):
        self.ht.print_table()

    def test_print_empty_info(self):
        self.ht.print_info()


if __name__ == "__main__":
    unittest.main()