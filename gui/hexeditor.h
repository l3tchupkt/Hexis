#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QAbstractScrollArea>
#include <QByteArray>
#include <QPaintEvent>

class HexEditor : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit HexEditor(QWidget *parent = nullptr);
    void setData(const QByteArray &data);
    QByteArray data() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private:
    void adjustScrollbars();

    QByteArray m_data;
    int m_charWidth;
    int m_charHeight;
    int m_bytesPerLine;
};

#endif // HEXEDITOR_H
