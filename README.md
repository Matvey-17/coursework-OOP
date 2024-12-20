## Курсовая работа по программированию на C++

# Описание задания

Двоичный файл содержит записи переменной длины. Формат записи предполагает ее переменную размерность. Реализовать набор операций над записями без загрузки одновременно всей структуры данных в память (поэлементная загрузка СД): добавление строки, извлечение, удаление, вставка по логическому номеру и редактирование (обновление) строки, постраничный просмотр. При изменении размерности записи она переписывается в конец файла.

Доска объявлений. Категория объявления: куплю-продам, рубрика: транспорт, недвижимость, материалы и т.д. Текст объявления – строка (абзац) переменной длины, дата подачи, контактный телефон. Поиск по заданной рубрике и по шаблону искомого слова в тексте объявления. Сортировка по дате подачи.

В качестве структуры данных использую дерево, вершина которого содержит два указателя на поддеревья и структуированную переменную. 

Также для обхода дерева был добавлен вложенный класс, реализующий функции итератора.