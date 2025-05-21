#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QInputDialog>
#include "createSequenceDialog.h"
#include "operationdialog.h"
#include <QRegularExpressionValidator>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new MainController(this);

    connect(controller, &MainController::sequenceCreated, this, &MainWindow::onSequenceCreated);
    connect(controller, &MainController::sequenceRemoved, this, &MainWindow::onSequenceRemoved);
    connect(controller, &MainController::sequenceChanged, this, &MainWindow::onSequenceChanged);
    connect(controller, &MainController::errorOccurred, this, &MainWindow::onErrorOccurred);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);

    connect(ui->appendButton, &QPushButton::clicked, this, &MainWindow::onAppendButtonClicked);
    connect(ui->prependButton, &QPushButton::clicked, this, &MainWindow::onPrependButtonClicked);
    connect(ui->insertAtButton, &QPushButton::clicked, this, &MainWindow::onInsertAtButtonClicked);
    connect(ui->setButton, &QPushButton::clicked, this, &MainWindow::onSetButtonClicked);
    connect(ui->getButton, &QPushButton::clicked, this, &MainWindow::onGetButtonClicked);

    connect(ui->appendImmutableButton, &QPushButton::clicked, this, &MainWindow::appendImmutable);
    connect(ui->prependImmutableButton, &QPushButton::clicked, this, &MainWindow::prependImmutable);
    connect(ui->insertAtImmutableButton, &QPushButton::clicked, this, &MainWindow::insertAtImmutable);
    connect(ui->setImmutableButton, &QPushButton::clicked, this, &MainWindow::setImmutable);
    connect(ui->concatImmutableButton, &QPushButton::clicked, this, &MainWindow::concatImmutable);
    connect(ui->concatButton, &QPushButton::clicked, this, &MainWindow::concat);
    connect(ui->sortImmutableButton, &QPushButton::clicked, this, &MainWindow::sortImmutable);

    connect(ui->showSequenceButton, &QPushButton::clicked, this, &MainWindow::onShowSequenceButtonClicked);
    connect(ui->sequenceListWidget, &QListWidget::itemClicked, this, &MainWindow::onSequenceSelected);

    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortButtonClicked);
    connect(ui->whereButton, &QPushButton::clicked, this, &MainWindow::onWhereButtonClicked);
    connect(ui->reduceButton, &QPushButton::clicked, this, &MainWindow::onReduceButtonClicked);
    connect(ui->applyButton, &QPushButton::clicked, this, &MainWindow::onApplyButtonClicked);
    connect(ui->searchSubsequenceButton, &QPushButton::clicked, this, &MainWindow::onSearchSubsequenceButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSequenceCreated(const QString &name)
{
    try
    {
        ui->sequenceListWidget->addItem(name);
        logAction(QString("Deque created: %1").arg(name));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("onSequenceCreated error: %1").arg(e.what()));
    }
}

void MainWindow::onSequenceRemoved(const QString &name)
{
    try
    {
        for (int i = 0; i < ui->sequenceListWidget->count(); ++i)
        {
            if (ui->sequenceListWidget->item(i)->text() == name)
            {
                delete ui->sequenceListWidget->takeItem(i);
                logAction(QString("Deque removed: %1").arg(name));
                return;
            }
        }
        throw std::runtime_error("Deque not found in UI list");
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("onSequenceRemoved error: %1").arg(e.what()));
    }
}

void MainWindow::onSequenceChanged(const QString &name)
{
    try
    {
        logAction(QString("Deque changed: %1").arg(name));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("onSequenceChanged error: %1").arg(e.what()));
    }
}

void MainWindow::onErrorOccurred(const QString &message)
{
    QMessageBox::critical(this, "Error", message);
    logAction("Error: " + message);
}

QString MainWindow::askSequenceType()
{
    // Этот метод больше не нужен, так как тип выбирается в CreateSequenceDialog
    return QString();
}

void MainWindow::onAddButtonClicked()
{
    try
    {
        CreateSequenceDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted)
        {
            QString name = dialog.getSequenceName();
            QString typeStr = dialog.getSequenceType();
            int segmentSize = dialog.getSegmentSize();

            if (name.isEmpty())
            {
                throw std::runtime_error("Deque name cannot be empty");
            }

            MainController::ValueType type = stringToValueType(typeStr);
            controller->createSequence(name.toStdString(), type, segmentSize);
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Create error: %1").arg(e.what()));
    }
}

