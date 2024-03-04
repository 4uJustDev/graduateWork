#include "BuildMathModel.h"

using namespace BuildMathModel;

const int startX = 75;
const int startY = 30;
const double DEG_TO_RAD = M_PI / 180.0;

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

SPtr<MbSolid> ParametricModelCreator::CreateSolidKoleso()
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