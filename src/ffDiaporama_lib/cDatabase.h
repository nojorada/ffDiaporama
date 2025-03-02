/*======================================================================
    This file is part of ffDiaporama
    ffDiaporama is a tool to make diaporama as video
    Copyright (C) 2011-2013 Dominique Levray<domledom@laposte.net>

    This program is free software;you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation;either version 2 of the License,or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY;without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program;if not,write to the Free Software Foundation,Inc.,
    51 Franklin Street,Fifth Floor,Boston,MA 02110-1301 USA.
  ======================================================================*/

#ifndef CDATABASE_H
#define CDATABASE_H

// Include some additional standard class
//#ifdef Q_OS_WIN
//    #include <QtSql/QSqlDatabase>
//    #include <QtSql/QSqlQuery>
//    #include <QtSql/QSqlError>
//#else
    #include <QSqlDatabase>
    #include <QSqlQuery>
    #include <QSqlError>
//#endif
#include <QList>
#include <QStringList>
#include <QDir>
#include <QImage>
#include <QMap>
#include <QMutex>
//#include "cBaseAppConfig.h"
class cBaseAppConfig;
//*****************************************************************************
// Enum of all table type
//*****************************************************************************
enum eTypeTable {
    TypeTable_Undefined,
    TypeTable_SettingsTable,
    TypeTable_FolderTable,
    TypeTable_FileTable,
    TypeTable_SlideThumbsTable,
    TypeTable_LocationTable
};

//*****************************************************************************
// Utility function
//*****************************************************************************

void DisplayLastSQLError(QSqlQuery *Query);

//*****************************************************************************
// cDatabase : encapsulate a SQLite3 database
//*****************************************************************************
class cDatabaseTable;
class cDatabase {
public:
    QSqlDatabase            db;
    QString                 dbPath;
    QList<cDatabaseTable *> Tables;
    cBaseAppConfig          *ApplicationConfig;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))    
    QRecursiveMutex  dbMutex;
#else
    QMutex  dbMutex;
#endif

                            cDatabase(QString DBFNAME);
    virtual                 ~cDatabase();

    bool                    OpenDB();
    bool                    CloseDB();
    bool                    ResetDB();
    bool                    CheckDatabaseVersion();
    bool                    ValidateTables();
    QSqlError               LastError();
    cDatabaseTable          *GetTable(eTypeTable TableType);
};

//**********************************************************************************************
// cDatabaseTable : encapsulate a table
//      this class must not been used directly but must be use as ancestor by all table class
//**********************************************************************************************
class cDatabaseTable {
public:
    cDatabase               *Database;
    QString                 TableName;
    QString                 IndexKeyName;
    QString                 CreateTableQuery;
    QStringList             CreateIndexQuery;
    eTypeTable              TypeTable;
    qlonglong               NextIndex;

    explicit                cDatabaseTable(cDatabase *Database);
    virtual                 ~cDatabaseTable();

    virtual bool            CreateTable();
    virtual bool            ValidateTable();
    virtual bool            DoUpgradeTableVersion(qlonglong CurrentVersion);
};

//**********************************************************************************************
// cSettingsTable : encapsulate the settings table
//**********************************************************************************************
class cSettingsTable : public cDatabaseTable {
public:

    explicit                cSettingsTable(cDatabase *Database);

    virtual bool            CreateTable();

    virtual qlonglong       GetIntValue(QString SettingName,qlonglong DefaultValue);
    virtual bool            SetIntValue(QString SettingName,qlonglong Value);
    virtual QString         GetTextValue(QString SettingName,QString DefaultValue);
    virtual bool            SetTextValue(QString SettingName,QString Value);
    virtual bool            GetIntAndTextValue(QString SettingName,qlonglong *IntValue,QString *TextValue);
    virtual bool            SetIntAndTextValue(QString SettingName,qlonglong IntValue,QString TextValue);
};

//**********************************************************************************************
// cFolderTable : encapsulate folders in the table
//**********************************************************************************************
class cFolderTable : public cDatabaseTable 
{
   QMap<QString, qlonglong> folderMap;
   QMap<qlonglong, QString> folderKeyMap;
   public:
      explicit cFolderTable(cDatabase *Database);

