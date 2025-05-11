#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

#include "db/DatabaseManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_incomeWidget = new IncomeWidget(this);
    ui->stackedWidget->addWidget(m_incomeWidget);

    m_expenseWidget = new ExpenseWidget(this);
    ui->stackedWidget->addWidget(m_expenseWidget);

    m_productWidget = new ProductWidget(this);
    ui->stackedWidget->addWidget(m_productWidget);

    m_forecastWidget = new ForecastWidget(this);
    ui->stackedWidget->addWidget(m_forecastWidget);

    connect(ui->btnIncome, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(m_incomeWidget);
    });
    connect(ui->btnExpenses, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(m_expenseWidget);
    });

    connect(ui->btnProducts, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(m_productWidget);
    });

    connect(ui->btnForecast, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(m_forecastWidget);
    });

    connect(this, &MainWindow::sigUpdate, m_expenseWidget, &ExpenseWidget::onUpdate);
    connect(this, &MainWindow::sigUpdate, m_incomeWidget, &IncomeWidget::onUpdate);

    connect(ui->btnAddUser, &QPushButton::clicked, this, [=]() {
        bool ok;
        QString name = QInputDialog::getText(this, "Новый пользователь",
                                             "Введите имя:", QLineEdit::Normal,
                                             "", &ok);
        if (ok && !name.trimmed().isEmpty()) {
            if (DatabaseManager::instance().addUser(name.trimmed())) {
                emit sigUpdate();
            } else {
                QMessageBox::warning(this, "Ошибка", "Не удалось добавить пользователя (возможно, уже существует)");
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
