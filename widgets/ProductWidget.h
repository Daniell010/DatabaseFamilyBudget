#pragma once

#include <QWidget>

namespace Ui {
class ProductWidget;
}

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductWidget(QWidget *parent = nullptr);
    ~ProductWidget();

private slots:
    void onAddProductClicked();

private:
    Ui::ProductWidget *ui;
    void setupTable();
    void loadProductCategories();
    void loadProductRecords();
};

