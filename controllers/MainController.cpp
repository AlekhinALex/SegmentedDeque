#include "MainController.hpp"
#include <stdexcept>
#include <sstream>
#include <QRegularExpression>
#include <functional>

MainController::MainController(QObject *parent) : QObject(parent) {}

void MainController::createSequence(const std::string &name, ValueType valueType, int segmentSize)
{
    if (name.empty())
    {
        emit errorOccurred("Sequence name cannot be empty");
        return;
    }

    if (sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' already exists").arg(QString::fromStdString(name)));
        return;
    }

    SequenceData data;
    data.valueType = valueType;

    try
    {
        switch (valueType)
        {
        case ValueType::Int:
            data.sequence = std::make_unique<SegmentedDeque<int>>(segmentSize);
            break;
        case ValueType::Double:
            data.sequence = std::make_unique<SegmentedDeque<double>>(segmentSize);
            break;
        case ValueType::String:
            data.sequence = std::make_unique<SegmentedDeque<std::string>>(segmentSize);
            break;
        case ValueType::Person:
            data.sequence = std::make_unique<SegmentedDeque<Person>>(segmentSize);
            break;
        case ValueType::Complex:
            data.sequence = std::make_unique<SegmentedDeque<Complex>>(segmentSize);
            break;
        }
        sequences[name] = std::move(data);
        emit sequenceCreated(QString::fromStdString(name));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Failed to create sequence: %1").arg(e.what()));
    }
}

void MainController::removeSequence(const std::string &name)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }
    sequences.erase(name);
    emit sequenceRemoved(QString::fromStdString(name));
}

bool MainController::sequenceExists(const std::string &name) const
{
    return sequences.find(name) != sequences.end();
}

MainController::ValueType MainController::getSequenceValueType(const std::string &name) const
{
    if (!sequenceExists(name))
    {
        throw std::runtime_error("Sequence not found");
    }
    return sequences.at(name).valueType;
}

template <typename T>
SegmentedDeque<T> *MainController::getSequence(const std::string &name) const
{
    auto it = sequences.find(name);
    if (it == sequences.end())
    {
        throw std::runtime_error("Sequence not found");
    }

    try
    {
        if constexpr (std::is_same_v<T, int>)
        {
            if (std::holds_alternative<std::unique_ptr<SegmentedDeque<int>>>(it->second.sequence))
            {
                return std::get<std::unique_ptr<SegmentedDeque<int>>>(it->second.sequence).get();
            }
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            if (std::holds_alternative<std::unique_ptr<SegmentedDeque<double>>>(it->second.sequence))
            {
                return std::get<std::unique_ptr<SegmentedDeque<double>>>(it->second.sequence).get();
            }
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            if (std::holds_alternative<std::unique_ptr<SegmentedDeque<std::string>>>(it->second.sequence))
            {
                return std::get<std::unique_ptr<SegmentedDeque<std::string>>>(it->second.sequence).get();
            }
        }
        else if constexpr (std::is_same_v<T, Person>)
        {
            if (std::holds_alternative<std::unique_ptr<SegmentedDeque<Person>>>(it->second.sequence))
            {
                return std::get<std::unique_ptr<SegmentedDeque<Person>>>(it->second.sequence).get();
            }
        }
        else if constexpr (std::is_same_v<T, Complex>)
        {
            if (std::holds_alternative<std::unique_ptr<SegmentedDeque<Complex>>>(it->second.sequence))
            {
                return std::get<std::unique_ptr<SegmentedDeque<Complex>>>(it->second.sequence).get();
            }
        }
        throw std::runtime_error("Type mismatch");
    }
    catch (const std::bad_variant_access &)
    {
        throw std::runtime_error("Invalid sequence type access");
    }
}

