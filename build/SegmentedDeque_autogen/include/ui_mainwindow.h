/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *contentLayout;
    QVBoxLayout *sequenceLayout;
    QLabel *sequencesLabel;
    QListWidget *sequenceListWidget;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *showSequenceButton;
    QTabWidget *operationsTabs;
    QWidget *basicTab;
    QVBoxLayout *basicLayout;
    QPushButton *appendButton;
    QPushButton *prependButton;
    QPushButton *insertAtButton;
    QPushButton *setButton;
    QPushButton *getButton;
    QWidget *immutableTab;
    QVBoxLayout *immutableLayout;
    QPushButton *appendImmutableButton;
    QPushButton *prependImmutableButton;
    QPushButton *insertAtImmutableButton;
    QPushButton *setImmutableButton;
    QPushButton *concatImmutableButton;
    QPushButton *sortImmutableButton;
    QWidget *analysisTab;
    QVBoxLayout *analysisLayout;
    QPushButton *whereButton;
    QPushButton *applyButton;
    QPushButton *reduceButton;
    QPushButton *searchSubsequenceButton;
    QWidget *sortingTab;
    QVBoxLayout *sortingLayout;
    QPushButton *concatButton;
    QPushButton *sortButton;
    QTextEdit *logTextEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 750);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        contentLayout = new QHBoxLayout();
        contentLayout->setObjectName("contentLayout");
        sequenceLayout = new QVBoxLayout();
        sequenceLayout->setObjectName("sequenceLayout");
        sequencesLabel = new QLabel(centralwidget);
        sequencesLabel->setObjectName("sequencesLabel");

        sequenceLayout->addWidget(sequencesLabel);

        sequenceListWidget = new QListWidget(centralwidget);
        sequenceListWidget->setObjectName("sequenceListWidget");

        sequenceLayout->addWidget(sequenceListWidget);

        addButton = new QPushButton(centralwidget);
        addButton->setObjectName("addButton");

        sequenceLayout->addWidget(addButton);

        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName("deleteButton");

        sequenceLayout->addWidget(deleteButton);

        showSequenceButton = new QPushButton(centralwidget);
        showSequenceButton->setObjectName("showSequenceButton");

        sequenceLayout->addWidget(showSequenceButton);


        contentLayout->addLayout(sequenceLayout);

        operationsTabs = new QTabWidget(centralwidget);
        operationsTabs->setObjectName("operationsTabs");
        basicTab = new QWidget();
        basicTab->setObjectName("basicTab");
        basicLayout = new QVBoxLayout(basicTab);
        basicLayout->setObjectName("basicLayout");
        appendButton = new QPushButton(basicTab);
        appendButton->setObjectName("appendButton");

        basicLayout->addWidget(appendButton);

        prependButton = new QPushButton(basicTab);
        prependButton->setObjectName("prependButton");

        basicLayout->addWidget(prependButton);

        insertAtButton = new QPushButton(basicTab);
        insertAtButton->setObjectName("insertAtButton");

        basicLayout->addWidget(insertAtButton);

        setButton = new QPushButton(basicTab);
        setButton->setObjectName("setButton");

        basicLayout->addWidget(setButton);

        getButton = new QPushButton(basicTab);
        getButton->setObjectName("getButton");

        basicLayout->addWidget(getButton);

        operationsTabs->addTab(basicTab, QString());
        immutableTab = new QWidget();
        immutableTab->setObjectName("immutableTab");
        immutableLayout = new QVBoxLayout(immutableTab);
        immutableLayout->setObjectName("immutableLayout");
        appendImmutableButton = new QPushButton(immutableTab);
        appendImmutableButton->setObjectName("appendImmutableButton");

        immutableLayout->addWidget(appendImmutableButton);

        prependImmutableButton = new QPushButton(immutableTab);
        prependImmutableButton->setObjectName("prependImmutableButton");

        immutableLayout->addWidget(prependImmutableButton);

        insertAtImmutableButton = new QPushButton(immutableTab);
        insertAtImmutableButton->setObjectName("insertAtImmutableButton");

        immutableLayout->addWidget(insertAtImmutableButton);

        setImmutableButton = new QPushButton(immutableTab);
        setImmutableButton->setObjectName("setImmutableButton");

        immutableLayout->addWidget(setImmutableButton);

        concatImmutableButton = new QPushButton(immutableTab);
        concatImmutableButton->setObjectName("concatImmutableButton");

        immutableLayout->addWidget(concatImmutableButton);

        sortImmutableButton = new QPushButton(immutableTab);
        sortImmutableButton->setObjectName("sortImmutableButton");

        immutableLayout->addWidget(sortImmutableButton);

        operationsTabs->addTab(immutableTab, QString());
        analysisTab = new QWidget();
        analysisTab->setObjectName("analysisTab");
        analysisLayout = new QVBoxLayout(analysisTab);
        analysisLayout->setObjectName("analysisLayout");
        whereButton = new QPushButton(analysisTab);
        whereButton->setObjectName("whereButton");

        analysisLayout->addWidget(whereButton);

        applyButton = new QPushButton(analysisTab);
        applyButton->setObjectName("applyButton");

        analysisLayout->addWidget(applyButton);

        reduceButton = new QPushButton(analysisTab);
        reduceButton->setObjectName("reduceButton");

        analysisLayout->addWidget(reduceButton);

        searchSubsequenceButton = new QPushButton(analysisTab);
        searchSubsequenceButton->setObjectName("searchSubsequenceButton");

        analysisLayout->addWidget(searchSubsequenceButton);

        operationsTabs->addTab(analysisTab, QString());
        sortingTab = new QWidget();
        sortingTab->setObjectName("sortingTab");
        sortingLayout = new QVBoxLayout(sortingTab);
        sortingLayout->setObjectName("sortingLayout");
        concatButton = new QPushButton(sortingTab);
        concatButton->setObjectName("concatButton");

        sortingLayout->addWidget(concatButton);

        sortButton = new QPushButton(sortingTab);
        sortButton->setObjectName("sortButton");

        sortingLayout->addWidget(sortButton);

        operationsTabs->addTab(sortingTab, QString());

        contentLayout->addWidget(operationsTabs);


        mainLayout->addLayout(contentLayout);

        logTextEdit = new QTextEdit(centralwidget);
        logTextEdit->setObjectName("logTextEdit");
        logTextEdit->setReadOnly(true);

        mainLayout->addWidget(logTextEdit);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        operationsTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Sequence Manager", nullptr));
        centralwidget->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"  QWidget {\n"
