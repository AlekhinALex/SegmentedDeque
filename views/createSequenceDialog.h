#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QSpinBox>

class CreateSequenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSequenceDialog(QWidget *parent = nullptr);
    QString getSequenceName() const;
    QString getSequenceType() const;
    int getSegmentSize() const;

private:
    QLineEdit *nameLineEdit;
    QComboBox *typeComboBox;
    QSpinBox *segmentSizeSpinBox;
    QDialogButtonBox *buttonBox;
};
