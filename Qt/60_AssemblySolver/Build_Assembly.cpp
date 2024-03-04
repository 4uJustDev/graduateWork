#include "BuildMathModel.h"

using namespace BuildMathModel;

SPtr<MbAssembly> ParametricModelCreator::CreatePneumocylinderAssembly(BuildParams params)
{
#pragma region PARAMS
    
    double param_length = params.length;
   
#pragma endregion

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
    {
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
    }
    assm->EvaluateConstraints();

	return assm;
}