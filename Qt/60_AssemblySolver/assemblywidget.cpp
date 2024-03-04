//////////////////////////////////////////////////////////////////////////////////////////
//
// Виджет демонстрирующий сцену со сборкой.
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "assemblywidget.h"

#include <QFileDialog>
#include <qt_aboutscenewidget.h>

#include <conv_model_exchange.h>
#include <conv_exchange_settings.h>

#include "reporter.h"
#include "assemblysolver.h"
#include "prbinconstraint.h"
#include "prinsertItem.h"
#include "prrotation.h"
#include "prbase.h"

#include <last.h>

//---------------------------------------------------------------------------------------
// 
// ---
AssmSolverSceneWidget::AssmSolverSceneWidget(QWidget* pParent)
    : QtOpenGLSceneWidget(pParent)
    , m_pTextEdit(nullptr)
#if VSN_LIST_CONSTRAINTS
    , m_pListConstraints(new QTableWidget(this))
#endif
    , m_pReporter(nullptr)
    , m_pConsManager(nullptr)
    , m_pModel(nullptr)
    , m_pSolver(nullptr)
    , m_pCurrentProcess(nullptr)
    , m_ptrSelectManager(std::make_shared<SelectionManager>())
    , m_pModelSeg(nullptr)
{    
    m_pModel = new MbModel;
    m_pSolver = std::make_shared<AssemblySolver>(UniteToMainAssembly(m_pModel));
    m_pConsManager = new ConstraintsManager(m_pSolver, this, this);
    QtVision::ProcessTypes ptTypes = QtVision::pt_Pan | QtVision::pt_Zoom | QtVision::pt_Rotate;
    QtVision::createProcessesCameraControls(this, ptTypes);
}

AssmSolverSceneWidget::~AssmSolverSceneWidget()
{
    VSN_DELETE_AND_NULL(m_pCurrentProcess);
}

QTextEdit* AssmSolverSceneWidget::textEdit()
{
    if (m_pTextEdit == nullptr)
    {
        m_pTextEdit = new QTextEdit(this);
        m_pTextEdit->setFontPointSize(9.5);
        m_pTextEdit->setFontFamily("Consolas");
    }
    if (m_pReporter == nullptr)
    {
        m_pReporter = new Reporter(solver(), this);
        QObject::connect(m_pReporter, &Reporter::addJournalLine, m_pTextEdit, &QTextEdit::append);
        QObject::connect(m_pReporter, &Reporter::journalChanged, m_pTextEdit, &QTextEdit::clear);
    }

    return m_pTextEdit;
}

#if VSN_LIST_CONSTRAINTS
QTableWidget* AssmSolverSceneWidget::listConstraints()
{
    return m_pListConstraints;
}
#endif

void AssmSolverSceneWidget::initializeGL()
{
    QtOpenGLSceneWidget::initializeGL();

    Light* pLight = graphicsScene()->CreateExtraLight();
    pLight->SetType(Light::DirectionLight);
    pLight->SetPosition(-100.0, -1.0, -1.0);
    pLight->SetDiffuseColor(Color(0.2f, 0.2f, 0.2f, 1.0f));
    pLight->SetDoubleSided(false);

    GlobalFlags::SetPixelCullingEnabled(true);
    GlobalFlags::SetFaceCulling(CullFaces::Back);
    graphicsView()->SetOrientationUpVector(Vector3DF::yAxis);
    graphicsView()->SetSmoothTransition(true);

    viewport()->GetCamera()->SetViewOrientation(Orientation::IsoXYZ);
    viewport()->SetGradientBackgroundColour(QtVision::topColor, QtVision::bottomColor);
    viewport()->SetPixelCullingSize(35);
    mainLight()->SetDoubleSided(false);

    uint nFactor = 2.0;
    uint a, b, c, d, e, f,h;
    OrientationMarker* pMarker = graphicsView()->GetOrientationMarker();
    pMarker->GetRepOptionTriad(a, b, c, d, e, f, h);
    pMarker->SetRepOptionTriad(nFactor *a, nFactor *b, nFactor * c, nFactor *d, nFactor * e, nFactor * f, nFactor * h);

    // надо написать manager для процессов
    ObjectPickSelection* pPickSelection = objectPickSelection();
    m_ptrSelectManager->SetSceneContent(sceneContent());
    m_ptrSelectManager->SetObjectPickSelection(pPickSelection);
    m_ptrSelectManager->SetSelectionMode(SelectionManager::MultiSelection);
    m_ptrSelectManager->SetDynamicHighlighting(true);
    Object::Connect(m_ptrSelectManager.get(), &SelectionManager::signalStateModified, this, &QtOpenGLWidget::updateWidget);
    Object::Connect(viewport(), &Viewport::RefreshScreen, this, &QtOpenGLWidget::repaintWidget);
    viewCommands(Select);
}