      virtual bool DoUpgradeTableVersion(qlonglong CurrentVersion);

      qlonglong GetFolderKey(QString FolderPath);
      QString   GetFolderPath(qlonglong FolderKey);
      void UpdateFolderTimestamp(QDir Folder,qlonglong FolderKey);
      bool CheckFolderTimestamp(QDir Folder,qlonglong FolderKey);
};

//**********************************************************************************************
// cFilesTable : encapsulate media files in the table
//**********************************************************************************************
class cFilesTable : public cDatabaseTable 
{
   QSqlQuery *gfk;
   QSqlQuery *gfkfr;
   QSqlQuery *gfkInsert;
   QSqlQuery *getshortname;
   QSqlQuery *getbasicprops;
   QSqlQuery *updthumbs;

public:
   explicit cFilesTable(cDatabase *Database);
   virtual ~cFilesTable();
                
   virtual bool DoUpgradeTableVersion(qlonglong CurrentVersion);
                
   qlonglong GetFileKey(qlonglong FolderKey,QString ShortName,int MediaFileType,bool ForceRefresh=false);
   QString   GetShortName(qlonglong FileKey);
   QString   GetFileName(qlonglong FileKey);
   void      UpdateTableForFolder(qlonglong FolderKey,bool ForceRefresh);
   int       CleanTableForFolder(qlonglong FolderKey);
                
   bool SetBasicProperties(qlonglong FileKey,QString Properties);
   bool GetBasicProperties(qlonglong FileKey,QString *Properties,QString FileName,int64_t *FileSize,QDateTime *CreatDateTime,QDateTime *ModifDateTime);
   bool SetExtendedProperties(qlonglong FileKey,QStringList *PropertiesList);
   bool GetExtendedProperties(qlonglong FileKey,QStringList *PropertiesList);
   bool HasExtendedProperties(qlonglong FileKey);
   bool SetThumbs(qlonglong FileKey,QImage *Icon16,QImage *Icon100);
   bool GetThumbs(qlonglong FileKey,QImage *Icon16,QImage *Icon100);
   bool GetAnalyseSound(qlonglong FileKey,QList<qreal> *Peak,QList<qreal> *Moyenne,int64_t *RealAudioDuration,int64_t *RealVideoDuration,qreal *MaxMoyenneValue);
   void SetAnalyseSound(qlonglong FileKey,QList<qreal> *Peak,QList<qreal> *Moyenne,int64_t RealAudioDuration,int64_t *RealVideoDuration,qreal MaxMoyenneValue);

};

//**********************************************************************************************
// cSlideThumbsTable : encapsulate thumbnails in the table
//**********************************************************************************************
class cSlideThumbsTable : public cDatabaseTable 
{
   QMap<qlonglong, QImage> ThumbsMap;
   public:
      struct TRResKeyItem {       // struct use to translate key item when adding a project to a project
         qlonglong   OrigKey;
         qlonglong   NewKey;
      };

      explicit cSlideThumbsTable(cDatabase *Database);

      virtual bool ClearTable();
      virtual bool SetThumb(qlonglong *ThumbnailKey, const QImage &Thumb);
      virtual TRResKeyItem AppendThumb(qlonglong OrigKey,const QImage &Thumb);

      virtual bool GetThumb(qlonglong *ThumbnailKey,QImage *Thumb);
      virtual bool ClearThumb(qlonglong ThumbnailKey);
      virtual bool RemoveThumb(qlonglong ThumbnailKey);
};
typedef QList<cSlideThumbsTable::TRResKeyItem> TResKeyList;

//**********************************************************************************************
// cLocationTable : encapsulate locations in the table
//**********************************************************************************************
class cLocationTable : public cDatabaseTable {
public:

    explicit                cLocationTable(cDatabase *Database);

    virtual bool            DoUpgradeTableVersion(qlonglong CurrentVersion);
    virtual qlonglong       AppendLocation(QString Name,QString Address,QString FAddress,double Latitude,double Longitude,int Zoomlevel,QString Icon,QImage Thumbnail);
    virtual qlonglong       UpdateLocation(qlonglong Key,QString Name,QString Address,QString FAddress,double Latitude,double Longitude,int Zoomlevel,QString Icon,QImage Thumbnail);
};

#endif // CDATABASE_H
