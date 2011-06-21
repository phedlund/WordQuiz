// This file is generated by kconfig_compiler from documentsettings.kcfg.
// All changes you do to this file will be lost.
#ifndef DOCUMENTSETTINGS_H
#define DOCUMENTSETTINGS_H

#include <QtCore/QString>
//#include <kconfigskeleton.h>
//#include <kdebug.h>

class DocumentSettings /*: public KConfigSkeleton*/
{
  public:

    DocumentSettings( const QString & fileurl );
    ~DocumentSettings();

    void readConfig();
    void writeConfig();

    /**
      Set The width for the first table column.
    */
    void setSizeHintColumn( int section, int v )
    {
      if (!isImmutable( QString::fromLatin1( "SizeHintColumn%1" ).arg(section + 1) ))
        mSizeHintColumn[section] = v;
    }

    /**
      Get The width for the first table column.
    */
    int sizeHintColumn(int section) const
    {
      return mSizeHintColumn[section];
    }

    /**
      Get Item object corresponding to SizeHintColumn1()

    ItemInt *sizeHintColumn1Item()
    {
      return mSizeHintColumn1Item;
    }*/

    /**
      Set The width for the second table column.

    void setSizeHintColumn2( int v )
    {
      if (!isImmutable( QString::fromLatin1( "SizeHintColumn2" ) ))
        mSizeHintColumn2 = v;
    }*/

    /**
      Get The width for the second table column.

    int sizeHintColumn2() const
    {
      return mSizeHintColumn2;
    }*/

    /**
      Get Item object corresponding to SizeHintColumn2()

    ItemInt *sizeHintColumn2Item()
    {
      return mSizeHintColumn2Item;
    }*/

    /**
      Set Keyboard layout for column 1
    */
    void setKeyboardLayoutColumn( int section, const QString & v )
    {
      if (!isImmutable( QString::fromLatin1( "KeyboardLayoutColumn%1" ).arg(section + 1) ))
        mKeyboardLayoutColumn[section] = v;
    }

    /**
      Get Keyboard layout for column 1
    */
    QString keyboardLayoutColumn(int section) const
    {
      return mKeyboardLayoutColumn[section];
    }

    /**
      Get Item object corresponding to KeyboardLayoutColumn1()

    ItemString *keyboardLayoutColumn1Item()
    {
      return mKeyboardLayoutColumn1Item;
    }*/

    /**
      Set Keyboard layout for column 2

    void setKeyboardLayoutColumn2( const QString & v )
    {
      if (!isImmutable( QString::fromLatin1( "KeyboardLayoutColumn2" ) ))
        mKeyboardLayoutColumn2 = v;
    }*/

    /**
      Get Keyboard layout for column 2

    QString keyboardLayoutColumn2() const
    {
      return mKeyboardLayoutColumn2;
    }*/

    /**
      Get Item object corresponding to KeyboardLayoutColumn2()

    ItemString *keyboardLayoutColumn2Item()
    {
      return mKeyboardLayoutColumn2Item;
    }*/

  protected:
    bool isImmutable(const QString &) { return false;}

    QString mParamfileurl;

    // Document $(fileurl)
    int mSizeHintColumn[2];
    //int mSizeHintColumn2;
    QString mKeyboardLayoutColumn[2];
    //QString mKeyboardLayoutColumn2;
/*
  private:
    ItemInt *mSizeHintColumn1Item;
    ItemInt *mSizeHintColumn2Item;
    ItemString *mKeyboardLayoutColumn1Item;
    ItemString *mKeyboardLayoutColumn2Item;*/
};

#endif
