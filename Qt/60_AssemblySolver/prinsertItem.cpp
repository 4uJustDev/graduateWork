#include "prinsertItem.h"
#include <QFileDialog>

#include <conv_model_exchange.h>
#include <conv_exchange_settings.h>

#include <vsn_transform.h>
#include <vsn_planemarker.h>

#include <last.h>

//-----------------------------------------------------------------------------
// Recursive function counting 3d-solid bodies of the assembly sub-structure.
// ---
static size_t SolidCounter(const MbItem * item, size_t limCount)
{
    if (item == nullptr)
      return 0;
    switch(item->IsA())
    { 
    case st_Solid:
        return 1;
    case st_Instance:
        return SolidCounter(static_cast<const MbInstance*>(item)->GetItem(), limCount);
    case st_Assembly:
        size_t solidNb = 0; // Counter of solid sub-structures.
        const MbAssembly * assmItem = static_cast<const MbAssembly*>(item);
        for (size_t i = 0, iCount = assmItem->ItemsCount(); i < iCount && solidNb<=limCount; ++i)
        {
            if (const MbItem * subItem = assmItem->GetItem(i))
            {
                solidNb += SolidCounter(static_cast<const MbAssembly*>(subItem), limCount);
            }
        }
        return solidNb;
    }
      
    return 0;
}

//-----------------------------------------------------------------------------
// 
// ---
PrInsertItem::PrInsertItem(const c3d::path_string& fileName, AssmSolverSceneWidget* pSceneWidget, bool& bRuning)
    : PrBase(pSceneWidget)
    , m_pLocalModel(nullptr)
    , m_pLocalSeg(nullptr)
    , m_pTransformWidget(nullptr)
    , m_pOriginProcess(CreateOriginProcess())
    , m_pMoveProcessX(CreateCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 1, 0, 0 }))))
    , m_pMoveProcessY(CreateCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 0, 1, 0 }))))
    , m_pMoveProcessZ(CreateCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 0, 0, 1 }))))
{    
    SetCustomKey(NodeKey(AssmSolverSceneWidget::InsertItem));
    // stop searching for objects
    SetEnableFindProcess(false);

    // load local model
    m_pLocalModel = LoadFile(fileName);

    SceneSegment* pTopSegment = GetSceneWidget()->sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    SceneGenerator sceneGenerator;
    m_pLocalSeg = sceneGenerator.CreateSceneSegment(m_pLocalModel, nullptr, false, false);
    sceneGenerator.StartBuildGeometry(CommandType::Synchronous);
    pTopSegment->AddSegment(m_pLocalSeg);
    QApplication::restoreOverrideCursor();

    m_pTransformWidget = new SceneWidget(GetSceneWidget()->graphicsScene(), CreateBaseModel());
    Object::Connect(m_pTransformWidget, &SceneWidget::OnModified, this, &PrInsertItem::slotMoveObject);

    // assign processes for the axis
    m_pTransformWidget->AssignProcess(0, m_pOriginProcess);
    m_pTransformWidget->AssignProcess(1, m_pMoveProcessX);
    m_pTransformWidget->AssignProcess(2, m_pMoveProcessY);
    m_pTransformWidget->AssignProcess(3, m_pMoveProcessZ);
    // Zoom To Fit
//    pSceneWidget->viewport()->ZoomToFit(pSceneWidget->sceneContent()->GetBoundingBox());

    Object::Connect(this, &PrBase::signalReadyChanged,
        GetSceneWidget(), &AssmSolverSceneWidget::slotUpdateCommands);

    bRuning = m_pLocalModel != nullptr;
    SetReady(bRuning);
}

//-----------------------------------------------------------------------------
// 
// ---
PrInsertItem::~PrInsertItem()
{
    SetEnableFindProcess(true);
    VSN_DELETE_AND_NULL(m_pTransformWidget);
    VSN_DELETE_AND_NULL(m_pOriginProcess);
    VSN_DELETE_AND_NULL(m_pMoveProcessX);
    VSN_DELETE_AND_NULL(m_pMoveProcessY);
    VSN_DELETE_AND_NULL(m_pMoveProcessZ);
}

