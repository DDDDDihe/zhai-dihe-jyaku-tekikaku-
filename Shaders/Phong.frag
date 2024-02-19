// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

//平行光源用に構造体を作る
struct DirectionalLight
{
	//光の方向
	vec3 mDirection;
	//拡散反射色
	vec3 mDiffuseColor;
	//鏡面反射色
	vec3 mSpecColor;
};

//ライティング用uniform
//カメラの位置(ワールド空間)
uniform vec3 uCameraPos;
//鏡面光の強さ
uniform float uSpecPower;
//環境光の強さ
uniform vec3 uAmbientLight;

//平行光源
uniform DirectionalLight uDirLight;

void main()
{
	//表面法線N
	vec3 N = normalize(fragNormal);
	//表面から光源へのベクトルL
	vec3 L = normalize(-uDirLight.mDirection);
	//表面からカメラへのベクトルV
	vec3 V = normalize(uCameraPos - fragWorldPos);
	//Nに関する‐Lの反射R
	vec3 R = normalize(reflect(-L, N));

	//フォンの反射を計算する
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	//最終的な色はテクスチャの色xフォンの光　alpha = 1
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
