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
//�@��
out vec3 fragNormal;
//�ʒu
out vec3 fragWorldPos;

void main()
{
	//�ʒu�𓯎����W�n�ɕϊ�����
	vec4 pos = vec4(inPosition, 1.0);
	//�ʒu�����[���h��Ԃɕϊ�����
	pos = pos * uWorldTransform;
	//���[���h��Ԃ̈ʒu��ۑ�
	fragWorldPos = pos.xyz;
	//�N���b�v��Ԃɕϊ�����
	gl_Position = pos * uViewProj;

	//�@�������[���h��Ԃɕϊ��iw=0�j
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	//�e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}