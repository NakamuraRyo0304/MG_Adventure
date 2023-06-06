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
#include<ostream>
#include<fstream>
#include<Windows.h>

class MapLoad
{
public:
	// ���̒���
	const static int MAP_COLUMN = 15;
	// �c�̒���
	const static int MAP_RAW = 15;

private:
	// �}�b�v�f�[�^�i�[�p�z��
	int m_mapData[MAP_RAW][MAP_COLUMN];
public:

	MapLoad();
	~MapLoad() = default;

	// �}�b�v�̓ǂݍ���
	void LoadMap(const wchar_t* filename);

	void WriteMap(const wchar_t* filename);

	// �}�b�v�̃f�[�^��n��
	int GetMapData(int x, int y);
};

#endif // MAPLOAD