// database.cpp
#include "database.h"
#include <QStandardPaths>
#include <QCoreApplication>

// Constructor
Database::Database() {
    // Set the database path to the user's home directory or a directory of your choice
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/linkedlist.db";

    // Ensure the directory exists
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");  // Create the directory if it doesn't exist
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbPath);

    // Output the actual database path for debugging
    qDebug() << "Database path:" << db_.databaseName();
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
}

// Destructor
Database::~Database() {
    if (db_.isOpen()) {
        db_.close();
    }
}

// Establish database connection
bool Database::connect() {
    if (!db_.open()) {
        qDebug() << "Error: connection with database failed:" << db_.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok";

    // Check or create the table
    QSqlQuery query(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS list (id INTEGER PRIMARY KEY, data INTEGER)")) {
        qDebug() << "Failed to create table 'list':" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Table 'list' created or already exists";
    }
    return true;
}

// Save the linked list to the database
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
    int id = 1;
    while (current) {
        query.prepare("INSERT INTO list (id, data) VALUES (:id, :data)");
        query.bindValue(":id", id++);
        query.bindValue(":data", current->data);
        if (!query.exec()) {
            qDebug() << "Error inserting data into 'list':" << query.lastError().text();
        }
        current = current->next;
    }
}

// Load the linked list from the database
// Load the linked list from the database
LinkedList<int>* Database::loadList() {
    LinkedList<int>* list = new LinkedList<int>();
    if (!db_.isOpen()) {
        qDebug() << "Database is not open!";
        return list;
    }

    QSqlQuery query(db_);
    if (!query.exec("SELECT data FROM list ORDER BY id")) {
        qDebug() << "Error reading from the list table:" << query.lastError().text();
        return list;
    }

    Node<int>* lastNode = nullptr;
    while (query.next()) {
        int data = query.value(0).toInt();
        if (lastNode == nullptr) {
            list->addAfter(nullptr, data); // Add as the first node
            lastNode = list->head();       // Update lastNode to point to the first node
        } else {
            list->addAfter(lastNode, data);
            lastNode = lastNode->next;     // Move to the next node
        }
    }
    return list;
}

