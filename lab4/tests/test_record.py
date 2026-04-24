import unittest
from record import Record


class TestRecord(unittest.TestCase):

    def test_record_init(self):
        r = Record("ID1", "Тест", "Данные")
        self.assertEqual(r.key_id, "ID1")
        self.assertEqual(r.keyword, "Тест")
        self.assertEqual(r.data, "Данные")
        self.assertEqual(r.C, 0)
        self.assertEqual(r.U, 1)
        self.assertEqual(r.T, 1)
        self.assertEqual(r.L, 0)
        self.assertEqual(r.D, 0)
        self.assertEqual(r.Po, -1)

    def test_record_is_active(self):
        r = Record("ID1", "Тест", "Данные")
        self.assertTrue(r.is_active())
        r.mark_deleted()
        self.assertFalse(r.is_active())

    def test_record_mark_deleted(self):
        r = Record("ID1", "Тест", "Данные")
        r.mark_deleted()
        self.assertEqual(r.D, 1)
        self.assertEqual(r.U, 0)


if __name__ == "__main__":
    unittest.main()