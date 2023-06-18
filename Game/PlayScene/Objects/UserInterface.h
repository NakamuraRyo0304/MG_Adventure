/*
 *	@File	UserInterface.h
 *	@Brief	UIÇÃï\é¶ÅB
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE

class DrawSprite;
class UserInterface
{
private:
	std::unique_ptr<DrawSprite> m_drawSprite;
public:
	UserInterface();
	~UserInterface();

	void Update(float elapsedTime);
	void Render();


};

#endif // USERINTERFACE