template <typename T>
T MainController::parseValue(const QVariant &value, ValueType valueType) const
{
    if constexpr (std::is_same_v<T, int>)
    {
        if (valueType != ValueType::Int)
            throw std::invalid_argument("Type mismatch: expected int");
        bool ok;
        int result = value.toInt(&ok);
        if (!ok)
            throw std::invalid_argument("Invalid integer value");
        return result;
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        if (valueType != ValueType::Double)
            throw std::invalid_argument("Type mismatch: expected double");
        bool ok;
        double result = value.toDouble(&ok);
        if (!ok)
            throw std::invalid_argument("Invalid double value");
        return result;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        if (valueType != ValueType::String)
            throw std::invalid_argument("Type mismatch: expected string");
        return value.toString().toStdString();
    }
    else if constexpr (std::is_same_v<T, Person>)
    {
        if (valueType != ValueType::Person)
            throw std::invalid_argument("Type mismatch: expected Person");
        QRegularExpression re(R"(^(.+)\s\((\d+)\)$)");
        QRegularExpressionMatch match = re.match(value.toString());
        if (!match.hasMatch())
        {
            throw std::invalid_argument("Invalid Person format. Expected: 'Name (age)'");
        }
        QString name = match.captured(1);
        bool ok;
        int age = match.captured(2).toInt(&ok);
        if (!ok)
            throw std::invalid_argument("Invalid age in Person");
        return Person(name.toStdString(), age);
    }
    else if constexpr (std::is_same_v<T, Complex>)
    {
        if (valueType != ValueType::Complex)
            throw std::invalid_argument("Type mismatch: expected Complex");
        QRegularExpression re(R"(^(-?\d*\.?\d*)\s*([+-])\s*(\d*\.?\d*)i$)");
        QRegularExpressionMatch match = re.match(value.toString());
        if (!match.hasMatch())
        {
            throw std::invalid_argument("Invalid Complex format. Expected: 'real + imag i' or 'real - imag i'");
        }
        bool ok;
        double real = match.captured(1).toDouble(&ok);
        if (!ok)
            throw std::invalid_argument("Invalid real part in Complex");
        double imag = match.captured(3).toDouble(&ok);
        if (!ok)
            throw std::invalid_argument("Invalid imaginary part in Complex");
        if (match.captured(2) == "-")
            imag = -imag;
        return Complex(real, imag);
    }
}

template <typename T>
QVariant MainController::valueToQVariant(const T &value) const
{
    if constexpr (std::is_same_v<T, int>)
    {
        return QVariant(value);
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        return QVariant(value);
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        return QVariant(QString::fromStdString(value));
    }
    else if constexpr (std::is_same_v<T, Person>)
    {
        return QVariant(QString::fromStdString(value.getName() + " (" + std::to_string(value.getAge()) + ")"));
    }
    else if constexpr (std::is_same_v<T, Complex>)
    {
        std::stringstream ss;
        ss << value;
        return QVariant(QString::fromStdString(ss.str()));
    }
}

void MainController::appendToSequence(const std::string &name, const QVariant &value)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }

    try
    {
        auto &data = sequences[name];
        switch (data.valueType)
        {
        case ValueType::Int:
            getSequence<int>(name)->append(parseValue<int>(value, data.valueType));
            break;
        case ValueType::Double:
            getSequence<double>(name)->append(parseValue<double>(value, data.valueType));
            break;
        case ValueType::String:
            getSequence<std::string>(name)->append(parseValue<std::string>(value, data.valueType));
            break;
        case ValueType::Person:
            getSequence<Person>(name)->append(parseValue<Person>(value, data.valueType));
            break;
        case ValueType::Complex:
            getSequence<Complex>(name)->append(parseValue<Complex>(value, data.valueType));
            break;
        }
        emit sequenceChanged(QString::fromStdString(name));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Append failed: %1").arg(e.what()));
    }
}

void MainController::prependToSequence(const std::string &name, const QVariant &value)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }

    try
    {
        auto &data = sequences[name];
        switch (data.valueType)
        {
        case ValueType::Int:
            getSequence<int>(name)->prepend(parseValue<int>(value, data.valueType));
            break;
        case ValueType::Double:
            getSequence<double>(name)->prepend(parseValue<double>(value, data.valueType));
            break;
        case ValueType::String:
            getSequence<std::string>(name)->prepend(parseValue<std::string>(value, data.valueType));
            break;
        case ValueType::Person:
            getSequence<Person>(name)->prepend(parseValue<Person>(value, data.valueType));
            break;
        case ValueType::Complex:
            getSequence<Complex>(name)->prepend(parseValue<Complex>(value, data.valueType));
            break;
        }
        emit sequenceChanged(QString::fromStdString(name));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Prepend failed: %1").arg(e.what()));
    }
}

