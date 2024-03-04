#ifndef __VSN_PRROTATION_H
#define __VSN_PRROTATION_H

#include <tool_cstring.h>
#include <topology_item.h>

#include "prbase.h"
#include "assemblysolver.h"

VSN_USE_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////////////
// 
// Process of the component rotation about selected axial geometry.
// 
/////////////////////////////////////////////////////////////////////////////////////////
class PrRotationAboutAxis : public PrBase
{
    VSN_OBJECT(PrRotationAboutAxis)
public:
    explicit PrRotationAboutAxis(AssmSolverSceneWidget::Commands cmd, 
        AssmSolverSceneWidget* pSceneWidget, bool& bRuning);
    virtual ~PrRotationAboutAxis();
    
    // Get selected axial geometry that is a subject of the rotation process.
    static AsMatingGeom SelectedAxialGeom(AssmSolverSceneWidget* pSceneWidget, SceneSegment**);
protected:
    void slotObjectHover(const InstSelectionItem& hoverItem, bool& bHover);
    void slotTransformObject(SceneWidget* pWidget);
    void slotStartProcessWidget(SceneWidget* pWidget);
    void slotStopProcessWidget(SceneWidget* pWidget);
private:
    void UpdateSelected();
public:
    virtual void CreateObject() override;
    virtual void CancelObject() override;
protected:
    void SolveReposition(const MbMatrix3D&);
protected:
    WidgetProcess* CreateRotateProcess(const MbAxis3D& axis);
    std::shared_ptr<Widget3DModel> CreateBaseModel();
private:
    MbInstance* m_pCurrentComp;
    //SceneSegment* m_pCurrentSeg;
    SceneWidget* m_pTransformWidget;
    WidgetProcess* m_pRotationProc;
    MbMatrix3D m_stRotatorMat; // Start matrix of the rotator.
private:
    VSN_DISABLE_COPY(PrRotationAboutAxis)
};

#endif // __VSN_PRROTATION_H
