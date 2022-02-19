#include "twothree.h"

TwoThree3::TwoThree3()
{

}

node* TwoThree3::split(node *item) {
    if (item->size < 3) return item;

    node *x = new node(item->key[0], item->first, item->second, nullptr, nullptr, item->parent); // Создаем две новые вершины,
    node *y = new node(item->key[2], item->third, item->fourth, nullptr, nullptr, item->parent);  // которые имеют такого же родителя, как и разделяющийся элемент.
    if (x->first)  x->first->parent = x;    // Правильно устанавливаем "родителя" "сыновей".
    if (x->second) x->second->parent = x;   // После разделения, "родителем" "сыновей" является "дедушка",
    if (y->first)  y->first->parent = y;    // Поэтому нужно правильно установить указатели.
    if (y->second) y->second->parent = y;

    if (item->parent) {
        item->parent->insert_to_node(item->key[1]);

        if (item->parent->first == item) item->parent->first = nullptr;
        else if (item->parent->second == item) item->parent->second = nullptr;
        else if (item->parent->third == item) item->parent->third = nullptr;

        // Дальше происходит своеобразная сортировка ключей при разделении.
        if (item->parent->first == nullptr) {
            item->parent->fourth = item->parent->third;
            item->parent->third = item->parent->second;
            item->parent->second = y;
            item->parent->first = x;
        } else if (item->parent->second == nullptr) {
            item->parent->fourth = item->parent->third;
            item->parent->third = y;
            item->parent->second = x;
        } else {
            item->parent->fourth = y;
            item->parent->third = x;
        }

        node *tmp = item->parent;
        delete item;
        return tmp;
    } else {
        x->parent = item;   // Так как в эту ветку попадает только корень,
        y->parent = item;   // то мы "родителем" новых вершин делаем разделяющийся элемент.
        item->become_node2(item->key[1], x, y);
        return item;
    }
}

node* TwoThree3::insert(node *p, int k) { // вставка ключа k в дерево с корнем p; всегда возвращаем корень дерева, т.к. он может меняться
    if (!p) return new node(k); // если дерево пусто, то создаем первую 2-3-вершину (корень)

    if (p->is_leaf()) p->insert_to_node(k);
    else if (k <= p->key[0]) insert(p->first, k);
    else if ((p->size == 1) || ((p->size == 2) && k <= p->key[1])) insert(p->second, k);
    else insert(p->third, k);

    return split(p);
}

node* TwoThree3::search(node *p, int k) { // Поиск ключа k в 2-3 дереве с корнем p.
    if (!p) return nullptr;

    if (p->find(k)) return p;
    else if (k < p->key[0]) return search(p->first, k);
    else if ((p->size == 2) && (k < p->key[1]) || (p->size == 1)) return search(p->second, k);
    else return search(p->third, k);
}

node* TwoThree3::search_min(node *p) { // Поиск узла с минимальным элементов в 2-3-дереве с корнем p.
    if (!p) return p;
    if (!(p->first)) return p;
    else return search_min(p->first);
}