void MainWindow::onDeleteButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected to delete.");
        controller->removeSequence(item->text().toStdString());
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Delete error: %1").arg(e.what()));
    }
}

void MainWindow::onAppendButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            return onErrorOccurred("No deque selected");

        OperationDialog dialog(this, false, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            QString value = dialog.getValue();
            if (value.isEmpty())
                return; // Ошибка уже показана в диалоге
            controller->appendToSequence(item->text().toStdString(), value);
            logAction(QString("Appended value to %1").arg(item->text()));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Append error: %1").arg(e.what()));
    }
}

void MainWindow::onPrependButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            return onErrorOccurred("No deque selected");

        OperationDialog dialog(this, false, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            QString value = dialog.getValue();
            if (value.isEmpty())
                return; // Ошибка уже показана в диалоге
            controller->prependToSequence(item->text().toStdString(), value);
            logAction(QString("Prepended value to %1").arg(item->text()));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Prepend error: %1").arg(e.what()));
    }
}

void MainWindow::onInsertAtButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            return onErrorOccurred("No deque selected");

        OperationDialog dialog(this, true, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            int index = dialog.getIndex();
            QString value = dialog.getValue();
            if (index < 0 || value.isEmpty())
                return; // Ошибки уже показаны в диалоге

            int length = controller->getSequenceLength(item->text().toStdString());
            if (index > length)
            {
                onErrorOccurred(QString("Index %1 is out of bounds (length: %2)").arg(index).arg(length));
                return;
            }

            controller->insertAtSequence(item->text().toStdString(), index, value);
            logAction(QString("Inserted value at index %1 in %2").arg(index).arg(item->text()));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Insert error: %1").arg(e.what()));
    }
}

void MainWindow::onSetButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            return onErrorOccurred("No deque selected");

        OperationDialog dialog(this, true, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            int index = dialog.getIndex();
            QString value = dialog.getValue();
            if (index < 0 || value.isEmpty())
                return; // Ошибки уже показаны в диалоге

            int length = controller->getSequenceLength(item->text().toStdString());
            if (index >= length)
            {
                onErrorOccurred(QString("Index %1 is out of bounds (length: %2)").arg(index).arg(length));
                return;
            }

            controller->setInSequence(item->text().toStdString(), index, value);
            logAction(QString("Set value at index %1 in %2").arg(index).arg(item->text()));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Set error: %1").arg(e.what()));
    }
}

void MainWindow::onGetButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            return onErrorOccurred("No deque selected");

        OperationDialog dialog(this, true, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            int index = dialog.getIndex();
            if (index < 0)
                return; // Ошибка уже показана в диалоге

            int length = controller->getSequenceLength(item->text().toStdString());
            if (index >= length)
            {
                onErrorOccurred(QString("Index %1 is out of bounds (length: %2)").arg(index).arg(length));
                return;
            }

            QVariant value = controller->getFromSequence(item->text().toStdString(), index);
            showSequenceValueDialog(item->text(), index, value.toString());
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Get error: %1").arg(e.what()));
    }
}

void MainWindow::onShowSequenceButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            return onErrorOccurred("No deque selected");
        QString contents = controller->getSequenceAsString(item->text().toStdString());
        showSequenceDialog(QString("Deque %1:\n%2").arg(item->text()).arg(contents));
        logAction(QString("Displayed deque: %1").arg(item->text()));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Show deque error: %1").arg(e.what()));
    }
}

void MainWindow::showSequenceValueDialog(const QString &name, int index, const QString &value)
{
    QMessageBox::information(this, "Deque Value",
                             QString("Deque %1\nIndex: %2\nValue: %3").arg(name).arg(index).arg(value));
    logAction(QString("Got value at index %1 from %2: %3").arg(index).arg(name).arg(value));
}

void MainWindow::logAction(const QString &action)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->logTextEdit->append(QString("[%1] %2").arg(timestamp).arg(action));
}

void MainWindow::onSequenceSelected(QListWidgetItem *item)
{
    if (item)
    {
        selectedSequenceName = item->text().toStdString();
        logAction(QString("Selected deque: %1").arg(item->text()));
    }
}

void MainWindow::showSequenceDialog(const QString &content)
{
    QMessageBox::information(this, "Deque Contents", content);
}

QVariant MainWindow::getValidatedValue() const
{
    // Этот метод больше не используется, так как ввод обрабатывается в OperationDialog
    return QVariant();
}