void MainController::insertAtSequence(const std::string &name, int index, const QVariant &value)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }

    try
    {
        auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
            getSequence<int>(name)->insertAt(parseValue<int>(value, data.valueType), index);
            break;
        case ValueType::Double:
            getSequence<double>(name)->insertAt(parseValue<double>(value, data.valueType), index);
            break;
        case ValueType::String:
            getSequence<std::string>(name)->insertAt(parseValue<std::string>(value, data.valueType), index);
            break;
        case ValueType::Person:
            getSequence<Person>(name)->insertAt(parseValue<Person>(value, data.valueType), index);
            break;
        case ValueType::Complex:
            getSequence<Complex>(name)->insertAt(parseValue<Complex>(value, data.valueType), index);
            break;
        }
        emit sequenceChanged(QString::fromStdString(name));
    }
    catch (const std::out_of_range &)
    {
        emit errorOccurred("Invalid index for insertion");
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Insert failed: %1").arg(e.what()));
    }
}

void MainController::setInSequence(const std::string &name, int index, const QVariant &value)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }

    try
    {
        auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
            getSequence<int>(name)->set(index, parseValue<int>(value, data.valueType));
            break;
        case ValueType::Double:
            getSequence<double>(name)->set(index, parseValue<double>(value, data.valueType));
            break;
        case ValueType::String:
            getSequence<std::string>(name)->set(index, parseValue<std::string>(value, data.valueType));
            break;
        case ValueType::Person:
            getSequence<Person>(name)->set(index, parseValue<Person>(value, data.valueType));
            break;
        case ValueType::Complex:
            getSequence<Complex>(name)->set(index, parseValue<Complex>(value, data.valueType));
            break;
        }
        emit sequenceChanged(QString::fromStdString(name));
    }
    catch (const std::out_of_range &)
    {
        emit errorOccurred("Index out of range");
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Set failed: %1").arg(e.what()));
    }
}

void MainController::concat(const std::string &name1, const std::string &name2)
{
    if (!sequenceExists(name1) || !sequenceExists(name2))
    {
        emit errorOccurred("One or both sequences not found");
        return;
    }

    try
    {
        const auto &s1 = sequences.at(name1);
        const auto &s2 = sequences.at(name2);
        if (s1.valueType != s2.valueType)
        {
            emit errorOccurred("Sequences must be of the same type to concatenate");
            return;
        }

        switch (s1.valueType)
        {
        case ValueType::Int:
            getSequence<int>(name1)->concat(getSequence<int>(name2));
            break;
        case ValueType::Double:
            getSequence<double>(name1)->concat(getSequence<double>(name2));
            break;
        case ValueType::String:
            getSequence<std::string>(name1)->concat(getSequence<std::string>(name2));
            break;
        case ValueType::Person:
            getSequence<Person>(name1)->concat(getSequence<Person>(name2));
            break;
        case ValueType::Complex:
            getSequence<Complex>(name1)->concat(getSequence<Complex>(name2));
            break;
        }
        emit sequenceChanged(QString::fromStdString(name1));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Concat failed: %1").arg(e.what()));
    }
}