node* TwoThree3::redistribute(node *leaf) {
    node *parent = leaf->parent;
    node *first = parent->first;
    node *second = parent->second;
    node *third = parent->third;

    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            parent->first = parent->second;
            parent->second = parent->third;
            parent->third = nullptr;
            parent->first->insert_to_node(parent->key[0]);
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;

            if (leaf->first != nullptr) parent->first->first = leaf->first;
            else if (leaf->second != nullptr) parent->first->first = leaf->second;

            if (parent->first->first != nullptr) parent->first->first->parent = parent->first;

            parent->remove_from_node(parent->key[0]);
            delete first;
        } else if (second == leaf) {
            first->insert_to_node(parent->key[0]);
            parent->remove_from_node(parent->key[0]);
            if (leaf->first != nullptr) first->third = leaf->first;
            else if (leaf->second != nullptr) first->third = leaf->second;

            if (first->third != nullptr) first->third->parent = first;

            parent->second = parent->third;
            parent->third = nullptr;

            delete second;
        } else if (third == leaf) {
            second->insert_to_node(parent->key[1]);
            parent->third = nullptr;
            parent->remove_from_node(parent->key[1]);
            if (leaf->first != nullptr) second->third = leaf->first;
            else if (leaf->second != nullptr) second->third = leaf->second;

            if (second->third != nullptr)  second->third->parent = second;

            delete third;
        }
    } else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first != nullptr) {
                leaf->second = leaf->first;
                leaf->first = nullptr;
            }

            leaf->insert_to_node(parent->key[1]);
            if (second->size == 2) {
                parent->key[1] = second->key[1];
                second->remove_from_node(second->key[1]);
                leaf->first = second->third;
                second->third = nullptr;
                if (leaf->first != nullptr) leaf->first->parent = leaf;
            } else if (first->size == 2) {
                parent->key[1] = second->key[0];
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first != nullptr) leaf->first->parent = leaf;

                second->key[0] = parent->key[0];
                parent->key[0] = first->key[1];
                first->remove_from_node(first->key[1]);
                second->first = first->third;
                if (second->first != nullptr) second->first->parent = second;
                first->third = nullptr;
            }
        } else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->first == nullptr) {
                    leaf->first = leaf->second;
                    leaf->second = nullptr;
                }
                second->insert_to_node(parent->key[1]);
                parent->key[1] = third->key[0];
                third->remove_from_node(third->key[0]);
                second->second = third->first;
                if (second->second != nullptr) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = nullptr;
            } else if (first->size == 2) {
                if (leaf->second == nullptr) {
                    leaf->second = leaf->first;
                    leaf->first = nullptr;
                }
                second->insert_to_node(parent->key[0]);
                parent->key[0] = first->key[1];
                first->remove_from_node(first->key[1]);
                second->first = first->third;
                if (second->first != nullptr) second->first->parent = second;
                first->third = nullptr;
            }
        } else if (first == leaf) {
            if (leaf->first == nullptr) {
                leaf->first = leaf->second;
                leaf->second = nullptr;
            }
            first->insert_to_node(parent->key[0]);
            if (second->size == 2) {
                parent->key[0] = second->key[0];
                second->remove_from_node(second->key[0]);
                first->second = second->first;
                if (first->second != nullptr) first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = nullptr;
            } else if (third->size == 2) {
                parent->key[0] = second->key[0];
                second->key[0] = parent->key[1];
                parent->key[1] = third->key[0];
                third->remove_from_node(third->key[0]);
                first->second = second->first;
                if (first->second != nullptr) first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != nullptr) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = nullptr;
            }
        }
    } else if (parent->size == 1) {
        leaf->insert_to_node(parent->key[0]);

        if (first == leaf && second->size == 2) {
            parent->key[0] = second->key[0];
            second->remove_from_node(second->key[0]);

            if (leaf->first == nullptr) leaf->first = leaf->second;

            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = nullptr;
            if (leaf->second != nullptr) leaf->second->parent = leaf;
        } else if (second == leaf && first->size == 2) {
            parent->key[0] = first->key[1];
            first->remove_from_node(first->key[1]);

            if (leaf->second == nullptr) leaf->second = leaf->first;

            leaf->first = first->third;
            first->third = nullptr;
            if (leaf->first != nullptr) leaf->first->parent = leaf;
        }
    }
    return parent;
}

node* TwoThree3::merge(node *leaf) {
    node *parent = leaf->parent;

    if (parent->first == leaf) {
        parent->second->insert_to_node(parent->key[0]);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;

        if (leaf->first != nullptr) parent->second->first = leaf->first;
        else if (leaf->second != nullptr) parent->second->first = leaf->second;

        if (parent->second->first != nullptr) parent->second->first->parent = parent->second;

        parent->remove_from_node(parent->key[0]);
        delete parent->first;
        parent->first = nullptr;
    } else if (parent->second == leaf) {
        parent->first->insert_to_node(parent->key[0]);

        if (leaf->first != nullptr) parent->first->third = leaf->first;
        else if (leaf->second != nullptr) parent->first->third = leaf->second;

        if (parent->first->third != nullptr) parent->first->third->parent = parent->first;

        parent->remove_from_node(parent->key[0]);
        delete parent->second;
        parent->second = nullptr;
    }

    if (parent->parent == nullptr) {
        node *tmp = nullptr;
        if (parent->first != nullptr) tmp = parent->first;
        else tmp = parent->second;
        tmp->parent = nullptr;
        delete parent;
        return tmp;
    }
    return parent;
}

