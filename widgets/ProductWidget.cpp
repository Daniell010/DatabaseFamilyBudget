#include "ProductWidget.h"
#include "ui_ProductWidget.h"
#include "db/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

ProductWidget::ProductWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProductWidget)
{
    ui->setupUi(this);
    setupTable();
    loadProductCategories();
    loadProductRecords();

    connect(ui->btnAddProduct, &QPushButton::clicked,
            this, &ProductWidget::onAddProductClicked);
}

ProductWidget::~ProductWidget()
{
    delete ui;
}

void ProductWidget::setupTable()
{
    ui->tableProducts->setColumnCount(4);
    ui->tableProducts->setHorizontalHeaderLabels({ "Название", "Категория", "Калории", "Обязательный" });
    ui->tableProducts->horizontalHeader()->setStretchLastSection(true);
}

void ProductWidget::loadProductCategories()
{
    QStringList categories = DatabaseManager::instance().loadProductCategories();
    ui->comboCategory->clear();
    ui->comboCategory->addItems(categories);
}

void ProductWidget::loadProductRecords()
{
    ui->tableProducts->setRowCount(0);
    QSqlQuery query("SELECT name, category, calories_per_100g, is_required FROM products");

    int row = 0;
    while (query.next()) {
        ui->tableProducts->insertRow(row);
        ui->tableProducts->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableProducts->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableProducts->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toDouble(), 'f', 1)));
        ui->tableProducts->setItem(row, 3, new QTableWidgetItem(query.value(3).toInt() ? "Да" : "Нет"));
        row++;
    }
}

void ProductWidget::onAddProductClicked()
{
    QString name = ui->editName->text().trimmed();
    QString category = ui->comboCategory->currentText();
    double calories = ui->editCalories->value();
    bool isRequired = ui->checkIsRequired->isChecked();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название продукта");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO products (name, category, calories_per_100g, is_required) VALUES (?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(category);
    query.addBindValue(calories);
    query.addBindValue(isRequired ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "Insert product failed:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить продукт");
        return;
    }

    ui->editName->clear();
    ui->editCalories->setValue(0.0);
    ui->checkIsRequired->setChecked(false);
    loadProductRecords();
}