void MainController::appendToImmutableSequence(const std::string &originalName, const QVariant &value, const std::string &newName)
{
    if (!sequenceExists(originalName))
    {
        emit errorOccurred(QString("Original sequence '%1' not found").arg(QString::fromStdString(originalName)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &original = sequences.at(originalName);
        SequenceData newSequenceData;
        newSequenceData.valueType = original.valueType;

        switch (original.valueType)
        {
        case ValueType::Int:
        {
            auto *originalSeq = getSequence<int>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<int> *>(originalSeq->appendImmutable(parseValue<int>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto *originalSeq = getSequence<double>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<double> *>(originalSeq->appendImmutable(parseValue<double>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto *originalSeq = getSequence<std::string>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<std::string> *>(originalSeq->appendImmutable(parseValue<std::string>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto *originalSeq = getSequence<Person>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Person> *>(originalSeq->appendImmutable(parseValue<Person>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto *originalSeq = getSequence<Complex>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Complex> *>(originalSeq->appendImmutable(parseValue<Complex>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Immutable append failed: %1").arg(e.what()));
    }
}

void MainController::prependToImmutableSequence(const std::string &originalName, const QVariant &value, const std::string &newName)
{
    if (!sequenceExists(originalName))
    {
        emit errorOccurred(QString("Original sequence '%1' not found").arg(QString::fromStdString(originalName)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &original = sequences.at(originalName);
        SequenceData newSequenceData;
        newSequenceData.valueType = original.valueType;

        switch (original.valueType)
        {
        case ValueType::Int:
        {
            auto *originalSeq = getSequence<int>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<int> *>(originalSeq->prependImmutable(parseValue<int>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto *originalSeq = getSequence<double>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<double> *>(originalSeq->prependImmutable(parseValue<double>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto *originalSeq = getSequence<std::string>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<std::string> *>(originalSeq->prependImmutable(parseValue<std::string>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto *originalSeq = getSequence<Person>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Person> *>(originalSeq->prependImmutable(parseValue<Person>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto *originalSeq = getSequence<Complex>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Complex> *>(originalSeq->prependImmutable(parseValue<Complex>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Immutable prepend failed: %1").arg(e.what()));
    }
}

void MainController::insertAtImmutableSequence(const std::string &originalName, int index, const QVariant &value, const std::string &newName)
{
    if (!sequenceExists(originalName))
    {
        emit errorOccurred(QString("Original sequence '%1' not found").arg(QString::fromStdString(originalName)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &original = sequences.at(originalName);
        SequenceData newSequenceData;
        newSequenceData.valueType = original.valueType;

        switch (original.valueType)
        {
        case ValueType::Int:
        {
            auto *originalSeq = getSequence<int>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<int> *>(originalSeq->insertAtImmutable(parseValue<int>(value, original.valueType), index));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto *originalSeq = getSequence<double>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<double> *>(originalSeq->insertAtImmutable(parseValue<double>(value, original.valueType), index));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto *originalSeq = getSequence<std::string>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<std::string> *>(originalSeq->insertAtImmutable(parseValue<std::string>(value, original.valueType), index));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto *originalSeq = getSequence<Person>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Person> *>(originalSeq->insertAtImmutable(parseValue<Person>(value, original.valueType), index));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto *originalSeq = getSequence<Complex>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Complex> *>(originalSeq->insertAtImmutable(parseValue<Complex>(value, original.valueType), index));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::out_of_range &)
    {
        emit errorOccurred("Invalid index for insertion");
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Immutable insert failed: %1").arg(e.what()));
    }
}

void MainController::setInImmutableSequence(const std::string &originalName, int index, const QVariant &value, const std::string &newName)
{
    if (!sequenceExists(originalName))
    {
        emit errorOccurred(QString("Original sequence '%1' not found").arg(QString::fromStdString(originalName)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &original = sequences.at(originalName);
        SequenceData newSequenceData;
        newSequenceData.valueType = original.valueType;

        switch (original.valueType)
        {
        case ValueType::Int:
        {
            auto *originalSeq = getSequence<int>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<int> *>(originalSeq->setImmutable(index, parseValue<int>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto *originalSeq = getSequence<double>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<double> *>(originalSeq->setImmutable(index, parseValue<double>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto *originalSeq = getSequence<std::string>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<std::string> *>(originalSeq->setImmutable(index, parseValue<std::string>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto *originalSeq = getSequence<Person>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Person> *>(originalSeq->setImmutable(index, parseValue<Person>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto *originalSeq = getSequence<Complex>(originalName);
            auto *newSeq = static_cast<SegmentedDeque<Complex> *>(originalSeq->setImmutable(index, parseValue<Complex>(value, original.valueType)));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::out_of_range &)
    {
        emit errorOccurred("Index out of range");
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Immutable set failed: %1").arg(e.what()));
    }
}

void MainController::concatImmutable(const std::string &name1, const std::string &name2, const std::string &newName)
{
    if (!sequenceExists(name1) || !sequenceExists(name2))
    {
        emit errorOccurred("One or both sequences not found");
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &s1 = sequences.at(name1);
        const auto &s2 = sequences.at(name2);
        if (s1.valueType != s2.valueType)
        {
            emit errorOccurred("Sequences must be of the same type to concatenate");
            return;
        }

        SequenceData newSequenceData;
        newSequenceData.valueType = s1.valueType;

        switch (s1.valueType)
        {
        case ValueType::Int:
        {
            auto *seq1 = getSequence<int>(name1);
            auto *seq2 = getSequence<int>(name2);
            auto *newSeq = static_cast<SegmentedDeque<int> *>(seq1->concatImmutable(seq2));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto *seq1 = getSequence<double>(name1);
            auto *seq2 = getSequence<double>(name2);
            auto *newSeq = static_cast<SegmentedDeque<double> *>(seq1->concatImmutable(seq2));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto *seq1 = getSequence<std::string>(name1);
            auto *seq2 = getSequence<std::string>(name2);
            auto *newSeq = static_cast<SegmentedDeque<std::string> *>(seq1->concatImmutable(seq2));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto *seq1 = getSequence<Person>(name1);
            auto *seq2 = getSequence<Person>(name2);
            auto *newSeq = static_cast<SegmentedDeque<Person> *>(seq1->concatImmutable(seq2));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto *seq1 = getSequence<Complex>(name1);
            auto *seq2 = getSequence<Complex>(name2);
            auto *newSeq = static_cast<SegmentedDeque<Complex> *>(seq1->concatImmutable(seq2));
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Immutable concat failed: %1").arg(e.what()));
    }
}

void MainController::sortSequence(const std::string &name, bool ascending)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }

    try
    {
        auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
            if (ascending)
                getSequence<int>(name)->sort(getSequence<int>(name)->begin(), getSequence<int>(name)->end());
            else
                getSequence<int>(name)->sort(getSequence<int>(name)->begin(), getSequence<int>(name)->end(), std::greater<int>());
            break;
        case ValueType::Double:
            if (ascending)
                getSequence<double>(name)->sort(getSequence<double>(name)->begin(), getSequence<double>(name)->end());
            else
                getSequence<double>(name)->sort(getSequence<double>(name)->begin(), getSequence<double>(name)->end(), std::greater<double>());
            break;
        case ValueType::String:
            if (ascending)
                getSequence<std::string>(name)->sort(getSequence<std::string>(name)->begin(), getSequence<std::string>(name)->end());
            else
                getSequence<std::string>(name)->sort(getSequence<std::string>(name)->begin(), getSequence<std::string>(name)->end(), std::greater<std::string>());
            break;
        case ValueType::Person:
            if (ascending)
                getSequence<Person>(name)->sort(getSequence<Person>(name)->begin(), getSequence<Person>(name)->end());
            else
                getSequence<Person>(name)->sort(getSequence<Person>(name)->begin(), getSequence<Person>(name)->end(), std::greater<Person>());
            break;
        case ValueType::Complex:
            if (ascending)
                getSequence<Complex>(name)->sort(getSequence<Complex>(name)->begin(), getSequence<Complex>(name)->end());
            else
                getSequence<Complex>(name)->sort(getSequence<Complex>(name)->begin(), getSequence<Complex>(name)->end(), std::greater<Complex>());
            break;
        }
        emit sequenceChanged(QString::fromStdString(name));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Sort failed: %1").arg(e.what()));
    }
}

void MainController::sortImmutableSequence(const std::string &originalName, const std::string &newName, bool ascending)
{
    if (!sequenceExists(originalName))
    {
        emit errorOccurred(QString("Original sequence '%1' not found").arg(QString::fromStdString(originalName)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &original = sequences.at(originalName);
        SequenceData newSequenceData;
        newSequenceData.valueType = original.valueType;

        switch (original.valueType)
        {
        case ValueType::Int:
        {
            auto *originalSeq = getSequence<int>(originalName);
            auto *newSeq = ascending ? originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end()) : originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end(), std::greater<int>());
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto *originalSeq = getSequence<double>(originalName);
            auto *newSeq = ascending ? originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end()) : originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end(), std::greater<double>());
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto *originalSeq = getSequence<std::string>(originalName);
            auto *newSeq = ascending ? originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end()) : originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end(), std::greater<std::string>());
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto *originalSeq = getSequence<Person>(originalName);
            auto *newSeq = ascending ? originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end()) : originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end(), std::greater<Person>());
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto *originalSeq = getSequence<Complex>(originalName);
            auto *newSeq = ascending ? originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end()) : originalSeq->sortImmutable(originalSeq->begin(), originalSeq->end(), std::greater<Complex>());
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Immutable sort failed: %1").arg(e.what()));
    }
}

void MainController::where(const std::string &name, const std::string &newName, const QString &condition)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &data = sequences.at(name);
        SequenceData newSequenceData;
        newSequenceData.valueType = data.valueType;

        switch (data.valueType)
        {
        case ValueType::Int:
        {
            auto pred = condition == "positive"     ? [](int x)
            { return x > 0; } : condition == "even" ? [](int x)
            { return x % 2 == 0; }
                            : condition == "greater_ten" ? [](int x)
            { return x > 10; }
                                                         : throw std::invalid_argument("Invalid condition for int");
            auto *newSeq = getSequence<int>(name)->where(pred);
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto pred = condition == "positive"              ? [](double x)
            { return x > 0.0; } : condition == "greater_one" ? [](double x)
            { return x > 1.0; }
                              : condition == "less_five" ? [](double x)
            { return x < 5.0; }
                                                         : throw std::invalid_argument("Invalid condition for double");
            auto *newSeq = getSequence<double>(name)->where(pred);
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto pred = condition == "non_empty"                  ? [](const std::string &s)
            { return !s.empty(); } : condition == "starts_with_a" ? [](const std::string &s)
            { return !s.empty() && s[0] == 'A' || s[0] == 'a'; }
                                 : condition == "longer_than_five" ? [](const std::string &s)
            { return s.length() > 5; }
                                                                   : throw std::invalid_argument("Invalid condition for string");
            auto *newSeq = getSequence<std::string>(name)->where(pred);
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto pred = condition == "adult"                     ? [](const Person &p)
            { return p.getAge() >= 18; } : condition == "senior" ? [](const Person &p)
            { return p.getAge() >= 65; }
                                                                 : throw std::invalid_argument("Invalid condition for Person");
            auto *newSeq = getSequence<Person>(name)->where(pred);
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto pred = condition == "non_zero"                            ? [](const Complex &c)
            { return c.magnitude() > 0.0; } : condition == "real_positive" ? [](const Complex &c)
            { return c.getReal() > 0.0; }
                                                                           : throw std::invalid_argument("Invalid condition for Complex");
            auto *newSeq = getSequence<Complex>(name)->where(pred);
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Where failed: %1").arg(e.what()));
    }
}

QVariant MainController::reduceSequence(const std::string &name, const QVariant &initialValue)
{
    if (!sequenceExists(name))
    {
        throw std::runtime_error("Sequence not found");
    }

    try
    {
        const auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
        {
            int init = parseValue<int>(initialValue, data.valueType);
            return valueToQVariant(getSequence<int>(name)->reduce(std::plus<int>(), init));
        }
        case ValueType::Double:
        {
            double init = parseValue<double>(initialValue, data.valueType);
            return valueToQVariant(getSequence<double>(name)->reduce(std::plus<double>(), init));
        }
        case ValueType::String:
        {
            std::string init = parseValue<std::string>(initialValue, data.valueType);
            return valueToQVariant(getSequence<std::string>(name)->reduce([](const std::string &a, const std::string &b)
                                                                          { return a + b; }, init));
        }
        case ValueType::Person:
        {
            Person init = parseValue<Person>(initialValue, data.valueType);
            return valueToQVariant(getSequence<Person>(name)->reduce(
                [](const Person &a, const Person &b)
                {
                    return Person(a.getName() + "," + b.getName(), a.getAge() + b.getAge());
                },
                init));
        }
        case ValueType::Complex:
        {
            Complex init = parseValue<Complex>(initialValue, data.valueType);
            return valueToQVariant(getSequence<Complex>(name)->reduce(std::plus<Complex>(), init));
        }
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Reduce failed: ") + e.what());
    }
    return QVariant();
}

void MainController::applyUnaryOp(const std::string &name, const std::string &newName, const QString &operation)
{
    if (!sequenceExists(name))
    {
        emit errorOccurred(QString("Sequence '%1' not found").arg(QString::fromStdString(name)));
        return;
    }
    if (sequenceExists(newName))
    {
        emit errorOccurred(QString("New sequence name '%1' already exists").arg(QString::fromStdString(newName)));
        return;
    }

    try
    {
        const auto &data = sequences.at(name);
        SequenceData newSequenceData;
        newSequenceData.valueType = data.valueType;

        switch (data.valueType)
        {
        case ValueType::Int:
        {
            auto op = operation == "square"              ? [](int x)
            { return x * x; } : operation == "increment" ? [](int x)
            { return x + 1; }
                            : operation == "negate" ? [](int x)
            { return -x; }
                                                    : throw std::invalid_argument("Invalid operation for int");
            auto *originalSeq = getSequence<int>(name);
            auto *newSeq = new SegmentedDeque<int>(originalSeq->getSegmentSize());
            for (auto it = originalSeq->begin(); it != originalSeq->end(); ++it)
            {
                newSeq->append(op(*it));
            }
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<int>>(newSeq);
            break;
        }
        case ValueType::Double:
        {
            auto op = operation == "square"              ? [](double x)
            { return x * x; } : operation == "increment" ? [](double x)
            { return x + 1.0; }
                            : operation == "negate" ? [](double x)
            { return -x; }
                                                    : throw std::invalid_argument("Invalid operation for double");
            auto *originalSeq = getSequence<double>(name);
            auto *newSeq = new SegmentedDeque<double>(originalSeq->getSegmentSize());
            for (auto it = originalSeq->begin(); it != originalSeq->end(); ++it)
            {
                newSeq->append(op(*it));
            }
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<double>>(newSeq);
            break;
        }
        case ValueType::String:
        {
            auto op = operation == "to_upper" ? [](const std::string &s)
            {
                std::string result = s;
                std::transform(result.begin(), result.end(), result.begin(), ::toupper);
                return result;
            }
                      : operation == "add_exclamation" ? [](const std::string &s)
            { return (s + "!"); }
                                                       : throw std::invalid_argument("Invalid operation for string");
            auto *originalSeq = getSequence<std::string>(name);
            auto *newSeq = new SegmentedDeque<std::string>(originalSeq->getSegmentSize());
            for (auto it = originalSeq->begin(); it != originalSeq->end(); ++it)
            {
                newSeq->append(op(*it));
            }
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<std::string>>(newSeq);
            break;
        }
        case ValueType::Person:
        {
            auto op = operation == "increment_age" ? [](const Person &p)
            {
                return Person(p.getName(), p.getAge() + 1);
            }
                      : operation == "add_prefix" ? [](const Person &p)
            {
                return Person("Mr/Ms " + p.getName(), p.getAge());
            }
                                                  : throw std::invalid_argument("Invalid operation for Person");
            auto *originalSeq = getSequence<Person>(name);
            auto *newSeq = new SegmentedDeque<Person>(originalSeq->getSegmentSize());
            for (auto it = originalSeq->begin(); it != originalSeq->end(); ++it)
            {
                newSeq->append(op(*it));
            }
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Person>>(newSeq);
            break;
        }
        case ValueType::Complex:
        {
            auto op = operation == "conjugate" ? [](const Complex &c)
            {
                return Complex(c.getReal(), -c.getImag());
            }
                      : operation == "square_magnitude" ? [](const Complex &c)
            {
                return Complex(c.magnitude() * c.magnitude(), 0.0);
            }
                                                        : throw std::invalid_argument("Invalid operation for Complex");
            auto *originalSeq = getSequence<Complex>(name);
            auto *newSeq = new SegmentedDeque<Complex>(originalSeq->getSegmentSize());
            for (auto it = originalSeq->begin(); it != originalSeq->end(); ++it)
            {
                newSeq->append(op(*it));
            }
            newSequenceData.sequence = std::unique_ptr<SegmentedDeque<Complex>>(newSeq);
            break;
        }
        }
        sequences[newName] = std::move(newSequenceData);
        emit sequenceCreated(QString::fromStdString(newName));
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Apply failed: %1").arg(e.what()));
    }
}

bool MainController::searchSubsequence(const std::string &name, const QVariantList &values)
{
    if (!sequenceExists(name))
    {
        throw std::runtime_error("Sequence not found");
    }

    try
    {
        const auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
        {
            std::vector<int> searchValues;
            for (const QVariant &value : values)
                searchValues.push_back(parseValue<int>(value, data.valueType));
            auto searchDeque = new SegmentedDeque<int>();
            for (const auto &val : searchValues)
                searchDeque->append(val);
            bool found = getSequence<int>(name)->searchSubsequence(
                getSequence<int>(name)->begin(), getSequence<int>(name)->end(),
                searchDeque->begin(), searchDeque->end());
            delete searchDeque;
            return found;
        }
        case ValueType::Double:
        {
            std::vector<double> searchValues;
            for (const QVariant &value : values)
                searchValues.push_back(parseValue<double>(value, data.valueType));
            auto searchDeque = new SegmentedDeque<double>();
            for (const auto &val : searchValues)
                searchDeque->append(val);
            bool found = getSequence<double>(name)->searchSubsequence(
                getSequence<double>(name)->begin(), getSequence<double>(name)->end(),
                searchDeque->begin(), searchDeque->end());
            delete searchDeque;
            return found;
        }
        case ValueType::String:
        {
            std::vector<std::string> searchValues;
            for (const QVariant &value : values)
                searchValues.push_back(parseValue<std::string>(value, data.valueType));
            auto searchDeque = new SegmentedDeque<std::string>();
            for (const auto &val : searchValues)
                searchDeque->append(val);
            bool found = getSequence<std::string>(name)->searchSubsequence(
                getSequence<std::string>(name)->begin(), getSequence<std::string>(name)->end(),
                searchDeque->begin(), searchDeque->end());
            delete searchDeque;
            return found;
        }
        case ValueType::Person:
        {
            std::vector<Person> searchValues;
            for (const QVariant &value : values)
                searchValues.push_back(parseValue<Person>(value, data.valueType));
            auto searchDeque = new SegmentedDeque<Person>();
            for (const auto &val : searchValues)
                searchDeque->append(val);
            bool found = getSequence<Person>(name)->searchSubsequence(
                getSequence<Person>(name)->begin(), getSequence<Person>(name)->end(),
                searchDeque->begin(), searchDeque->end());
            delete searchDeque;
            return found;
        }
        case ValueType::Complex:
        {
            std::vector<Complex> searchValues;
            for (const QVariant &value : values)
                searchValues.push_back(parseValue<Complex>(value, data.valueType));
            auto searchDeque = new SegmentedDeque<Complex>();
            for (const auto &val : searchValues)
                searchDeque->append(val);
            bool found = getSequence<Complex>(name)->searchSubsequence(
                getSequence<Complex>(name)->begin(), getSequence<Complex>(name)->end(),
                searchDeque->begin(), searchDeque->end());
            delete searchDeque;
            return found;
        }
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Search subsequence failed: ") + e.what());
    }
    return false;
}

QVariant MainController::getFromSequence(const std::string &name, int index) const
{
    if (!sequenceExists(name))
    {
        throw std::runtime_error("Sequence not found");
    }

    try
    {
        const auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
            return valueToQVariant(getSequence<int>(name)->get(index));
        case ValueType::Double:
            return valueToQVariant(getSequence<double>(name)->get(index));
        case ValueType::String:
            return valueToQVariant(getSequence<std::string>(name)->get(index));
        case ValueType::Person:
            return valueToQVariant(getSequence<Person>(name)->get(index));
        case ValueType::Complex:
            return valueToQVariant(getSequence<Complex>(name)->get(index));
        }
    }
    catch (const std::out_of_range &)
    {
        throw std::runtime_error("Index out of range");
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Get failed: ") + e.what());
    }
    return QVariant();
}

int MainController::getSequenceLength(const std::string &name) const
{
    if (!sequenceExists(name))
    {
        throw std::runtime_error("Sequence not found");
    }

    try
    {
        const auto &data = sequences.at(name);
        switch (data.valueType)
        {
        case ValueType::Int:
            return getSequence<int>(name)->getLength();
        case ValueType::Double:
            return getSequence<double>(name)->getLength();
        case ValueType::String:
            return getSequence<std::string>(name)->getLength();
        case ValueType::Person:
            return getSequence<Person>(name)->getLength();
        case ValueType::Complex:
            return getSequence<Complex>(name)->getLength();
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Get length failed: ") + e.what());
    }
    return 0;
}

QString MainController::getSequenceAsString(const std::string &name) const
{
    if (!sequenceExists(name))
    {
        throw std::runtime_error("Sequence not found");
    }

    try
    {
        const auto &data = sequences.at(name);
        std::ostringstream oss;
        bool first = true;

        switch (data.valueType)
        {
        case ValueType::Int:
        {
            auto *seq = getSequence<int>(name);
            int len = seq->getLength();
            for (int i = 0; i < len; i++)
            {
                if (!first)
                    oss << ", ";
                oss << seq->get(i);
                first = false;
            }
            break;
        }
        case ValueType::Double:
        {
            auto *seq = getSequence<double>(name);
            int len = seq->getLength();
            for (int i = 0; i < len; i++)
            {
                if (!first)
                    oss << ", ";
                oss << seq->get(i);
                first = false;
            }
            break;
        }
        case ValueType::String:
        {
            auto *seq = getSequence<std::string>(name);
            int len = seq->getLength();
            for (int i = 0; i < len; i++)
            {
                if (!first)
                    oss << ", ";
                oss << seq->get(i);
                first = false;
            }
            break;
        }
        case ValueType::Person:
        {
            auto *seq = getSequence<Person>(name);
            int len = seq->getLength();
            for (int i = 0; i < len; i++)
            {
                if (!first)
                    oss << ", ";
                oss << seq->get(i);
                first = false;
            }
            break;
        }
        case ValueType::Complex:
        {
            auto *seq = getSequence<Complex>(name);
            int len = seq->getLength();
            for (int i = 0; i < len; i++)
            {
                if (!first)
                    oss << ", ";
                oss << seq->get(i);
                first = false;
            }
            break;
        }
        }
        return QString::fromStdString(oss.str());
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Get as string failed: ") + e.what());
    }
}

std::vector<std::string> MainController::getAllSequenceNames() const
{
    std::vector<std::string> names;
    for (const auto &pair : sequences)
    {
        names.push_back(pair.first);
    }
    return names;
}
