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

#ifndef DLGMANAGEFAVORITE_H
#define DLGMANAGEFAVORITE_H

// Basic inclusions (common to all files)
#include "CustomCtrl/_QCustomDialog.h"

namespace Ui {
    class DlgManageFavorite;
}

class DlgManageFavorite : public QCustomDialog {
Q_OBJECT
public:
    QStringList    *Collection;
    QStringList    UndoCollection;

    explicit DlgManageFavorite(QStringList *Collection,cApplicationConfig *ApplicationConfig,QWidget *parent=0);
    ~DlgManageFavorite();

    void            PopulateList(QString ActiveName);

    // function to be overloaded
    virtual void    DoInitDialog();                             // Initialise dialog
    virtual bool    DoAccept()          {return true;}          // Call when user click on Ok button
    virtual void    DoRejet()           {/*Nothing to do*/}     // Call when user click on Cancel button
    virtual void    PrepareGlobalUndo();                        // Initiale Undo
    virtual void    DoGlobalUndo();                             // Apply Undo : call when user click on Cancel button

private slots:
    void        s_currentRowChanged(int);
    void        s_DBRename();
    void        s_DBRemove();

private:
    Ui::DlgManageFavorite *ui;
};

#endif // DLGMANAGEFAVORITE_H
