#include "DatabaseManager.h"
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager() {}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::open(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    return m_db.open();
}

void DatabaseManager::init() {
    QSqlQuery query;

    // Доходы
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS income (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            source TEXT NOT NULL,
            amount REAL NOT NULL,
            user TEXT NOT NULL
        )
    )");

    // Пользователи
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        )
    )");

    // Расходы
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS expense (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            category TEXT NOT NULL,
            amount REAL NOT NULL,
            user TEXT NOT NULL
        )
    )");

    // Категории расходов
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS expense_categories (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE
        )
    )");
    query.exec(R"(
        INSERT OR IGNORE INTO expense_categories (name) VALUES
        ('Еда'), ('Одежда'), ('Мебель'), ('Развлечения'), ('Отдых'), ('Другое')
    )");

    // Продукты
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            category TEXT NOT NULL,
            calories_per_100g REAL NOT NULL,
            is_required INTEGER NOT NULL
        )
    )");

    // Категории продуктов
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS product_categories (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE
        )
    )");
    query.exec(R"(
        INSERT OR IGNORE INTO product_categories (name) VALUES
        ('Овощи'), ('Фрукты'), ('Мясо'), ('Молочные продукты'),
        ('Крупы'), ('Сладости'), ('Напитки')
    )");

    // Прогнозы
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS forecasts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            forecast_date TEXT NOT NULL,
            expected_income REAL NOT NULL,
            expected_expense REAL NOT NULL,
            comment TEXT
        )
    )");
}

bool DatabaseManager::insertIncome(const QString& date, const QString& source, double amount, const QString& user) {
    QSqlQuery query;
    query.prepare("INSERT INTO income (date, source, amount, user) VALUES (?, ?, ?, ?)");
    query.addBindValue(date);
    query.addBindValue(source);
    query.addBindValue(amount);
    query.addBindValue(user);

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError().text();
        return false;
    }

    return true;
}

QStringList DatabaseManager::loadUsers() {
    QStringList users;
    QSqlQuery query("SELECT name FROM users");
    while (query.next()) {
        users << query.value(0).toString();
    }
    return users;
}

bool DatabaseManager::addUser(const QString& name) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (name) VALUES (?)");
    query.addBindValue(name);
    return query.exec();
}

QStringList DatabaseManager::loadExpenseCategories() {
    QStringList list;
    QSqlQuery query("SELECT name FROM expense_categories ORDER BY name ASC");
    while (query.next())
        list << query.value(0).toString();
    return list;
}

bool DatabaseManager::addExpenseCategory(const QString& name) {
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO expense_categories (name) VALUES (?)");
    query.addBindValue(name);
    return query.exec();
}

QStringList DatabaseManager::loadProductCategories() {
    QStringList list;
    QSqlQuery query("SELECT name FROM product_categories ORDER BY name ASC");
    while (query.next())
        list << query.value(0).toString();
    return list;
}

bool DatabaseManager::addProductCategory(const QString& name) {
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO product_categories (name) VALUES (?)");
    query.addBindValue(name);
    return query.exec();
}
