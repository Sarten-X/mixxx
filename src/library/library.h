// library.h
// Created 8/23/2009 by RJ Ryan (rryan@mit.edu)

// A Library class is a container for all the model-side aspects of the library.
// A library widget can be attached to the Library object by calling bindWidget.

#ifndef LIBRARY_H
#define LIBRARY_H

#include <QList>
#include <QObject>
#include <QAbstractItemModel>
#include <QFont>

#include "preferences/usersettings.h"
#include "track/track.h"
#include "recording/recordingmanager.h"
#include "analysisfeature.h"
#include "library/coverartcache.h"
#include "library/setlogfeature.h"
#include "library/scanner/libraryscanner.h"
#include "library/librarypanemanager.h"

#include "widget/wtracktableview.h"
#include "widget/wlibrary.h"

class TrackModel;
class TrackCollection;
class SidebarModel;
class LibraryFeature;
class LibraryTableModel;
class WLibrarySidebar;
class WLibrary;
class WSearchLineEdit;
class MixxxLibraryFeature;
class PlaylistFeature;
class CrateFeature;
class LibraryControl;
class KeyboardEventFilter;
class PlayerManagerInterface;

class Library : public QObject {
    Q_OBJECT
public:
    Library(QObject* parent,
            UserSettingsPointer pConfig,
            PlayerManagerInterface* pPlayerManager,
            RecordingManager* pRecordingManager);
    virtual ~Library();

    void bindLibraryWidget(WLibrary* libraryWidget,
                           KeyboardEventFilter* pKeyboard, int id);
    void bindSearchBar(WSearchLineEdit* searchLine, int id);
    void bindSidebarExpanded(WLibrary* leftPane, 
                             KeyboardEventFilter* pKeyboard);
    //void bindSidebarWidget(WLibrarySidebar* sidebarWidget);
    void bindSidebarWidget(WButtonBar* sidebar);

    void addFeature(LibraryFeature* feature);
    QStringList getDirs();

    // TODO(rryan) Transitionary only -- the only reason this is here is so the
    // waveform widgets can signal to a player to load a track. This can be
    // fixed by moving the waveform renderers inside player and connecting the
    // signals directly.
    TrackCollection* getTrackCollection() {
        return m_pTrackCollection;
    }

    inline int getTrackTableRowHeight() const {
        return m_iTrackTableRowHeight;
    }

    inline const QFont& getTrackTableFont() const {
        return m_trackTableFont;
    }

    //static Library* buildDefaultLibrary();

    enum RemovalType {
        LeaveTracksUnchanged = 0,
        HideTracks,
        PurgeTracks
    };

    static const int kDefaultRowHeightPx;

  public slots:
    void slotShowTrackModel(QAbstractItemModel* model);
    void slotSwitchToView(const QString& view);
    void slotLoadTrack(TrackPointer pTrack);
    void slotLoadTrackToPlayer(TrackPointer pTrack, QString group, bool play);
    void slotLoadLocationToPlayer(QString location, QString group);
    void slotRestoreSearch(const QString& text);
    void slotRefreshLibraryModels();
    void slotCreatePlaylist();
    void slotCreateCrate();
    void slotRequestAddDir(QString directory);
    void slotRequestRemoveDir(QString directory, Library::RemovalType removalType);
    void slotRequestRelocateDir(QString previousDirectory, QString newDirectory);
    void onSkinLoadFinished();
    void slotSetTrackTableFont(const QFont& font);
    void slotSetTrackTableRowHeight(int rowHeight);
    void slotPaneFocused();

    void scan() {
        m_scanner.scan();
    }

  signals:
    //void showTrackModel(QAbstractItemModel* model);
    //void switchToView(const QString& view);
    void loadTrack(TrackPointer pTrack);
    void loadTrackToPlayer(TrackPointer pTrack, QString group, bool play = false);
    //void restoreSearch(const QString&);
    //void search(const QString& text);
    //void searchCleared();
    //void searchStarting();
    // emit this signal to enable/disable the cover art widget
    void enableCoverArtDisplay(bool);
    void trackSelected(TrackPointer pTrack);

    void setTrackTableFont(const QFont& font);
    void setTrackTableRowHeight(int rowHeight);

    // Emitted when a library scan starts and finishes.
    void scanStarted();
    void scanFinished();

  private:
    
    void createPane(int id);
    
    LibraryPaneManager* getFocusedPane();
    
    UserSettingsPointer m_pConfig;
    SidebarModel* m_pSidebarModel;
    TrackCollection* m_pTrackCollection;
    const static QString m_sTrackViewName;
    const static QString m_sAutoDJViewName;
    MixxxLibraryFeature* m_pMixxxLibraryFeature;
    PlaylistFeature* m_pPlaylistFeature;
    CrateFeature* m_pCrateFeature;
    AnalysisFeature* m_pAnalysisFeature;
    LibraryControl* m_pLibraryControl;
    RecordingManager* m_pRecordingManager;
    LibraryScanner m_scanner;
    QFont m_trackTableFont;
    int m_iTrackTableRowHeight;
    
    QHash<int, LibraryPaneManager*> m_panes;
    LibraryPaneManager* m_pSidebarExpanded;
    QList<LibraryFeature*> m_features;
    
    /*// Panes widgets
    QList<WTrackTableView*> m_trackTables;
    //QList<WLibrary*> m_panes;
    QList<WSearchLineEdit*> m_searches;*/
    
    
    // -1 for the Sidebar Expanded and >= 0 for the other widgets
    int m_focusedPane;
    
    void createFeatures(UserSettingsPointer pConfig, PlayerManagerInterface *pPlayerManager);
};

#endif /* LIBRARY_H */
