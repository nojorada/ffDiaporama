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

#include "FFDFileExplorer.h"
#include "ui_FFDFileExplorer.h"

FFDFileExplorer::FFDFileExplorer(BROWSER_TYPE_ID BrowserType, bool AllowMultipleSelection, bool AllowDragDrop,
      bool AllowAddToProject,QString BoxTitle,cApplicationConfig *ApplicationConfig,QWidget *parent) :
                QCustomDialog(ApplicationConfig,parent),ui(new Ui::FFDFileExplorer) 
{
   // Initialise UI
   ui->setupUi(this);
   OkBt     = ui->OKBT;
   CancelBt = ui->CancelBt;
   HelpBt   = ui->HelpBT;
   HelpFile = "0109";
   UndoBt   = NULL;

   TypeWindowState = TypeWindowState_withsplitterpos;
   Splitter = ui->Browser->GetSplitter();

   setWindowTitle(BoxTitle);

   ui->Browser->DoInitWidget(BrowserType, AllowMultipleSelection, AllowDragDrop, AllowAddToProject, ApplicationConfig);
}

//====================================================================================================================

FFDFileExplorer::~FFDFileExplorer() 
{
    delete ui;
}

//====================================================================================================================

void FFDFileExplorer::DoInitDialog() 
{
   connect(ui->Browser,SIGNAL(DoRefreshControls()),this,SLOT(RefreshControls()));
   connect(ui->Browser,SIGNAL(DoAddFiles()),this,SLOT(accept()));
   connect(ui->Browser,SIGNAL(DoOpenFile()),this,SLOT(OpenFile()));
   ui->Browser->DoInitDialog();
}

//====================================================================================================================

void FFDFileExplorer::RefreshControls() 
{
    ui->OKBT->setEnabled(GetCurrentSelectedFiles().count() > 0);
}

//====================================================================================================================

QStringList FFDFileExplorer::GetCurrentSelectedFiles() 
{
   return ui->Browser->GetCurrentSelectedFiles();
}

//====================================================================================================================

bool FFDFileExplorer::DoAccept() 
{
   QList<cBaseMediaFile*> MediaList;
   ui->Browser->GetCurrentSelectedMediaFile(&MediaList);
   bool HaveFolder = false;
   bool Ret = true;
   for (int i = 0; i < MediaList.count() && !HaveFolder; i++) 
      if (MediaList[i]->is_Folder()) 
         HaveFolder = true;
   if (MediaList.count() == 0) 
      Ret = false; 
   else if (HaveFolder) 
   {
      if (MediaList.count() == 1) 
      {
         ui->Browser->OpenFolder();
         Ret = false;
      } 
      else 
      {
         CustomMessageBox(this,QMessageBox::Information,windowTitle(),QApplication::translate("wgt_QMultimediaBrowser","The selection can't include folders."),QMessageBox::Ok,QMessageBox::Ok);
         Ret = false;
      }
   }
   while (!MediaList.isEmpty()) 
      delete MediaList.takeLast();
   return Ret;
}

//====================================================================================================================

void FFDFileExplorer::OpenFile() 
{
   cBaseMediaFile *Media = ui->Browser->GetCurrentMediaFile();
   if (Media) 
   {
      if (Media->is_Imagefile() || Media->is_Videofile() || Media->is_Musicfile() || Media->is_Thumbnail())
         QDesktopServices::openUrl(QUrl().fromLocalFile(Media->FileName()));
      delete Media;
   }
}
