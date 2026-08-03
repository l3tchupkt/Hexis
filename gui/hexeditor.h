#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QAbstractScrollArea>
#include <QPaintEvent>
#include "hexis_models.h"

class HexEditor : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit HexEditor(QWidget *parent = nullptr);
    void setFirmware(HexisFirmware* fw);
    HexisFirmware* firmware() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private:
    void adjustScrollbars();

    HexisFirmware* m_fw;
    int m_charWidth;
    int m_charHeight;
    int m_bytesPerLine;
};

#endif // HEXEDITOR_H
