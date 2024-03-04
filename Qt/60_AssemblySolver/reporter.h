#ifndef __VSN_REPORTER_H
#define __VSN_REPORTER_H

#include <qevent.h>
#include <qobject.h>
#include "assemblysolver.h"
#include "assemblywidget.h"
#include <vsn_global.h>

///////////////////////////////////////////////////////////////////////////////
// 
// This class responds to output log messages from the assembly solver.
// 
///////////////////////////////////////////////////////////////////////////////
class Reporter : public QObject
{
    Q_OBJECT
public:     
    /// Construct the reporter.
    /**  
      \param solver - Assembly Solver is an emitter of log messages.
      \param parent - The object owning the reporter.    
    */
    Reporter(const AssemblySolverConstPtr & solver, QObject* parent);

private:
    virtual ~Reporter() {}

public:
    // The call stopts the outputting messages.
    void LockOutput() {}
    // The call enables the outputting messages.
    void UnlockOutput() {}
    // Add journal string.
    void JournalLine(const char* jrnStr);
    // Add message string.
    void MessageLine(const char* jrnStr);
    // Add error string.
    void ErrorLine(const char* jrnStr);
    // Assign other solver.
    void AssignSolver(const AssemblySolverConstPtr & solver);

public Q_SLOTS:

Q_SIGNALS:
    void addJournalLine(const QString& text);    
    /*
    void addMessage(const QString& text);
    void addError(const QString& text);
    */
    // The event that the journal subscription changed.
    void journalChanged();

private:
  AssemblySolverConstPtr m_solver;
    
private:
    VSN_DISABLE_COPY(Reporter);
};

#if VSN_LIST_CONSTRAINTS
/* ConstraintsManager */
class ConstraintsWidgetItem : public QTableWidgetItem
{
public:
    ConstraintsWidgetItem(GCM_constraint cItem, const QString& text, int type = Type);
    virtual ~ConstraintsWidgetItem();
public:
    // Get a descriptor specifing a constraint inside the assembly solver.
    GCM_constraint objectId() const { return m_cItem; }
private:
    GCM_constraint m_cItem;
};

class ConstraintsManager;

///////////////////////////////////////////////////////////////////////////////
// 
// This class responds to the assembly solver proccessing.
// 
///////////////////////////////////////////////////////////////////////////////
class ConstraintsEventHandler : public MtRefItem, public AsEventHandler
{
public:
    ConstraintsEventHandler(ConstraintsManager* pConsManager);
public:
    // Working with the assembly solver is started.
    virtual void EvSubscribed(const AssemblySolver &);
    // The constraints list of the solver changed (added/removed/edited).
    virtual void EvConstraintsChanged() const;
    // The geometry list of the solver changed.
    virtual void EvGeomsChanged() const;
    // Scene that under solver control is re-evaluated. Geometry state of the assembly updated.
    virtual void EvSceneReEvaluated(const MbAssembly&, GCM_result evalRes) const;
    // Working with the assembly solver is finished.
    virtual void EvFinish();
protected:
    ConstraintsManager* m_pConstraintsManager;  ///< Constraint manager that coordinate GUI and the assembly solver (always not nullptr).
    const AssemblySolver* m_pSolver;  ///< The assembly solver which the handler work with.
private:
    VSN_DISABLE_COPY(ConstraintsEventHandler);
};

///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////

class ConstraintsManager : public QObject
{
    Q_OBJECT
public:
    ConstraintsManager(const AssemblySolverPtr& pSolver, AssmSolverSceneWidget* pWidget, QObject* parent);
    virtual ~ConstraintsManager();
public:
    void AssignSolver(const AssemblySolverPtr& pSolver);
protected:
    void fillList();
public Q_SLOTS:
    void deleteConstraint();
    void currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);
protected:
    void contextMenuEvent(const QPoint& pos);
protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);
private:
    SPtr<ConstraintsEventHandler> m_pEventHandler;
    AssmSolverSceneWidget* m_pSceneWidget;
    ConstraintsWidgetItem* m_pCurWidgetItem;
    QAction* m_actDeleteConstraint;
private:
    friend class ConstraintsEventHandler;
    VSN_DISABLE_COPY(ConstraintsManager);
};
#endif // VSN_LIST_CONSTRAINTS

#endif // __VSN_REPORTER_H

