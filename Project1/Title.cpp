#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {

    
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    newObjectSystem = LoadModel_ObjFile("player");

    scale1 = { 5.0,5.0,5.0 };
    rot1 = { 0,0,0 };
    pos1 = { -50,0,0 };

    scale2 = { 10.0,10.0,10.0 };
    rot2 = { 0,90,0 };
    pos2 = { 50,0,0 };

    tiletex = TexManager::LoadTexture("Resources/grif.png");
    tileObject = NY_Object3DManager::Get()->CreateModel_Tile(100, 100, 25, 25, tiletex);
    tileObject->color = DirectX::XMFLOAT4(1, 1, 1, 0.5);
    tileObject->SetAffineParam(RVector3(1, 1, 1), RVector3(90, 0, 0), RVector3(0, 0, 0));

    //NY_Object3DManager::Get()->SetCamera(&cam);
    //model1.LoadObjModel("player");
    //model2.CreateBoxModel(2.5, 1, 1, TexManager::LoadTexture("Resources/warning.png"));
    //grid.CreatePlaneModelXY(50, 50, 10, 10, TexManager::LoadTexture("Resources/grif.png"),nullptr);
    //UINT animtex = TexManager::LoadDivTextureTest(&testData, "Resources/animation_test.png", 5, 32);
    //animModel.CreatePlaneModelXY(10, 10, 1, 1, animtex, &testData);
    //obj1 = CreateObject3d(&model1, RVector3(0, 0, 0));
    //obj2 = CreateObject3d(&model2, RVector3(10, 0, 0));
    //gmodel = CreateObject3d(&grid, RVector3(0, 0, 10));
    //obj1->scale = scale;
    //animTestModel = CreateObject3d(&animModel, RVector3(-7.5f, 0, -2.0f));
    //animationTime = 0;

    particle1 = ParticleManager::Create();
    bomProto = new BomParticle(RVector3(0, 0, 0));
    //プロトタイプをセット
    particle1->Prototype_Set(bomProto);

    //パーティクルのテクスチャ
    ptex = TexManager::LoadTexture("Resources/effect1.png");
    //スポーン座標
    //p1state.spawnPos = RVector3(50, 40, 0);
    //p1state.isRandomSpawn = true;
    //p1state.spawnRange1 = RVector3(15, 15, 15);
    //p1state.spawnRange2 = RVector3(-15, -15, -15);
    ////速度
    //p1state.vel = RVector3(0, 0, 0);
    //p1state.isRandomVelocity = true;
    //p1state.velRange1 = RVector3(0.5, 0.3, 0.5);
    //p1state.velRange2 = RVector3(-0.5, 0.1, -0.5);
    ////加速度
    //p1state.acc = RVector3(0, -0.01, 0);
    //p1state.isRandomAccel = false;
    ////色
    //p1state.color = DirectX::XMFLOAT4(1, 1, 1, 1);
    //p1state.endColor = DirectX::XMFLOAT4(1, 1, 1, 0.1);
    //p1state.isRandomColor = false;
    ////スケーリング
    //p1state.scale = 5.0f;
    //p1state.endScale = 1.0f;
    //p1state.isRandomScale = false;
    ////回転
    //p1state.rot = 0.0f;
    //p1state.startRotate = 0.0f;
    //p1state.endRotate = 0.0f;
    ////寿命
    //p1state.activeTime = 90;

    //mpRenderSp.CreateSprite({ 1280,720 }, 0, false, nullptr);
    //mpRenderSp.spdata.position = { 0,0,0 };
    //mpRenderSp.UpdateSprite();

}

//初期化
void Title::Initialize() {
    /// <summary>
    /// フィールド管理部初期化
    /// </summary>

    //StageMoveParticle::Get()->Init(&cam);
}

void Title::Finalize()
{
    //DeleteObject3d(obj1);
    //DeleteObject3d(obj2);
    //DeleteObject3d(gmodel);
    //DeleteObject3d(animTestModel);

    DeleteObject(newObjectSystem);
    DeleteObject(tileObject);

    delete particle1;
    delete bomProto;
}

