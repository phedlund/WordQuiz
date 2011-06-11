/****************************************************************************
**
** Copyright (C) 2004-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.1, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "characterwidget.h"

CharacterWidget::CharacterWidget(QWidget *parent)
    : QWidget(parent)
{
    squareSize = 24;
    columns = 16;
    lastKey = -1;
    setMouseTracking(true);
}

void CharacterWidget::updateFont(const QFont &font)
{
    displayFont.setFamily(font.family());
    squareSize = qMax(24, QFontMetrics(displayFont).xHeight() * 3);
    adjustSize();
    update();
}

void CharacterWidget::updateSize(const QString &fontSize)
{
    displayFont.setPointSize(fontSize.toInt());
    squareSize = qMax(24, QFontMetrics(displayFont).xHeight() * 3);
    adjustSize();
    update();
}

void CharacterWidget::updateStyle(const QString &fontStyle)
{
    QFontDatabase fontDatabase;
    const QFont::StyleStrategy oldStrategy = displayFont.styleStrategy();
    displayFont = fontDatabase.font(displayFont.family(), fontStyle, displayFont.pointSize());
    displayFont.setStyleStrategy(oldStrategy);
    squareSize = qMax(24, QFontMetrics(displayFont).xHeight() * 3);
    adjustSize();
    update();
}

void CharacterWidget::updateFontMerging(bool enable)
{
    if (enable)
        displayFont.setStyleStrategy(QFont::PreferDefault);
    else
        displayFont.setStyleStrategy(QFont::NoFontMerging);
    adjustSize();
    update();
}

QSize CharacterWidget::sizeHint() const
{
    return QSize(columns*squareSize, (65536/columns)*squareSize);
}

void CharacterWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint widgetPosition = mapFromGlobal(event->globalPos());
    uint key = (widgetPosition.y()/squareSize)*columns + widgetPosition.x()/squareSize;

    QString text = QString::fromLatin1("<p>Character: <span style=\"font-size: 24pt; font-family: %1\">").arg(displayFont.family())
                  + QChar(key)
                  + QString::fromLatin1("</span><p>Value: 0x")
                  + QString::number(key, 16);
    QToolTip::showText(event->globalPos(), text, this);
}

void CharacterWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastKey = (event->y()/squareSize)*columns + event->x()/squareSize;
        if (QChar(lastKey).category() != QChar::NoCategory)
            emit characterSelected(QString(QChar(lastKey)));
        update();
    }
    else
        QWidget::mousePressEvent(event);
}

void CharacterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.setFont(displayFont);

    QRect redrawRect = event->rect();
    int beginRow = redrawRect.top()/squareSize;
    int endRow = redrawRect.bottom()/squareSize;
    int beginColumn = redrawRect.left()/squareSize;
    int endColumn = redrawRect.right()/squareSize;

    painter.setPen(QPen(Qt::gray));
    for (int row = beginRow; row <= endRow; ++row) {
        for (int column = beginColumn; column <= endColumn; ++column) {
            painter.drawRect(column*squareSize, row*squareSize, squareSize, squareSize);
        }
    }

    QFontMetrics fontMetrics(displayFont);
    painter.setPen(QPen(Qt::black));
    for (int row = beginRow; row <= endRow; ++row) {

        for (int column = beginColumn; column <= endColumn; ++column) {

            int key = row*columns + column;
            painter.setClipRect(column*squareSize, row*squareSize, squareSize, squareSize);

            if (key == lastKey)
                painter.fillRect(column*squareSize + 1, row*squareSize + 1, squareSize, squareSize, QBrush(Qt::red));

            painter.drawText(column*squareSize + (squareSize / 2) - fontMetrics.width(QChar(key))/2,
                             row*squareSize + 4 + fontMetrics.ascent(),
                             QString(QChar(key)));
        }
    }
}
