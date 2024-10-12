#include "database.h"
#include <QStandardPaths>
#include <QCoreApplication>


Database::Database() {
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/linkedlist.db";

    //Checking if the dir exist >_<
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbPath);

    qDebug() << "Database path:" << db_.databaseName();
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
}

Database::~Database() {
    if (db_.isOpen()) {
        db_.close();
    }
}

bool Database::connect() {
    if (!db_.open()) {
        qDebug() << "Error: connection with database failed:" << db_.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok";

    QSqlQuery query(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS list (id INTEGER PRIMARY KEY, data INTEGER, prev INTEGER, next INTEGER)")) {
        qDebug() << "Failed to create table 'list':" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Table 'list' created or already exists";
    }
    return true;
}


void Database::saveList(const LinkedList<int>& list) {
    if (!db_.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db_);
    if (!query.exec("DELETE FROM list")) {
        qDebug() << "Error clearing the list table:" << query.lastError().text();
    }

    Node<int>* current = list.head();
    Node<int>* previous = nullptr;
    int id = 0;

    while (current) {
        Node<int>* nextNode = current->next;

        query.prepare("INSERT INTO list (id, data, prev, next) VALUES (:id, :data, :prev, :next)");
        query.bindValue(":id", id++);
        query.bindValue(":data", current->data);
        query.bindValue(":prev", previous ? previous->data : QVariant(QVariant::Int)); // Set prev value
        query.bindValue(":next", nextNode ? nextNode->data : QVariant(QVariant::Int)); // Set next value

        if (!query.exec()) {
            qDebug() << "Error inserting data into 'list':" << query.lastError().text();
        }

        previous = current;  // Keep track of the previous node
        current = nextNode;  // Move to the next node
    }
}



LinkedList<int>* Database::loadList() {
    LinkedList<int>* list = new LinkedList<int>();
    if (!db_.isOpen()) {
        qDebug() << "Database is not open!";
        return list;
    }

    QSqlQuery query(db_);
    if (!query.exec("SELECT id, data, prev, next FROM list ORDER BY id")) {
        qDebug() << "Error reading from the list table:" << query.lastError().text();
        return list;
    }

    QMap<int, Node<int>*> nodeMap;  // To keep track of nodes using their IDs

    while (query.next()) {
        int id = query.value(0).toInt();
        int data = query.value(1).toInt();
        Node<int>* newNode = new Node<int>(data);

        if (list->head() == nullptr) {
            list->addAfter(nullptr, data);  // Add the first node
        } else {
            Node<int>* lastNode = list->access(list->size() - 1);
            list->addAfter(lastNode, data);
        }

        // Add the new node to the map with its ID
        nodeMap[id] = newNode;
    }

    return list;
}

