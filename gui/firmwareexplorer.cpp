#include "firmwareexplorer.h"

FirmwareExplorer::FirmwareExplorer(QWidget *parent)
    : QTreeWidget(parent)
{
    setHeaderLabels({"Name", "Type", "Size", "Offset"});
    setColumnWidth(0, 250);
}

void FirmwareExplorer::populateSimulatedData()
{
    clear();

    QTreeWidgetItem *root = new QTreeWidgetItem(this);
    root->setText(0, "firmware.bin");
    root->setText(1, "Raw Image");
    
    // U-Boot
    FirmwareNode uboot = {"u-boot.bin", "Bootloader", 262144, 0x000000};
    addNode(root, uboot);

    // Kernel
    FirmwareNode kernel = {"uImage", "Linux Kernel", 2097152, 0x040000};
    addNode(root, kernel);

    // SquashFS
    QTreeWidgetItem *fs = new QTreeWidgetItem(root);
    fs->setText(0, "rootfs.sqsh");
    fs->setText(1, "SquashFS");
    fs->setText(2, "4194304");
    fs->setText(3, "0x240000");

    FirmwareNode bin = {"/bin", "Directory", 0, 0};
    addNode(fs, bin);
    FirmwareNode etc = {"/etc", "Directory", 0, 0};
    addNode(fs, etc);
    FirmwareNode shadow = {"/etc/shadow", "File", 512, 0x240150};
    addNode(fs, shadow);

    expandAll();
}

void FirmwareExplorer::addNode(QTreeWidgetItem *parent, const FirmwareNode &data)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, data.name);
    item->setText(1, data.type);
    item->setText(2, data.size > 0 ? QString::number(data.size) : "");
    item->setText(3, data.offset > 0 ? QString("0x%1").arg(data.offset, 0, 16) : "");
}
