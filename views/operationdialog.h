#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QString>

class OperationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OperationDialog(QWidget *parent = nullptr, bool needIndex = false, const QString &type = "int");
    QString getValue() const;
    int getIndex() const;

private:
    QLineEdit *valueEdit;
    QLineEdit *indexEdit;
    QLabel *valueLabel;
    QLabel *indexLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    bool hasIndex;
    QString dequeType;

    void setupValidators();
};
