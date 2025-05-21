#pragma once

#include <QMainWindow>
#include <QListWidgetItem>
#include <QVariant>
#include "../controllers/MainController.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSequenceCreated(const QString &name);
    void onSequenceRemoved(const QString &name);
    void onSequenceChanged(const QString &name);
    void onErrorOccurred(const QString &message);

    void onAddButtonClicked();
    void onDeleteButtonClicked();

    void onAppendButtonClicked();
    void onPrependButtonClicked();
    void onInsertAtButtonClicked();
    void onSetButtonClicked();
    void onGetButtonClicked();
    void onSortButtonClicked();
    void onWhereButtonClicked();
    void onReduceButtonClicked();
    void onApplyButtonClicked();
    void onSearchSubsequenceButtonClicked();

    void onShowSequenceButtonClicked();
    void onSequenceSelected(QListWidgetItem *item);

    QString generateCopyName(const QString &baseName);
    void appendImmutable();
    void prependImmutable();
    void insertAtImmutable();
    void setImmutable();
    void concatImmutable();
    void concat();
    void sortImmutable();
    int getSequenceLength(const std::string &name) const;

    MainController::ValueType stringToValueType(const QString &typeStr);

private:
    Ui::MainWindow *ui;
    MainController *controller;
    std::string selectedSequenceName;

    void showSequenceValueDialog(const QString &name, int index, const QString &value);
    void showSequenceDialog(const QString &content);
    void logAction(const QString &action);
    QVariant getValidatedValue() const;
    QString askSequenceType();
};
