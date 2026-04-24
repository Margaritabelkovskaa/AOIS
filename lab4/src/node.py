from record import Record


class Node:
    """Узел связного списка"""

    def __init__(self, record: Record, idx: int):
        self.record = record
        self.next = None
        self.idx = idx