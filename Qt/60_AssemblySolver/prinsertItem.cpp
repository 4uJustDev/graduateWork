#include "prinsertItem.h"
#include <QFileDialog>

#include <conv_model_exchange.h>
#include <conv_exchange_settings.h>

#include <vsn_transform.h>
#include <vsn_planemarker.h>

#include <last.h>

#include <cassert>

#include <action_general.h>
#include <action_point.h>
#include <action_curve.h>
#include <action_curve3d.h>
#include <action_surface_curve.h>
#include <action_surface.h>
#include <action_direct.h>
#include <action_mesh.h>
#include <action_phantom.h>
#include <action_shell.h>
#include <action_sheet.h>
#include <action_solid.h>

#include <templ_s_array.h>
#include <templ_rp_array.h>
#include <templ_array2.h>
#include <templ_sptr.h>
#include <templ_dptr.h>

#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mb_matrix.h>
#include <mb_matrix3d.h>
#include <mb_matrixnn.h>
#include <mb_placement.h>
#include <mb_placement3d.h>
#include <mb_axis3d.h>
#include <mb_thread.h>
#include <mb_nurbs_function.h>

#include <function.h>
#include <function_factory.h>

#include <curve.h>
#include <cur_line_segment.h>
#include <cur_arc.h>
#include <cur_nurbs.h>
#include <cur_hermit.h>
#include <cur_reparam_curve.h>
#include <cur_trimmed_curve.h>
#include <cur_character_curve.h>
#include <cur_polyline.h>
#include <cur_projection_curve.h>
#include <cur_contour.h>

#include <region.h>
#include <contour_graph.h>

#include <curve3d.h>
#include <cur_line_segment3d.h>
#include <cur_arc3d.h>
#include <cur_nurbs3d.h>
#include <cur_hermit3d.h>
#include <cur_trimmed_curve3d.h>
#include <cur_reparam_curve3d.h>
#include <cur_character_curve3d.h>
#include <cur_contour3d.h>
#include <cur_plane_curve.h>
#include <cur_surface_curve.h>
#include <cur_contour_on_surface.h>
#include <cur_silhouette_curve.h>
#include <cur_surface_intersection.h>
#include <cur_spiral.h>

#include <surface.h>
#include <surf_plane.h>
#include <surf_cylinder_surface.h>
#include <surf_cone_surface.h>
#include <surf_sphere_surface.h>
#include <surf_torus_surface.h>
#include <surf_extrusion_surface.h>
#include <surf_revolution_surface.h>
#include <surf_lofted_surface.h>
#include <surf_evolution_surface.h>
#include <surf_spiral_surface.h>
#include <surf_spline_surface.h>
#include <surf_offset_surface.h>
#include <surf_curve_bounded_surface.h>

#include <attribute_item.h>
#include <attr_color.h>
#include <attr_common_attribute.h>
#include <attr_dencity.h>
#include <attr_identifier.h>
#include <attr_selected.h>
#include <attr_user_attribute.h>
#include <attribute_container.h>

#include <name_item.h>
#include <name_check.h>

#include <topology.h>
#include <topology_faceset.h>
#include <check_geometry.h>

#include <point3d.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <creator.h>
#include <cr_simple_creator.h>
#include <solid.h>
#include <instance_item.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <assembly.h>

#include <mesh.h>
#include <mesh_grid.h>
#include <mesh_primitive.h>
#include <mesh_polygon.h>

#include <map_create.h>
#include <map_lump.h>
#include <map_section.h>
#include <map_section_complex.h>
#include <map_vestige.h>
#include <map_thread.h>

#include <tri_face.h>

