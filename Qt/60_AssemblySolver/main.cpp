#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCloseEvent>
#include <QMainWindow>
#include <QToolBar>
#include <QDockWidget>
#include <QSettings>
#include <QTextEdit>
#include <QToolButton>

#include <vsn_application.h>

#include "assemblywidget.h"

#include <last.h>

static QSize sizeIcons = QSize(32, 32);

//----------------------------------------------------------------------------
// 
// ---
static void createCommandActions(QWidget* pOpenScene, QToolBar* commandBar, QWidget* parent)
{
    QAction* pOpenAction = new QAction(QIcon(":/res/open_fromfile_32.png"), QObject::tr("&Open"), parent);
    pOpenAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Open));
    parent->addAction(pOpenAction);
    commandBar->addAction(pOpenAction);
    QObject::connect(pOpenAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pSaveAction = new QAction(QIcon(":/res/save_file32.png"), QObject::tr("&Save"), parent);
    pSaveAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Save));
    parent->addAction(pSaveAction);
    commandBar->addAction(pSaveAction);
    QObject::connect(pSaveAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    commandBar->addSeparator();

    QAction* pSelectAction = new QAction(QIcon(":/res/proc_select_32.png"), QObject::tr("&Select"), parent);
    pSelectAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Select));
    pSelectAction->setPriority(QAction::LowPriority);
    pSelectAction->setShortcut(QKeySequence::Cancel);
    parent->addAction(pSelectAction);
    commandBar->addAction(pSelectAction);
    pSelectAction->setCheckable(true);
    QObject::connect(pSelectAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));
    
    QAction* pOkAction = new QAction(QIcon(":/res/proc_ok_32.png"), QObject::tr("&Comfirm"), parent);
    pOkAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Comfirm));
    parent->addAction(pOkAction);
    commandBar->addAction(pOkAction);
    QObject::connect(pOkAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    commandBar->addSeparator();

    QAction* pInsertAction = new QAction(QIcon(":/res/proc_insert_32.png"), QObject::tr("&Insert"), parent);
    pInsertAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::InsertItem));
    parent->addAction(pInsertAction);
    commandBar->addAction(pInsertAction);
    pInsertAction->setCheckable(true);
    QObject::connect(pInsertAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    commandBar->addSeparator();

    QAction* pFixingAction = new QAction(QIcon(":/res/proc_fix_32.png"), QObject::tr("&Fix component"), parent);
    pFixingAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::FixItem));
    parent->addAction(pFixingAction);
    commandBar->addAction(pFixingAction);
    pFixingAction->setCheckable(true);
    QObject::connect(pFixingAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pCoincidenceAction = new QAction(QIcon(":/res/proc_coin_32.png"), QObject::tr("&Coincident"), parent);
    pCoincidenceAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Coincident));
    parent->addAction(pCoincidenceAction);
    commandBar->addAction(pCoincidenceAction);
    pCoincidenceAction->setCheckable(true);
    QObject::connect(pCoincidenceAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pCoaxialAction = new QAction(QIcon(":/res/proc_coax_32.png"), QObject::tr("&Coaxial"), parent);
    pCoaxialAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Coaxial));
    parent->addAction(pCoaxialAction);
    commandBar->addAction(pCoaxialAction);
    pCoaxialAction->setCheckable(true);
    QObject::connect(pCoaxialAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pParallelAction = new QAction(QIcon(":/res/proc_parll_32.png"), QObject::tr("&Parallel"), parent);
    pParallelAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Parallel));
    parent->addAction(pParallelAction);
    commandBar->addAction(pParallelAction);
    pParallelAction->setCheckable(true);
    QObject::connect(pParallelAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pPerpendicularAction = new QAction(QIcon(":/res/proc_perp_32.png"), QObject::tr("&Perpendicular"), parent);
    pPerpendicularAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Perpendicular));
    parent->addAction(pPerpendicularAction);
    commandBar->addAction(pPerpendicularAction);
    pPerpendicularAction->setCheckable(true);
    QObject::connect(pPerpendicularAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pDistanceAction = new QAction(QIcon(":/res/proc_dis_32.png"), QObject::tr("&Distance dimension"), parent);
    pDistanceAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Distance));
    parent->addAction(pDistanceAction);
    commandBar->addAction(pDistanceAction);
    pDistanceAction->setCheckable(true);
    QObject::connect(pDistanceAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    QAction* pAngleAction = new QAction(QIcon(":/res/proc_ang_32.png"), QObject::tr("&Angular dimensiom"), parent);
    pAngleAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Angular));
    parent->addAction(pAngleAction);
    commandBar->addAction(pAngleAction);
    pAngleAction->setCheckable(true);
    QObject::connect(pAngleAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    commandBar->addSeparator();

    QAction* pRotationAction = new QAction(QIcon(":/res/proc_rotate_32.png"), QObject::tr("&Rotation about axis"), parent);
    pRotationAction->setProperty("Commands", QVariant((int)AssmSolverSceneWidget::Rotation));
    parent->addAction(pRotationAction);
    commandBar->addAction(pRotationAction);
    pRotationAction->setCheckable(true);
    QObject::connect(pRotationAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));
}

