from hash_table import HashTable


def load_math_data(ht: HashTable):
    """Загрузка математических терминов"""
    ht.insert_with_id("ID1", "Аксиома", "Исходное утверждение, принимаемое без доказательства")
    ht.insert_with_id("ID2", "Теорема", "Утверждение, выводимое из аксиом")
    ht.insert_with_id("ID3", "Лемма", "Вспомогательная теорема для доказательства")
    ht.insert_with_id("ID4", "Функция", "Зависимость одной переменной от другой")
    ht.insert_with_id("ID5", "Производная", "Скорость изменения функции в точке")
    ht.insert_with_id("ID6", "Интеграл", "Площадь под кривой или накопленная величина")
    ht.insert_with_id("ID7", "Вектор", "Направленный отрезок, имеющий длину и направление")
    ht.insert_with_id("ID8", "Матрица", "Прямоугольная таблица чисел")
    ht.insert_with_id("ID9", "Дискриминант", "Выражение для нахождения корней уравнения")
    ht.insert_with_id("ID10", "Логарифм", "Показатель степени, в которую нужно возвести основание")
    ht.insert_with_id("ID11", "Синус", "Отношение противолежащего катета к гипотенузе")
    ht.insert_with_id("ID12", "Косинус", "Отношение прилежащего катета к гипотенузе")
    ht.insert_with_id("ID13", "Тангенс", "Отношение синуса к косинусу")
    ht.insert_with_id("ID14", "Гипотенуза", "Сторона прямоугольного треугольника против прямого угла")
    ht.insert_with_id("ID15", "Радиан", "Единица измерения углов")


def main():
    ht = HashTable(15, 0)
    load_math_data(ht)

    while True:
        print("\n1-Вывод 2-Добавить 3-Найти 4-Обновить 5-Удалить 6-Инфо 7-Коэфф 0-Выход")
        choice = input("Выбор: ").strip()

        if choice == "0":
            break
        elif choice == "1":
            ht.print_table()
        elif choice == "2":
            kw = input("Ключ: ").strip()
            data = input("Данные: ").strip()
            if kw and data:
                ht.insert(kw, data)
        elif choice == "3":
            kw = input("Ключ: ").strip()
            r = ht.search(kw)
            if r:
                print(f"Найдено: {r.data}")
            else:
                print("Не найдено")
        elif choice == "4":
            kw = input("Ключ: ").strip()
            data = input("Новые данные: ").strip()
            ht.update(kw, data)
        elif choice == "5":
            kw = input("Ключ: ").strip()
            ht.delete(kw)
        elif choice == "6":
            ht.print_info()
        elif choice == "7":
            print(f"Коэффициент: {ht.fill_factor():.2f}")
        else:
            print("Неверный выбор")

        input("\nEnter...")


if __name__ == "__main__":
    main()