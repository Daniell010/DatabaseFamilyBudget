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
    connect(ui->tableProducts, &QTableWidget::cellChanged, this, &ProductWidget::onProductCellChanged);

}

ProductWidget::~ProductWidget()
{
    delete ui;
}

void ProductWidget::setupTable()
{
    ui->tableProducts->setColumnCount(5);
    ui->tableProducts->setHorizontalHeaderLabels({ "Название", "Категория", "Калории", "Обязательный" });
    ui->tableProducts->setColumnHidden(4, true);
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
    QSqlQuery query("SELECT id, name, category, calories_per_100g, is_required FROM products");
    int row = 0;
    while (query.next()) {
        ui->tableProducts->insertRow(row);
        ui->tableProducts->setItem(row, 0, new QTableWidgetItem(query.value("name").toString()));
        ui->tableProducts->setItem(row, 1, new QTableWidgetItem(query.value("category").toString()));
        ui->tableProducts->setItem(row, 2, new QTableWidgetItem(QString::number(query.value("calories_per_100g").toDouble(), 'f', 1)));
        ui->tableProducts->setItem(row, 3, new QTableWidgetItem(query.value("is_required").toInt() ? "Да" : "Нет"));
        ui->tableProducts->setItem(row, 4, new QTableWidgetItem(query.value("id").toString()));
        row++;
    }
}

void ProductWidget::onAddProductClicked()
{
    ui->tableProducts->blockSignals(true);
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
    ui->tableProducts->blockSignals(false);
}

void ProductWidget::onProductCellChanged(int row, int column)
{
    if (row < 0 || column < 0) return;
    ui->tableProducts->blockSignals(true);

    QString name = ui->tableProducts->item(row, 0)->text();
    QString category = ui->tableProducts->item(row, 1)->text();
    double calories = ui->tableProducts->item(row, 2)->text().toDouble();
    QString isRequiredText = ui->tableProducts->item(row, 3)->text().trimmed().toLower();
    bool isRequired = (isRequiredText == "да" || isRequiredText == "yes" || isRequiredText == "1");
    int id = ui->tableProducts->item(row, 4)->text().toInt();

    QSqlQuery query;
    query.prepare("UPDATE products SET name = ?, category = ?, calories_per_100g = ?, is_required = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(category);
    query.addBindValue(calories);
    query.addBindValue(isRequired ? 1 : 0);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "UPDATE product failed:" << query.lastError().text();
    }

    ui->tableProducts->blockSignals(false);
}

