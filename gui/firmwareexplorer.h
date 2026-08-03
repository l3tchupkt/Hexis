#ifndef FIRMWAREEXPLORER_H
#define FIRMWAREEXPLORER_H

#include <QTreeWidget>
#include <QVector>

struct FirmwareNode {
    QString name;
    QString type;
    uint64_t size;
    uint64_t offset;
};

class FirmwareExplorer : public QTreeWidget
{
    Q_OBJECT

public:
    explicit FirmwareExplorer(QWidget *parent = nullptr);
    void populateSimulatedData();

private:
    void addNode(QTreeWidgetItem *parent, const FirmwareNode &data);
};

#endif // FIRMWAREEXPLORER_H
