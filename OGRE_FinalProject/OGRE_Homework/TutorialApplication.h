#pragma once
/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
___                 __    __ _ _    _
/___\__ _ _ __ ___  / / /\ \ (_) | _(_)
//  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
|___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_


#include <OgreRoot.h>
#include <OgreRectangle2D.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreWindowEventUtilities.h>
#include <OgreEntity.h>
#include <OgrePrerequisites.h>
#include <OgreMeshManager.h>
#include <OgreColourValue.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreRenderQueueListener.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <OgreSingleton.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>



using namespace std;

class TutorialApplication : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
	TutorialApplication();
	virtual ~TutorialApplication();

	void get_Distance();
	bool go();
	

protected:

	Ogre::Radian angle;
	Ogre::Real Distance;
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	Ogre::OverlayManager* mOverlayMgr;
	Ogre::Overlay* mTextOverlay;
	Ogre::OverlayContainer *mPanel;

	//스테이지2 물고기 목숨2개
	int KillEnemy_2[3];

	//애니메이션
	Ogre::AnimationState* mAnimationState; // 펭귄
	Ogre::AnimationState* mAnimationState2; // 펭귄
	Ogre::AnimationState* EnemyAnimationState[3]; // 물고기
	Ogre::AnimationState* EnemyAnimationState_2[3]; // 스테이지2 물고기
	
	//파티클
	Ogre::ParticleSystem* enemyeffect[3]; // 물고기파티클
	Ogre::ParticleSystem* enemyeffect_2[3]; // 스테이지2 물고기파티클
	Ogre::ParticleSystem* snow; //눈
	Ogre::ParticleSystem* snow2; //눈	
	Ogre::ParticleSystem* snow3; //눈		
	Ogre::ParticleSystem* fireworkeffect; //불꽃놀이
	bool firework; //불꽃놀이


	//라벨출력을위한 매니저들
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::InputContext mInputContext;
	Ogre::OverlaySystem* mOverlaySystem;
	OgreBites::SdkCameraMan* mCameraMan;  

	//라벨들
	OgreBites::Label * StageLabel;
	OgreBites::Label * BulletLabel;
	OgreBites::TextBox * Quest;
	

	Ogre::RenderWindow* mWindow;
	//Game 씬
	Ogre::SceneManager* mSceneMgr;
	//시작화면 씬
	Ogre::SceneManager* StartSceneMgr;
	//종료화면 씬
	Ogre::SceneManager* OverSceneMgr;
	//클리어화면 씬
	Ogre::SceneManager* ClearSceneMgr;

	//시작화면 2d백그라운드
	Ogre::MaterialPtr Startmaterial;
	Ogre::Rectangle2D* Startrect;

	//오버화면 2d백그라운드
	Ogre::MaterialPtr Overmaterial;
	Ogre::Rectangle2D* Overrect;

	//클리어화면 2d백그라운드
	Ogre::MaterialPtr Clearmaterial;
	Ogre::Rectangle2D* Clearrect;

	//클리어화면 2d백그라운드

	//카메라
	Ogre::Camera* mCamera;
	Ogre::Camera* StartCamera;
	Ogre::Camera* OverCamera;
	Ogre::Camera* ClearCamera;

	//OIS들
	OIS::InputManager* mInputMgr;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::MouseListener* mListner;

	//바운딩박스
	Ogre::AxisAlignedBox CartBox;
	Ogre::AxisAlignedBox NPC1Box;
	Ogre::AxisAlignedBox NPC2Box;
	Ogre::AxisAlignedBox EnemyBox[3];
	Ogre::AxisAlignedBox EnemyBox_2[3];
	Ogre::AxisAlignedBox WallBox;
	Ogre::AxisAlignedBox UFOBox;


	// 파티클 이펙트 일정시간만 나오게 하기 위해
	Ogre::Real Time[6];
	Ogre::Real Time2;
	bool fenemyeffect[3];
	bool fenemyeffect_2[3];
	bool fsnow;
	void fTime(Ogre::Real time, Ogre::Real time2, int i);



	// NPC와의 상호작용을 위한 BOOL변수들
	bool NPC1touch;
	bool NPC2touch;
	bool DestroyNpc1;
	bool DestroyNpc2;

	bool DestroyWall;
	void NPCFollow();



	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);


	//장면 전환을 위해
	virtual void CreateSceneManager(void);
	virtual void CreateCamera(void);
	virtual void CreateViewports(void);
	virtual void createFrameListner(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	virtual void SetupViewport(Ogre::SceneManager *curr);

	//리소스 불러오는 함수
	void setUpResources(void);
	bool configure(void);
	void loadResources(void);

	// 현재 scene의 이름, start, game, over, clear예정
	const char* CurrentScene;

	//스테이지1 Patrol
	Ogre::Vector3 pDistance[3];
	//스테이지2 Patrol
	Ogre::Vector3 pDistance_2[3];

	// Patrol 함수
	void Patrol(Ogre::Vector3 pDistance, Ogre::Vector3 EnemyLocation, Ogre::Vector3 EnemyVector, Ogre::Real eMove,Ogre::Real time, Ogre::SceneNode* Node, int eNum);
	void Patrol2(Ogre::Vector3 pDistance, Ogre::Vector3 EnemyLocation, Ogre::Vector3 EnemyVector, Ogre::Real eMove,Ogre::Real time, Ogre::SceneNode* Node, int eNum);

	// Input 구현을 위해
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);
	virtual bool processUnbufferInput(const Ogre::FrameEvent& evt);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	//Start화면, Over화면, clear화면 2D 배경 이미지 띄우기
	virtual void ShowStart2Dbackground();
	virtual void ShowOver2Dbackground();
	virtual void ShowClear2Dbackground();

	//총알
	struct BULLET
	{
		Ogre::Entity *bEntity;
		Ogre::SceneNode *bSceneNode;
		Ogre::AxisAlignedBox bBox;

	};
	//총알갯수
	int nBullet;
	// 스테이지2 앞의 NewBullet
	Ogre::AxisAlignedBox NewBulletBox;
	bool DestroyNewBullet;

	// 총알들을 저장하는 list
	std::list<BULLET *>m_listBullet;

	// 총알들의 씬노드들을 저장하는 list
	std::list<Ogre::SceneNode *>m_listObject;
	
	// 총알 생성함수
	void CreateBullet();
	// 총알 이동, 삭제 함수
	void UpdateBullet(float fTime);



	//물고기 patrol
	bool DestroyEnemy[3];
	bool DestroyEnemy_2[3];

	void InitPatrol(int number);
	void InitPatrol2(int number);

	bool Patrolreach_x[3];
	bool Patrolreach_y[3];

	bool Patrolreach_x_2[3];
	bool Patrolreach_y_2[3];

	Ogre::Vector3 PatrolLocation[3];	
	Ogre::Vector3 EnemyLocation[3];

	Ogre::Vector3 PatrolLocation_2[3];	
	Ogre::Vector3 EnemyLocation_2[3];

	Ogre::Vector3 p[3];

	int Range(int a, int b);
	int a;

	//퀘스트창
	bool setOverlayCaption(const Ogre::String &elementName, const Ogre::String &caption);
	bool showOverlay(const Ogre::String &overlayName);
	bool ShowQuest;
	Ogre::Overlay* pOver;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
