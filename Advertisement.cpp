#include "Advertisement.h"

bool validateDate(const std::string &date)
{
    std::regex date_regex(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])$)");
    return std::regex_match(date, date_regex);
}

bool validatePhone(const std::string &phone)
{
    std::regex phone_regex(R"(^\d{11}$)");
    return std::regex_match(phone, phone_regex);
}

std::ostream &operator<<(std::ostream &os, const Advertisement &ad)
{
    os << ad.category << "\n"
       << ad.section << "\n"
       << ad.text << "\n"
       << ad.date << "\n"
       << ad.phone << "\n";
    return os;
}

std::istream &operator>>(std::istream &is, Advertisement &ad)
{
    do
    {
        std::cout << "Введите категорию: ";
        std::getline(is, ad.category);
        if (ad.category.empty())
        {
            std::cout << "Категория не может быть пустой. Попробуйте снова.\n";
        }
    } while (ad.category.empty());

    do
    {
        std::cout << "Введите секцию: ";
        std::getline(is, ad.section);
        if (ad.section.empty())
        {
            std::cout << "Секция не может быть пустой. Попробуйте снова.\n";
        }
    } while (ad.section.empty());

    do
    {
        std::cout << "Введите текст объявления: ";
        std::getline(is, ad.text);
        if (ad.text.empty())
        {
            std::cout << "Текст объявления не может быть пустым. Попробуйте снова.\n";
        }
    } while (ad.text.empty());

    do
    {
        std::cout << "Введите дату (YYYY-MM-DD): ";
        std::getline(is, ad.date);
        if (!validateDate(ad.date))
        {
            std::cout << "Дата должна быть в формате YYYY-MM-DD. Попробуйте снова.\n";
        }
    } while (!validateDate(ad.date));

    do
    {
        std::cout << "Введите телефон (11 цифр): ";
        std::getline(is, ad.phone);
        if (!validatePhone(ad.phone))
        {
            std::cout << "Телефон должен содержать ровно 11 цифр. Попробуйте снова.\n";
        }
    } while (!validatePhone(ad.phone));

    return is;
}

void Advertisement::deserialize(std::istream &is)
{
    std::getline(is, category);
    std::getline(is, section);
    std::getline(is, text);
    std::getline(is, date);
    std::getline(is, phone);
}

void Advertisement::print()
{
    std::cout << "--- Объявление ---\n"
              << "Категория: " << category << "\n"
              << "Секция: " << section << "\n"
              << "Текст объявления: " << text << "\n"
              << "Дата: " << date << "\n"
              << "Телефон: " << phone << "\n\n";
}

Tree::Node *Tree::insertRec(Node *node, const Advertisement &ad)
{
    if (!node)
        return new Node(ad);

    if (ad.getDate() < node->data.getDate())
        node->left = insertRec(node->left, ad);
    else
        node->right = insertRec(node->right, ad);

    return node;
}

Tree::Node *Tree::insertRecCategory(Node *node, const Advertisement &ad)
{
    if (!node)
        return new Node(ad);

    if (ad.getCategory() < node->data.getCategory())
        node->left = insertRecCategory(node->left, ad);
    else
        node->right = insertRecCategory(node->right, ad);

    return node;
}

void Tree::clearRec(Node *node)
{
    if (!node)
        return;

    clearRec(node->left);
    clearRec(node->right);

    delete node;
}

void Tree::searchByCategoryRec(const std::string &category, Node *node) const
{
    if (!node)
        return;

    if (node->data.getCategory() == category)
    {
        Advertisement ad = node->data;
        ad.print();
    }

    if (category < node->data.getCategory())
    {
        searchByCategoryRec(category, node->left);
    }

    else if (category > node->data.getCategory())
    {
        searchByCategoryRec(category, node->right);
    }

    if (node->left && node->data.getCategory() == category)
    {
        searchByCategoryRec(category, node->left);
    }

    if (node->right && node->data.getCategory() == category)
    {
        searchByCategoryRec(category, node->right);
    }
}

void Tree::inOrderRec(Node *node) const
{
    TreeIterator it(root);

    while (it.hasNext())
    {
        Node *node = it.next();
        Advertisement ad = node->data;
        ad.print();
    }
}

void Tree::TreeIterator::pushLeft(Node *node)
{
    while (node)
    {
        stack.push(node);
        node = node->left;
    }
}

bool Tree::TreeIterator::hasNext()
{
    return !stack.empty();
}

Tree::Node *Tree::TreeIterator::next()
{
    if (stack.empty())
        return nullptr;

    Node *node = stack.top();
    stack.pop();

    pushLeft(node->right);
    return node;
}

void Tree::insert(const Advertisement &ad)
{
    root = insertRec(root, ad);
}

