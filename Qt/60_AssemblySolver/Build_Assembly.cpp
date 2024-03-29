#include "BuildMathModel.h"

using namespace BuildMathModel;

SPtr<MbAssembly> ParametricModelCreator::CreatePneumocylinderAssembly(BuildParams params)
{
#pragma region PARAMS
    
    double param_length = params.length;
   
#pragma endregion

    MbPlacement3D lcs;
    // !!! Be carefule
    MbPlacement3D lcs1;

    // Item_008_001->Move(MbVector3D (3000,20,20));

    MbAxis3D axVert(MbVector3D(1, 0, 0));
    MbAxis3D ayVert(MbVector3D(0, 1, 0));
    MbAxis3D azVert(MbVector3D(0, 0, 1));

    SPtr<MbSolid> Detail_001(create_001_tubeTeploobmen());
    SPtr<MbSolid> Detail_002(create_002_tubeKozhux());
    SPtr<MbSolid> Detail_003(create_003_opora());
    SPtr<MbSolid> Detail_004(create_004_reshetkaKozhux());
    SPtr<MbSolid> Detail_005(create_005_kamera());
    SPtr<MbSolid> Detail_006(create_006_RezhetkaTeplTube());
    SPtr<MbSolid> Detail_007(create_007_FlanecKozhux());
    SPtr<MbSolid> Detail_008(create_008_FlanecSpecial());
    SPtr<MbSolid> Detail_009(create_009_curevedTube());

    std::vector<SPtr<MbItem>> pair;

    SPtr<MbInstance> Item_001_001(new MbInstance(*Detail_001, lcs));
    SPtr<MbInstance> Item_001_002(new MbInstance(*Detail_001, lcs));
    SPtr<MbInstance> Item_001_003(new MbInstance(*Detail_001, lcs));
    SPtr<MbInstance> Item_001_004(new MbInstance(*Detail_001, lcs));

    SPtr<MbInstance> Item_002_001(new MbInstance(*Detail_002, lcs));
    SPtr<MbInstance> Item_002_002(new MbInstance(*Detail_002, lcs));
    SPtr<MbInstance> Item_002_003(new MbInstance(*Detail_002, lcs));
    SPtr<MbInstance> Item_002_004(new MbInstance(*Detail_002, lcs));

    SPtr<MbInstance> Item_003_001(new MbInstance(*Detail_003, lcs1));
    SPtr<MbInstance> Item_003_002(new MbInstance(*Detail_003, lcs1));

    SPtr<MbInstance> Item_004_001(new MbInstance(*Detail_004, lcs));

    SPtr<MbInstance> Item_005_001(new MbInstance(*Detail_005, lcs));

    SPtr<MbInstance> Item_006_001(new MbInstance(*Detail_006, lcs));
    SPtr<MbInstance> Item_006_002(new MbInstance(*Detail_006, lcs));
    SPtr<MbInstance> Item_006_003(new MbInstance(*Detail_006, lcs));
    SPtr<MbInstance> Item_006_004(new MbInstance(*Detail_006, lcs));

    SPtr<MbInstance> Item_007_001(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_002(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_003(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_004(new MbInstance(*Detail_007, lcs));

    // Камера
    SPtr<MbInstance> Item_008_001(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_002(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_003(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_004(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_008_005(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_006(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_007(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_008(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_009_001(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_002(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_003(new MbInstance(*Detail_009, lcs));

    //Переменные для подсборки
    pair.push_back(Item_001_001);
    pair.push_back(Item_001_002);
    pair.push_back(Item_001_003);
    pair.push_back(Item_001_004);

    pair.push_back(Item_002_001);
    pair.push_back(Item_002_002);
    pair.push_back(Item_002_003);
    pair.push_back(Item_002_004);

    pair.push_back(Item_003_001);
    pair.push_back(Item_003_002);

    pair.push_back(Item_004_001);

    pair.push_back(Item_005_001);

    pair.push_back(Item_006_001);
    pair.push_back(Item_006_002);
    pair.push_back(Item_006_003);
    pair.push_back(Item_006_004);

    pair.push_back(Item_007_001);
    pair.push_back(Item_007_002);
    pair.push_back(Item_007_003);
    pair.push_back(Item_007_004);

    pair.push_back(Item_008_001);
    pair.push_back(Item_008_002);
    pair.push_back(Item_008_003);
    pair.push_back(Item_008_004);

    pair.push_back(Item_009_001);
    pair.push_back(Item_009_002);
    pair.push_back(Item_009_003);

    SPtr<MbAssembly> assm(new MbAssembly(pair));
    {

        double l0 = 3000.0;
        double distanceRezhetka = 500.0;
        double distanceTubesKozhux = (-1) * (distanceRezhetka + 90.0);
        double distanceTubesTeploobmen = -660.0;

        // Расстояние Опора - Опора
        {
            MtGeomArgument Face1(Detail_003->GetFace(2), Item_003_001);
            MtGeomArgument Face2(Detail_003->GetFace(4), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, Face1, Face2, MtParVariant(l0));
        }

        // Концентричность Опора - Трубы кожуховые
        {
            MtGeomArgument Otv1(Detail_003->GetFace(36), Item_003_001);
            MtGeomArgument Otv2(Detail_003->GetFace(38), Item_003_001);
            MtGeomArgument Otv3(Detail_003->GetFace(39), Item_003_001);
            MtGeomArgument Otv4(Detail_003->GetFace(37), Item_003_001);

            MtGeomArgument Otvtube1(Detail_002->GetFace(3), Item_002_001);
            MtGeomArgument Otvtube2(Detail_002->GetFace(3), Item_002_002);
            MtGeomArgument Otvtube3(Detail_002->GetFace(3), Item_002_003);
            MtGeomArgument Otvtube4(Detail_002->GetFace(3), Item_002_004);

            assm->AddConstraint(GCM_CONCENTRIC, Otv1, Otvtube1);
            assm->AddConstraint(GCM_CONCENTRIC, Otv2, Otvtube2);
            assm->AddConstraint(GCM_CONCENTRIC, Otv3, Otvtube3);
            assm->AddConstraint(GCM_CONCENTRIC, Otv4, Otvtube4);
        }

        // Концентричность Опора - Трубы теплообменные
        {
            MtGeomArgument Hole1(Detail_003->GetFace(36), Item_003_001);
            MtGeomArgument Hole2(Detail_003->GetFace(38), Item_003_001);
            MtGeomArgument Hole3(Detail_003->GetFace(39), Item_003_001);
            MtGeomArgument Hole4(Detail_003->GetFace(37), Item_003_001);

            MtGeomArgument OtvtubeTeploobmennaya1(Detail_002->GetFace(3), Item_001_001);
            MtGeomArgument OtvtubeTeploobmennaya2(Detail_002->GetFace(3), Item_001_002);
            MtGeomArgument OtvtubeTeploobmennaya3(Detail_002->GetFace(3), Item_001_003);
            MtGeomArgument OtvtubeTeploobmennaya4(Detail_002->GetFace(3), Item_001_004);

            assm->AddConstraint(GCM_CONCENTRIC, Hole1, OtvtubeTeploobmennaya1);
            assm->AddConstraint(GCM_CONCENTRIC, Hole2, OtvtubeTeploobmennaya2);
            assm->AddConstraint(GCM_CONCENTRIC, Hole3, OtvtubeTeploobmennaya3);
            assm->AddConstraint(GCM_CONCENTRIC, Hole4, OtvtubeTeploobmennaya4);
        }

        // Расстояние Опора - Решетка кожуховых труб
        {
            MtGeomArgument Plane1(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument Plane2(Detail_004->GetFace(0), Item_004_001);

            assm->AddConstraint(GCM_DISTANCE, Plane1, Plane2, MtParVariant(distanceRezhetka));
        }

        // Концентричность Опора - Решетка кожуховых труб
        {
            MtGeomArgument VariableForHole1(Detail_003->GetFace(36), Item_003_002);
            MtGeomArgument VariableForHole2(Detail_003->GetFace(39), Item_003_002);
            MtGeomArgument VariableForHole3(Detail_003->GetFace(38), Item_003_002);
            MtGeomArgument VariableForHole4(Detail_003->GetFace(37), Item_003_002);

            MtGeomArgument OtvtubeTeploobmennaya1(Detail_004->GetFace(2), Item_004_001);
            MtGeomArgument OtvtubeTeploobmennaya2(Detail_004->GetFace(4), Item_004_001);
            MtGeomArgument OtvtubeTeploobmennaya3(Detail_003->GetFace(38), Item_003_001);
            MtGeomArgument OtvtubeTeploobmennaya4(Detail_003->GetFace(37), Item_003_001);

            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeTeploobmennaya1, VariableForHole1);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeTeploobmennaya2, VariableForHole2);
            assm->AddConstraint(GCM_CONCENTRIC, VariableForHole3, OtvtubeTeploobmennaya3);
            assm->AddConstraint(GCM_CONCENTRIC, VariableForHole4, OtvtubeTeploobmennaya4);
        }

        // Расстояние Опора - Кожуховые трубы
        {
            MtGeomArgument PlaneOfTubeKozhux1(Detail_002->GetFace(0), Item_002_001);
            MtGeomArgument PlaneOfTubeKozhux2(Detail_002->GetFace(0), Item_002_002);
            MtGeomArgument PlaneOfTubeKozhux3(Detail_002->GetFace(0), Item_002_003);
            MtGeomArgument PlaneOfTubeKozhux4(Detail_002->GetFace(0), Item_002_004);

            MtGeomArgument PlaneOfOporaForward1(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForward2(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForward3(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForward4(Detail_003->GetFace(2), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux1, PlaneOfOporaForward1, MtParVariant(distanceTubesKozhux));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux2, PlaneOfOporaForward2, MtParVariant(distanceTubesKozhux));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux3, PlaneOfOporaForward3, MtParVariant(distanceTubesKozhux));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux4, PlaneOfOporaForward4, MtParVariant(distanceTubesKozhux));
        }

        // Расстояние Опора - Теплообменные трубы
        {
            MtGeomArgument PlaneOfTubeTeploobmeny1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument PlaneOfTubeTeploobmeny2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument PlaneOfTubeTeploobmeny3(Detail_001->GetFace(0), Item_001_003);
            MtGeomArgument PlaneOfTubeTeploobmeny4(Detail_001->GetFace(0), Item_001_004);

            MtGeomArgument PlaneOfOporaForwards1(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards2(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards3(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards4(Detail_003->GetFace(2), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny1, PlaneOfOporaForwards1, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny2, PlaneOfOporaForwards2, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny3, PlaneOfOporaForwards3, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny4, PlaneOfOporaForwards4, MtParVariant(distanceTubesTeploobmen));
        }

        // Совмещение Решетка кожуховых труб - Камера
        {
            MtGeomArgument Planeofkamera1(Detail_005->GetFace(1), Item_005_001);
            MtGeomArgument Planeofkamera2(Detail_005->GetFace(3), Item_005_001);

            MtGeomArgument PlaneViaRezhetka1(Detail_004->GetFace(41), Item_004_001);
            MtGeomArgument PlaneViaRezhetka2(Detail_004->GetFace(3), Item_004_001);

            assm->AddConstraint(GCM_COINCIDENT, Planeofkamera1, PlaneViaRezhetka1);
            assm->AddConstraint(GCM_CONCENTRIC, Planeofkamera2, PlaneViaRezhetka2);
        }

        // Совмещение Фланец кожуховый - Кожуховые трубы
        {
            MtGeomArgument PlaneOfTubeKozhux1(Detail_002->GetFace(1), Item_002_001);
            MtGeomArgument PlaneOfTubeKozhux2(Detail_002->GetFace(1), Item_002_002);
            MtGeomArgument PlaneOfTubeKozhux3(Detail_002->GetFace(1), Item_002_003);
            MtGeomArgument PlaneOfTubeKozhux4(Detail_002->GetFace(1), Item_002_004);

            MtGeomArgument PlaneFlanecKozhux1(Detail_007->GetFace(12), Item_007_001);
            MtGeomArgument PlaneFlanecKozhux2(Detail_007->GetFace(12), Item_007_002);
            MtGeomArgument PlaneFlanecKozhux3(Detail_007->GetFace(12), Item_007_003);
            MtGeomArgument PlaneFlanecKozhux4(Detail_007->GetFace(12), Item_007_004);


            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux1, PlaneFlanecKozhux1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux2, PlaneFlanecKozhux2);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux3, PlaneFlanecKozhux3);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux4, PlaneFlanecKozhux4);
        }

        // Концентричность Фланец кожуховый - Кожуховые трубы
        {
            MtGeomArgument SubPlaneOfTubeKozhux1(Detail_002->GetFace(2), Item_002_001);
            MtGeomArgument SubPlaneOfTubeKozhux2(Detail_002->GetFace(2), Item_002_002);
            MtGeomArgument SubPlaneOfTubeKozhux3(Detail_002->GetFace(2), Item_002_003);
            MtGeomArgument SubPlaneOfTubeKozhux4(Detail_002->GetFace(2), Item_002_004);

            MtGeomArgument SubPlaneFlanecKozhux1(Detail_007->GetFace(3), Item_007_001);
            MtGeomArgument SubPlaneFlanecKozhux2(Detail_007->GetFace(3), Item_007_002);
            MtGeomArgument SubPlaneFlanecKozhux3(Detail_007->GetFace(3), Item_007_003);
            MtGeomArgument SubPlaneFlanecKozhux4(Detail_007->GetFace(3), Item_007_004);


            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux1, SubPlaneFlanecKozhux1);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux2, SubPlaneFlanecKozhux2);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux3, SubPlaneFlanecKozhux3);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux4, SubPlaneFlanecKozhux4);
        }

        // Совмещение Фланец кожуховый - Решетка теплообменная
        {
            MtGeomArgument PlaneOfTubeTeploobmennaya1(Detail_006->GetFace(2), Item_006_001);
            MtGeomArgument PlaneOfTubeTeploobmennaya2(Detail_006->GetFace(2), Item_006_002);
            MtGeomArgument PlaneOfTubeTeploobmennaya3(Detail_006->GetFace(2), Item_006_003);
            MtGeomArgument PlaneOfTubeTeploobmennaya4(Detail_006->GetFace(2), Item_006_004);

            MtGeomArgument PlaneFlanecTeploobmennaya1(Detail_007->GetFace(9), Item_007_001);
            MtGeomArgument PlaneFlanecTeploobmennaya2(Detail_007->GetFace(9), Item_007_002);
            MtGeomArgument PlaneFlanecTeploobmennaya3(Detail_007->GetFace(9), Item_007_003);
            MtGeomArgument PlaneFlanecTeploobmennaya4(Detail_007->GetFace(9), Item_007_004);


            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya1, PlaneFlanecTeploobmennaya1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya2, PlaneFlanecTeploobmennaya2);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya3, PlaneFlanecTeploobmennaya3);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya4, PlaneFlanecTeploobmennaya4);
        }

        // Концентричность Фланец кожуховый - Решетка теплообменная
        {
            MtGeomArgument SubPlaneOfTubeTeploobmennaya1(Detail_006->GetFace(3), Item_006_001);
            MtGeomArgument SubPlaneOfTubeTeploobmennaya2(Detail_006->GetFace(3), Item_006_002);
            MtGeomArgument SubPlaneOfTubeTeploobmennaya3(Detail_006->GetFace(3), Item_006_003);
            MtGeomArgument SubPlaneOfTubeTeploobmennaya4(Detail_006->GetFace(3), Item_006_004);

            MtGeomArgument SubPlaneFlanecTeploobmennaya1(Detail_007->GetFace(3), Item_007_001);
            MtGeomArgument SubPlaneFlanecTeploobmennaya2(Detail_007->GetFace(3), Item_007_002);
            MtGeomArgument SubPlaneFlanecTeploobmennaya3(Detail_007->GetFace(3), Item_007_003);
            MtGeomArgument SubPlaneFlanecTeploobmennaya4(Detail_007->GetFace(3), Item_007_004);


            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya1, SubPlaneFlanecTeploobmennaya1);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya2, SubPlaneFlanecTeploobmennaya2);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya3, SubPlaneFlanecTeploobmennaya3);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya4, SubPlaneFlanecTeploobmennaya4);
        }

        // Совмещение Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument getPlaneOfTubeTeploobmennaya1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument getPlaneOfTubeTeploobmennaya2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument getPlaneOfTubeTeploobmennaya3(Detail_001->GetFace(0), Item_001_003);
            MtGeomArgument getPlaneOfTubeTeploobmennaya4(Detail_001->GetFace(0), Item_001_004);

            MtGeomArgument getPlaneFlanecSpec1(Detail_008->GetFace(12), Item_008_001);
            MtGeomArgument getPlaneFlanecSpec2(Detail_008->GetFace(12), Item_008_002);
            MtGeomArgument getPlaneFlanecSpec3(Detail_008->GetFace(12), Item_008_003);
            MtGeomArgument getPlaneFlanecSpec4(Detail_008->GetFace(12), Item_008_004);


            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya1, getPlaneFlanecSpec1);
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya2, getPlaneFlanecSpec2);
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya3, getPlaneFlanecSpec3);
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya4, getPlaneFlanecSpec4);
        }

        // Концентричность Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya1(Detail_001->GetFace(2), Item_001_001);
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya2(Detail_001->GetFace(2), Item_001_002);
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya3(Detail_001->GetFace(2), Item_001_003);
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya4(Detail_001->GetFace(2), Item_001_004);

            MtGeomArgument subgetPlaneFlanecSpec1(Detail_008->GetFace(3), Item_008_001);
            MtGeomArgument subgetPlaneFlanecSpec2(Detail_008->GetFace(3), Item_008_002);
            MtGeomArgument subgetPlaneFlanecSpec3(Detail_008->GetFace(3), Item_008_003);
            MtGeomArgument subgetPlaneFlanecSpec4(Detail_008->GetFace(3), Item_008_004);


            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya1, subgetPlaneFlanecSpec1);
            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya2, subgetPlaneFlanecSpec2);
            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya3, subgetPlaneFlanecSpec3);
            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya4, subgetPlaneFlanecSpec4);
        }

        //// Совмещение Фланец Специальный - Фланец Специальный (В Камере)
        //{
        //    MtGeomArgument PlaneOfFlanecFirst1(Detail_008->GetFace(2), Item_008_001);
        //    MtGeomArgument PlaneOfFlanecFirst2(Detail_008->GetFace(2), Item_008_002);
        //    MtGeomArgument PlaneOfFlanecFirst3(Detail_008->GetFace(2), Item_008_003);
        //    MtGeomArgument PlaneOfFlanecFirst4(Detail_008->GetFace(2), Item_008_004);

        //    MtGeomArgument PlaneOfFlanecSecond1(Detail_008->GetFace(9), Item_008_001);
        //    MtGeomArgument PlaneOfFlanecSecond2(Detail_008->GetFace(9), Item_008_002);
        //    MtGeomArgument PlaneOfFlanecSecond3(Detail_008->GetFace(9), Item_008_003);
        //    MtGeomArgument PlaneOfFlanecSecond4(Detail_008->GetFace(9), Item_008_004);


        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst1, PlaneOfFlanecSecond1);
        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst2, PlaneOfFlanecSecond2);
        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst3, PlaneOfFlanecSecond3);
        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst4, PlaneOfFlanecSecond4);
        //}

        //// Концентричность Фланец Специальный - Фланец Специальный (В Камере)
        //{
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya1(Detail_006->GetFace(3), Item_006_001);
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya2(Detail_006->GetFace(3), Item_006_002);
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya3(Detail_006->GetFace(3), Item_006_003);
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya4(Detail_006->GetFace(3), Item_006_004);

        //    MtGeomArgument SubPlaneFlanecTeploobmennaya1(Detail_007->GetFace(3), Item_007_001);
        //    MtGeomArgument SubPlaneFlanecTeploobmennaya2(Detail_007->GetFace(3), Item_007_002);
        //    MtGeomArgument SubPlaneFlanecTeploobmennaya3(Detail_007->GetFace(3), Item_007_003);
        //    MtGeomArgument SubPlaneFlanecTeploobmennaya4(Detail_007->GetFace(3), Item_007_004);


        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya1, SubPlaneFlanecTeploobmennaya1);
        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya2, SubPlaneFlanecTeploobmennaya2);
        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya3, SubPlaneFlanecTeploobmennaya3);
        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya4, SubPlaneFlanecTeploobmennaya4);
        //}

        // Совмещение Изогнутая труба - Теплообменные трубы 
        {
            MtGeomArgument PlaneofCurevedTube1(Detail_009->GetFace(0), Item_009_001);
            MtGeomArgument PlaneofCurevedTube2(Detail_009->GetFace(0), Item_009_002);
            MtGeomArgument PlaneofCurevedTube3(Detail_009->GetFace(0), Item_009_003);

            MtGeomArgument PlaneForCoincedent1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument PlaneForCoincedent2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument PlaneForCoincedent3(Detail_001->GetFace(1), Item_001_003);

            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube1, PlaneForCoincedent1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube2, PlaneForCoincedent2);
            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube3, PlaneForCoincedent3);
        }

        // !!! Концентричность Изогнутая труба - Теплообменные трубы 
        {
            /*MtGeomArgument VariableForPlany1(Detail_001->GetFace(2), Item_001_001);
            MtGeomArgument VariableForPlany2(Detail_001->GetFace(2), Item_001_002);
            MtGeomArgument VariableForPlany4(Detail_001->GetFace(2), Item_001_003);
            MtGeomArgument VariableForPlany5(Detail_001->GetFace(2), Item_001_004);

            MtGeomArgument OtvtubeCureved1(Detail_009->GetFace(3), Item_009_001);
            MtGeomArgument OtvtubeCureved2(Detail_009->GetFace(3), Item_009_002);
            MtGeomArgument OtvtubeCureved4(Detail_009->GetFace(2), Item_009_001);
            MtGeomArgument OtvtubeCureved5(Detail_009->GetFace(2), Item_009_002);

            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved1, VariableForPlany1);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved2, VariableForPlany2);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved4, VariableForPlany4);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved5, VariableForPlany5);*/

            MtGeomArgument VariableForPlany6(Detail_001->GetFace(1), Item_001_002);
            MtGeomArgument OtvtubeCureved6(Detail_009->GetFace(0), Item_009_003);
            assm->AddConstraint(GCM_COINCIDENT, OtvtubeCureved6, VariableForPlany6, MtParVariant(GCM_OPPOSITE));

            MtGeomArgument VariableForPlany3(Detail_003->GetFace(36), Item_003_001);
            MtGeomArgument OtvtubeCureved3(Detail_009->GetFace(5), Item_009_003);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved3, VariableForPlany3);
        }
    }

    assm->EvaluateConstraints();

	return assm;
}