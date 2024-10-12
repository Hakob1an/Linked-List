#include "mainwindow.h"
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    list_(new LinkedList<int>()),
    scene_(new QGraphicsScene(this)),
    db_(new Database()) {

    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);

    titleLabel_ = new QLabel("Linked List", this);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setFont(QFont("Arial", 16, QFont::Bold));

    graphicsView_ = new QGraphicsView(scene_, this);
    graphicsView_->setMinimumHeight(150);
    graphicsView_->setAlignment(Qt::AlignCenter);

    //addButton_ = new QPushButton("Add", this);
    addBeforeButton_ = new QPushButton("Add Before", this);
    addAfterButton_ = new QPushButton("Add After", this);
    removeButton_ = new QPushButton("Remove", this);
    accessButton_ = new QPushButton("Access", this);
    cleanButton_ = new QPushButton("Clean", this);
    sizeCapacityButton_ = new QPushButton("Size/Capacity", this);
    copyButton_ = new QPushButton("Copy", this);

    QString buttonStyle = "QPushButton {"
                          "background-color: #3498db;"
                          "color: white;"
                          "border-radius: 10px;"
                          "padding: 10px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #2980b9;"
                          "}";

    //addButton_->setStyleSheet(buttonStyle);
    addBeforeButton_->setStyleSheet(buttonStyle);
    addAfterButton_->setStyleSheet(buttonStyle);
    removeButton_->setStyleSheet(buttonStyle);
    accessButton_->setStyleSheet(buttonStyle);
    cleanButton_->setStyleSheet(buttonStyle);
    sizeCapacityButton_->setStyleSheet(buttonStyle);
    copyButton_->setStyleSheet(buttonStyle);

    titleLabel_->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50;");

    nodeInput_ = new QLineEdit(this);
    nodeInput_->setPlaceholderText("Enter node value");
    indexInput_ = new QLineEdit(this);
    indexInput_->setPlaceholderText("Enter index");
    statusLabel_ = new QLabel("Status: Ready", this);
    statusLabel_->setAlignment(Qt::AlignLeft);

    mainLayout_ = new QVBoxLayout();
    buttonLayout_ = new QHBoxLayout();
    inputLayout_ = new QHBoxLayout();

    mainLayout_->addWidget(titleLabel_);
    mainLayout_->addWidget(graphicsView_);

    //buttonLayout_->addWidget(addButton_);
    buttonLayout_->addWidget(addBeforeButton_);
    buttonLayout_->addWidget(addAfterButton_);
    buttonLayout_->addWidget(removeButton_);
    buttonLayout_->addWidget(accessButton_);
    buttonLayout_->addWidget(cleanButton_);
    buttonLayout_->addWidget(sizeCapacityButton_);
    buttonLayout_->addWidget(copyButton_);

    mainLayout_->addLayout(buttonLayout_);

    inputLayout_->addWidget(new QLabel("Node input:", this));
    inputLayout_->addWidget(nodeInput_);
    inputLayout_->addWidget(new QLabel("Index:", this));  // Label for index input
    inputLayout_->addWidget(indexInput_);
    inputLayout_->addWidget(statusLabel_);

    mainLayout_->addLayout(inputLayout_);

    centralWidget_->setLayout(mainLayout_);

    //connect(addButton_, &QPushButton::clicked, this, &MainWindow::on_addButton_clicked);
    connect(addBeforeButton_, &QPushButton::clicked, this, &MainWindow::on_addBeforeButton_clicked);
    connect(addAfterButton_, &QPushButton::clicked, this, &MainWindow::on_addAfterButton_clicked);
    connect(removeButton_, &QPushButton::clicked, this, &MainWindow::on_removeButton_clicked);
    connect(accessButton_, &QPushButton::clicked, this, &MainWindow::on_accessButton_clicked);
    connect(cleanButton_, &QPushButton::clicked, this, &MainWindow::on_cleanButton_clicked);
    connect(sizeCapacityButton_, &QPushButton::clicked, this, &MainWindow::on_sizeCapacityButton_clicked);
    connect(copyButton_, &QPushButton::clicked, this, &MainWindow::on_copyButton_clicked);

    db_->connect();

    updateVisualization();
}

MainWindow::~MainWindow() {
    delete list_;
    delete db_;
}

