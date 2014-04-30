#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabBar>
#include <QDockWidget>
#include <Astroprocessor/Gui>
#include "help.h"
#include "slidewidget.h"

class QListWidget;
class QLineEdit;
class QActionGroup;
class AstroFileEditor;
class GeoSearchWidget;
class QComboBox;

/* =========================== ASTRO FILE INFO ====================================== */

class AstroFileInfo : public AstroFileHandler
{
    Q_OBJECT

    private:
        int currentIndex;
        QPushButton* edit;
        QLabel* shadow;
        bool showAge;

        AstroFile* currentFile()             { return file(currentIndex); }
        void setText(const QString& str);
        void refresh();

    protected:                            // AstroFileHandler implementations
        void filesUpdated(MembersList members);

    signals:
        void clicked();

    public:
        AstroFileInfo (QWidget *parent = 0);
        void setCurrentIndex(int i)          { currentIndex = i; }

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings       ( const AppSettings& );
        void setupSettingsEditor ( AppSettingsEditor* );
};


/* =========================== ASTRO WIDGET ========================================= */

class AstroWidget : public QWidget
{
    Q_OBJECT

    private:
        AstroFileList f;

        AstroFileEditor*  editor;
        GeoSearchWidget*  geoWdg;
        QToolBar*         toolBar;
        QActionGroup*     actionGroup;
        SlideWidget*      slides;
        AstroFileInfo*    fileView, *fileView2nd;
        QList<AstroFileHandler*> handlers;

        QComboBox* zodiacSelector;
        QComboBox* hsystemSelector;
        QComboBox* levelSelector;
        QList<QComboBox*> horoscopeControls;

        void setupFile (AstroFile* file);
        QString vectorToString (const QVector3D& v);
        QVector3D vectorFromString (const QString& str);

        void addHoroscopeControls();
        void addSlide(AstroFileHandler* w, const QIcon& icon, QString title);

    private slots:
        void applyGeoSettings(AppSettings&);
        void toolBarActionClicked();
        void currentSlideChanged();
        void horoscopeControlChanged();
        void destroyEditor();

    public slots:
        void openEditor();

    signals:
        void helpRequested(QString tag);

    public:
        AstroWidget(QWidget *parent = 0);
        QToolBar* getToolBar()      { return toolBar; }
        const QList<QComboBox*>& getHoroscopeControls() { return horoscopeControls; }

        void setFiles (const AstroFileList& files);
        AstroFile* currentFile()                     { if (f.count()) return f[0]; else return 0; }

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings       ( const AppSettings& );
        void setupSettingsEditor ( AppSettingsEditor* );
};


/* =========================== FILES BAR ============================================ */

class FilesBar : public QTabBar
{
    Q_OBJECT

    private:
        bool askToSave;
        QList<AstroFileList> files;

        void updateTab(int index);
        int getTabIndex(AstroFile* f);
        int getTabIndex(QString name);

    private slots:
        void swapFiles(int,int);
        void fileUpdated(AstroFile::Members);
        void fileDestroyed();
        //void removeTab();

    public slots:
        void addNewFile() { addFile(new AstroFile); }
        //void deleteFile(QString name);
        void openFile(QString name);
        void openFileInNewTab(QString name);
        void openFileAsSecond(QString name);
        void nextTab()            { setCurrentIndex((currentIndex() + 1) % count()); }
        bool closeTab(int);

    public:
        FilesBar(QWidget *parent = 0);

        void addFile(AstroFile* file);
        void setAskToSave(bool b) { askToSave = b; }
        const AstroFileList& currentFiles()  { return files[currentIndex()]; }
        int count()               { return files.count(); }
};


/* =========================== ASTRO FILE DATABASE ================================== */

class AstroDatabase : public QFrame
{
    Q_OBJECT

    private:
        QListWidget* fileList;
        QLineEdit* search;

    protected:
        virtual void keyPressEvent(QKeyEvent*);
        virtual bool eventFilter(QObject *, QEvent *);

    private slots:
        void showContextMenu(QPoint);
        void openSelected();
        void openSelectedInNewTab();
        void openSelectedAsSecond();
        void deleteSelected();
        void searchFilter(QString);

    public slots:
        void updateList();

    signals:
        void fileRemoved(QString);
        void openFile(QString);
        void openFileInNewTab(QString);
        void openFileAsSecond(QString);

    public:
        AstroDatabase(QWidget *parent = 0);
};


/* =========================== MAIN WINDOW ========================================== */

class MainWindow : public QMainWindow, public Customizable
{
    Q_OBJECT

    private:
        bool askToSave;

        FilesBar*      filesBar;
        AstroWidget*   astroWidget;
        AstroDatabase* astroDatabase;
        HelpWidget*    help;
        QDockWidget*   databaseDockWidget;
        QToolBar*      toolBar;
        QToolBar*      toolBar2;
        QAction*       databaseToggle, *helpToggle;

        void addToolBarActions();

    private slots:
        void saveFile()             { astroWidget->currentFile()->save(); astroDatabase->updateList(); }
        void currentTabChanged();
        void showSettingsEditor()   { openSettingsEditor(); }
        void showAbout();
        void gotoUrl(QString url = "");

    protected:
        AppSettings defaultSettings ();      // 'Customizable' class implementations
        AppSettings currentSettings ();
        void applySettings        ( const AppSettings& );
        void setupSettingsEditor  ( AppSettingsEditor* );

        void closeEvent ( QCloseEvent* );

    public:
        MainWindow(QWidget *parent = 0);

};

#endif // MAINWINDOW_H