QString MainWindow::generateCopyName(const QString &baseName)
{
    int copyNumber = 1;
    QString newName;
    do
    {
        newName = QString("%1_copy_%2").arg(baseName).arg(copyNumber);
        copyNumber++;
    } while (controller->sequenceExists(newName.toStdString()));
    return newName;
}

void MainWindow::appendImmutable()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        OperationDialog dialog(this, false, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            QString originalName = item->text();
            QString newName = generateCopyName(originalName);
            QString value = dialog.getValue();
            if (value.isEmpty())
                return; // Ошибка уже показана в диалоге
            controller->appendToImmutableSequence(originalName.toStdString(), value, newName.toStdString());
            logAction(QString("Immutable Append: created %1 from %2").arg(newName, originalName));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Immutable Append error: %1").arg(e.what()));
    }
}

void MainWindow::prependImmutable()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        OperationDialog dialog(this, false, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            QString originalName = item->text();
            QString newName = generateCopyName(originalName);
            QString value = dialog.getValue();
            if (value.isEmpty())
                return; // Ошибка уже показана в диалоге
            controller->prependToImmutableSequence(originalName.toStdString(), value, newName.toStdString());
            logAction(QString("Immutable Prepend: created %1 from %2").arg(newName, originalName));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Immutable Prepend error: %1").arg(e.what()));
    }
}

void MainWindow::insertAtImmutable()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        OperationDialog dialog(this, true, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            int index = dialog.getIndex();
            QString value = dialog.getValue();
            if (index < 0 || value.isEmpty())
                return; // Ошибки уже показаны в диалоге

            QString originalName = item->text();
            QString newName = generateCopyName(originalName);
            controller->insertAtImmutableSequence(originalName.toStdString(), index, value, newName.toStdString());
            logAction(QString("Immutable Insert: created %1 from %2").arg(newName, originalName));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Immutable Insert error: %1").arg(e.what()));
    }
}

void MainWindow::setImmutable()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        OperationDialog dialog(this, true, item->text().split("_").last());
        if (dialog.exec() == QDialog::Accepted)
        {
            int index = dialog.getIndex();
            QString value = dialog.getValue();
            if (index < 0 || value.isEmpty())
                return; // Ошибки уже показаны в диалоге

            QString originalName = item->text();
            QString newName = generateCopyName(originalName);
            controller->setInImmutableSequence(originalName.toStdString(), index, value, newName.toStdString());
            logAction(QString("Immutable Set: created %1 from %2").arg(newName, originalName));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Immutable Set error: %1").arg(e.what()));
    }
}

void MainWindow::concatImmutable()
{
    try
    {
        QListWidgetItem *item1 = ui->sequenceListWidget->currentItem();
        if (!item1)
            throw std::runtime_error("Select first deque");

        QString secondName = QInputDialog::getText(this, "Concat Immutable", "Enter name of deque to concatenate:");
        if (secondName.isEmpty())
            return;

        QString newName = generateCopyName(item1->text());
        controller->concatImmutable(item1->text().toStdString(), secondName.toStdString(), newName.toStdString());
        logAction(QString("Immutable Concat: created %1 from %2 + %3").arg(newName, item1->text(), secondName));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Immutable Concat error: %1").arg(e.what()));
    }
}

void MainWindow::concat()
{
    try
    {
        QListWidgetItem *item1 = ui->sequenceListWidget->currentItem();
        if (!item1)
            throw std::runtime_error("Select first deque");

        QString secondName = QInputDialog::getText(this, "Concat", "Enter name of deque to concatenate:");
        if (secondName.isEmpty())
            return;

        controller->concat(item1->text().toStdString(), secondName.toStdString());
        logAction(QString("Concat: %1 += %2").arg(item1->text(), secondName));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Concat error: %1").arg(e.what()));
    }
}

void MainWindow::sortImmutable()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        QString newName = generateCopyName(item->text());
        controller->sortImmutableSequence(item->text().toStdString(), newName.toStdString()); //TODO Ascending or not has to be added
        logAction(QString("Immutable Sort: created %1 from %2").arg(newName, item->text()));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Immutable Sort error: %1").arg(e.what()));
    }
}

MainController::ValueType MainWindow::stringToValueType(const QString &typeStr)
{
    if (typeStr == "int") return MainController::ValueType::Int;
    if (typeStr == "double") return MainController::ValueType::Double;
    if (typeStr == "string") return MainController::ValueType::String;
    if (typeStr == "person") return MainController::ValueType::Person;
    if (typeStr == "complex") return MainController::ValueType::Complex;
    throw std::invalid_argument("Unknown type string");
}

