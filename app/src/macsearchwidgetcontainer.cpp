
#include "macsearchwidgetcontainer.h"

#ifdef Q_WS_MAC

Spacer::Spacer(QWidget *parent) : QWidget(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);
}

QSize Spacer::sizeHint() const
{
    return QSize(1, 1);
}


SearchWrapper::SearchWrapper(QWidget *parent) : QWidget(parent)
{
    m_searchBox = new MacSearchWidget(this);
    m_searchBox->move(2, 2);
    setFocusProxy(m_searchBox);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

QSize SearchWrapper::sizeHint() const
{
    return m_searchBox->sizeHint() + QSize(12, 2);
}


QSize SearchWrapper::minimumSizeHint() const
{
    return m_searchBox->minimumSizeHint() + QSize(12, 2);
}

#endif