void Tree::insert_category(const Advertisement &ad)
{
    root = insertRecCategory(root, ad);
}

void Tree::searchByCategory(const std::string &category, Tree &tree) const
{
    searchByCategoryRec(category, tree.getRoot());
}

void Tree::inOrderTraversal() const
{
    inOrderRec(root);
}

Tree::Node *Tree::getRoot() const
{
    return root;
}

Tree::TreeIterator Tree::getIterator()
{
    return TreeIterator(root);
}

void Tree::clear()
{
    clearRec(root);
    root = nullptr;
}

void BinaryFileHandler::addRecord(const Advertisement &ad)
{
    std::ofstream file(file_name, std::ios::binary | std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Ошибка открытия файла для записи.\n";
        return;
    }

    std::stringstream ss;
    ss << ad;
    std::string serialized = ss.str();
    size_t size = serialized.size();

    file.write(reinterpret_cast<const char *>(&size), sizeof(size));
    file.write(serialized.c_str(), size);
    file.close();
}

void BinaryFileHandler::insertRecordAtIndex(size_t index, const Advertisement &ad)
{
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    std::ofstream output_file("temp_file", std::ios::binary);
    if (!output_file.is_open())
    {
        std::cerr << "Ошибка открытия временного файла для записи.\n";
        return;
    }

    size_t current_index = 0;
    bool inserted = false;

    while (input_file.peek() != EOF)
    {
        size_t size;
        input_file.read(reinterpret_cast<char *>(&size), sizeof(size));

        if (input_file.eof())
            break;

        std::string buffer(size, '\0');
        input_file.read(&buffer[0], size);

        if (current_index == index && !inserted)
        {
            std::stringstream ss;
            ss << ad;
            std::string serialized = ss.str();
            size_t serialized_size = serialized.size();
            output_file.write(reinterpret_cast<const char *>(&serialized_size), sizeof(serialized_size));
            output_file.write(serialized.c_str(), serialized_size);
            inserted = true;
        }

        output_file.write(reinterpret_cast<const char *>(&size), sizeof(size));
        output_file.write(buffer.c_str(), size);

        current_index++;
    }

    if (!inserted)
    {
        std::stringstream ss;
        ss << ad;
        std::string serialized = ss.str();
        size_t serialized_size = serialized.size();
        output_file.write(reinterpret_cast<const char *>(&serialized_size), sizeof(serialized_size));
        output_file.write(serialized.c_str(), serialized_size);
    }

    input_file.close();
    output_file.close();

    std::remove(file_name.c_str());
    std::rename("temp_file", file_name.c_str());
}

void BinaryFileHandler::loadFromFile(Tree &adTree, std::string param)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    while (file.peek() != EOF)
    {
        size_t size;
        file.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (file.eof())
            break;

        std::string buffer(size, '\0');
        file.read(&buffer[0], size);

        std::istringstream iss(buffer);
        Advertisement ad;
        std::getline(iss, ad.category);
        std::getline(iss, ad.section);
        std::getline(iss, ad.text);
        std::getline(iss, ad.date);
        std::getline(iss, ad.phone);

        if (param == "Категория")
        {
            adTree.insert_category(ad);
        }
        else if (param == "Дата")
        {
            adTree.insert(ad);
        }
    }
    file.close();
}

void BinaryFileHandler::deleteRecordByIndex(size_t index, Tree &adTree)
{
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    bool deleted = false;

    std::ofstream temp_file("temp_file.bin", std::ios::binary);
    if (!temp_file.is_open())
    {
        std::cerr << "Ошибка открытия временного файла для записи.\n";
        input_file.close();
        return;
    }

    size_t currentIndex = 0;

    while (input_file.peek() != EOF)
    {
        size_t size;
        input_file.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (input_file.eof())
            break;

        std::string buffer(size, '\0');
        input_file.read(&buffer[0], size);

        if (currentIndex != index)
        {
            temp_file.write(reinterpret_cast<const char *>(&size), sizeof(size));
            temp_file.write(buffer.c_str(), size);
        }
        else
        {
            deleted = true;
        }
        currentIndex++;
    }

    input_file.close();
    temp_file.close();

    if (std::remove(file_name.c_str()) != 0)
    {
        std::cerr << "Ошибка удаления исходного файла.\n";
        return;
    }
    if (std::rename("temp_file.bin", file_name.c_str()) != 0)
    {
        std::cerr << "Ошибка переименования временного файла.\n";
        return;
    }

    if (deleted)
    {
        std::cout << "Запись успешно удалена.\n";
    }
    else
    {
        std::cout << "Запись не найдена" << std::endl;
    }
}