//-----------------------------------------------------------------------------
// 
// ---
void PrInsertItem::AddItemFromModel(const MbItem* pItem, const MbMatrix3D& mx)
{
    MbAssembly * pRootAssembly = RootAssembly();

    if (pItem == nullptr || pRootAssembly == nullptr)
        return;
    
    switch (pItem->IsA())
    {
        case st_Solid:
            {
                MbItem* pSolid = (MbItem*)pItem;
                pRootAssembly->AddInstance(*pSolid, mx);
            }
            return;

        case st_Instance:
            {   
                MbInstance* pInstance = (MbInstance*)pItem;
                MbPlacement3D lcs; 
                pInstance->GetPlacement(lcs);
                pRootAssembly->AddInstance(*pInstance->SetItem(), lcs.Transform(mx));
            }
            return;

        case st_Assembly:
            switch(SolidCounter(pItem,2)) 
            {
                case 0: // Ignore an assembly that has no solid.
                    break;
                case 1: // The simple case of single solid assembly.
                    {
                        const MbAssembly* pAssembly = (const MbAssembly*)pItem;
                        for (size_t i = 0, iCount = pAssembly->ItemsCount(); i < iCount; ++i)
                            AddItemFromModel(pAssembly->GetItem(i), mx); 
                    }
                    break;
                default: // The general case interpreted as an assembly rigid substructure. Insert it whole.
                    MbItem* pNewItem = (MbItem*)pItem;
                    pRootAssembly->AddInstance(*pNewItem, mx);
                    break;
            }
            return;
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void PrInsertItem::CreateObject()
{
    PrBase::CreateObject();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    Features::Transform& trans = m_pLocalSeg->GetTransform();
    MbMatrix3D mx = trans.GetMatrix();

    MbModel::ItemConstIterator drawIter(m_pLocalModel->CBegin());
    MbModel::ItemConstIterator drawEnd(m_pLocalModel->CEnd());
    for (; drawIter != drawEnd; ++drawIter)
        AddItemFromModel(*drawIter, mx);

    SceneSegment* pTopSegment = GetSceneWidget()->sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    pTopSegment->RemoveSegment(m_pLocalSeg);
    VSN_DELETE_AND_NULL(m_pLocalSeg);
    m_pLocalModel.reset();
    QApplication::restoreOverrideCursor();
    
    GetSceneWidget()->rebuildScene();
}

//-----------------------------------------------------------------------------
// 
// ---
void PrInsertItem::CancelObject()
{
    if (m_pLocalSeg == nullptr)
        return;
    SceneSegment* pTopSegment = GetSceneWidget()->sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    pTopSegment->RemoveSegment(m_pLocalSeg);
    VSN_DELETE_AND_NULL(m_pLocalSeg);
    m_pLocalModel.reset();
    PrBase::CancelObject();
}

//-----------------------------------------------------------------------------
// 
// ---
MbModel* PrInsertItem::LoadFile(const c3d::path_string& fileName) const
{
    if (fileName.empty())
        return nullptr;

    MbModel* pModel = new MbModel;
    ConvConvertorProperty3D convProperties;
    convProperties.enableAutostitch = true;
    convProperties.autostitchPrecision = Math::visualSag;
    convProperties.fileName = fileName;
    if (c3d::ImportFromFile(*pModel, fileName, &convProperties, nullptr) == cnv_Success)
    {
        UniteToMainAssembly(pModel);
        return pModel;
    }
    VSN_DELETE_AND_NULL(pModel);
    return pModel;
}

//-----------------------------------------------------------------------------
// 
// ---
std::shared_ptr<Widget3DModel> PrInsertItem::CreateBaseModel()
{
    AssmSolverSceneWidget* pSceneWidget = GetSceneWidget();
    Q_ASSERT(pSceneWidget != nullptr);
    QFont font = pSceneWidget->font();
    QFontMetrics fm(font);
    static float height = static_cast<float>(fm.boundingRect(QChar('X')).height());

    const float l = height * 7.f;
    const float ln = l * 1.4f;
    const float rad = height / 2.5f;
    const float radCone = height / 1.5f;

    std::shared_ptr<Widget3DModel> model(new Widget3DModel());
    model->AddMarker(-1, { ln + 5, 0, 0 }, MarkerID::labelX);
    model->AddMarker(-1, { 0, ln + 5, 0 }, MarkerID::labelY);
    model->AddMarker(-1, { 0, 0, ln + 5 }, MarkerID::labelZ);

    // add Sphere to 1 state
    model->AddSphere(DefaultItems::Origin, {}, radCone * 2.f, Color(125, 125, 0), 1);
    // add axis x
    model->AddCylinder(DefaultItems::wRight, {}, { l, 0, 0 }, rad, Color(200, 0, 0), 2);
    model->AddCone(DefaultItems::wRight, { l,0,0 }, { ln, 0, 0 }, radCone, Color(200, 0, 0), 2);
    // add axis y
    model->AddCylinder(DefaultItems::wTop, {}, { 0, l, 0 }, rad, Color(0, 200, 0), 3);
    model->AddCone(DefaultItems::wTop, { 0,l,0 }, { 0, ln, 0 }, radCone, Color(0, 200, 0), 3);
    // add axis z
    model->AddCylinder(DefaultItems::wFront, {}, { 0, 0, l }, rad, Color(0, 0, 200), 4);
    model->AddCone(DefaultItems::wFront, { 0, 0, l }, { 0, 0, ln }, radCone, Color(0, 0, 200), 4);

    model->InsertState(1, 0);
    model->InsertState(2, 0);
    model->InsertState(3, 0);
    model->InsertState(4, 0);

    return model;
}

//---------------------------------------------------------------------------
//
// ---
WidgetProcess* PrInsertItem::CreateCurveMoveProcess(SPtr<const MbCurve3D> curve, bool local)
{
    MoveWidgetProcess* process = new MoveWidgetProcess(nullptr);
    process->SetLocator(new CurveScreenCoordLocator(curve, process));
    process->SetConvertPlacement(local ? WidgetConvertPlacements::Widget : WidgetConvertPlacements::Scene);
    return process;
}

//-----------------------------------------------------------------------------
// 
// ---
WidgetProcess* PrInsertItem::CreateOriginProcess()
{
    MoveWidgetProcess* process = new MoveWidgetProcess(nullptr);
    process->SetLocator(new ModelCoordLocator());
    process->SetConvertPlacement(WidgetConvertPlacements::Widget);
    return process;
}

//-----------------------------------------------------------------------------
// 
// ---
void PrInsertItem::slotMoveObject(SceneWidget* widget)
{
    if (m_pLocalSeg != nullptr)
    {
        VSN::Features::Transform* transform = nullptr;
        for (auto feature : m_pLocalSeg->GetFeatures())
        {
            if (transform = vobject_cast<VSN::Features::Transform*>(feature)) 
                break;
        }

        if (!transform)
        {
            transform = new VSN::Features::Transform();
            m_pLocalSeg->AddFeature(transform);
        }

        Matrix3DF mx = widget->GetTransform();
        transform->SetMatrix(mx);
        m_pTransformWidget->Init(transform->GetMatrix().GetOrigin());
    }
}