#ifndef ADVERTISEMENT_H
#define ADVERTISEMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <stack>
#include <regex>

class Advertisement
{

public:
    std::string category;
    std::string section;
    std::string text;
    std::string date;
    std::string phone;

    Advertisement() = default;

    Advertisement(const std::string &category, const std::string &section,
                  const std::string &text, const std::string &date, const std::string &phone)
        : category(category), section(section), text(text), date(date), phone(phone) {}

    friend std::ostream &operator<<(std::ostream &os, const Advertisement &ad);

    friend std::istream &operator>>(std::istream &is, Advertisement &ad);

    void deserialize(std::istream &is);

    const std::string &getCategory() const { return category; }
    const std::string &getDate() const { return date; }

    void print();
};

class Tree
{
    class Node
    {
    public:
        Advertisement data;
        Node *left;
        Node *right;

        Node(const Advertisement &ad) : data(ad), left(nullptr), right(nullptr) {}
    };

    Node *root;

    Node *insertRec(Node *node, const Advertisement &ad);

    Node *insertRecCategory(Node *node, const Advertisement &ad);

    void clearRec(Node *node);

    void searchByCategoryRec(const std::string &category, Node *node) const;

    void inOrderRec(Node *node) const;

    class TreeIterator
    {
    private:
        std::stack<Node *> stack;

        void pushLeft(Node *node);

    public:
        TreeIterator(Node *root)
        {
            pushLeft(root);
        }

        bool hasNext();

        Node *next();
    };

public:
    Tree() : root(nullptr) {}

    void insert(const Advertisement &ad);

    void insert_category(const Advertisement &ad);

    void searchByCategory(const std::string &category, Tree &tree) const;

    void inOrderTraversal() const;

    Node *getRoot() const;

    TreeIterator getIterator();

    void clear();
};

class BinaryFileHandler
{
    std::string file_name;

public:
    BinaryFileHandler(const std::string &file) : file_name(file) {}

    void addRecord(const Advertisement &ad);

    void insertRecordAtIndex(size_t index, const Advertisement &ad);

    void loadFromFile(Tree &adTree, std::string param);

    void deleteRecordByIndex(size_t index, Tree &adTree);

    void displayAllRecords();

    void updateRecordByIndex(size_t index, const Advertisement &newAd, Tree &adTree);

    std::string extractRecordByIndex(size_t index);

    void displayPage(size_t page, size_t items_per_page);

    void navigatePages(size_t items_per_page);
};

#endif