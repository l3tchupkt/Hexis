#ifndef FIRMWAREEXPLORER_H
#define FIRMWAREEXPLORER_H

#include <QTreeWidget>
#include "hexis_models.h"

class FirmwareExplorer : public QTreeWidget
{
    Q_OBJECT

public:
    explicit FirmwareExplorer(QWidget *parent = nullptr);
    void setFirmware(HexisFirmware* fw);
    void populateFromFirmware();

private:
    HexisFirmware* m_fw;
};

#endif // FIRMWAREEXPLORER_H