"    background-color: #121212;\n"
"    color: #e0e0e0;\n"
"    font-family: \"Segoe UI\", Tahoma, Geneva, Verdana, sans-serif;\n"
"    font-size: 11pt;\n"
"  }\n"
"  QPushButton {\n"
"    background-color: #2b2b72;\n"
"    border: 1px solid #5a5a99;\n"
"    border-radius: 6px;\n"
"    padding: 8px 14px;\n"
"    color: #dbe1ff;\n"
"    font-weight: 600;\n"
"  }\n"
"  QPushButton:hover {\n"
"    background-color: #4a4aa5;\n"
"    border-color: #7a7adb;\n"
"  }\n"
"  QPushButton:pressed {\n"
"    background-color: #1e1e60;\n"
"    border-color: #555599;\n"
"  }\n"
"  QListWidget {\n"
"    background-color: #1e1e2f;\n"
"    border: 1px solid #444466;\n"
"    selection-background-color: #5566ff;\n"
"    selection-color: #fff;\n"
"    font-weight: 500;\n"
"  }\n"
"  QTabWidget::pane {\n"
"    border: 1px solid #444466;\n"
"    background: #1e1e2f;\n"
"    border-radius: 6px;\n"
"  }\n"
"  QTabBar::tab {\n"
"    background: #2a2a6a;\n"
"    color: #cfcfff;\n"
"    padding: 10px 18px;\n"
"    bord"
                        "er: 1px solid #555588;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 6px;\n"
"    border-top-right-radius: 6px;\n"
"    margin-right: 3px;\n"
"    font-weight: 600;\n"
"  }\n"
"  QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    background: #5566ff;\n"
"    color: #fff;\n"
"    border-color: #7a7adb;\n"
"  }\n"
"  QTextEdit {\n"
"    background-color: #1e1e2f;\n"
"    border: 1px solid #444466;\n"
"    color: #e0e0e0;\n"
"    font-family: Consolas, monospace;\n"
"    font-size: 10pt;\n"
"    border-radius: 6px;\n"
"  }\n"
"  QLabel#sequencesLabel {\n"
"    font-size: 14pt;\n"
"    font-weight: 700;\n"
"    color: #8899ff;\n"
"    margin-bottom: 6px;\n"
"  }\n"
" ", nullptr));
        sequencesLabel->setText(QCoreApplication::translate("MainWindow", "Sequences", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "+ Segmented Deque", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        showSequenceButton->setText(QCoreApplication::translate("MainWindow", "Show Sequence", nullptr));
        appendButton->setText(QCoreApplication::translate("MainWindow", "Append", nullptr));
        prependButton->setText(QCoreApplication::translate("MainWindow", "Prepend", nullptr));
        insertAtButton->setText(QCoreApplication::translate("MainWindow", "InsertAt", nullptr));
        setButton->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        getButton->setText(QCoreApplication::translate("MainWindow", "Get", nullptr));
        operationsTabs->setTabText(operationsTabs->indexOf(basicTab), QCoreApplication::translate("MainWindow", "\320\236\321\201\320\275\320\276\320\262\320\275\321\213\320\265", nullptr));
        appendImmutableButton->setText(QCoreApplication::translate("MainWindow", "Append Immutable", nullptr));
        prependImmutableButton->setText(QCoreApplication::translate("MainWindow", "Prepend Immutable", nullptr));
        insertAtImmutableButton->setText(QCoreApplication::translate("MainWindow", "InsertAt Immutable", nullptr));
        setImmutableButton->setText(QCoreApplication::translate("MainWindow", "Set Immutable", nullptr));
        concatImmutableButton->setText(QCoreApplication::translate("MainWindow", "Concat Immutable", nullptr));
        sortImmutableButton->setText(QCoreApplication::translate("MainWindow", "Sort Immutable", nullptr));
        operationsTabs->setTabText(operationsTabs->indexOf(immutableTab), QCoreApplication::translate("MainWindow", "\320\230\320\274\320\274\321\203\321\202\320\260\320\261\320\265\320\273\321\214\320\275\321\213\320\265", nullptr));
        whereButton->setText(QCoreApplication::translate("MainWindow", "Where", nullptr));
        applyButton->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        reduceButton->setText(QCoreApplication::translate("MainWindow", "Reduce", nullptr));
        searchSubsequenceButton->setText(QCoreApplication::translate("MainWindow", "Search Subsequence", nullptr));
        operationsTabs->setTabText(operationsTabs->indexOf(analysisTab), QCoreApplication::translate("MainWindow", "\320\220\320\275\320\260\320\273\320\270\320\267", nullptr));
        concatButton->setText(QCoreApplication::translate("MainWindow", "Concat", nullptr));
        sortButton->setText(QCoreApplication::translate("MainWindow", "Sort", nullptr));
        operationsTabs->setTabText(operationsTabs->indexOf(sortingTab), QCoreApplication::translate("MainWindow", "\320\241\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\272\320\260/Concat", nullptr));
        logTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Operation log...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
