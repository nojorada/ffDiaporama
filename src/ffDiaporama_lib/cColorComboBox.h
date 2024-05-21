/* ======================================================================
    This file is part of ffDiaporama
    ffDiaporama is a tool to make diaporama as video
    Copyright (C) 2011-2014 Dominique Levray <domledom@laposte.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
   ====================================================================== */

#ifndef CCCOLORCOMBOBOX_H
#define CCCOLORCOMBOBOX_H

#include "BasicDefines.h"
#include <QComboBox>
#include <QItemSelection>
#include <QStyledItemDelegate>

//******************************************************************************************************************
// Custom QAbstractItemDelegate for Color ComboBox
//******************************************************************************************************************

class cCColorComboBox;
class cCColorComboBoxItem : public QStyledItemDelegate {
Q_OBJECT
public:
    cCColorComboBox    *ComboBox;

    explicit cCColorComboBoxItem(QObject *parent=0);

    virtual void    paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    virtual QSize   sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;
};

//******************************************************************************************************************
// Custom Color ComboBox
//******************************************************************************************************************

class cCColorComboBox : public QComboBox {
Q_OBJECT
public:
    QString             *CurrentColor;
    QString             SavedCustomColor;
    bool                StandardColor;
    bool                STOPMAJ;
    bool                IsPopupOpen;
    cCColorComboBoxItem ItemDelegate;

    explicit            cCColorComboBox(QWidget *parent = 0);
    void                MakeIcons();
    void                SetCurrentColor(QString *Color);
    QString             GetCurrentColor();

protected:
    virtual void        hidePopup() { IsPopupOpen=false; QComboBox::hidePopup(); emit PopupClosed(0); }
    virtual void        showPopup() { IsPopupOpen=true;  QComboBox::showPopup(); }

signals:
    void                PopupClosed(int);

public slots:
    void                s_ItemSelectionChanged();
    void                s_ItemPressed(int Row,int Col);
};

class cColorComboBox;
class cColorComboBoxItem : public QStyledItemDelegate 
{
	Q_OBJECT
	public:
		cColorComboBox *ComboBox;

		explicit cColorComboBoxItem(QObject *parent=0);

      virtual void  paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
      virtual QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;
};

class colorModel : public QAbstractItemModel
{
   cColorComboBox *comboBox;
   public:
      colorModel(cColorComboBox *parent );// : QAbstractItemModel(parent) { comboBox = parent; }
      virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
      virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
      virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
      virtual QModelIndex parent(const QModelIndex & index) const;
      virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

      QString colorFromIndex(QModelIndex idx) const;
      QModelIndex getIndexForColor(QString color) const;
      QModelIndex home() { return index(0,0); }
      QModelIndex next(QModelIndex current);
      QModelIndex prev(QModelIndex current);
      QModelIndex last();
};

class cColorComboBox : public QComboBox 
{
   Q_OBJECT
      colorModel *model;
   public:
      QString             CurrentColor;
      QString             SavedCustomColor;
      bool                StandardColor;
      bool                STOPMAJ;
      cColorComboBoxItem  ItemDelegate;

      explicit            cColorComboBox(QWidget *parent = 0);
      void                MakeIcons();
      void                SetCurrentColor(QString Color);
      QString             GetCurrentColor();

   protected:
      virtual void keyPressEvent(QKeyEvent *event);
      virtual void wheelEvent(QWheelEvent *e);
      virtual void resizeEvent(QResizeEvent *);

   public slots:
      void s_ItemSelectionChanged();
      void s_ItemPressed(int Row,int Col);
      void slotActivated(int index);
      //void setCurrentIndex(int index);
      void currentIndexChanged(int index);
      void item_clicked(const QModelIndex &);
};

#endif // CCCOLORCOMBOBOX_H
