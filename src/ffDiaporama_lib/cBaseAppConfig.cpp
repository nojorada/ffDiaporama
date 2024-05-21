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

#include "cBaseAppConfig.h"

cBaseAppConfig* cBaseAppConfig::baseAppConfig = NULL;
cBaseAppConfig::cBaseAppConfig(QObject *TheTopLevelWindow) : QObject(TheTopLevelWindow) 
{
   MemCacheMaxValue = 512*1024*1024;     // 512 Mb for image cache
   Smoothing        = true;              // True do smoothing in preview
   Database         = NULL;
   SettingsTable    = NULL;
   FoldersTable     = NULL;
   FilesTable       = NULL;
   SlideThumbsTable = NULL;
   LocationTable    = NULL;
   baseAppConfig = this;
}                     

cBaseAppConfig::~cBaseAppConfig() 
{
   if (Database) 
      delete Database;
   ImagesCache.clear();
}

bool cBaseAppConfig::initDatabase(const QString dbPath)
{
   Database = new cDatabase(dbPath);
   Database->ApplicationConfig = this;
   bool IsDBCreation = !QFileInfo(Database->dbPath).exists();
   if (Database->OpenDB())
   {
      //==== Tables definition
      Database->Tables.append(SettingsTable = new cSettingsTable(Database));
      Database->Tables.append(FoldersTable = new cFolderTable(Database));
      Database->Tables.append(FilesTable = new cFilesTable(Database));
      Database->Tables.append(SlideThumbsTable = new cSlideThumbsTable(Database));
      Database->Tables.append(LocationTable = new cLocationTable(Database));
      ImagesCache.setFilesTable(FilesTable);
      ImagesCache.setThumbsTable(SlideThumbsTable);
      //==== End of tables definition
      if (((!IsDBCreation) && (!Database->CheckDatabaseVersion())) || (!Database->ValidateTables()))
      {
         return false;
         //ToLog(LOGMSG_CRITICAL, QApplication::translate("MainWindow", "Error initialising home user database..."));
         //CustomMessageBox(NULL, QMessageBox::Critical, QApplication::translate("MainWindow", "Error", "Error message"),
         //   QApplication::translate("MainWindow", "Error initialising home user database\nffDiaporama can't start", "Error message"), QMessageBox::Close);
         //exit(1);
      }
      return true;
   }
   return false;
}