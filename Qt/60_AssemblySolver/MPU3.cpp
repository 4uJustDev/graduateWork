#include "BuildMathModel.h"

using namespace BuildMathModel;

const int startX = 75;
const int startY = 30;
const double DEG_TO_RAD = M_PI / 180.0;

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


SPtr<MbSolid> ParametricModelCreator::CreateSolidVilka()
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