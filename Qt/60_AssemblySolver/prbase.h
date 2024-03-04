#ifndef __VSN_PRBASE_H
#define __VSN_PRBASE_H

#include <vsn_node.h>
#include "assemblywidget.h"

VSN_USE_NAMESPACE


///////////////////////////////////////////////////////////////////////////////
// 
// Base class for input processes, adding and editing model objects.
// 
///////////////////////////////////////////////////////////////////////////////
class PrBase : public VSN::Node
{
    VSN_OBJECT(PrBase)
public:
    explicit PrBase(AssmSolverSceneWidget* pSceneWidget);
    virtual ~PrBase();
public:
    inline bool IsReady() const { return m_bReady; }
    void SetReady(bool bReady);
public:
    virtual void CreateObject();
    virtual void CancelObject();
public:
    VSN_SIGNAL(Public, signalReadyChanged, void signalReadyChanged())
protected:
    void SetEnableFindProcess(bool bEnabled);
    AssmSolverSceneWidget* GetSceneWidget() const { return m_pSceneWidget; }
    MbAssembly* RootAssembly() const;
    AssemblySolver& GetSolver();
    void UpdateAssemblyScene();
private:
    bool m_bReady;
    AssmSolverSceneWidget* m_pSceneWidget;
private:
    VSN_DISABLE_COPY(PrBase);
};

//-----------------------------------------------------------------------------
// Start the fix item procces.
// ---
PrBase * StartFixingProcess(AssmSolverSceneWidget* pSceneWidget);

#endif // __VSN_PRBASE_H
