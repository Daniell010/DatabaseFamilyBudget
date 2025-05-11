#include "ForecastWidget.h"
#include "ui_ForecastWidget.h"
#include "db/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

ForecastWidget::ForecastWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForecastWidget)
{
    ui->setupUi(this);
    setupTable();
    loadForecastRecords();
    ui->editDate->setDate(QDate::currentDate());

    connect(ui->btnAddForecast, &QPushButton::clicked,
            this, &ForecastWidget::onAddForecastClicked);
}

ForecastWidget::~ForecastWidget()
{
    delete ui;
}

void ForecastWidget::setupTable()
{
    ui->tableForecasts->setColumnCount(4);
    ui->tableForecasts->setHorizontalHeaderLabels({ "Дата", "Доходы", "Расходы", "Комментарий" });
    ui->tableForecasts->horizontalHeader()->setStretchLastSection(true);
}

void ForecastWidget::loadForecastRecords()
{
    ui->tableForecasts->setRowCount(0);
    QSqlQuery query("SELECT forecast_date, expected_income, expected_expense, comment FROM forecasts");

    int row = 0;
    while (query.next()) {
        ui->tableForecasts->insertRow(row);
        ui->tableForecasts->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableForecasts->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble(), 'f', 2)));
        ui->tableForecasts->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toDouble(), 'f', 2)));
        ui->tableForecasts->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        row++;
    }
}

void ForecastWidget::onAddForecastClicked()
{
    QString dateStr = ui->editDate->date().toString("yyyy-MM-dd");
    double expectedIncome = ui->editExpectedIncome->value();
    double expectedExpense = ui->editExpectedExpense->value();
    QString comment = ui->editComment->text();

    QSqlQuery query;
    query.prepare("INSERT INTO forecasts (forecast_date, expected_income, expected_expense, comment) VALUES (?, ?, ?, ?)");
    query.addBindValue(dateStr);
    query.addBindValue(expectedIncome);
    query.addBindValue(expectedExpense);
    query.addBindValue(comment);

    if (!query.exec()) {
        qDebug() << "Insert forecast failed:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить прогноз");
        return;
    }

    loadForecastRecords();
    ui->editExpectedIncome->setValue(0.0);
    ui->editExpectedExpense->setValue(0.0);
    ui->editComment->clear();
}

