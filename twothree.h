#ifndef TWOTHREE_H
#define TWOTHREE_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>

struct node {

    int size;      // количество занятых ключей
    int key[3];
    node *first;   // *first <= key[0];
    node *second;  // key[0] <= *second < key[1];
    node *third;   // key[1] <= *third < key[2];
    node *fourth;  // kye[2] <= *fourth.
    node *parent; //Указатель на родителя нужен для того, потому что адрес корня может меняться при удалении

    bool find(int k) { // Этот метод возвращает true, если ключ k находится в вершине, иначе false.
        for (int i = 0; i < size; ++i)
            if (key[i] == k) return true;
        return false;
    }

    void swap(int &x, int &y) {
        int r = x;
        x = y;
        y = r;
    }

    void sort2(int &x, int &y) {
        if (x > y) swap(x, y);
    }

    void sort3(int &x, int &y, int &z) {
        if (x > y) swap(x, y);
        if (x > z) swap(x, z);
        if (y > z) swap(y, z);
    }

    void sort() { // Ключи в вершинах должны быть отсортированы
        if (size == 1) return;
        if (size == 2) sort2(key[0], key[1]);
        if (size == 3) sort3(key[0], key[1], key[2]);
    }

    void insert_to_node(int k) {  // Вставляем ключ k в вершину (не в дерево)
        key[size] = k;
        size++;
        sort();
    }

    void remove_from_node(int k) { // Удаляем ключ k из вершины (не из дерева)
        if (size >= 1 && key[0] == k) {
            key[0] = key[1];
            key[1] = key[2];
            size--;
        } else if (size == 2 && key[1] == k) {
            key[1] = key[2];
            size--;
        }
    }

    void become_node2(int k, node *first_, node *second_) {  // Преобразовать в 2-вершину.
        key[0] = k;
        first = first_;
        second = second_;
        third = nullptr;
        fourth = nullptr;
        parent = nullptr;
        size = 1;
    }

    bool is_leaf() { // Является ли узел листом; проверка используется при вставке и удалении.
        return (first == nullptr) && (second == nullptr) && (third == nullptr);
    }


    // Создавать всегда будем вершину только с одним ключом
    node(int k): size(1), key{k, 0, 0}, first(nullptr), second(nullptr),
                 third(nullptr), fourth(nullptr), parent(nullptr) {}

    node (int k, node *first_, node *second_, node *third_, node *fourth_, node *parent_):
                            size(1), key{k, 0, 0}, first(first_), second(second_),
                            third(third_), fourth(fourth_), parent(parent_) {}

};


class TwoThree3
{
public:
    TwoThree3();

    node *split(node *item);

    node *insert(node *p, int k);

    node *search(node *p, int k);

    node *search_min(node *p);

    node *merge(node *leaf);

    node *redistribute(node *leaf); // Перераспределение также используется при удалении;

    node *fix(node *leaf); // Используется после удаления для возвращения свойств дереву (использует merge или redistribute)

    node *remove(node *p, int k); // Собственно, из названия понятно;

    void Draw(QGraphicsScene* Scene, node* cur, int x, int y, int x_prev, int y_prev);

    node* root;
};

#endif // TWOTHREE_H
