/////////////////////////////////////////////////////////////////////////////////////////
//
// Reporting the log messages into text window.
// 
/////////////////////////////////////////////////////////////////////////////////////////
#include <QMenu>
#include <gcm_routines.h>
#include "reporter.h"
#include "assemblysolver.h"
#include "assemblywidget.h"

#include <last.h>

//---------------------------------------------------------------------------------------
// Implementation of the callback that receives log messages from the assembly solver.
//---
void GCM_LoggerCallback(GCM_journal jrn, GCM_log_type logType, const char* recStr)
{    
    switch (logType)
    {
        case GCM_LOG_JournalLine:
          return static_cast<Reporter*>(jrn.extra.funcData)->JournalLine(recStr);
        case GCM_LOG_Message:
          return static_cast<Reporter*>(jrn.extra.funcData)->MessageLine(recStr);
        case GCM_LOG_Error:
          return static_cast<Reporter*>(jrn.extra.funcData)->ErrorLine(recStr);
    }
}

//---------------------------------------------------------------------------------------
// Construct the reporter.
//---
Reporter::Reporter(const AssemblySolverConstPtr & solver, QObject* parent)
  : QObject(parent)
  , m_solver(nullptr)
{
    AssignSolver(solver);
}

//---------------------------------------------------------------------------------------
//
//---
void Reporter::AssignSolver(const AssemblySolverConstPtr & solver)
{
    if (m_solver != solver)
    { 
        m_solver = solver;
        GCM_extra_param extra;
        extra.funcId = 0;
        extra.funcData = this;
        m_solver->SubscribeLogs(GCM_LoggerCallback, extra);
        emit journalChanged();
    }
}

//---------------------------------------------------------------------------------------
// Add Journal line
//---
void Reporter::JournalLine(const char* jrnStr)
{
    QString str(jrnStr);
    emit addJournalLine(str);
}

//---------------------------------------------------------------------------------------
// Add message string.
//---
void Reporter::MessageLine(const char* jrnStr)
{
}


//---------------------------------------------------------------------------------------
// Add error string.
//---
void Reporter::ErrorLine(const char* jrnStr)
{
}

#if VSN_LIST_CONSTRAINTS
/* ConstraintsWidgetItem */
ConstraintsWidgetItem::ConstraintsWidgetItem( GCM_constraint cItem, const QString& text, int type)
    : QTableWidgetItem(text, type)
    , m_cItem(cItem)
{
}

//---------------------------------------------------------------------------------------
//
//---
ConstraintsWidgetItem::~ConstraintsWidgetItem()
{
}

//---------------------------------------------------------------------------------------
/* ConstraintsEventHandler */
//---
ConstraintsEventHandler::ConstraintsEventHandler(ConstraintsManager* pConsManager)
    : MtRefItem()
    , m_pConstraintsManager(pConsManager)
    , m_pSolver(nullptr)
{
    VSN_ASSERT(pConsManager!=nullptr);
}

//---------------------------------------------------------------------------------------
// Working with the assembly solver is started.
//---
void ConstraintsEventHandler::EvSubscribed(const AssemblySolver& assmSlv)
{
    VSN_ASSERT(m_pSolver == nullptr);
    m_pSolver = &assmSlv;
    AddRef();
    m_pConstraintsManager->fillList();  // refresh the constraints list after restoring the assembly from *.c3d file.
}

//---------------------------------------------------------------------------------------
// Working with the assembly solver is finished.
//---
void ConstraintsEventHandler::EvFinish()
{
    m_pSolver = nullptr;
    Release();
}

//---------------------------------------------------------------------------------------
// The constraints list of the solver changed.
//---
void ConstraintsEventHandler::EvConstraintsChanged() const
{
    m_pConstraintsManager->fillList();
}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsEventHandler::EvGeomsChanged() const
{}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsEventHandler::EvSceneReEvaluated(const MbAssembly&, GCM_result evalRes) const
{}

//---------------------------------------------------------------------------------------
//
//---
ConstraintsManager::ConstraintsManager(const AssemblySolverPtr& pSolver, AssmSolverSceneWidget* pWidget, QObject* parent)
    : QObject(parent)
    , m_pEventHandler(nullptr)
    , m_pSceneWidget(pWidget)
    , m_pCurWidgetItem(nullptr)
{
    m_actDeleteConstraint = new QAction(QIcon(":/res/proc_cancel_32.png"), QString("&Delete"), this);
    m_actDeleteConstraint->setPriority(QAction::LowPriority);
    m_actDeleteConstraint->setShortcut(QKeySequence::Delete);
    m_actDeleteConstraint->setToolTip(tr("Delete"));
    connect(m_actDeleteConstraint, SIGNAL(triggered()), this, SLOT(deleteConstraint()));

    QTableWidget* pTableWidget = m_pSceneWidget->listConstraints();
    QFont font(QString("Consolas"), 9.5);
    pTableWidget->setFont(font);

    pTableWidget->installEventFilter(this);
    pTableWidget->setColumnCount(1);
    pTableWidget->setShowGrid(false);
    pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    pTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Name")));
    m_pEventHandler = new ConstraintsEventHandler(this);
    pSolver->Subscribe(m_pEventHandler);
    QObject::connect(pTableWidget, &QTableWidget::currentItemChanged, this, &ConstraintsManager::currentItemChanged);
}

