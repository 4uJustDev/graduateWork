#include "prrotation.h"

#include <mb_class_traits.h>
#include "assemblysolver.h"
#include <vsn_planemarker.h>
#include <vsn_transform.h>
#include <scenegenerator.h>

#include <last.h>

//---------------------------------------------------------------------------------------
// Get selected axial geometry that is a subject of the rotation process.
// ---
AsMatingGeom PrRotationAboutAxis::SelectedAxialGeom(AssmSolverSceneWidget* pSceneWidget, SceneSegment** scSeg)
{
    const MbAssembly * pRootAssm = GetMainAssembly(pSceneWidget->model()); // The main assembly.
    if (pRootAssm==nullptr)
        return AsMatingGeom();
    
    auto lstItem = pSceneWidget->selectManager()->GetSelectionList();
    for (auto it = lstItem.begin(); it != lstItem.end(); ++it)
    {
        if (InstSelectionItem* pSelitem = dynamic_cast<InstSelectionItem*>(*it))
        {            
            MbPath path = pSelitem->GetMathPath();
            if (!path.empty() && (pRootAssm->GetItemName() == path.front()) )
            {   
                path.RemoveInd(0);
                // It's an assembly component repositioned by the solver.
                const MbItem* component = pRootAssm->SubItem(path.front());
                // Mating geometry is specified by the component and its geometric (topoligic) element, face, edge, vertex etc.
                if (component != nullptr)
                {                    
                    AsMatingGeom matedGeom(*component, path, pSelitem->GetPrimitiveItem());                    
                    switch(matedGeom.SubGeomRecord().type)
                    {
                        case GCM_LINE:
                        case GCM_CYLINDER:
                        case GCM_CONE:
                        case GCM_TORUS:
                        case GCM_CIRCLE:
                        case GCM_AXIS:
                            if (scSeg != nullptr)
                                *scSeg = const_cast<SceneSegment*>(pSelitem->GetSceneSegment());
                            return matedGeom; // It allows only axial geometry type.
                    }
                }
            }
        }
    }  

    return AsMatingGeom();
}

//---------------------------------------------------------------------------------------
// 
// ---
PrRotationAboutAxis::PrRotationAboutAxis(AssmSolverSceneWidget::Commands cmd, 
                           AssmSolverSceneWidget* pSceneWidget, bool& bRuning)
    : PrBase(pSceneWidget)
    , m_pCurrentComp(nullptr)
    //, m_pCurrentSeg(nullptr)
    , m_pTransformWidget(nullptr)
    , m_pRotationProc(CreateRotateProcess(MbAxis3D::zAxis))
    , m_stRotatorMat()
{
    SetCustomKey(NodeKey(cmd));
    m_pTransformWidget = new SceneWidget(GetSceneWidget()->graphicsScene(), CreateBaseModel());
    Object::Connect(m_pTransformWidget, &SceneWidget::OnModified, this, &PrRotationAboutAxis::slotTransformObject);

    // Assign processes for the rotational ring.
    m_pTransformWidget->AssignProcess(1, m_pRotationProc);
    m_pTransformWidget->SetVisible(false);

    Object::Connect(m_pTransformWidget, &SceneWidget::OnStartProcess,
        this, &PrRotationAboutAxis::slotStartProcessWidget);
    Object::Connect(m_pTransformWidget, &SceneWidget::OnStopProcess,
        this, &PrRotationAboutAxis::slotStopProcessWidget);
    Object::Connect(this, &PrBase::signalReadyChanged, 
        GetSceneWidget(), &AssmSolverSceneWidget::slotUpdateCommands);

    UpdateSelected();

    bRuning = true;
}

//---------------------------------------------------------------------------------------
// 
// ---
PrRotationAboutAxis::~PrRotationAboutAxis()
{
    VSN_DELETE_AND_NULL(m_pTransformWidget);
    VSN_DELETE_AND_NULL(m_pRotationProc);
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::slotTransformObject(SceneWidget* pWidget)
{ 
     SolveReposition(pWidget->GetTransform());
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::slotStartProcessWidget(SceneWidget* pWidget)
{
    VSN_UNUSED(pWidget);
    SetEnableFindProcess(false);
    if (pWidget == m_pTransformWidget)
        m_stRotatorMat.Init(m_pTransformWidget->GetTransform());
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::slotStopProcessWidget(SceneWidget* pWidget)
{
    VSN_UNUSED(pWidget);
    // the left mouse button up.
    SetEnableFindProcess(true);
    
    if (pWidget == m_pTransformWidget)
        m_stRotatorMat = MbMatrix3D::identity;
    if (m_pCurrentComp != nullptr)
        GetSolver().DynamicRotationStop(*m_pCurrentComp);
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::UpdateSelected()
{
    AsMatingGeom axialGeom = SelectedAxialGeom(GetSceneWidget(), NULL/*& m_pCurrentSeg*/);
    
    if (axialGeom.CompItem() != nullptr)
    { 
        m_pTransformWidget->Init(axialGeom.WcsMatrix());
        m_pTransformWidget->SetVisible(true);
        m_pCurrentComp = const_cast<MbInstance*>(isa_cast<const MbInstance*>(axialGeom.CompItem()));
    }   
    else 
    { 
        m_pTransformWidget->SetVisible(false);
    }
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::CreateObject()
{
    PrBase::CreateObject();
    if (m_pCurrentComp == nullptr)
        GetSolver().DynamicRotationStop(*m_pCurrentComp);
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::CancelObject()
{
    // TODO: твой код
    PrBase::CancelObject();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrRotationAboutAxis::SolveReposition(const MbMatrix3D& rotorMat)
{    
    if (m_pCurrentComp != nullptr)
    {
        GCM_geom compId = GetSolver().QueryCompId(m_pCurrentComp);
        if (compId != GCM_NULL)
        { 
            const double angPos = atan2(rotorMat.GetAxisX()*m_stRotatorMat.GetAxisY(), rotorMat.GetAxisX() * m_stRotatorMat.GetAxisX());
            GetSolver().DynamicRotation(*m_pCurrentComp,MbAxis3D(rotorMat.GetOrigin(),rotorMat.GetAxisZ()),angPos);
            UpdateAssemblyScene();
        }
    }
}

//---------------------------------------------------------------------------------------
// 
// ---
std::shared_ptr<Widget3DModel> PrRotationAboutAxis::CreateBaseModel()
{
    AssmSolverSceneWidget* pSceneWidget = GetSceneWidget();
    Q_ASSERT(pSceneWidget != nullptr);
    QFont font = pSceneWidget->font();
    QFontMetrics fm(font);
    static float height = static_cast<float>(fm.boundingRect(QChar('X')).height());

    std::shared_ptr<Widget3DModel> model(new Widget3DModel());
    float l = height * 7.f;
    float ln = l * 1.4f;
    model->AddTorus(1, {}, { 0, 0, l }, 6.5f, Color(200, 0, 0));
    return model;
}

//---------------------------------------------------------------------------------------
// 
// ---
WidgetProcess* PrRotationAboutAxis::CreateRotateProcess(const MbAxis3D& axis)
{
    RotateWidgetProcess* process = new RotateWidgetProcess(axis);
    process->SetStep(Math::PI2 / 72.0);
    return process;
}
