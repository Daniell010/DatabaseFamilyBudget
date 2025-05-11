#include "IncomeWidget.h"
#include "ui_IncomeWidget.h"
#include "db/DatabaseManager.h"

IncomeWidget::IncomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IncomeWidget)
{
    ui->setupUi(this);

    setupTable();
    loadUsers();
    loadIncomeRecords();
    ui->editDate->setDate(QDate::currentDate());

    connect(ui->btnAddIncome, &QPushButton::clicked,
            this, &IncomeWidget::onAddIncomeClicked);
}

IncomeWidget::~IncomeWidget()
{
    delete ui;
}

void IncomeWidget::setupTable()
{
    ui->tableIncome->setColumnCount(4);
    ui->tableIncome->setHorizontalHeaderLabels({ "Дата", "Источник", "Сумма", "Пользователь" });
    ui->tableIncome->horizontalHeader()->setStretchLastSection(true);
}

void IncomeWidget::onAddIncomeClicked()
{
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

    // Очистка полей
    ui->editSource->clear();
    ui->editAmount->setValue(0.0);
    DatabaseManager::instance().insertIncome(dateStr, source, amount, user);
    loadIncomeRecords();
    loadUsers();
}

void IncomeWidget::onUpdate()
{
    loadUsers();
}

void IncomeWidget::loadIncomeRecords()
{
    ui->tableIncome->setRowCount(0);  // очистка

    QSqlQuery query("SELECT date, source, amount, user FROM income");
    int row = 0;

    while (query.next()) {
        ui->tableIncome->insertRow(row);
        ui->tableIncome->setItem(row, 0, new QTableWidgetItem(query.value("date").toString()));
        ui->tableIncome->setItem(row, 1, new QTableWidgetItem(query.value("source").toString()));
        ui->tableIncome->setItem(row, 2, new QTableWidgetItem(QString::number(query.value("amount").toDouble(), 'f', 2)));
        ui->tableIncome->setItem(row, 3, new QTableWidgetItem(query.value("user").toString()));
        row++;
    }
}

void IncomeWidget::loadUsers()
{
    ui->comboUser->clear();
    QStringList users = DatabaseManager::instance().loadUsers();
    ui->comboUser->addItems(users);
}

