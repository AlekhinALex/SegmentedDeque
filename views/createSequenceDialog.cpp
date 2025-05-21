#include "createSequenceDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

CreateSequenceDialog::CreateSequenceDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Create Segmented Deque");

    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setPlaceholderText("Enter deque name...");

    typeComboBox = new QComboBox(this);
    typeComboBox->addItems({"int", "double", "string", "person", "complex"});

    segmentSizeSpinBox = new QSpinBox(this);
    segmentSizeSpinBox->setRange(1, 1000);
    segmentSizeSpinBox->setValue(32);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *formLayout = new QFormLayout;
    formLayout->addRow("Name:", nameLineEdit);
    formLayout->addRow("Type:", typeComboBox);
    formLayout->addRow("Segment Size:", segmentSizeSpinBox);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

QString CreateSequenceDialog::getSequenceName() const
{
    return nameLineEdit->text().trimmed();
}

QString CreateSequenceDialog::getSequenceType() const
{
    return typeComboBox->currentText();
}

int CreateSequenceDialog::getSegmentSize() const
{
    return segmentSizeSpinBox->value();
}