//更新
void Title::Update() {
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    if (Input::isKey(DIK_W)) { eye.y += 0.3; }
    if (Input::isKey(DIK_S)) { eye.y -= 0.3; }
    if (Input::isKey(DIK_D)) { eye.x += 0.3; }
    if (Input::isKey(DIK_A)) { eye.x -= 0.3; }

    NY_Object3DManager::Get()->UpdateAllObjects();

    particle1->Prototype_Add(3, RVector3(0, 10, 0));

    //パーティクル生成
    //pSpawnDelay++;
    //if (pSpawnDelay > PSPAWN_DELAY_TIME) {
    //    pSpawnDelay = 5;
    //    for (int i = 0; i < pSpawnNum; i++) {
    //        //パーティクル生成
    //        particle1->Add(p1state);
    //    }
    //}
    particle1->Prototype_Update();

    //animationTime++;
    //if (animationTime > ANIM_TIME) {
    //    animationTime = 0;
    //    testData.usingNo++;
    //    cursorAnim.usingNo++;
    //    if (testData.usingNo > 4) {
    //        testData.usingNo = 0;
    //        cursorAnim.usingNo = 0;
    //    }
    //}


    //Object3dのテスト
    pos1.x -= 0.1;
    newObjectSystem->SetAffineParam(scale1, rot1, pos1);


    //if (mpRenderSp.spdata.color.w > 1.0f) {
    //    isadd = false;
    //    mpRenderSp.spdata.color.w = 1.0f;
    //}
    //else if(mpRenderSp.spdata.color.w < 0.0f){
    //    isadd = true;
    //    mpRenderSp.spdata.color.w = 0.0f;
    //}

    //if (isadd) {
    //    mpRenderSp.spdata.color.w += 0.001;
    //}
    //else {
    //    mpRenderSp.spdata.color.w -= 0.001;
    //}

    //mpRenderSp.UpdateSprite();

}

