#ifndef __VSN_PRINSERTITEM_H
#define __VSN_PRINSERTITEM_H

#include <tool_cstring.h>
#include <scenegenerator.h>
#include "prbase.h"

VSN_USE_NAMESPACE

/* DefaultProcesses */
class DefaultProcesses
{
public:
    static const std::shared_ptr<WidgetProcess> spaceMove;
    static const std::shared_ptr<WidgetProcess> xlocalMove;
    static const std::shared_ptr<WidgetProcess> ylocalMove;
    static const std::shared_ptr<WidgetProcess> zlocalMove;
    static const std::shared_ptr<WidgetProcess> xylocalMove;
};

enum DefaultItems
{
    Origin = 0,
    wRight,
    wTop,
    wFront,
    wLeft,
    wBottom,
    wRear,
    TorusX,
    TorusY,
    TorusZ,
};

///////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////
class PrInsertItem : public PrBase
{
    VSN_OBJECT(PrInsertItem)
public:
    explicit PrInsertItem(const c3d::path_string& fileName, AssmSolverSceneWidget* pSceneWidget, bool& bRuning);
    virtual ~PrInsertItem();
public:
    virtual void CreateObject() override;
    virtual void CancelObject() override;
protected:
    void AddItemFromModel(const MbItem* pItem, const MbMatrix3D& mx);
    MbModel* LoadFile(const c3d::path_string& fileName) const;
    std::shared_ptr<Widget3DModel> CreateBaseModel();
    WidgetProcess* CreateCurveMoveProcess(SPtr<const MbCurve3D> curve, bool local = true);
    WidgetProcess* CreateOriginProcess();
protected:
    VSN_SLOT(Protected, slotMoveObject, void slotMoveObject(SceneWidget*));
private:
    SPtr<MbModel> m_pLocalModel;
    SceneSegment* m_pLocalSeg;
    SceneWidget* m_pTransformWidget;
    WidgetProcess* m_pOriginProcess;
    WidgetProcess* m_pMoveProcessX;
    WidgetProcess* m_pMoveProcessY;
    WidgetProcess* m_pMoveProcessZ;
private:
    VSN_DISABLE_COPY(PrInsertItem)
};

#endif // __VSN_PRINSERTITEM_H