//---------------------------------------------------------------------------------------
//
//---
ConstraintsManager::~ConstraintsManager()
{
    QTableWidget* pTableWidget = m_pSceneWidget->listConstraints();
    pTableWidget->removeEventFilter(this);
}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsManager::AssignSolver(const AssemblySolverPtr& pSolver)
{
    if (m_pEventHandler)
        pSolver->Subscribe(m_pEventHandler);
}

//---------------------------------------------------------------------------------------
//
//---
static QIcon getIconConstraint(GCM_c_type cType)
{
    switch (cType)
    {
        case GCM_COINCIDENT:     return QIcon(":/res/proc_coin_32.png");
        case GCM_CONCENTRIC:     return QIcon(":/res/proc_coax_32.png");
        case GCM_PARALLEL:       return QIcon(":/res/proc_parll_32.png");
        case GCM_PERPENDICULAR:  return QIcon(":/res/proc_perp_32.png");
        case GCM_DISTANCE:       return QIcon(":/res/proc_dis_32.png");
        case GCM_ANGLE:          return QIcon(":/res/proc_ang_32.png");
    }
    return QIcon();
}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsManager::fillList()
{
    QTableWidget* pTableWidget = m_pSceneWidget->listConstraints();
    pTableWidget->clearContents();

    QFont font = m_pSceneWidget->font();
    QFontMetrics fm(font);

    int width = fm.boundingRect(QChar('X')).width();
    int height = fm.boundingRect(QChar('X')).height();

    GeConstraintsRange cRng = m_pSceneWidget->solver()->Constraints();
    pTableWidget->setRowCount(static_cast<int>(cRng.size()));
    int row = 0;
    for ( ; !cRng.empty(); cRng.drop_front() )
    {
        GCM_c_type cType = m_pSceneWidget->solver()->ConstraintType(cRng.frontId());
        pTableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(getIconConstraint(cType), ""));

        std::string cStrName = m_pSceneWidget->solver()->StrRecord(cRng.frontId()); // a name of constraint.
        ConstraintsWidgetItem* pWidgetItem = new ConstraintsWidgetItem(cRng.frontId(), QString(cStrName.c_str()));

        QRect rect = fm.boundingRect(QString(cStrName.c_str()));
        pTableWidget->setRowHeight(row, rect.height());
        pTableWidget->setColumnWidth(0, rect.width() + (height * 4));
        pTableWidget->setItem(row, 0/*column*/, pWidgetItem);
        row++;
    }
}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsManager::deleteConstraint()
{
    if (m_pCurWidgetItem != nullptr)
    {
        GCM_constraint constraint = m_pCurWidgetItem->objectId();
        m_pSceneWidget->solver()->RemoveConstraint(constraint);
    }
}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsManager::currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous)
{
    Q_UNUSED(previous);
    m_pCurWidgetItem = nullptr;
    if (current != nullptr)
        m_pCurWidgetItem = dynamic_cast<ConstraintsWidgetItem*>(current);
}

//---------------------------------------------------------------------------------------
//
//---
void ConstraintsManager::contextMenuEvent(const QPoint& pos)
{
    QTableWidget* pTableWidget = m_pSceneWidget->listConstraints();
    QMenu menu(pTableWidget);
    menu.addAction(m_actDeleteConstraint);
    menu.exec(pos);
}

//---------------------------------------------------------------------------------------
//
//---
bool ConstraintsManager::eventFilter(QObject* watched, QEvent* event)
{
    QTableWidget* pTableWidget = m_pSceneWidget->listConstraints();
    if (watched == pTableWidget)
    {
        if (m_pCurWidgetItem != nullptr && event->type() == QEvent::ContextMenu)
        {
            QContextMenuEvent* ev = static_cast<QContextMenuEvent*>(event);
            contextMenuEvent(ev->globalPos());
        }
        return false;
    }
    return false;
}

#endif // VSN_LIST_CONSTRAINTS

// eof