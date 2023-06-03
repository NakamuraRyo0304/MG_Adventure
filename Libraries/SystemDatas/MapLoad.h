/*
 *	@File	MapLoad.h
 *	@Brief	�}�b�v��ǂݍ��݁B
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

#include<iostream>
#include<sstream>
#include<regex>
#include<fstream>
#include<Windows.h>

class MapLoad
{
public:
	// ���̒���
	const static int MAP_COLUMN = 15;
	// �c�̒���
	const static int MAP_RAW = 15;

	// �ǂݍ��ރt�@�C���̖��O���w��
	const wchar_t* m_fileName;

private:
	// �}�b�v�f�[�^�i�[�p�z��
	int m_mapData[MAP_RAW][MAP_COLUMN];

private:
	// �}�b�v�̓ǂݍ���(�Z�b�g������MapLoad���ŏ��������֐�)
	void LoadMap();

public:

	MapLoad();
	~MapLoad() = default;
	// �}�b�v�̃f�[�^��n��
	int GetMapData(int x, int y);

	// �t�@�C�����Z�b�^�[
	void SetMapData(const wchar_t* filename);
};

#endif // MAPLOAD