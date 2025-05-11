#include "IncomeWidget.h"
#include "ui_IncomeWidget.h"
#include "db/DatabaseManager.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

IncomeWidget::IncomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IncomeWidget)
{
    ui->setupUi(this);

    setupTable();
    loadUsers();
    loadIncomeRecords();
    ui->editDate->setDate(QDate::currentDate());

    connect(ui->btnAddIncome, &QPushButton::clicked, this, &IncomeWidget::onAddIncomeClicked);
    connect(ui->tableIncome, &QTableWidget::cellChanged, this, &IncomeWidget::onIncomeCellChanged);
}

IncomeWidget::~IncomeWidget()
{
    delete ui;
}

void IncomeWidget::setupTable()
{
    ui->tableIncome->setColumnCount(5);
    ui->tableIncome->setHorizontalHeaderLabels({ "Дата", "Источник", "Сумма", "Пользователь" });
    ui->tableIncome->horizontalHeader()->setStretchLastSection(true);
    ui->tableIncome->setColumnHidden(4, true);  // Скрываем ID
}

void IncomeWidget::onAddIncomeClicked()
{
    ui->tableIncome->blockSignals(true);
    QString dateStr = ui->editDate->date().toString("yyyy-MM-dd");
    QString source = ui->editSource->text();
    double amount = ui->editAmount->value();
    QString user = ui->comboUser->currentText();

    int row = ui->tableIncome->rowCount();
    ui->tableIncome->insertRow(row);
    ui->tableIncome->setItem(row, 0, new QTableWidgetItem(dateStr));
    ui->tableIncome->setItem(row, 1, new QTableWidgetItem(source));
    ui->tableIncome->setItem(row, 2, new QTableWidgetItem(QString::number(amount, 'f', 2)));
    ui->tableIncome->setItem(row, 3, new QTableWidgetItem(user));

    if (user.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Создайте пользователя");
        return;
    }

    // Очистка полей
    ui->editSource->clear();
    ui->editAmount->setValue(0.0);
    DatabaseManager::instance().insertIncome(dateStr, source, amount, user);
    loadIncomeRecords();
    loadUsers();
    ui->tableIncome->blockSignals(false);
}

void IncomeWidget::onUpdate()
{
    loadUsers();
}

void IncomeWidget::loadIncomeRecords()
{
    ui->tableIncome->setRowCount(0);  // очистка

    QSqlQuery query("SELECT id, date, source, amount, user FROM income");
    int row = 0;

    while (query.next()) {
        ui->tableIncome->insertRow(row);
        ui->tableIncome->setItem(row, 0, new QTableWidgetItem(query.value("date").toString()));
        ui->tableIncome->setItem(row, 1, new QTableWidgetItem(query.value("source").toString()));
        ui->tableIncome->setItem(row, 2, new QTableWidgetItem(QString::number(query.value("amount").toDouble(), 'f', 2)));
        ui->tableIncome->setItem(row, 3, new QTableWidgetItem(query.value("user").toString()));
        ui->tableIncome->setItem(row, 4, new QTableWidgetItem(query.value("id").toString()));
        row++;
    }
}

void IncomeWidget::loadUsers()
{
    ui->comboUser->clear();
    QStringList users = DatabaseManager::instance().loadUsers();
    ui->comboUser->addItems(users);
}

void IncomeWidget::onIncomeCellChanged(int row, int column)
{
    if (column < 0 || row < 0) return;

    QString date = ui->tableIncome->item(row, 0)->text();
    QString source = ui->tableIncome->item(row, 1)->text();
    double amount = ui->tableIncome->item(row, 2)->text().toDouble();
    QString user = ui->tableIncome->item(row, 3)->text();
    int id = ui->tableIncome->item(row, 4)->text().toInt();

    QSqlQuery query;
    query.prepare("UPDATE income SET date = ?, source = ?, amount = ?, user = ? WHERE id = ?");
    query.addBindValue(date);
    query.addBindValue(source);
    query.addBindValue(amount);
    query.addBindValue(user);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "UPDATE failed:" << query.lastError().text();
    }
}

void IncomeWidget::onDeleteIncomeClicked()
{
    int row = ui->tableIncome->currentRow();
    if (row < 0) return;

    int id = ui->tableIncome->item(row, 4)->text().toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM income WHERE id = ?");
    query.addBindValue(id);
    query.exec();

    ui->tableIncome->removeRow(row);
}
