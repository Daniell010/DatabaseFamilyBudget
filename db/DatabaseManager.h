#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DatabaseManager {
public:
    static DatabaseManager& instance();
    bool open(const QString& path);
    void init();

    bool insertIncome(const QString& date, const QString& source, double amount, const QString& user);

    QStringList loadUsers();
    bool addUser(const QString &name);

    QStringList loadExpenseCategories();
    bool addExpenseCategory(const QString& name);

    QStringList loadProductCategories();
    bool addProductCategory(const QString& name);

private:
    DatabaseManager();
    QSqlDatabase m_db;
};