int MainWindow::getSequenceLength(const std::string &name) const
{
    return controller->getSequenceLength(name);
}

void MainWindow::onSortButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        bool ascending = QInputDialog::getItem(this, "Sort Direction", "Choose sort direction:",
                                               QStringList{"Ascending", "Descending"}, 0, false).toLower() == "ascending";
        controller->sortSequence(item->text().toStdString(), ascending);
        logAction(QString("Sorted deque %1 (%2)").arg(item->text()).arg(ascending ? "ascending" : "descending"));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Sort error: %1").arg(e.what()));
    }
}

void MainWindow::onWhereButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        QStringList conditions;
        QString type = item->text().split("_").last();
        if (type == "int") conditions = {"positive", "even"};
        else if (type == "double") conditions = {"positive", "greater_one"};
        else if (type == "string") conditions = {"non_empty"};
        else if (type == "person") conditions = {"adult"};
        else if (type == "complex") conditions = {"non_zero"};
        else throw std::runtime_error("Unknown deque type");

        bool ok;
        QString condition = QInputDialog::getItem(this, "Where Condition", "Choose condition:", conditions, 0, false, &ok);
        if (!ok || condition.isEmpty())
            return;

        QString newName = generateCopyName(item->text());
        controller->where(item->text().toStdString(), newName.toStdString(), condition);
        logAction(QString("Filtered deque %1 to %2 with condition %3").arg(item->text(), newName, condition));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Where error: %1").arg(e.what()));
    }
}

void MainWindow::onReduceButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        OperationDialog dialog(this, false, item->text().split("_").last());
        dialog.setWindowTitle("Enter Initial Value for Reduce");
        if (dialog.exec() == QDialog::Accepted)
        {
            QString initialValue = dialog.getValue();
            if (initialValue.isEmpty())
                return; // Ошибка уже показана в диалоге

            QVariant result = controller->reduceSequence(item->text().toStdString(), initialValue);
            showSequenceDialog(QString("Reduce Result for %1:\n%2").arg(item->text(), result.toString()));
            logAction(QString("Reduced deque %1, result: %2").arg(item->text(), result.toString()));
        }
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Reduce error: %1").arg(e.what()));
    }
}

void MainWindow::onApplyButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        QStringList operations = item->text().split("_").last() == "int" || item->text().split("_").last() == "double" ?
                                QStringList{"square", "increment"} : QStringList{};
        if (operations.isEmpty())
            throw std::runtime_error("Apply operation not supported for this type");

        bool ok;
        QString operation = QInputDialog::getItem(this, "Apply Operation", "Choose operation:", operations, 0, false, &ok);
        if (!ok || operation.isEmpty())
            return;

        QString newName = generateCopyName(item->text());
        controller->applyUnaryOp(item->text().toStdString(), newName.toStdString(), operation);
        logAction(QString("Applied %1 to %2, result in %3").arg(operation, item->text(), newName));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Apply error: %1").arg(e.what()));
    }
}

void MainWindow::onSearchSubsequenceButtonClicked()
{
    try
    {
        QListWidgetItem *item = ui->sequenceListWidget->currentItem();
        if (!item)
            throw std::runtime_error("No deque selected");

        QString type = item->text().split("_").last();
        bool ok;
        QStringList stringValues;
        while (true)
        {
            OperationDialog dialog(this, false, type);
            dialog.setWindowTitle("Enter Subsequence Value");
            if (dialog.exec() != QDialog::Accepted)
                break;
            QString value = dialog.getValue();
            if (value.isEmpty())
                break;
            stringValues.append(value);
        }
        if (stringValues.isEmpty())
            return;

        // Convert QStringList to QVariantList
        QVariantList values;
        for (const QString& value : stringValues)
        {
            values.append(QVariant(value));
        }

        bool found = controller->searchSubsequence(item->text().toStdString(), values);
        showSequenceDialog(QString("Search Subsequence in %1:\n%2").arg(item->text(), found ? "Found" : "Not found"));
        logAction(QString("Searched subsequence in %1: %2").arg(item->text(), found ? "found" : "not found"));
    }
    catch (const std::exception &e)
    {
        onErrorOccurred(QString("Search subsequence error: %1").arg(e.what()));
    }
}
