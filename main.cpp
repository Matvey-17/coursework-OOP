#include "Advertisement.h"

void SortFileTime()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    ExternalSort::sortBinaryFile("ads.dat");
    BinaryFileHandler fileHandlerSort("sorted_1.bin");

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Время выполнения операции:" << elapsed.count() << " секунд.\n";
}

void getIndexTime(BinaryFileHandler &fileHandler)
{
    auto start = std::chrono::high_resolution_clock::now();

    size_t index = 300;
    std::string search_by_index = fileHandler.extractRecordByIndex(index);
    if (search_by_index.empty())
    {
        std::cout << "Запись с индексом " << index << " не найдена" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Время выполнения операции: " << duration.count() << " секунд.\n";
}

void addIndexTime(BinaryFileHandler &fileHandler)
{
    auto start = std::chrono::high_resolution_clock::now();

    size_t index = 1001;
    Advertisement ad(
        "Category" + std::to_string(5),
        "Section" + std::to_string(5),
        "Text of advertisement " + std::to_string(5),
        "2024-12-" + std::string(5 % 31 < 9 ? "0" : "") + std::to_string((5 % 31) + 1),
        "8999123456" + std::to_string(5 % 10));
    fileHandler.insertRecordAtIndex(index, ad);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Время выполнения операции: " << duration.count() << " секунд.\n";
}

void deleteTime(BinaryFileHandler &fileHandler, Tree adTree)
{
    auto start = std::chrono::high_resolution_clock::now();

    size_t index = 600;
    fileHandler.deleteRecordByIndex(index, adTree);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Время выполнения операции: " << duration.count() << " секунд.\n";
}

void displaySortTime(BinaryFileHandler &fileHandler, Tree adTree)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::string param = "Дата";
    fileHandler.loadFromFile(adTree, param);
    adTree.inOrderTraversal();
    adTree.clear();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Время выполнения операции: " << duration.count() << " секунд.\n";
}

void measureDisplayTime(BinaryFileHandler &fileHandler)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    fileHandler.displayAllRecords();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Вывод всех записей занял " << elapsed.count() << " секунд.\n";
}

void addMultipleRecords(BinaryFileHandler &fileHandler, int count)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Advertisement ad(
            "Category" + std::to_string(i),
            "Section" + std::to_string(i),
            "Text of advertisement " + std::to_string(i),
            "2024-12-" + std::string(i % 31 < 9 ? "0" : "") + std::to_string((i % 31) + 1),
            "8999123456" + std::to_string(i % 10));
        fileHandler.addRecord(ad);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << count << " записей добавлены за " << elapsed.count() << " секунд.\n";
}

void menu()
{
    BinaryFileHandler fileHandler("ads.dat");
    Tree adTree;
    int choice;

    do
    {
        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить запись\n";
        std::cout << "2. Отобразить все записи из файла\n";
        std::cout << "3. Отобразить все записи в отсортированном порядке\n";
        std::cout << "4. Удалить запись по индексу\n";
        std::cout << "5. Добавить запись по индексу\n";
        std::cout << "6. Найти записи по категории объявления\n";
        std::cout << "7. Обновить запись по индексу\n";
        std::cout << "8. Извлечь запись по индексу\n";
        std::cout << "9. Постраничный просмотр записей\n";
        std::cout << "10. Сортировка записей в файле\n";
        std::cout << "11. Выход из меню\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case 1:
        {
            Advertisement ad;
            std::cin >> ad;
            fileHandler.addRecord(ad);
            break;
        }
        case 2:
        {
            fileHandler.displayAllRecords();
            break;
        }
        case 3:
        {
            std::cout << "Отсортированные объявления по дате:\n";
            std::string param = "Дата";
            fileHandler.loadFromFile(adTree, param);
            adTree.inOrderTraversal();
            adTree.clear();
            break;
        }
        case 4:
        {
            size_t index;
            std::cout << "Введите индекс записи для удаления: ";
            std::cin >> index;
            fileHandler.deleteRecordByIndex(index, adTree);
            break;
        }
        case 5:
        {
            size_t index;
            Advertisement ad;
            std::cout << "Введите индекс записи для добавления: ";
            std::cin >> index;
            std::cin.ignore();
            std::cin >> ad;
            fileHandler.insertRecordAtIndex(index, ad);
            std::cout << "Объявление добавлено.\n";
            break;
        }
        case 6:
        {
            std::string category;
            std::cout << "Введите категорию для поиска объявлений: ";
            std::getline(std::cin, category);
            std::cout << "Объявления с категорией '" << category << "':\n";
            std::string param = "Категория";
            fileHandler.loadFromFile(adTree, param);
            adTree.searchByCategory(category, adTree);
            adTree.clear();
            break;
        }
        case 7:
        {
            size_t index;
            Advertisement newAd;
            std::cout << "Введите индекс записи для обновления: ";
            std::cin >> index;
            std::cin.ignore();
            std::cout << "Введите новые даныне для объявления:\n";
            std::cin >> newAd;

            fileHandler.updateRecordByIndex(index, newAd, adTree);
            break;
        }
        case 8:
        {
            size_t index;
            std::cout << "Введите индекс записи для извлечения: ";
            std::cin >> index;
            std::cin.ignore();
            std::string search_by_index = fileHandler.extractRecordByIndex(index);
            if (search_by_index.empty())
            {
                std::cout << "Запись с индексом " << index << " не найдена" << std::endl;
            }
            break;
        }
        case 9:
        {
            size_t items_per_page = 1;
            fileHandler.navigatePages(items_per_page);
        }
        case 10:
        {
            ExternalSort::sortBinaryFile("ads.dat");
            BinaryFileHandler fileHandlerSort("sorted_1.bin");
            std::cout << "Отсортированные объявления по дате в новом файле:\n";
            fileHandlerSort.displayAllRecords();
            break;
        }
        case 11:
        {
            std::cout << "Выход...\n";
            break;
        }
        default:
        {
            std::cout << "Ошибка при выборе команды. Попробуйте снова.\n";
            break;
        }
        }
    } while (choice != 11);
}

int main()
{
    menu();
    return 0;
}