#include <mip_curve_properties.h>
#include <mip_solid_area_volume.h>
#include <mip_solid_mass_inertia.h>
#include "gcm_api.h"
#include "gcm_constraint.h"
#include "gcm_manager.h"
#include "gcm_types.h"
#include <cdet_bool.h>
#include <cdet_data.h>
#include <cdet_utility.h>
#include <part_solid.h>
//для сборки
#include <mb_class_traits.h>
#include <tool_time_test.h>
#include <constraint.h>
#include <alg_base.h>
#include <alg_draw.h>
#include <generic_utility.h>
#include <conv_model_exchange.h>
#include <algorithm>
#include <ctime>
#include <functional>
#include <map>
#include <vector>
#include <utility>

#include <math_namespace.h>
#include <last.h>


using namespace c3d;

const int startX = 75;
const int startY = 30;
const double DEG_TO_RAD = M_PI / 180.0;


// Штырь - эскиз 1
void CreateSketch1Shtyr(RPArray<MbContour>& _arrContours)
{
    // Центр окружностей и дуг в локальной СК
    MbCartPoint cnt(0, 0);
    // Построение окружности по центру и радиусу
    const double RAD1 = 22.5;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours.push_back(pContour);

}

// Штырь - эскиз 2
void CreateSketch2Shtyr(RPArray<MbContour>& _arrContours1)
{
    // Центр окружностей и дуг в локальной СК
    MbCartPoint cnt(0, 0);
    // Построение окружности по центру и радиусу
    const double RAD1 = 30;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours1.push_back(pContour);

}

static SPtr <MbSolid>  CreateSolidShtyr()
{
    RPArray<MbContour> arrContours;
    CreateSketch1Shtyr(arrContours);
    RPArray<MbContour> arrContours1;
    CreateSketch2Shtyr(arrContours1);
    // Названия элементов модели твердого тела и контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    // Базовый вектор
    MbVector3D dirY(0, 1, 0);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    // Объявление твердых тел
    MbSolid* shtyrBody = nullptr;
    MbSolid* shtyrBody1 = nullptr;

    int shtyrLength = 180;
    int shtyrLength1 = 20;

    // Плоскость, в которой строим первое выдавливание
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1));
    // Плоскость, в которой строим второевыдавливание
    MbPlane* pPlaneXZ1 = new MbPlane(MbCartPoint3D(0, shtyrLength, 0), MbCartPoint3D(1, shtyrLength, 0),
        MbCartPoint3D(0, shtyrLength, 1));

    MbSweptData shtyrSweptData(*pPlaneXZ, arrContours);
    MbSweptData shtyrSweptData1(*pPlaneXZ1, arrContours1);

    // Параметры направления выдавливания
    ExtrusionValues shtyrExtrusionParams(shtyrLength, 0);
    ExtrusionValues shtyrExtrusionParams1(shtyrLength1, 0);

    MbResultType shtyrBodyExtrusion = ::ExtrusionSolid(shtyrSweptData, dirY, nullptr,
        nullptr, false, shtyrExtrusionParams, operNames, cNames, shtyrBody);
    MbResultType shtyrBodyExtrusion1 = ::ExtrusionSolid(shtyrSweptData1, dirY, nullptr,
        nullptr, false, shtyrExtrusionParams1, operNames, cNames, shtyrBody1);

    // Скругление
    MbSNameMaker filletNames(ct_FilletSolid, MbSNameMaker::i_SideNone, 0);
    SmoothValues params;
    params.distance1 = params.distance2 = 5;
    params.form = st_Fillet;
    params.conic = 0;
    params.prolong = false;
    params.smoothCorner = SmoothValues::ec_uniform;
    params.keepCant = ts_negative;
    params.strict = true;

    RPArray<MbCurveEdge> allEdges(4, 1);
    shtyrBody1->GetEdges(allEdges);

    c3d::EdgesSPtrVector initCurves;
    initCurves.emplace_back(allEdges[0]);// номера нужных рёбер
    initCurves.emplace_back(allEdges[2]);
    FacesSPtrVector initBounds;
    RPArray<MbFace> initBounds2(0, 1);

    SolidSPtr pResult;
    MbResultType res = ::FilletSolid(*(MbSolid*)shtyrBody1, cm_Copy, MbShellFilletValues(initCurves, initBounds, params, filletNames), pResult);

    SolidSPtr shtyr(shtyrBody);
    SolidSPtr resultSolid;
    ::BooleanResult(pResult, cm_Copy, shtyr, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultSolid);

    // Отображаем полученное тело
   /* TestVariables::viewManager->AddObject(Style(1, TestVariables::LIGHTGREEN), shtyrBody);
    TestVariables::viewManager->AddObject(Style(1, TestVariables::LIGHTGREEN), pResult);*/

    // Удаляем указатели;
    ::DeleteItem(shtyrBody);
    ::DeleteItem(shtyrBody1);
    ::DeleteItem(pPlaneXZ);
    ::DeleteItem(pPlaneXZ1);

    SolidSPtr MainSolid(resultSolid);
    //TestVariables::viewManager->AddObject(Style(1, TestVariables::LIGHTGREEN), MainSolid);
    return MainSolid;
}