void AssmSolverSceneWidget::slotUpdateCommands()
{
    if (m_pCurrentProcess == nullptr)
        return;
    QWidget* parentWidget = qobject_cast<QWidget*>(parent());
    QList<QAction*> lstAction = parentWidget->actions();
    for (QAction* pAction : lstAction)
    {
        Commands value = static_cast<Commands>(pAction->property("Commands").toInt());

        if (m_pCurrentProcess != nullptr)
        {
            const MbAssembly* assm = GetMainAssembly(m_pModel);  // The main assembly.
            VSN_ASSERT(assm!=nullptr);
            switch(value)
            { 
                case Save:
                case FixItem:
                case Rotation:
                    pAction->setEnabled(assm != nullptr && assm->ItemsCount() > 0);
                    break;
                case Cancel:
                    pAction->setEnabled(m_pCurrentProcess->IsReady());
                    break;
                case Comfirm:
                    pAction->setEnabled(m_pCurrentProcess->IsReady());
                    break;
                case InsertItem:
                    pAction->setEnabled(m_pCurrentProcess != nullptr && 
                        m_pCurrentProcess->GetCustomKey() == NodeKey(Select));
                    break;
                case Coincident:
                case Coaxial:
                case Parallel:
                case Perpendicular:
                case Angular:
                case Distance:
                    pAction->setEnabled(assm != nullptr && (assm->ItemsCount() > 1));
                    break;
                /*
                case Rotation:                                           
                    const bool bEnabledAxis = (assm != nullptr) && (assm->ItemsCount() > 1) &&
                         PrRotationAboutAxis::SelectedAxialGeom(this,nullptr).SubGeomRecord().type != GCM_NULL_GTYPE;
                    pAction->setEnabled(bEnabledAxis);
                    break;
                */
            }
        }

        if (pAction->isCheckable())
        {
            if (value == static_cast<Commands>(m_pCurrentProcess->GetCustomKey().GetKey()))
                pAction->setChecked(true);
            else
                pAction->setChecked(false);
        }
    }
}

QString AssmSolverSceneWidget::openSelectModel()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);

    QString file = "File";
    
    return file;
}

void AssmSolverSceneWidget::viewCommands(Commands cmd)
{
    Commands value = Select;
    if (cmd == None)
    {
        QObject* action = sender();
        value = static_cast<Commands>(action->property("Commands").toInt());
    }

    switch (value)
    {
        case AssmSolverSceneWidget::Open:
        {
            viewCommands(AssmSolverSceneWidget::Select);
            loadModel();
            break;
        }
        case AssmSolverSceneWidget::Save:
        {
            viewCommands(AssmSolverSceneWidget::Select);
            saveModel();
            break;
        }
        case AssmSolverSceneWidget::Select:
        {
            if (m_pCurrentProcess != nullptr)
                m_pCurrentProcess->CancelObject();
            VSN_DELETE_AND_NULL(m_pCurrentProcess);
            m_pCurrentProcess = new PrBase(this);
            m_pCurrentProcess->SetCustomKey(NodeKey(AssmSolverSceneWidget::Select));
            break;
        }
/*
        case AssmSolverSceneWidget::Cancel:
        {
            if (m_pCurrentProcess != nullptr)
                m_pCurrentProcess->CancelObject();
            viewCommands(AssmSolverSceneWidget::Select);
            break;
        }
*/
        case AssmSolverSceneWidget::Comfirm:
        {
            if (m_pCurrentProcess != nullptr)
                m_pCurrentProcess->CreateObject();
            VSN_DELETE_AND_NULL(m_pCurrentProcess);
            viewCommands(AssmSolverSceneWidget::Select);
            break;
        }
        case AssmSolverSceneWidget::InsertItem:
        {
            QString fileName = openSelectModel();
            if (!fileName.isEmpty())
            {
                VSN_DELETE_AND_NULL(m_pCurrentProcess);
                bool bRuning = false;
                m_pCurrentProcess = new PrInsertItem(c3d::WToPathstring(fileName.toStdWString()), this, bRuning);
                if (!bRuning)
                    viewCommands(AssmSolverSceneWidget::Select);
            }
            break;
        }
        case AssmSolverSceneWidget::FixItem:
        {
            VSN_DELETE_AND_NULL(m_pCurrentProcess);
            m_pCurrentProcess = StartFixingProcess(this);
            if (m_pCurrentProcess==nullptr)
                viewCommands(AssmSolverSceneWidget::Select);
            break;
        }

        // Commands to create assembly mates.
        case AssmSolverSceneWidget::Coincident:
        case AssmSolverSceneWidget::Coaxial:
        case AssmSolverSceneWidget::Parallel:
        case AssmSolverSceneWidget::Perpendicular:
        case AssmSolverSceneWidget::Angular:
        case AssmSolverSceneWidget::Distance:
        {
            if (m_pCurrentProcess != nullptr)
                m_pCurrentProcess->CancelObject();
            VSN_DELETE_AND_NULL(m_pCurrentProcess);
            bool bRuning = false;
            m_pCurrentProcess = new PrBinConstraint(value, this, bRuning);
            if (!bRuning)
                viewCommands(AssmSolverSceneWidget::Select);
            break;
        }
        case AssmSolverSceneWidget::Rotation:
        {
            if (m_pCurrentProcess != nullptr)
                m_pCurrentProcess->CancelObject();
            VSN_DELETE_AND_NULL(m_pCurrentProcess);
            bool bRuning = false;
            m_pCurrentProcess = new PrRotationAboutAxis(value, this, bRuning);
            if (!bRuning)
                viewCommands(AssmSolverSceneWidget::Select);
            break;
        }
        default:
            break;
    };
    slotUpdateCommands();
    update();
}