//描画
void Title::Draw() {

    //背景に常にいる
    Raki_DX12B::Get()->StartDrawRenderTarget();

    Raki_DX12B::Get()->StartDrawBackbuffer();

    //描画1回目
    newObjectSystem->DrawObject();
    tileObject->DrawObject();
    //パーティクル描画
    particle1->Prototype_Draw(ptex);

    //ImguiMgr::Get()->StartDrawImgui("ParticleParam", 200, 700);

    //ImGui::Checkbox("isRandomSpawn", &p1state.isRandomSpawn);
    //
    //float sliderstate[3];
    //sliderstate[0] = p1state.spawnRange1.x;
    //sliderstate[1] = p1state.spawnRange1.y;
    //sliderstate[2] = p1state.spawnRange1.z;
    //ImGui::SliderFloat3("spawn range 1", sliderstate, -100.0, 100.0, "%.3f",0.01f);
    //p1state.spawnRange1.x = sliderstate[0];
    //p1state.spawnRange1.y = sliderstate[1];
    //p1state.spawnRange1.z = sliderstate[2];

    //sliderstate[0] = p1state.spawnRange2.x;
    //sliderstate[1] = p1state.spawnRange2.y;
    //sliderstate[2] = p1state.spawnRange2.z;
    //ImGui::SliderFloat3("spawn range 2", sliderstate, -100.0, 100.0);
    //p1state.spawnRange2.x = sliderstate[0];
    //p1state.spawnRange2.y = sliderstate[1];
    //p1state.spawnRange2.z = sliderstate[2];

    //ImGui::Checkbox("isRandomVel", &p1state.isRandomVelocity);
    //sliderstate[0] = p1state.velRange1.x;
    //sliderstate[1] = p1state.velRange1.y;
    //sliderstate[2] = p1state.velRange1.z;
    //ImGui::SliderFloat3("vel range 1", sliderstate, -10.0, 10.0);
    //p1state.velRange1.x = sliderstate[0];
    //p1state.velRange1.y = sliderstate[1];
    //p1state.velRange1.z = sliderstate[2];

    //sliderstate[0] = p1state.velRange2.x;
    //sliderstate[1] = p1state.velRange2.y;
    //sliderstate[2] = p1state.velRange2.z;
    //ImGui::SliderFloat3("vel range 2", sliderstate, -10.0, 10.0);
    //p1state.velRange2.x = sliderstate[0];
    //p1state.velRange2.y = sliderstate[1];
    //p1state.velRange2.z = sliderstate[2];

    //ImGui::Checkbox("isRandomAcc", &p1state.isRandomAccel);
    //if (p1state.isRandomAccel) {
    //    sliderstate[0] = p1state.accRange1.x;
    //    sliderstate[1] = p1state.accRange1.y;
    //    sliderstate[2] = p1state.accRange1.z;
    //    ImGui::SliderFloat3("accel range 1", sliderstate, -5.0, 5.0);
    //    p1state.accRange1.x = sliderstate[0];
    //    p1state.accRange1.y = sliderstate[1];
    //    p1state.accRange1.z = sliderstate[2];

    //    sliderstate[0] = p1state.accRange2.x;
    //    sliderstate[1] = p1state.accRange2.y;
    //    sliderstate[2] = p1state.accRange2.z;
    //    ImGui::SliderFloat3("accel range 2", sliderstate, -5.0, 5.0);
    //    p1state.accRange2.x = sliderstate[0];
    //    p1state.accRange2.y = sliderstate[1];
    //    p1state.accRange2.z = sliderstate[2];
    //}
    //else {
    //    sliderstate[0] = p1state.acc.x;
    //    sliderstate[1] = p1state.acc.y;
    //    sliderstate[2] = p1state.acc.z;
    //    ImGui::SliderFloat3("accel", sliderstate, -5.0, 5.0);
    //    p1state.acc.x = sliderstate[0];
    //    p1state.acc.y = sliderstate[1];
    //    p1state.acc.z = sliderstate[2];
    //}

    //ImGui::Text("Control color select");
    //ImGui::RadioButton("start color", &isControlColor1,0);
    //ImGui::SameLine();
    //ImGui::RadioButton("end color", &isControlColor1,1);

    //float colorstate[4];
    //if (isControlColor1 == 0)
    //{
    //    colorstate[0] = p1state.color.x;
    //    colorstate[1] = p1state.color.y;
    //    colorstate[2] = p1state.color.z;
    //    colorstate[3] = p1state.color.w;
    //    ImGui::ColorPicker4("Particle Color 4", colorstate);
    //    p1state.color.x = colorstate[0];
    //    p1state.color.y = colorstate[1];
    //    p1state.color.z = colorstate[2];
    //    p1state.color.w = colorstate[3];
    //}
    //else if(isControlColor1 == 1)
    //{
    //    colorstate[0] = p1state.endColor.x;
    //    colorstate[1] = p1state.endColor.y;
    //    colorstate[2] = p1state.endColor.z;
    //    colorstate[3] = p1state.endColor.w;
    //    ImGui::ColorPicker4("Particle Color 4", colorstate);
    //    p1state.endColor.x = colorstate[0];
    //    p1state.endColor.y = colorstate[1];
    //    p1state.endColor.w = colorstate[3];
    //    p1state.endColor.z = colorstate[2];
    //}

    //float scaling[2];
    //scaling[0] = p1state.scale;
    //scaling[1] = p1state.endScale;
    //ImGui::SliderFloat2("scaling   start : end", scaling, -50.0f, 50.0f);
    //p1state.scale = scaling[0];
    //p1state.endScale = scaling[1];

    //int activeTime = p1state.activeTime;
    //ImGui::SliderInt("active time", &activeTime, 0, 180);
    //p1state.activeTime = activeTime;

    //ImGui::Text("\n");
    //ImGui::SliderInt("Spawn delay", &PSPAWN_DELAY_TIME, 0, 300);
    //ImGui::SliderInt("Spawn num", &pSpawnNum, 1, 10);

    //ImguiMgr::Get()->EndDrawImgui();

    //描画終了
    Raki_DX12B::Get()->CloseDraw();

}