void CreateSketch1Koleso(RPArray<MbContour>& _arrContours)
{

    MbPolyline* polyline = nullptr;
    MbContour* contour = nullptr;

    int startHeight = 22.5;
    SArray<MbCartPoint> Points(11);
    Points.Add(MbCartPoint(30, startHeight));
    Points.Add(MbCartPoint(30, startHeight + 50));
    Points.Add(MbCartPoint(25, startHeight + 50));
    Points.Add(MbCartPoint(25, startHeight + 50 + 37.5));
    Points.Add(MbCartPoint(40, startHeight + 50 + 37.5));
    Points.Add(MbCartPoint(40, startHeight + 50 + 37.5 + 10));
    Points.Add(MbCartPoint(-40, startHeight + 50 + 37.5 + 10));
    Points.Add(MbCartPoint(-40, startHeight + 50 + 37.5));
    Points.Add(MbCartPoint(-25, startHeight + 50 + 37.5));
    Points.Add(MbCartPoint(-25, startHeight + 50));
    Points.Add(MbCartPoint(-30, startHeight + 50));
    Points.Add(MbCartPoint(-30, startHeight));

    MbPolyline* pPolyline = new MbPolyline(Points, true);

    MbContour* pContour = new MbContour(*pPolyline, true);
    _arrContours.push_back(pContour);
}

static SPtr <MbSolid> CreateSolidKoleso()
{
    MbPlacement3D pl;
    RPArray<MbContour> arrContours;
    CreateSketch1Koleso(arrContours);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    MbAxis3D axisX(pl.GetAxisX());

    // Плоскость, в которой строим первое выдавливание
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1));

    MbSolid* koleso = nullptr;

    MbSweptData sweptData(*pPlaneXZ, arrContours);

    // Параметры операции вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    MbResultType Revolution = ::RevolutionSolid(sweptData, axisX, revParms, operNames,
        cNames, koleso);
    // Удаляем указатели

    SolidSPtr MainSolid(koleso);
    ::DeleteItem(koleso);
    ::DeleteItem(pPlaneXZ);
    return MainSolid;
}

void CreateSketch1Vilka(RPArray<MbContour>& _arrContours)
{

    MbPolyline* polyline = nullptr;
    MbContour* contour = nullptr;

    int blockWidth = 260;
    int blockHeight = 30;
    SArray<MbCartPoint> Points(4);
    Points.Add(MbCartPoint(0, 0));
    Points.Add(MbCartPoint(blockWidth, 0));
    Points.Add(MbCartPoint(blockWidth, blockHeight));
    Points.Add(MbCartPoint(0, blockHeight));


    MbPolyline* pPolyline = new MbPolyline(Points, true);

    MbContour* pContour = new MbContour(*pPolyline, true);
    _arrContours.push_back(pContour);

}

