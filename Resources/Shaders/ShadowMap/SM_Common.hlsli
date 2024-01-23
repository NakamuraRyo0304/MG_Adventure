/*
 *  @File   SM_Common.hlsli
 *  @Brief  �V���h�E�}�b�v�ɕK�v�ȃZ�}���e�B�N�X���܂Ƃ߂��w�b�_�[�B
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

// DXTK�̃p�����[�^�[�w�b�_�[
#include "ParametersHeader.hlsli"

// �V���h�E�}�b�v�Ɏg�p����e�N�X�`���ƃT���v���[
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

// �萔�o�b�t�@
cbuffer Parameters : register(b1)
{
    float4x4 LightViewProj          : packoffset(c0);   // ���C�g�̃r���[�v���W�F�N�V�����s��
    float4   LightPos               : packoffset(c4);   // ���C�g�̈ʒu
    float3   LightAmbient           : packoffset(c5);   // ���C�g�̃A���r�G���g�J���[
};
