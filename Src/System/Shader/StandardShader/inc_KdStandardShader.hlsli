
//===========================================
//
// 定数バッファ
//  ゲームプログラム側から持ってくるデータ
//
//===========================================

// 定数バッファ(オブジェクト単位更新)
cbuffer cbObject : register(b0)
{
    // オブジェクト情報
    row_major float4x4 g_mW; // ワールド変換行列

    // UV操作
    float2  g_UVOffset;
    float2  g_UVTiling;

    // ライト有効
    int     g_LightEnable;
    // アルファテストの閾値
    float   g_AlphaTestThreshold;    
    
     //法線UV操作
    float2 g_NormalUVOffset;
    
     //　90度回転行列
    float4x4 g_mR;
    
    //　法線マップ有効
    int g_NormalEnable;
    
    // モデルを現す
    int g_AppearEnable;
    float g_AppearHeight;
    
    // 光学迷彩有効化
    int g_OpticalCamoEnable;
};

//------------------------------
// 定数バッファ(マテリアル)
//------------------------------

// マテリアル構造体
struct Material
{
    float4 BaseColor;   // ベース色
    float3 Emissive;    // 自己発光色
    float Metallic;     // 金属度
    float Roughness;    // 粗さ
};

cbuffer cbMaterial : register(b1)
{
    Material g_Material;
};

// 頂点シェーダから出力するデータ
struct VSOutput
{
    float4 Pos : SV_Position;   // 射影座標
    float2 UV : TEXCOORD0;      // UV座標
    float3 wN : TEXCOORD1;      // ワールド法線
    float4 Color : TEXCOORD2;   // 色
    float3 wPos : TEXCOORD3;    // ワールド3D座標
};
