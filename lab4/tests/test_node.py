import unittest
from record import Record
from node import Node


class TestNode(unittest.TestCase):

    def test_node_init(self):
        r = Record("ID1", "Тест", "Данные")
        node = Node(r, 42)
        self.assertEqual(node.record, r)
        self.assertEqual(node.idx, 42)
        self.assertIsNone(node.next)


if __name__ == "__main__":
    unittest.main()