// ---
class MainWindow : public QMainWindow
{
public:
    MainWindow() : QMainWindow() { readSettings(); }
protected:
    void readSettings()
    {
        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        QByteArray geo_data = settings.value("geometry", QByteArray()).toByteArray();
        bool result = restoreGeometry(geo_data);
        if (!result)
        {
            const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
            resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
            move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
        }

        QByteArray layout_data = settings.value("layout", QByteArray()).toByteArray();
        restoreState(layout_data);
    }
    void writeSettings()
    {
        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        QByteArray geo_data = saveGeometry();
        settings.setValue("geometry", geo_data);

        QByteArray layout_data = saveState();
        settings.setValue("layout", layout_data);
    }
protected:
    virtual void closeEvent(QCloseEvent* event) override
    {
        writeSettings();
        event->accept();
    }
private:
    Q_DISABLE_COPY(MainWindow)
};

// ---
int main(int argc, char** argv)
{
#if defined(WIN32) /*&& defined(HUNT_FOR_MEM_LEAKS)*/
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    Math::SetMultithreadedMode(mtm_Off);
    Application vapp;
    QApplication app(argc, argv);
    app.setApplicationName("Assembly Solver");
    app.setOrganizationName("C3DLabs");

    if (!QtVision::activateLicense())
        return 0;

    AssmSolverSceneWidget* pOpenScene = new AssmSolverSceneWidget();
    pOpenScene->setMinimumSize(QSize(200, 100));
    pOpenScene->createHeadToolbar();

    MainWindow mainWindow;
    mainWindow.setWindowTitle(QStringLiteral("Assembly Solver"));
    mainWindow.setCentralWidget(pOpenScene);
    // Create commandBar
    QToolBar* commandBar = new QToolBar();
    commandBar->setBaseSize(sizeIcons);
    ::createCommandActions(pOpenScene, commandBar, &mainWindow);
    mainWindow.addToolBar(Qt::LeftToolBarArea, commandBar);
#if VSN_LIST_CONSTRAINTS
    QDockWidget* pDockConstraints = new QDockWidget(QObject::tr("List Constraints"));
    pDockConstraints->setFeatures(QDockWidget::NoDockWidgetFeatures);
    pDockConstraints->setWidget(pOpenScene->listConstraints());
    mainWindow.addDockWidget(Qt::RightDockWidgetArea, pDockConstraints);
#endif
    QDockWidget* pDockOutputJournal = new QDockWidget(QObject::tr("Journal Output"));
    pDockOutputJournal->setFeatures(QDockWidget::NoDockWidgetFeatures);
    pDockOutputJournal->setWidget(pOpenScene->textEdit());
    mainWindow.addDockWidget(Qt::RightDockWidgetArea, pDockOutputJournal);

    // Show window
//    QtVision::setWindowPosition(mainWindow);
    mainWindow.show();
    return app.exec();
}
