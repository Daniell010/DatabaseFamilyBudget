#pragma once

#include <QWidget>

namespace Ui {
class ExpenseWidget;
}

class ExpenseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExpenseWidget(QWidget *parent = nullptr);
    ~ExpenseWidget();

public slots:
        void onUpdate();

private slots:
    void onAddExpenseClicked();

private:
    Ui::ExpenseWidget *ui;
    void setupTable();
    void loadExpenseRecords();
    void loadUsers();
    void loadCategories();
};