void CreateSketch2Vilka(RPArray<MbContour>& _arrContours)
{

    MbPolyline* polyline = nullptr;
    MbContour* contour = nullptr;
    int radius = 55;
    SArray<MbCartPoint> Points(4);
    Points.Add(MbCartPoint(startX, startY + 150));
    Points.Add(MbCartPoint(startX, startY));
    Points.Add(MbCartPoint(startX + 110, startY));
    Points.Add(MbCartPoint(startX + 110, startY + 150));


    MbPolyline* pPolyline = new MbPolyline(Points, false);
    MbCartPoint arcCenter(startX + 110 / 2, startY + 150);

    MbArc* pArc = new MbArc(arcCenter, radius, Points[3], Points[0], 1);
    MbContour* pContour = new MbContour(*pPolyline, true);
    pContour->AddSegment(pArc);

    _arrContours.push_back(pContour);
}

void CreateSketch3Vilka(RPArray<MbContour>& _arrContours)
{
    // Центр окружностей и дуг в локальной СК
    MbCartPoint cnt(startX + 110 / 2, startY + 150);
    // Построение окружности по центру и радиусу
    const double RAD1 = 42.5;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours.push_back(pContour);
}

void CreateSketch4Vilka(RPArray<MbContour>& _arrContours)
{
    // Центр окружностей и дуг в локальной СК
    MbCartPoint cnt(startX + 110 / 2, startY + 150);
    // Построение окружности по центру и радиусу
    const double RAD1 = 55;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours.push_back(pContour);
}

void CreateSketch5Vilka(RPArray<MbContour>& _arrContours)
{
    // Центр окружностей и дуг в локальной СК
    MbCartPoint cnt(startX + 110 / 2, startY + 150);
    // Построение окружности по центру и радиусу
    const double RAD1 = 22.5;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours.push_back(pContour);
}