void BinaryFileHandler::displayAllRecords()
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    while (file.peek() != EOF)
    {
        size_t size;
        file.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (file.eof())
            break;

        std::string buffer(size, '\0');
        file.read(&buffer[0], size);

        std::istringstream iss(buffer);
        Advertisement ad;
        std::getline(iss, ad.category);
        std::getline(iss, ad.section);
        std::getline(iss, ad.text);
        std::getline(iss, ad.date);
        std::getline(iss, ad.phone);

        ad.print();
    }
    file.close();
}

void BinaryFileHandler::updateRecordByIndex(size_t index, const Advertisement &newAd, Tree &adTree)
{
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }
    std::ofstream temp_file("temp.bin", std::ios::binary);
    if (!temp_file.is_open())
    {
        std::cerr << "Ошибка открытия временного файла.\n";
        return;
    }
    size_t currentIndex = 0;
    bool updated = false;
    while (input_file.peek() != EOF)
    {
        size_t size;
        input_file.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (input_file.eof())
            break;

        std::string buffer(size, '\0');
        input_file.read(&buffer[0], size);

        if (currentIndex == index)
        {
            std::stringstream ss;
            ss << newAd;
            std::string serialized = ss.str();
            size = serialized.size();
            temp_file.write(reinterpret_cast<const char *>(&size), sizeof(size));
            temp_file.write(serialized.c_str(), size);
            updated = true;
        }
        else
        {
            temp_file.write(reinterpret_cast<const char *>(&size), sizeof(size));
            temp_file.write(buffer.c_str(), size);
        }

        currentIndex++;
    }

    input_file.close();
    temp_file.close();

    if (updated)
    {
        std::remove(file_name.c_str());
        std::rename("temp.bin", file_name.c_str());
        std::cout << "Объявление успешно обновлено.\n";
    }
    else
    {
        std::cout << "По индексу не найдено ни одного объявления " << index << ".\n";
        std::remove("temp.bin");
    }
}

std::string BinaryFileHandler::extractRecordByIndex(size_t index)
{
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return "";
    }

    size_t currentIndex = 0;

    while (input_file.peek() != EOF)
    {
        size_t recordSize;
        input_file.read(reinterpret_cast<char *>(&recordSize), sizeof(recordSize));
        if (input_file.eof())
            break;

        std::string buffer(recordSize, '\0');
        input_file.read(&buffer[0], recordSize);

        if (currentIndex == index)
        {
            input_file.close();
            std::istringstream iss(buffer);
            Advertisement ad;
            std::getline(iss, ad.category);
            std::getline(iss, ad.section);
            std::getline(iss, ad.text);
            std::getline(iss, ad.date);
            std::getline(iss, ad.phone);

            ad.print();

            return buffer;
        }

        ++currentIndex;
    }

    input_file.close();

    std::cerr << "Индекс выходит за пределы.\n";
    return "";
}

void BinaryFileHandler::displayPage(size_t page, size_t items_per_page)
{
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    size_t skipped = 0;
    size_t start_index = (page - 1) * items_per_page;

    while (skipped < start_index && !input_file.eof())
    {
        size_t size;
        input_file.read(reinterpret_cast<char *>(&size), sizeof(size));

        if (input_file.eof())
            break;

        input_file.seekg(size, std::ios::cur);
        skipped++;
    }

    size_t current_index = 0;
    std::cout << "Страница " << page << ":\n";

    while (current_index < items_per_page && !input_file.eof())
    {
        size_t size;
        input_file.read(reinterpret_cast<char *>(&size), sizeof(size));

        if (input_file.eof())
            break;

        std::string buffer(size, '\0');
        input_file.read(&buffer[0], size);
        std::cout << buffer << "\n";

        current_index++;
    }

    std::cout << "Конец страницы " << page << "\n";

    input_file.close();
}

void BinaryFileHandler::navigatePages(size_t items_per_page)
{
    size_t current_page = 1;
    char user_input;

    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    size_t total_records = 0;
    while (!input_file.eof())
    {
        size_t size;
        input_file.read(reinterpret_cast<char *>(&size), sizeof(size));

        if (input_file.eof())
            break;

        total_records++;
        input_file.seekg(size, std::ios::cur);
    }
    input_file.close();

    size_t total_pages = (total_records + items_per_page - 1) / items_per_page;

    do
    {
        displayPage(current_page, items_per_page);

        std::cout << "Введите 'n' для перехода к следующей страницы, 'p' для перехода к предыдущей, 'q' для выхода из режима: ";
        std::cin >> user_input;

        if (user_input == 'n' && current_page < total_pages)
        {
            current_page++;
        }
        else if (user_input == 'p' && current_page > 1)
        {
            current_page--;
        }

    } while (user_input != 'q');
}

