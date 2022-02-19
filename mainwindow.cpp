#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , avl_tree()
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Scene = new QGraphicsScene;
    View = new QGraphicsView(this);
    View->setGeometry(QRect(0, 0, 600, 600));
    View->setScene(Scene);

    ui->comboBox->addItem("AVL-tree");
    ui->comboBox->addItem("Red-Black tree");
    ui->comboBox->addItem("2,3-tree");
    ui->comboBox->addItem("Decart tree");
    ui->horizontalSlider->setMaximum(200);
    ui->horizontalSlider->setValue(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int key;
double mas = 1;
const int x_start = 300 - 30, y_start = 50;
std::vector<int> keys;

void MainWindow::on_Val_editingFinished()
{
    key = ui->Val->text().toInt();
}

int state = 1;
// 1 - avl
// 2 - red black
// 3 - 2,3
// 4 - decart

void MainWindow::on_AddButton_clicked()
{
    if (std::find(keys.begin(), keys.end(), key) != keys.end()) {
        return;
    }

    auto erase = Scene->addRect(0, 0, 1000, 1000);
    erase->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    erase->setPen(QPen(Qt::white));


    avl_tree.root = avl_tree.insert(avl_tree.root, key);
    decart_tree.add(key);
    twothree_tree.root = twothree_tree.insert(twothree_tree.root, key);
    keys.push_back(key);

    if (state == 1)
        avl_tree.Draw(Scene, avl_tree.root, x_start, y_start, -1, -1);
    if (state == 3)
        twothree_tree.Draw(Scene, twothree_tree.root, x_start, y_start, -1, -1);
    if (state == 4)
        decart_tree.Draw(Scene, decart_tree.Decart_root, x_start, y_start, -1, -1);
}

void MainWindow::on_DeleteButton_clicked()
{
    if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
        return;
    }

    auto erase = Scene->addRect(0, 0, 1000, 1000);
    erase->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    erase->setPen(QPen(Qt::white));


    avl_tree.root = avl_tree.remove(avl_tree.root, key);
    decart_tree.del(key);
    twothree_tree.root = twothree_tree.remove(twothree_tree.root, key);
    keys.push_back(key);

    if (state == 1)
        avl_tree.Draw(Scene, avl_tree.root, x_start, y_start, -1, -1);
    if (state == 3)
        twothree_tree.Draw(Scene, twothree_tree.root, x_start, y_start, -1, -1);
    if (state == 4)
        decart_tree.Draw(Scene, decart_tree.Decart_root, x_start, y_start, -1, -1);
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if (arg1 == "AVL-tree") {
        state = 1;
        Scene->clear();
        avl_tree.Draw(Scene, avl_tree.root, x_start, y_start, -1, -1);
    }
    if (arg1 == "2,3-tree") {
        state = 3;
        Scene->clear();
        twothree_tree.Draw(Scene, twothree_tree.root, x_start, y_start, -1, -1);
    }
    if (arg1 == "Decart tree") {
        state = 4;
        Scene->clear();
        decart_tree.Draw(Scene, decart_tree.Decart_root, x_start, y_start, -1, -1);
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if (mas == 0) return;
    View->scale(1.0/mas, 1.0/mas);
    mas = 50.0/value;
    if (value == 0) return;
    View->scale(50.0/value, 50.0/value);
}

void MainWindow::on_pushButton_clicked()
{
    Scene->clear();
    for (int i = 0; i < 5; ++i) {
        key = rand() % 101;
        while (std::find(keys.begin(), keys.end(), key) != keys.end()) {
            key = rand() % 101;
        }
        avl_tree.root = avl_tree.insert(avl_tree.root, key);
        decart_tree.add(key);
        twothree_tree.root = twothree_tree.insert(twothree_tree.root, key);
        keys.push_back(key);
    }
    if (state == 1)
        avl_tree.Draw(Scene, avl_tree.root, x_start, y_start, -1, -1);
    if (state == 3)
        twothree_tree.Draw(Scene, twothree_tree.root, x_start, y_start, -1, -1);
    if (state == 4)
        decart_tree.Draw(Scene, decart_tree.Decart_root, x_start, y_start, -1, -1);
}
