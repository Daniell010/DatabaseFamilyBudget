#include "ExpenseWidget.h"
#include "ui_ExpenseWidget.h"
#include "db/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

ExpenseWidget::ExpenseWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExpenseWidget)
{
    ui->setupUi(this);
    setupTable();
    loadUsers();
    loadCategories();
    loadExpenseRecords();
    ui->editDate->setDate(QDate::currentDate());

    connect(ui->btnAddExpense, &QPushButton::clicked,
            this, &ExpenseWidget::onAddExpenseClicked);
}

ExpenseWidget::~ExpenseWidget()
{
    delete ui;
}

void ExpenseWidget::setupTable()
{
    ui->tableExpense->setColumnCount(4);
    ui->tableExpense->setHorizontalHeaderLabels({ "Дата", "Категория", "Сумма", "Пользователь" });
    ui->tableExpense->horizontalHeader()->setStretchLastSection(true);
}

void ExpenseWidget::loadUsers()
{
    ui->comboUser->clear();
    QStringList users = DatabaseManager::instance().loadUsers();
    ui->comboUser->addItems(users);
}

void ExpenseWidget::loadCategories()
{
    QStringList categories = DatabaseManager::instance().loadExpenseCategories();
    ui->comboCategory->clear();
    ui->comboCategory->addItems(categories);
}

void ExpenseWidget::loadExpenseRecords()
{
    ui->tableExpense->setRowCount(0);
    QSqlQuery query("SELECT date, category, amount, user FROM expense");

    int row = 0;
    while (query.next()) {
        ui->tableExpense->insertRow(row);
        ui->tableExpense->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableExpense->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableExpense->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toDouble(), 'f', 2)));
        ui->tableExpense->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        row++;
    }
}

void ExpenseWidget::onAddExpenseClicked()
{
    QString dateStr = ui->editDate->date().toString("yyyy-MM-dd");
    QString category = ui->comboCategory->currentText();
    double amount = ui->editAmount->value();
    QString user = ui->comboUser->currentText();

    if (user.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO expense (date, category, amount, user) VALUES (?, ?, ?, ?)");
    query.addBindValue(dateStr);
    query.addBindValue(category);
    query.addBindValue(amount);
    query.addBindValue(user);

    if (!query.exec()) {
        qDebug() << "Insert expense failed:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить расход");
        return;
    }

    loadExpenseRecords();
    ui->editAmount->setValue(0.0);
}

void ExpenseWidget::onUpdate()
{
    loadUsers();
}
