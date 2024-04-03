#pragma once

// Level List
void CreateLumiaIsland();

// Loding

// Lumia
class CLevel;
void SetLayer(CLevel* _Level);
void SetCamera();
void SetLight();
void SetMapCollider();
void CreateTestPlayer();
void CreateTestEnemy();

void LumiaIsland();

void Create_Archery();
void Create_Forest();
void Create_Hotel();
void Create_SandyBeach();
void Create_School();
void Create_Uptown();

void TestObject();

// func
class CGameObject;
void SetRot_x(CGameObject* _obj, float _x);
void SetRot_y(CGameObject* _obj, float _y);
void SetRot_z(CGameObject* _obj, float _z);

void XMConv_Rot_X(CGameObject* _obj, float _x);
void XMConv_Rot_Y(CGameObject* _obj, float _y);
void XMConv_Rot_Z(CGameObject* _obj, float _z);

void XMConv_Rot_XYZ(CGameObject* _obj, float _x, float _y, float _z);