static SPtr <MbSolid> CreateSolidVilka()
{
    RPArray<MbContour> arrContours;
    CreateSketch1Vilka(arrContours);
    RPArray<MbContour> arrContours1;
    CreateSketch2Vilka(arrContours1);
    RPArray<MbContour> arrContours2;
    CreateSketch3Vilka(arrContours2);
    RPArray<MbContour> arrContours3;
    CreateSketch4Vilka(arrContours3);
    RPArray<MbContour> arrContours4;
    CreateSketch5Vilka(arrContours4);
    // Названия элементов модели твердого тела и контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    // Именователь для функции SymmetrySolid
    MbSNameMaker operNamesMirror(ct_SymmetrySolid, MbSNameMaker::i_SideNone);
    MbSNameMaker names(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    // Именователь для отверстий
    MbSNameMaker holeNames(ct_HoleSolid, MbSNameMaker::i_SideNone, 0);
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    // Базовый вектор
    MbVector3D dir(0, 1, 0);

    // Объявление твердых тел
    MbSolid* vilkaBody = nullptr;
    MbSolid* vilkaBody1 = nullptr;
    MbSolid* vilkaBody2 = nullptr;
    MbSolid* vilkaBody3 = nullptr;

    int vilkaExtrusionForward = 70;
    int vilkaExtrusionBackward = 70;
    int vilkaExtrusionForward1 = 24;
    int vilkaExtrusionDop = 6;

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    MbPlacement3D plHole(MbCartPoint3D(0, 0, 0));
    plHole.Rotate(MbAxis3D(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0))), 90 * DEG_TO_RAD);
    plHole.Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(130, 100, 180)));

    // Плоскость для зеркалирования
    MbPlacement3D plSym(
        MbVector3D(1, 0, 0), /* Ось X локальной СК */
        MbVector3D(0, 0, 1), /* Ось Z локальной СК */
        MbCartPoint3D(0, 0, 0 /* Начало координат локальной СК */));

    // Параметры для отверстия
    BorerValues paramsHole;
    paramsHole.type = BorerValues::bt_SimpleCylinder;
    paramsHole.diameter = 45; // Диаметр отверстия
    paramsHole.depth = 60; //глубина отверстия
    paramsHole.down = false;

    // Плоскость, в которой строим первое выдавливание
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1));
    // Плоскость, в которой строим второе выдавливание
    MbPlane* pPlaneXZ1 = new MbPlane(MbCartPoint3D(0, vilkaExtrusionForward - vilkaExtrusionForward1, 0), MbCartPoint3D(1, vilkaExtrusionForward - vilkaExtrusionForward1, 0),
        MbCartPoint3D(0, vilkaExtrusionForward - vilkaExtrusionForward1, 1));
    MbPlane* pPlaneXZ2 = new MbPlane(MbCartPoint3D(0, vilkaExtrusionForward, 0), MbCartPoint3D(1, vilkaExtrusionForward, 0),
        MbCartPoint3D(0, vilkaExtrusionForward, 1));

    MbSweptData vilkaSweptData(*pPlaneXZ, arrContours);
    MbSweptData vilkaSweptData1(*pPlaneXZ1, arrContours1);
    MbSweptData vilkaSweptData2(*pPlaneXZ1, arrContours2);
    MbSweptData vilkaSweptData3(*pPlaneXZ2, arrContours3);

    // Параметры направления выдавливания
    ExtrusionValues vilkaExtrusionParams(vilkaExtrusionForward, vilkaExtrusionBackward);
    ExtrusionValues vilkaExtrusionParams1(vilkaExtrusionForward1, 0);
    ExtrusionValues vilkaExtrusionParams2(0, vilkaExtrusionDop);
    ExtrusionValues vilkaExtrusionParams3(vilkaExtrusionDop, 0);

    MbResultType res1 = ::ExtrusionSolid(vilkaSweptData, dir, nullptr,
        nullptr, false, vilkaExtrusionParams, operNames, cNames, vilkaBody);
    MbResultType res2 = ::ExtrusionSolid(vilkaSweptData1, dir, nullptr,
        nullptr, false, vilkaExtrusionParams1, operNames, cNames, vilkaBody1);
    MbResultType res3 = ::ExtrusionSolid(vilkaSweptData2, dir, nullptr,
        nullptr, false, vilkaExtrusionParams2, operNames, cNames, vilkaBody2); // внутри
    MbResultType res4 = ::ExtrusionSolid(vilkaSweptData3, dir, nullptr,
        nullptr, false, vilkaExtrusionParams3, operNames, cNames, vilkaBody3); // сзади

    // Преобразование MbSolid в SolidSPtr
    SolidSPtr MainBody(vilkaBody);
    SolidSPtr vilkaBody1New(vilkaBody1);
    SolidSPtr vilkaBody2New(vilkaBody2);
    SolidSPtr vilkaBody3New(vilkaBody3);
    SolidSPtr vilkaHole;
    SolidSPtr vilkaHole2;
    SolidSPtr vilkaHole3;
    SolidSPtr resultSolid;
    // Отверстие сквозь 3 тела
    ::HoleSolid(vilkaBody1New, cm_Copy, MbHoleSolidParams(plHole, paramsHole, holeNames), vilkaHole);
    ::HoleSolid(vilkaBody2New, cm_Copy, MbHoleSolidParams(plHole, paramsHole, holeNames), vilkaHole2);
    ::HoleSolid(vilkaBody3New, cm_Copy, MbHoleSolidParams(plHole, paramsHole, holeNames), vilkaHole3);
    // Переменные для объединения
    SolidSPtr result1;
    SolidSPtr result2;
    SolidSPtr result3;
    SolidSPtr result4;
    SolidSPtr result5;
    SolidSPtr resultLast;
    ::BooleanResult(MainBody, cm_Copy, vilkaHole, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), result1);
    ::BooleanResult(result1, cm_Copy, vilkaHole2, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), result2);
    ::BooleanResult(result2, cm_Copy, vilkaHole3, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), result3);

    // Отзеркаливание
    SolidSPtr vilkaMirror = nullptr;
    SolidSPtr vilkaMirror1 = nullptr;
    SolidSPtr vilkaMirror2 = nullptr;

    MbResultType resMirror = ::SymmetrySolid(*vilkaHole, cm_Copy, MbSymmetrySolidParams(plSym, operNamesMirror, ts_neutral), vilkaMirror);
    MbResultType resMirror1 = ::SymmetrySolid(*vilkaHole2, cm_Copy, MbSymmetrySolidParams(plSym, operNamesMirror, ts_neutral), vilkaMirror1);
    MbResultType resMirror2 = ::SymmetrySolid(*vilkaHole3, cm_Copy, MbSymmetrySolidParams(plSym, operNamesMirror, ts_neutral), vilkaMirror2);
    ::BooleanResult(result3, cm_Copy, vilkaMirror, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), result4);
    ::BooleanResult(result4, cm_Copy, vilkaMirror1, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), result5);
    ::BooleanResult(result5, cm_Copy, vilkaMirror2, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultLast);

    SolidSPtr MainSolid(resultLast);

    return MainSolid;

}



