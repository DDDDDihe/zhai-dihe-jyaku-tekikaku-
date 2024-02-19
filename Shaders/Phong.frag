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

//���s�����p�ɍ\���̂����
struct DirectionalLight
{
	//���̕���
	vec3 mDirection;
	//�g�U���ːF
	vec3 mDiffuseColor;
	//���ʔ��ːF
	vec3 mSpecColor;
};

//���C�e�B���O�puniform
//�J�����̈ʒu(���[���h���)
uniform vec3 uCameraPos;
//���ʌ��̋���
uniform float uSpecPower;
//�����̋���
uniform vec3 uAmbientLight;

//���s����
uniform DirectionalLight uDirLight;

void main()
{
	//�\�ʖ@��N
	vec3 N = normalize(fragNormal);
	//�\�ʂ�������ւ̃x�N�g��L
	vec3 L = normalize(-uDirLight.mDirection);
	//�\�ʂ���J�����ւ̃x�N�g��V
	vec3 V = normalize(uCameraPos - fragWorldPos);
	//N�Ɋւ���]L�̔���R
	vec3 R = normalize(reflect(-L, N));

	//�t�H���̔��˂��v�Z����
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	//�ŏI�I�ȐF�̓e�N�X�`���̐Fx�t�H���̌��@alpha = 1
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}