/*void MainWindow::updateVisualization() {
    scene_->clear();

    Node<int>* current = list_->head();
    int x = 0;
    const int nodeSize = 40;
    const int spacing = 60;
    QFont font("Arial", 10, QFont::Bold);

    while (current) {
        QGraphicsEllipseItem *circle = scene_->addEllipse(x, 0, nodeSize, nodeSize, QPen(Qt::black), QBrush(Qt::white));

        QGraphicsTextItem *text = scene_->addText(QString::number(current->data), font);
        text->setDefaultTextColor(Qt::black);
        text->setPos(x + nodeSize / 2 - text->boundingRect().width() / 2, nodeSize / 2 - text->boundingRect().height() / 2);

        if (current->next) {
            QGraphicsLineItem *line = scene_->addLine(x + nodeSize, nodeSize / 2, x + spacing, nodeSize / 2, QPen(Qt::black, 2));
            QPolygonF arrowHead;
            arrowHead << QPointF(x + spacing, nodeSize / 2)
                      << QPointF(x + spacing - 5, nodeSize / 2 - 5)
                      << QPointF(x + spacing - 5, nodeSize / 2 + 5);
            scene_->addPolygon(arrowHead, QPen(Qt::black), QBrush(Qt::black));
        }

        x += spacing;
        current = current->next;
    }

    // Center the scene in the view
    scene_->setSceneRect(0, 0, x, nodeSize);
    graphicsView_->setSceneRect(scene_->itemsBoundingRect());
    graphicsView_->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}*/
void MainWindow::updateVisualization() {
    scene_->clear();
    ellipses_.clear();

    Node<int>* current = list_->head();
    int x = 0;
    const int nodeSize = 40;
    const int spacing = 60;
    QFont font("Arial", 10, QFont::Bold);

    while (current) {
        QGraphicsEllipseItem *circle = scene_->addEllipse(x, 0, nodeSize, nodeSize, QPen(Qt::black), QBrush(Qt::white));
        ellipses_.append(circle);

        QGraphicsTextItem *text = scene_->addText(QString::number(current->data), font);
        text->setDefaultTextColor(Qt::black);
        text->setPos(x + nodeSize / 2 - text->boundingRect().width() / 2, nodeSize / 2 - text->boundingRect().height() / 2);

        if (current->next) {
            QGraphicsLineItem *line = scene_->addLine(x + nodeSize, nodeSize / 2, x + spacing, nodeSize / 2, QPen(Qt::black, 2));
            QPolygonF arrowHead;
            arrowHead << QPointF(x + spacing, nodeSize / 2)
                      << QPointF(x + spacing - 5, nodeSize / 2 - 5)
                      << QPointF(x + spacing - 5, nodeSize / 2 + 5);
            scene_->addPolygon(arrowHead, QPen(Qt::black), QBrush(Qt::black));
        }

        x += spacing;
        current = current->next;
    }

    scene_->setSceneRect(0, 0, x, nodeSize);
    graphicsView_->setSceneRect(scene_->itemsBoundingRect());
    graphicsView_->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}


