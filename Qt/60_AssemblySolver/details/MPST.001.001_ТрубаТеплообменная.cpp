#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

SPtr<MbSolid> ParametricModelCreator::create_001_tubeTeploobmen()
{
    //�������������
    const double length = 5000;
    const double inner = 89;
    const double outer = 99;


    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);

    SolidSPtr outerPipe, bigRadiusCyl, smallRadiusCyl;

    SpacePointsVector bigRadiusCylPnts;

    bigRadiusCylPnts.push_back(MbCartPoint3D(0, 0, 0));
    bigRadiusCylPnts.push_back(MbCartPoint3D(0, 0, length));
    bigRadiusCylPnts.push_back(MbCartPoint3D(outer / 2, 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPnts, blockNames), bigRadiusCyl);

    SpacePointsVector smallRadiusCylPnts;

    smallRadiusCylPnts.push_back(MbCartPoint3D(0, 0, 0));
    smallRadiusCylPnts.push_back(MbCartPoint3D(0, 0, length));
    smallRadiusCylPnts.push_back(MbCartPoint3D(inner / 2, 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallRadiusCylPnts, blockNames), smallRadiusCyl);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    ::BooleanResult(bigRadiusCyl, cm_Copy, smallRadiusCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), outerPipe);

    return outerPipe;
}