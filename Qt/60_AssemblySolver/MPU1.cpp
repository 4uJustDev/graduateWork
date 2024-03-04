#include "BuildMathModel.h"

using namespace BuildMathModel;

const int startX = 75;
const int startY = 30;
const double DEG_TO_RAD = M_PI / 180.0;

// ����� - ����� 1
void CreateSketch1Shtyr(RPArray<MbContour>& _arrContours)
{
    // ����� ����������� � ��� � ��������� ��
    MbCartPoint cnt(0, 0);
    // ���������� ���������� �� ������ � �������
    const double RAD1 = 22.5;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours.push_back(pContour);

}

// ����� - ����� 2
void CreateSketch2Shtyr(RPArray<MbContour>& _arrContours1)
{
    // ����� ����������� � ��� � ��������� ��
    MbCartPoint cnt(0, 0);
    // ���������� ���������� �� ������ � �������
    const double RAD1 = 30;
    MbArc* circle1 = new MbArc(cnt, RAD1);
    MbContour* pContour = new MbContour();
    pContour->AddSegment(circle1);
    _arrContours1.push_back(pContour);

}

SPtr<MbSolid> ParametricModelCreator::CreateSolidShtyr()
{
    RPArray<MbContour> arrContours;
    CreateSketch1Shtyr(arrContours);
    RPArray<MbContour> arrContours1;
    CreateSketch2Shtyr(arrContours1);
    // �������� ��������� ������ �������� ���� � �������� ����������
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    // ������� ������
    MbVector3D dirY(0, 1, 0);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    // ���������� ������� ���
    MbSolid* shtyrBody = nullptr;
    MbSolid* shtyrBody1 = nullptr;

    int shtyrLength = 180;
    int shtyrLength1 = 20;

    // ���������, � ������� ������ ������ ������������
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1));
    // ���������, � ������� ������ ������������������
    MbPlane* pPlaneXZ1 = new MbPlane(MbCartPoint3D(0, shtyrLength, 0), MbCartPoint3D(1, shtyrLength, 0),
        MbCartPoint3D(0, shtyrLength, 1));

    MbSweptData shtyrSweptData(*pPlaneXZ, arrContours);
    MbSweptData shtyrSweptData1(*pPlaneXZ1, arrContours1);

    // ��������� ����������� ������������
    ExtrusionValues shtyrExtrusionParams(shtyrLength, 0);
    ExtrusionValues shtyrExtrusionParams1(shtyrLength1, 0);

    MbResultType shtyrBodyExtrusion = ::ExtrusionSolid(shtyrSweptData, dirY, nullptr,
        nullptr, false, shtyrExtrusionParams, operNames, cNames, shtyrBody);
    MbResultType shtyrBodyExtrusion1 = ::ExtrusionSolid(shtyrSweptData1, dirY, nullptr,
        nullptr, false, shtyrExtrusionParams1, operNames, cNames, shtyrBody1);

    // ����������
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
    initCurves.emplace_back(allEdges[0]);// ������ ������ ����
    initCurves.emplace_back(allEdges[2]);
    FacesSPtrVector initBounds;
    RPArray<MbFace> initBounds2(0, 1);

    SolidSPtr pResult;
    MbResultType res = ::FilletSolid(*(MbSolid*)shtyrBody1, cm_Copy, MbShellFilletValues(initCurves, initBounds, params, filletNames), pResult);

    SolidSPtr shtyr(shtyrBody);
    SolidSPtr resultSolid;
    ::BooleanResult(pResult, cm_Copy, shtyr, cm_Copy, MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultSolid);

    // ���������� ���������� ����
   /* TestVariables::viewManager->AddObject(Style(1, TestVariables::LIGHTGREEN), shtyrBody);
    TestVariables::viewManager->AddObject(Style(1, TestVariables::LIGHTGREEN), pResult);*/

    // ������� ���������;
    ::DeleteItem(shtyrBody);
    ::DeleteItem(shtyrBody1);
    ::DeleteItem(pPlaneXZ);
    ::DeleteItem(pPlaneXZ1);

    SolidSPtr MainSolid(resultSolid);
    //TestVariables::viewManager->AddObject(Style(1, TestVariables::LIGHTGREEN), MainSolid);
    return MainSolid;
}