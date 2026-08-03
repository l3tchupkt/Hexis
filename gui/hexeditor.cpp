#include "hexeditor.h"
#include <QPainter>
#include <QScrollBar>
#include <QFontDatabase>

HexEditor::HexEditor(QWidget *parent)
    : QAbstractScrollArea(parent), m_bytesPerLine(16)
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(10);
    setFont(font);

    QFontMetrics fm(font);
    m_charWidth = fm.horizontalAdvance('0');
    m_charHeight = fm.height();
}

void HexEditor::setData(const QByteArray &data)
{
    m_data = data;
    adjustScrollbars();
    viewport()->update();
}

QByteArray HexEditor::data() const
{
    return m_data;
}

void HexEditor::adjustScrollbars()
{
    int numLines = m_data.size() / m_bytesPerLine + ((m_data.size() % m_bytesPerLine) ? 1 : 0);
    verticalScrollBar()->setRange(0, std::max(0, numLines - viewport()->height() / m_charHeight));
    verticalScrollBar()->setPageStep(viewport()->height() / m_charHeight);
}

void HexEditor::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    adjustScrollbars();
}

void HexEditor::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(viewport());
    painter.setFont(font());
    
    int firstLineIdx = verticalScrollBar()->value();
    int lastLineIdx = firstLineIdx + viewport()->height() / m_charHeight + 1;
    
    int dataSize = m_data.size();
    
    int y = m_charHeight;
    for (int i = firstLineIdx; i < lastLineIdx; ++i)
    {
        int offset = i * m_bytesPerLine;
        if (offset >= dataSize) break;
        
        // Draw Address
        QString address = QString("%1").arg(offset, 8, 16, QLatin1Char('0')).toUpper();
        painter.setPen(QColor(120, 120, 120)); // Dimmed color for address
        painter.drawText(0, y, address);
        
        // Draw Hex
        int hexX = m_charWidth * 10;
        int asciiX = hexX + (m_bytesPerLine * 3 + 2) * m_charWidth;
        
        for (int j = 0; j < m_bytesPerLine; ++j)
        {
            if (offset + j >= dataSize) break;
            
            unsigned char byte = m_data[offset + j];
            QString hexStr = QString("%1").arg(byte, 2, 16, QLatin1Char('0')).toUpper();
            
            // Hex coloring for basic entropy/ASCII
            if (byte == 0x00 || byte == 0xFF) {
                painter.setPen(QColor(100, 100, 100)); // padding
            } else if (byte >= 0x20 && byte <= 0x7E) {
                painter.setPen(QColor(42, 130, 218)); // ASCII printable (blue)
            } else {
                painter.setPen(Qt::white); // general
            }
            
            painter.drawText(hexX + j * 3 * m_charWidth, y, hexStr);
            
            // Draw ASCII
            char asciiChar = (byte >= 0x20 && byte <= 0x7E) ? byte : '.';
            painter.drawText(asciiX + j * m_charWidth, y, QString(asciiChar));
        }
        
        y += m_charHeight;
    }
}
