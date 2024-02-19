// Request GLSL 3.3
#version 330

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;
//法線
out vec3 fragNormal;
//位置
out vec3 fragWorldPos;

void main()
{
	//位置を同次座標系に変換する
	vec4 pos = vec4(inPosition, 1.0);
	//位置をワールド空間に変換する
	pos = pos * uWorldTransform;
	//ワールド空間の位置を保存
	fragWorldPos = pos.xyz;
	//クリップ空間に変換する
	gl_Position = pos * uViewProj;

	//法線をワールド空間に変換（w=0）
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	//テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}
