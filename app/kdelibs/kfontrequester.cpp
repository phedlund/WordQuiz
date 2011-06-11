/*
    Copyright (C) 2003 Nadeem Hasan <nhasan@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kfontrequester.h"

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLayout>
#include <QtGui/QFontDatabase>
#include <QtGui/QFontDialog>
//#include <kfontdialog.h>
//#include <klocale.h>

// Determine if the font with given properties is available on the system,
// otherwise find and return the best fitting combination.
static QFont nearestExistingFont (const QFont &font)
{
    QFontDatabase dbase;

    // Initialize font data accoring to given font object.
    QString family = font.family();
    QString style = dbase.styleString(font);
    int size = font.pointSize();

    // Check if the family exists.
    QStringList families = dbase.families();
    if (!families.contains(family)) {
        // Chose another family.
        family = families.count() ? families[0] : "fixed";
        // TODO: Try to find nearest match?
    }

    // Check if the family has the requested style.
    // Easiest by piping it through font selection in the database.
    QString retStyle = dbase.styleString(dbase.font(family, style, 10));
    style = retStyle;

    // Check if the family has the requested size.
    // Only for bitmap fonts.
    if (!dbase.isSmoothlyScalable(family, style)) {
        QList<int> sizes = dbase.smoothSizes(family, style);
        if (!sizes.contains(size)) {
            // Find nearest available size.
            int mindiff = 1000;
            int refsize = size;
            foreach (int lsize, sizes) {
                int diff = qAbs(refsize - lsize);
                if (mindiff > diff) {
                    mindiff = diff;
                    size = lsize;
                }
            }
        }
    }

    // Select the font with confirmed properties.
    return dbase.font(family, style, size);
}
/*
class KFontRequester::KFontRequesterPrivate
{
public:
  KFontRequesterPrivate(KFontRequester *q): q(q) {}

  void displaySampleText();
  void setToolTip();

  void _k_buttonClicked();

  KFontRequester *q;
  bool m_onlyFixed;
  QString m_sampleText, m_title;
  QLabel *m_sampleLabel;
  QPushButton *m_button;
  QFont m_selFont;
};
*/
KFontRequester::KFontRequester( QWidget *parent, bool onlyFixed )
    : QWidget( parent )
{
  m_onlyFixed = onlyFixed;

  QHBoxLayout *layout = new QHBoxLayout( this );
  layout->setMargin( 0 );
  layout->setSpacing( 6 /*KDialog::spacingHint()*/ );

  m_sampleLabel = new QLabel( this );
  m_button = new QPushButton( tr( "Choose..." ), this );

  m_sampleLabel->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
  setFocusProxy( m_button );

  layout->addWidget( m_sampleLabel, 1 );
  layout->addWidget( m_button );

  connect( m_button, SIGNAL( clicked() ), SLOT( buttonClicked() ) );

  displaySampleText();
  setToolTip();
}

KFontRequester::~KFontRequester()
{
  delete m_sampleLabel;
  delete m_button;
}

QFont KFontRequester::font() const
{
  return m_selFont;
}

bool KFontRequester::isFixedOnly() const
{
  return m_onlyFixed;
}

QString KFontRequester::sampleText() const
{
  return m_sampleText;
}

QString KFontRequester::title() const
{
  return m_title;
}

QLabel *KFontRequester::label() const
{
  return m_sampleLabel;
}

QPushButton *KFontRequester::button() const
{
  return m_button;
}

void KFontRequester::setFont( const QFont &font, bool onlyFixed )
{
  //m_selFont = nearestExistingFont(font);
  m_selFont = font;
  m_onlyFixed = onlyFixed;

  displaySampleText();
  emit fontSelected( m_selFont );
}

void KFontRequester::setSampleText( const QString &text )
{
  m_sampleText = text;
  displaySampleText();
}

void KFontRequester::setTitle( const QString &title )
{
  m_title = title;
  setToolTip();
}

void KFontRequester::buttonClicked()
{
/*    KFontChooser::DisplayFlags flags = KFontChooser::NoDisplayFlags;
    if ( m_onlyFixed ) {
        flags |= KFontChooser::FixedFontsOnly;
    }
*/
    //int result = QFontDialog::getFont( m_selFont, flags, q->parentWidget() );
    bool ok;
    QFont result = QFontDialog::getFont( &ok, m_selFont, parentWidget() );
    if ( ok /*result == QDialog::Accepted*/ )
    {
       m_selFont = result;
       displaySampleText();

        emit fontSelected( m_selFont );
    }
}

void KFontRequester::displaySampleText()
{
  m_sampleLabel->setFont( m_selFont );

  int size = m_selFont.pointSize();
  if(size == -1)
    size = m_selFont.pixelSize();

  if ( m_sampleText.isEmpty() ) {
    // See the comment to the same message in kfontchooser.cpp.
    QString family = tr(/*"@item Font name", */ "%1").arg( m_selFont.family());
    m_sampleLabel->setText( QString( "%1 %2" ).arg( family ).arg( size ) );
  }
  else {
    m_sampleLabel->setText( m_sampleText );
  }
}

void KFontRequester::setToolTip()
{
  m_button->setToolTip( tr( "Click to select a font" ) );

  m_sampleLabel->setToolTip( QString() );
  m_sampleLabel->setWhatsThis(QString());

  if ( m_title.isNull() )
  {
    m_sampleLabel->setToolTip( tr( "Preview of the selected font" ) );
    m_sampleLabel->setWhatsThis( tr( "This is a preview of the selected font. You can change it"
        " by clicking the \"Choose...\" button." ) );
  }
  else
  {
    m_sampleLabel->setToolTip( tr( "Preview of the \"%1\" font").arg( m_title ) );
    m_sampleLabel->setWhatsThis( tr( "This is a preview of the \"%1\" font. You can change it"
        " by clicking the \"Choose...\" button." ).arg( m_title ) );
  }
}

//#include "kfontrequester.moc"

/* vim: et sw=2 ts=2
*/
