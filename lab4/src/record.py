class Record:
    """Запись в хеш-таблице"""

    def __init__(self, key_id: str, keyword: str, data: str):
        self.key_id = key_id
        self.keyword = keyword
        self.data = data
        self.C = 0  # коллизия
        self.U = 1  # занято
        self.T = 1  # терминальный (последний в цепочке)
        self.L = 0  # связь
        self.D = 0  # удалён
        self.Po = -1  # указатель на следующий

    def is_active(self) -> bool:
        return self.U == 1 and self.D == 0

    def mark_deleted(self):
        self.D = 1
        self.U = 0

    def __repr__(self):
        return f"Record({self.keyword}, active={self.is_active()})"