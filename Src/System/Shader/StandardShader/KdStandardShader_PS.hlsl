#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

// テクスチャ
Texture2D g_baseTex : register(t0);         // ベースカラーテクスチャ
Texture2D g_emissiveTex : register(t1);     // エミッシブテクスチャ
Texture2D g_mrTex : register(t2);            // メタリック/ラフネステクスチャ

// 法線マップ
Texture2D g_normalTex : register(t3);

// 光学迷彩
Texture2D g_screenTex : register(t4);

// サンプラ
SamplerState g_ss : register(s0);

/*
// UnityのBuild-In Shaderでの手法
// RoughnessからSpecularPowerを算出
float RoughnessToSpecPower(float roughness)
{
    float trueRoughness = roughness * roughness; // 学術的に正しいラフネスを求める
    float sq = max(0.0001, trueRoughness * trueRoughness);
    float n = (2.0 / sq) - 2.0;
    // Roughnessが1.0でNdotHがゼロの場合に発生する可能性のあるpow(0,0)のケースを防止
    n = max(n, 0.0001);
    return n;
}
*/

// BlinnPhong NDF
// ・lightDir    … ライトの方向
// ・vCam        … ピクセルからカメラへの方向
// ・normal      … 法線
// ・specPower   … 反射の鋭さ
float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
    float3 H = normalize(-lightDir + vCam);
    float NdotH = saturate(dot(normal, H)); // カメラの角度差(0～1)
    float spec = pow(NdotH, specPower);

    // 正規化Blinn-Phong
    return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//================================
