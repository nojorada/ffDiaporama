/* ======================================================================
    Copyright (C) 2026 Gerhard Kokerbeck ffDiaporama@SBK2day.com

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

#pragma once

#include <QComboBox>
#include <QObject>
class QListView;
class QStandardItemModel;

const int MAXCOLORREF = 99;
const int NBCOLORCOL  =  5;
const int NBCOLORROW  = 10;
const int ITEMSIZEW   = 32;
const int ITEMSIZEH   = 24;

class ColorComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit ColorComboBox(QWidget *parent = nullptr);

    QString currentColor() const;
//    void setCurrentColor(const QColor &color);
    void setCurrentColor(const QString &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void populateColors();

private slots:
    void item_clicked(const QModelIndex &index);

private:
    //QString mCurrentColor;
    QString mSavedCustomColor;

    QListView *m_view;
    QStandardItemModel *m_model;
    static QStringList predefinedColors;
};
