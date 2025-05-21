#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <unordered_map>
#include <variant>
#include <vector>
#include <string>
#include "../models/inc/segmentedDeque.hpp"
#include "../models/types/complex.hpp"
#include "../models/types/person.hpp"

class MainController : public QObject
{
    Q_OBJECT

public:
    enum class ValueType
    {
        Int,
        Double,
        String,
        Person,
        Complex
    };

    template <typename T>
    SegmentedDeque<T> *getSequence(const std::string &name) const;

    explicit MainController(QObject *parent = nullptr);
    ~MainController() = default;

    void createSequence(const std::string &name, ValueType valueType, int segmentSize = 32);
    void removeSequence(const std::string &name);

    bool sequenceExists(const std::string &name) const;
    ValueType getSequenceValueType(const std::string &name) const;

    void appendToSequence(const std::string &name, const QVariant &value);
    void prependToSequence(const std::string &name, const QVariant &value);
    void insertAtSequence(const std::string &name, int index, const QVariant &value);
    void setInSequence(const std::string &name, int index, const QVariant &value);
    void concat(const std::string &name1, const std::string &name2);

    void appendToImmutableSequence(const std::string &originalName, const QVariant &value, const std::string &newName);
    void prependToImmutableSequence(const std::string &originalName, const QVariant &value, const std::string &newName);
    void insertAtImmutableSequence(const std::string &originalName, int index, const QVariant &value, const std::string &newName);
    void setInImmutableSequence(const std::string &originalName, int index, const QVariant &value, const std::string &newName);
    void concatImmutable(const std::string &name1, const std::string &name2, const std::string &newName);

    void sortSequence(const std::string &name, bool ascending = 1);
    void sortImmutableSequence(const std::string &originalName, const std::string &newName, bool ascending = 1);
    void where(const std::string &name, const std::string &newName, const QString &condition);
    QVariant reduceSequence(const std::string &name, const QVariant &initialValue);
    void applyUnaryOp(const std::string &name, const std::string &newName, const QString &operation);
    bool searchSubsequence(const std::string &name, const QVariantList &values);

    QVariant getFromSequence(const std::string &name, int index) const;
    int getSequenceLength(const std::string &name) const;
    QString getSequenceAsString(const std::string &name) const;
    std::vector<std::string> getAllSequenceNames() const;

signals:
    void sequenceCreated(const QString &name);
    void sequenceRemoved(const QString &name);
    void sequenceChanged(const QString &name);
    void errorOccurred(const QString &message);

private:
    struct SequenceData
    {
        ValueType valueType;
        std::variant<
            std::unique_ptr<SegmentedDeque<int>>,
            std::unique_ptr<SegmentedDeque<double>>,
            std::unique_ptr<SegmentedDeque<std::string>>,
            std::unique_ptr<SegmentedDeque<Person>>,
            std::unique_ptr<SegmentedDeque<Complex>>> sequence;
    };

    std::unordered_map<std::string, SequenceData> sequences;

    template <typename T>
    T parseValue(const QVariant &value, ValueType valueType) const;

    template <typename T>
    QVariant valueToQVariant(const T &value) const;
};