// ピクセルシェーダ
//================================
float4 main(VSOutput In) : SV_Target0
{
    // カメラへの方向
    float3 vCam = g_CamPos - In.wPos;
    float camDist = length(vCam);       // カメラ - ピクセル距離
    vCam = normalize(vCam);

    // 法線正規化
    // 法線正規化
    float3 wN = 0;
    if (g_NormalEnable)
    {
        //法線マップあり
        //テクスチャの色を取得
     //   wN = g_normalTex.Sample(g_ss, In.UV+g_NormalUVOffset);
        float3 n1 = g_normalTex.Sample(g_ss, In.UV + g_NormalUVOffset);
        float3 n2 = g_normalTex.Sample(g_ss, In.UV - g_NormalUVOffset);
        n1 = n1 * 2 - 1;
        n2 = n2 * 2 - 1;
        //0~255(rbg) 0~1→　-1~1変更
     //   wN = wN * 2 - 1;
        //ベクトルを行列で変形
     //   wN = mul(wN, (float3x3)g_mR); //　wn float3の変数 g_mRはfloat4x4 3x3にキャスト
        wN = mul(n1 + n2, (float3x3) g_mR);
        //正規化
        wN = normalize(wN);
        
    }
    else
    {
        //法線マップなし
        wN = normalize(In.wN);
    }

    // 光学迷彩
    if (g_OpticalCamoEnable)
    {
        // テクスチャのサイズ取得
        float w = 0;
        float h = 0;
        
        g_screenTex.GetDimensions(w, h); //1280 720
        
        // 法線からズレを算出
        float2 offset = wN.xy / 50;
        
        // キャラの表示位置のテクスチャ座標をUV座標に変換
        float2 uv = float2(In.Pos.x / w, In.Pos.y / h); // 640/1280 360/720==> UV座標
        
        // テクスチャから色を取得
        float4 col = g_screenTex.Sample(g_ss, uv + offset);
        
        // 少し緑にする
        col.g *= 1.05;
        
        return col;
        
    }

    //------------------------------------------
    // 材質色
    //------------------------------------------

    float4 mr = g_mrTex.Sample(g_ss, In.UV);
    // 金属性
    float metallic = mr.b * g_Material.Metallic;
    // 粗さ
    float roughness = mr.g * g_Material.Roughness;


    // 材質の色
    float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor * In.Color;
    
    // 現す高さ
    
    if (g_AppearEnable)
    {
        if (In.wPos.y > g_AppearHeight)
        {
            discard;
        }
     
    }
    
    

    // アルファテスト
    if (baseColor.a <= g_AlphaTestThreshold)discard;

    //------------------------------------------
    //
    // ライティング
    //
    //------------------------------------------
    // 最終的な色
    float3 color = 0;

    // ライト有効時
	if (g_LightEnable)
	{
		// 材質の拡散色　非金属ほど材質の色になり、金属ほど拡散色は無くなる
		const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
		// 材質の反射色　非金属ほど光の色をそのまま反射し、金属ほど材質の色が乗る
		const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

		//------------------
		// 平行光
		//------------------

		// Diffuse(拡散光) 正規化Lambertを使用
		{
			// 光の方向と法線の方向との角度さが光の強さになる
			float lightDiffuse = dot(-g_DL_Dir, wN);
			lightDiffuse = saturate(lightDiffuse);  // マイナス値は0にする　0(暗)～1(明)になる

            //// トゥーンレンダリング
            if (lightDiffuse < 0.2)
            {
                lightDiffuse = 0.2;
            }
            //else if(lightDiffuse < 0.4)
            //{
            //    lightDiffuse = 0.4;
            //}else if(lightDiffuse < 0.6)
            //{
            //    lightDiffuse = 0.6;
            //}else if(lightDiffuse < 0.8)
            //{
            //    lightDiffuse = 0.8;
            //}else
            //{
            //    lightDiffuse = 1;
            //}
            
			// 正規化Lambert
			lightDiffuse /= 3.1415926535;

			// 光の色 * 材質の拡散色
			color += (g_DL_Color * lightDiffuse) * baseDiffuse * g_Material.BaseColor.a;
		}

		// Specular(反射色) 正規化Blinn-Phong NDFを使用

			// 反射した光の強さを求める

			// ラフネスから、Blinn-Phong用のSpecularPowerを求める
			// Call of Duty : Black OpsやFarCry3でのでの手法を使用
			// specPower = 2^(13*g)　グロス値g = 0～1で1～8192のスペキュラ強度を求める
			// 参考：https://hanecci.hatenadiary.org/entry/20130511/p1
		float smoothness = 1.0 - roughness;         // ラフネスを逆転させ「滑らか」さにする
		float specPower = pow(2, 13 * smoothness);  // 1～8192

		// UnityのBuild-In Shaderでの手法
		//float spec = RoughnessToSpecPower(roughness);
		{
			// Blinn-Phong NDF
			float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);
            
            spec = step(1,spec);

			// 光の色 * 反射光の強さ * 材質の反射色 * 正規化係数
			color += (g_DL_Color * spec) * baseSpecular * g_Material.BaseColor.a;
		}


        //------------------
        // 点光
        //------------------
		{
			for (int pi = 0; pi < g_PL_Num; pi++)
			{

				float3 PL_Dir;

				//点光源に向いたベクトル
				PL_Dir = g_PL[pi].pos - In.wPos;

				float  len;
				//点光源の距離
				len = length(PL_Dir);

				//効果半径かどうか
				if (len < g_PL[pi].radius)
				{

					//点光源の方向をnormalize
					PL_Dir = normalize(PL_Dir);

					float  lightDiffuse;
					float  lightAttenuation;
					//拡散
					lightDiffuse = saturate(dot(normalize(wN), PL_Dir));
					// 正規化Lambert
					lightDiffuse /= 3.1415926535;

					//減衰率
					lightAttenuation = saturate(1.0f - (len / g_PL[pi].radius));

					//ディフューズ減衰
					lightDiffuse *= lightAttenuation;

					color += g_PL[pi].color * lightDiffuse * baseDiffuse * g_Material.BaseColor.a;

					//スペキュラ
					float spec = BlinnPhong(-PL_Dir, vCam, wN, specPower);
					spec *= lightAttenuation;
					color += (g_PL[pi].color * spec) * baseSpecular * g_Material.BaseColor.a;
				}
			}
		}

        //------------------
        // 環境光
        //------------------
        color += g_AmbientLight * baseColor.rgb * g_Material.BaseColor.a;

        //------------------
        // エミッシブ
        //------------------
        color += g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Material.Emissive * g_Material.BaseColor.a;
    }
    // ライト無効時
    else
    {
        // 材質の色をそのまま使用
        color = baseColor.rgb * g_Material.BaseColor.a;
    }

    //------------------------------------------
    // 距離フォグ
    //------------------------------------------
    if (g_DistanceFogEnable)
    {
		// フォグ 1(近い)～0(遠い)
        float f = saturate(1.0 / exp(camDist * g_DistanceFogDensity));
        // 適用
        color.rgb = lerp(g_DistanceFogColor, color.rgb, f);
    }

    
    //------------------------------------------
    // 出力
    //------------------------------------------
    return float4(color, baseColor.a);

}