void MainWindow::animateNode(QGraphicsEllipseItem *node) {
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect();
    node->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::updateStatus(const QString &message) {
    statusLabel_->setText("Status: " + message);
}

/*void MainWindow::on_addButton_clicked() {
    bool ok;
    int value = nodeInput_->text().toInt(&ok);
    if (ok) {
        if (list_->size() == 0) {
            list_->addAfter(nullptr, value);
        } else {
            Node<int>* lastNode = list_->access(list_->size() - 1);
            list_->addAfter(lastNode, value);
        }

        db_->saveList(*list_);

        updateVisualization();
        //animateNode(qgraphicsitem_cast<QGraphicsEllipseItem*>(scene_->items().last()));
        animateNode(ellipses_.last());

        updateStatus("Added value " + QString::number(value));
        nodeInput_->clear();
    } else {
        updateStatus("Invalid input for adding node");
    }
}*/
void MainWindow::on_addBeforeButton_clicked() {
    bool ok;
    int value = nodeInput_->text().toInt(&ok);
    int index = indexInput_->text().toInt(&ok);

    if (ok && list_->size() == 0) {
        list_->addAfter(nullptr, value);
        db_->saveList(*list_);
        updateVisualization();
        updateStatus("Added value " + QString::number(value) + " as the first item");
    }
    else if (ok && index >= 0 && index < list_->size()) {
        Node<int>* targetNode = list_->access(index);
        if (targetNode) {
            list_->addBefore(targetNode, value);
            db_->saveList(*list_);
            updateVisualization();
            updateStatus("Added value " + QString::number(value) + " before index " + QString::number(index));
            nodeInput_->clear();
            indexInput_->clear();
        }
    } else {
        updateStatus("Invalid index or value for adding before");
    }
}

void MainWindow::on_addAfterButton_clicked() {
    bool ok;
    int value = nodeInput_->text().toInt(&ok);
    int index = indexInput_->text().toInt(&ok);

    if (ok && index >= 0 && index < list_->size()) {
        Node<int>* targetNode = list_->access(index);
        if (targetNode) {
            list_->addAfter(targetNode, value);
            db_->saveList(*list_);
            updateVisualization();
            updateStatus("Added value " + QString::number(value) + " after index " + QString::number(index));
            nodeInput_->clear();
            indexInput_->clear();
        }
    } else {
        updateStatus("Invalid index or value for adding after");
    }
}


void MainWindow::fadeOutNode(QGraphicsEllipseItem *node) {
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    node->setGraphicsEffect(effect);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(1);
    animation->setEndValue(0);

    connect(animation, &QPropertyAnimation::finished, this, [node]() {
        delete node;
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void MainWindow::on_removeButton_clicked() {
    bool ok;
    int index = indexInput_->text().toInt(&ok);
    if (ok && index >= 0 && index < list_->size()) {
        Node<int>* node = list_->access(index);
        if (node) {
            fadeOutNode(ellipses_[index]);

            list_->remove(node);

            QSqlQuery query;
            query.prepare("DELETE FROM list WHERE id = :id");
            query.bindValue(":id", index);
            if (!query.exec()) {
                qDebug() << "Failed to delete node from database: " << query.lastError().text();
            } else {
                qDebug() << "Node deleted from database";
            }

            updateVisualization();
            updateStatus("Removed node at index " + QString::number(index));
            indexInput_->clear();
        } else {
            updateStatus("Node not found at index " + QString::number(index));
        }
    } else {
        updateStatus("Invalid index for removal");
    }
}



void MainWindow::on_cleanButton_clicked() {
    list_->clear();
    list_->printList();
    updateVisualization();
    updateStatus("List cleaned");
}

void MainWindow::on_sizeCapacityButton_clicked() {
    int size = list_->size();
    int capacity = list_->capacity();
    updateStatus("Size: " + QString::number(size) + ", Capacity: " + QString::number(capacity));
    QMessageBox::information(this, "Size/Capacity", "Size: " + QString::number(size) + "\nCapacity: " + QString::number(capacity));
}

void MainWindow::on_copyButton_clicked() {
    // Load the list from the database
    LinkedList<int>* copiedList = db_->loadList();

    // If the list is successfully loaded, replace the current list
    if (copiedList) {
        delete list_;  // Delete the current list
        list_ = copiedList;  // Assign the loaded list

        // Update the visualization to reflect the new list
        updateVisualization();

        // Update the status
        updateStatus("List loaded from database");
        QMessageBox::information(this, "Copy List", "The linked list has been loaded from the database.");
    } else {
        updateStatus("Failed to load list from database");
        QMessageBox::warning(this, "Error", "Failed to load linked list from the database.");
    }
}


void MainWindow::highlightNode(QGraphicsEllipseItem *node) {
    // Define the blue color using the hex code #3498db
    QColor color("#3498db");

    // Change the node's color to the custom blue
    node->setBrush(QBrush(color));

    // Restore the original color (white) after a delay
    QTimer::singleShot(1000, [=]() {
        node->setBrush(QBrush(Qt::white));  // Reset to original color (or replace with any color you need)
    });
}




void MainWindow::on_accessButton_clicked() {
    bool ok;
    int index = indexInput_->text().toInt(&ok);
    if (ok && index >= 0 && index < list_->size()) {
        Node<int>* node = list_->access(index);
        if (node) {
            /*QGraphicsEllipseItem *item = dynamic_cast<QGraphicsEllipseItem *>(scene_->items()[index]);
            if (item) {
                highlightNode(item);
            }*/
            highlightNode(ellipses_[index]);
            updateStatus("Accessed node at index " + QString::number(index) + " with value " + QString::number(node->data));
            //QMessageBox::information(this, "Node Access", "Node at index " + QString::number(index) + " has value " + QString::number(node->data));
            indexInput_->clear();
        } else {
            updateStatus("Node not found at index " + QString::number(index));
        }
    } else {
        updateStatus("Invalid index for access");
    }
}