SceneSegment* AssmSolverSceneWidget::assemblySegment() const
{
    if (m_pModelSeg==nullptr)
    { 
        return nullptr;
    }

    if (const MbAssembly* rootAssm = GetMainAssembly(m_pModel))
    { 
        auto childList = m_pModelSeg->GetChildSegments();
        // Single assembly node is required.
        if (childList.size() == 1 )  
        { 
            // Check the assembly segment must have property C3D_PATHITEM_ID.
            if (childList.front()->GetProperty(C3D_PATHITEM_ID) == rootAssm->GetItemName() )
                return childList.front();
        }
    }

    return nullptr;
}

void AssmSolverSceneWidget::rebuildScene()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    SceneGenerator sceneGenerator;
    if (m_pModelSeg != nullptr )
    {
      sceneContent()->GetRootSegment()->RemoveSegment(m_pModelSeg);
      delete m_pModelSeg;
    }
    m_pModelSeg = sceneGenerator.CreateSceneSegment(model(), sceneContent()->GetRootSegment(), false, false);
    sceneGenerator.StartBuildGeometry(CommandType::Synchronous);
    QApplication::restoreOverrideCursor();
}

void AssmSolverSceneWidget::loadModel()
{
    m_fileName = openSelectModel();
    if (!m_fileName.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        MbModel model;
        ConvConvertorProperty3D convProperties;
        convProperties.enableAutostitch = true;
        convProperties.autostitchPrecision = Math::visualSag;
        convProperties.fileName = c3d::WToPathstring(m_fileName.toStdWString());
        
        if (c3d::ImportFromFile(model, c3d::WToPathstring(m_fileName.toStdWString()), &convProperties, nullptr) == cnv_Success)
        {
            if (m_pModel != nullptr)
                m_pModel->DeleteItems();

            for (MbModel::ItemIterator iter = model.Begin(), last = model.End(); iter != last; ++iter)
                m_pModel->AddItem(*(*iter));
            m_pSolver = std::make_shared<AssemblySolver>(UniteToMainAssembly(m_pModel)); // renew the solver.
            MbAssembly & solvedAssembly = solver()->SolverAssembly();
            RestoreConstraintSystem(*solver(), solver()->SolverAssembly());
            if (m_pReporter!=nullptr)
                m_pReporter->AssignSolver(m_pSolver);
            if ( m_pConsManager != nullptr )
                m_pConsManager->AssignSolver(m_pSolver);

            QApplication::restoreOverrideCursor();
        }
        else
        {
            QApplication::restoreOverrideCursor();
            return;
        }
    }
    else
        return;
    rebuildScene();
    viewport()->ZoomToFit(sceneContent()->GetBoundingBox());

    slotUpdateCommands();
}

void AssmSolverSceneWidget::saveModel()
{
    const QString lastUserPath = m_fileName;
    QStringList filters;
    filters.append("Files *.c3d");
    QString oneLineFilters = filters.join("\n");
#ifdef Q_OS_WIN
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model"), lastUserPath, oneLineFilters);
#else 
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model"), lastUserPath, oneLineFilters, nullptr, QFileDialog::DontUseNativeDialog);
#endif 

    if (!fileName.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        MbeConvResType readRes = cnv_Success;
        MbAssembly & solvedAssembly = solver()->SolverAssembly();
        SaveConstraintSystem(*solver(), solver()->SolverAssembly());
        readRes = c3d::ExportIntoFile(*m_pModel, c3d::WToPathstring(fileName.toStdWString()));
        if (readRes != cnv_Success)
            vsnWarning("Model write error");
        QApplication::restoreOverrideCursor();
    }
}

