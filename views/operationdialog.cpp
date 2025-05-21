#include "operationdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpressionValidator>

OperationDialog::OperationDialog(QWidget *parent, bool needIndex, const QString &type)
    : QDialog(parent), hasIndex(needIndex), dequeType(type)
{
    setWindowTitle("Enter Value");

    valueLabel = new QLabel("Value:", this);
    valueEdit = new QLineEdit(this);
    valueEdit->setPlaceholderText(dequeType == "person" ? "e.g., John (25)" :
                                 dequeType == "complex" ? "e.g., 3+4i" :
                                 "Enter value...");

    if (hasIndex)
    {
        indexLabel = new QLabel("Index:", this);
        indexEdit = new QLineEdit(this);
        indexEdit->setPlaceholderText("Enter index...");
    }
    else
    {
        indexEdit = nullptr;
        indexLabel = nullptr;
    }

    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    setupValidators();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(valueLabel);
    mainLayout->addWidget(valueEdit);

    if (hasIndex)
    {
        mainLayout->addWidget(indexLabel);
        mainLayout->addWidget(indexEdit);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void OperationDialog::setupValidators()
{
    if (dequeType == "int")
    {
        valueEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?\\d+"), this));
    }
    else if (dequeType == "double")
    {
        valueEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?\\d*\\.?\\d+([eE][+-]?\\d+)?"), this));
    }
    else if (dequeType == "string")
    {
        // Нет валидатора, так как строка может быть произвольной
    }
    else if (dequeType == "person")
    {
        valueEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(".+\\s*\\(\\d+\\)"), this));
    }
    else if (dequeType == "complex")
    {
        valueEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[+-]?\\d*\\.?\\d*\\s*[+-]\\s*\\d*\\.?\\d*i"), this));
    }

    if (hasIndex)
    {
        indexEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+"), this));
    }
}

QString OperationDialog::getValue() const
{
    QString value = valueEdit->text().trimmed();
    if (value.isEmpty())
    {
        QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter a value.");
        return QString();
    }

    if (dequeType == "int")
    {
        bool ok;
        value.toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter a valid integer.");
            return QString();
        }
    }
    else if (dequeType == "double")
    {
        bool ok;
        value.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter a valid number.");
            return QString();
        }
    }
    else if (dequeType == "person")
    {
        QRegularExpression re(".+\\s*\\(\\d+\\)");
        if (!re.match(value).hasMatch())
        {
            QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter a valid person format: 'Name (Age)'");
            return QString();
        }
    }
    else if (dequeType == "complex")
    {
        QRegularExpression re("[+-]?\\d*\\.?\\d*\\s*[+-]\\s*\\d*\\.?\\d*i");
        if (!re.match(value).hasMatch())
        {
            QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter a valid complex number: 'Real+Imagi' or 'Real-Imagi'");
            return QString();
        }
    }

    return value;
}

int OperationDialog::getIndex() const
{
    if (!hasIndex || !indexEdit)
    {
        return -1;
    }

    QString indexText = indexEdit->text().trimmed();
    if (indexText.isEmpty())
    {
        QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter an index.");
        return -1;
    }

    bool ok;
    int index = indexText.toInt(&ok);
    if (!ok || index < 0)
    {
        QMessageBox::warning(const_cast<OperationDialog*>(this), "Invalid Input", "Please enter a valid non-negative integer.");
        return -1;
    }
    return index;
}
