#ifndef __VSN_ASSMSOLVERSCENEMODIFIER_H
#define __VSN_ASSMSOLVERSCENEMODIFIER_H

#include <QAction>
#include <QTextEdit>
#include <QTableWidget>
#include <qt_openglwidget.h>

#include <model.h>
#include <model_item.h>

VSN_USE_NAMESPACE

#define VSN_LIST_CONSTRAINTS 1

class PrBase;
class AssemblySolver;
class Reporter;
class ConstraintsManager;

using AssemblySolverPtr = std::shared_ptr<AssemblySolver>;
using AssemblySolverConstPtr = std::shared_ptr<const AssemblySolver>;

///////////////////////////////////////////////////////////////////////////////
// 
// The main widget of the app
// 
///////////////////////////////////////////////////////////////////////////////
class AssmSolverSceneWidget : public QtVision::QtOpenGLSceneWidget
{
    Q_OBJECT
    VSN_OBJECT(AssmSolverSceneWidget)
public:
    explicit AssmSolverSceneWidget(QWidget* parent = nullptr);
    virtual ~AssmSolverSceneWidget();
public:    
    enum Commands
    {
        None,
        Open,
        Save,
        Select,
        Cancel,
        Comfirm,
        // Input processes.
        InsertItem,
        FixItem,
        Coincident,
        Coaxial,
        Parallel,
        Perpendicular,
        Distance,
        Angular,
        Rotation,
    };
public:
    QTextEdit* textEdit();
#if VSN_LIST_CONSTRAINTS
    QTableWidget* listConstraints();
#endif
    SPtr<MbModel> model() { return m_pModel; }
    SceneSegment* modelSegment() const { return m_pModelSeg; }
    SceneSegment* assemblySegment() const; // Get the main assembly node.
    AssemblySolverConstPtr solver() const { return m_pSolver; }  
    AssemblySolverPtr solver() { return m_pSolver; }
    SelectionManagerPtr selectManager() const { return m_ptrSelectManager; }
    void  rebuildScene();
public Q_SLOTS:
    QString openSelectModel();
    void viewCommands(Commands cmd = None);
protected:
    void loadModel();
    void saveModel();
public:
    VSN_SLOT(Public, slotUpdateCommands, void slotUpdateCommands());
private:
    virtual void initializeGL();
private:
    QTextEdit* m_pTextEdit;       // The text editor to output the log messages.
#if VSN_LIST_CONSTRAINTS
    QTableWidget* m_pListConstraints;
#endif
    Reporter* m_pReporter;
    ConstraintsManager* m_pConsManager;
    QString m_fileName;
    SPtr<MbModel> m_pModel;
    AssemblySolverPtr m_pSolver;  // Geometric constraint solver providing assembly model constraints and dimensions.
    PrBase* m_pCurrentProcess;
    SelectionManagerPtr m_ptrSelectManager;
    SceneSegment* m_pModelSeg;    // The root segment of the model. This node corresponds to m_pModel.
private:
    Q_DISABLE_COPY(AssmSolverSceneWidget)
};

#endif // __VSN_ASSMSOLVERSCENEMODIFIER_H
