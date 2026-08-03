#include "firmwareexplorer.h"

FirmwareExplorer::FirmwareExplorer(QWidget *parent)
    : QTreeWidget(parent), m_fw(nullptr)
{
    setHeaderLabels({"Name", "Type", "Size", "Offset"});
    setColumnWidth(0, 250);
}

void FirmwareExplorer::setFirmware(HexisFirmware* fw)
{
    m_fw = fw;
    populateFromFirmware();
}

void FirmwareExplorer::populateFromFirmware()
{
    clear();
    if (!m_fw) return;

    QTreeWidgetItem *root = new QTreeWidgetItem(this);
    root->setText(0, "HexisFirmware");
    root->setText(1, "Buffer");
    root->setText(2, QString::number(m_fw->size));
    root->setText(3, "0x0");

    // Populate Partitions
    HexisPartition* p = m_fw->partitions;
    while (p) {
        QTreeWidgetItem *pItem = new QTreeWidgetItem(root);
        pItem->setText(0, p->name);
        pItem->setText(1, p->type);
        pItem->setText(2, QString::number(p->size));
        pItem->setText(3, QString("0x%1").arg(p->offset, 0, 16));
        p = p->next;
    }

    // Populate Files
    HexisFile* f = m_fw->files;
    while (f) {
        QTreeWidgetItem *fItem = new QTreeWidgetItem(root);
        fItem->setText(0, f->path);
        fItem->setText(1, "File");
        fItem->setText(2, QString::number(f->size));
        fItem->setText(3, QString("0x%1").arg(f->offset, 0, 16));
        f = f->next;
    }

    expandAll();
}