node* TwoThree3::fix(node *leaf) {
    if (leaf->size == 0 && leaf->parent == nullptr) { // Случай 0, когда удаляем единственный ключ в дереве
        delete leaf;
        return nullptr;
    }
    if (leaf->size != 0) { // Случай 1, когда вершина, в которой удалили ключ, имела два ключа
        if (leaf->parent) return fix(leaf->parent);
        else return leaf;
    }

    node *parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
    else if (parent->size == 2 && parent->third->size == 2) leaf = redistribute(leaf); // Аналогично
    else leaf = merge(leaf); // Случай 3, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину

    return fix(leaf);
}

node* TwoThree3::remove(node *p, int k) { // Удаление ключа k в 2-3-дереве с корнем p.
    node *item = search(p, k); // Ищем узел, где находится ключ k

    if (!item) return p;

    node *min = nullptr;
    if (item->key[0] == k) min = search_min(item->second); // Ищем эквивалентный ключ
    else min = search_min(item->third);

    if (min) { // Меняем ключи местами
        int &z = (k == item->key[0] ? item->key[0] : item->key[1]);
        item->swap(z, min->key[0]);
        item = min; // Перемещаем указатель на лист, т.к. min - всегда лист
    }

    item->remove_from_node(k); // И удаляем требуемый ключ из листа
    return fix(item); // Вызываем функцию для восстановления свойств дерева.
}

const int Ellipse_width = 40, Ellipse_height = 30;
const int x_dist = 10, y_dist = 20;

void TwoThree3::Draw(QGraphicsScene* Scene, node* cur, int x, int y, int x_prev, int y_prev) {
    if (cur == nullptr) {
        /* Scene->addEllipse(x_start + drawn * (Ellipse_width + 10), y_start + drawn * (Ellipse_height + 10), Ellipse_width, Ellipse_height);
        QString s = "NULL";
        auto text = Scene->addText(s);
        text->setTextWidth(100);
        text->setPos(x_start + drawn * (Ellipse_width + 10) + Ellipse_width / 4, y_start + drawn * (Ellipse_height + 10)); */
        return;
    }

    // Draw(Scene, cur->left, x - (Ellipse_width + x_dist / 2) * (cur->left?cur->left->sz:0) / 2 - Ellipse_width - x_dist / 2, y + (Ellipse_height + y_dist), x, y);
    // Draw(Scene, cur->right, x + (Ellipse_width + x_dist / 2) * (cur->right?cur->right->sz:0) / 2 + Ellipse_width + x_dist / 2, y + (Ellipse_height + y_dist), x, y);

    if (cur->third != nullptr) {
        Draw(Scene, cur->first, x - (Ellipse_width + x_dist) - Ellipse_width - x_dist / 2, y + (Ellipse_height + y_dist), x, y);
        Draw(Scene, cur->second, x, y + (Ellipse_height + y_dist), x, y);
        Draw(Scene, cur->third, x + (Ellipse_width + x_dist) + Ellipse_width + x_dist / 2, y + (Ellipse_height + y_dist), x, y);
        QString s = QString::number(cur->key[0]);
        QString t = QString::number(cur->key[1]);
        s = s + " ";
        s = s + t;
        auto text = Scene->addText(s);
        text->setTextWidth(100);
        text->setPos(x + Ellipse_width / 4, y);
    }

    else {
        Draw(Scene, cur->first, x - (Ellipse_width + x_dist) - Ellipse_width - x_dist / 2, y + (Ellipse_height + y_dist), x, y);
        Draw(Scene, cur->second, x + (Ellipse_width + x_dist) + Ellipse_width + x_dist / 2, y + (Ellipse_height + y_dist), x, y);
        QString s = QString::number(cur->key[0]);
        auto text = Scene->addText(s);
        text->setTextWidth(100);
        text->setPos(x + Ellipse_width / 4, y);
    }

    if (x_prev != -1 && y_prev != -1) {
        Scene->addLine(QLineF(x_prev + Ellipse_width / 2, y_prev + Ellipse_height, x + Ellipse_width / 2, y));
    }
    Scene->addEllipse(x, y, Ellipse_width, Ellipse_height);

}
