QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    db/DatabaseManager.cpp \
    main.cpp \
    mainwindow.cpp \
    widgets/ExpenseWidget.cpp \
    widgets/ForecastWidget.cpp \
    widgets/IncomeWidget.cpp \
    widgets/ProductWidget.cpp

HEADERS += \
    db/DatabaseManager.h \
    mainwindow.h \
    widgets/ExpenseWidget.h \
    widgets/ForecastWidget.h \
    widgets/IncomeWidget.h \
    widgets/ProductWidget.h

FORMS += \
    mainwindow.ui \
    widgets/ExpenseWidget.ui \
    widgets/ForecastWidget.ui \
    widgets/IncomeWidget.ui \
    widgets/ProductWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