// Recursive function counting 3d-solid bodies of the assembly sub-structure.
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
    // m_pLocalModel = LoadFile(fileName);

    // Load own model
    MbModel* pModel = new MbModel();

    MbPlacement3D lcs;

    MbAxis3D axVert(MbVector3D(1, 0, 0));
    MbAxis3D ayVert(MbVector3D(0, 1, 0));
    MbAxis3D azVert(MbVector3D(0, 0, 1));

    SPtr<MbSolid> Koleso(CreateSolidKoleso());
    SPtr<MbSolid> Vilka(CreateSolidVilka());
    SPtr<MbSolid> Shtyr(CreateSolidShtyr());

    std::vector<SPtr<MbItem>> pair;

    SPtr<MbInstance> kolesoItem(new MbInstance(*Koleso, lcs));
    SPtr<MbInstance> vilkaItem(new MbInstance(*Vilka, lcs));
    SPtr<MbInstance> shtyrItem(new MbInstance(*Shtyr, lcs));


    //Переменные для подсборки
    pair.push_back(kolesoItem);
    pair.push_back(vilkaItem);
    pair.push_back(shtyrItem);

    SPtr<MbAssembly> assm(new MbAssembly(pair));

    //Ограничения
    MtGeomArgument Otv1(Shtyr->GetFace(1), shtyrItem); // 1 - это шпилька 
    MtGeomArgument Otv2(Vilka->GetFace(0), vilkaItem); // 0 - грань отверстия
    assm->AddConstraint(GCM_CONCENTRIC, Otv1, Otv2);

    MtGeomArgument Pl1(Shtyr->GetFace(0), shtyrItem); // 0 - грань для прижатия шляпки к отверстию вилки
    MtGeomArgument Pl2(Vilka->GetFace(2), vilkaItem); // 2 - грань для прижатия
    assm->AddConstraint(GCM_COINCIDENT, Pl1, Pl2);

    MtGeomArgument Otv3(Shtyr->GetFace(1), shtyrItem); // 1 - это шпилька 
    MtGeomArgument Otv4(Koleso->GetFace(11), kolesoItem); // 11 - центр отверстие
    assm->AddConstraint(GCM_CONCENTRIC, Otv3, Otv4);

    double distance = -100; // возможно 15 или 20
    MtParVariant distBeetween(distance);

    MtGeomArgument Pl3(Koleso->GetFace(0), kolesoItem); // 0 - грань для прижатия к вилке или 10
    MtGeomArgument Pl4(Vilka->GetFace(2), vilkaItem); // 1 - грань для прижатия
    assm->AddConstraint(GCM_DISTANCE, Pl3, Pl4, distBeetween);

    assm->EvaluateConstraints();

    pModel->AddItem(*assm);
    UniteToMainAssembly(pModel);

    m_pLocalModel = pModel;

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