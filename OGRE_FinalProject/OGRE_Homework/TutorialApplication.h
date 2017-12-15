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

	//��������2 ����� ���2��
	int KillEnemy_2[3];

	//�ִϸ��̼�
	Ogre::AnimationState* mAnimationState; // ���
	Ogre::AnimationState* mAnimationState2; // ���
	Ogre::AnimationState* EnemyAnimationState[3]; // �����
	Ogre::AnimationState* EnemyAnimationState_2[3]; // ��������2 �����
	
	//��ƼŬ
	Ogre::ParticleSystem* enemyeffect[3]; // �������ƼŬ
	Ogre::ParticleSystem* enemyeffect_2[3]; // ��������2 �������ƼŬ
	Ogre::ParticleSystem* snow; //��
	Ogre::ParticleSystem* snow2; //��	
	Ogre::ParticleSystem* snow3; //��		
	Ogre::ParticleSystem* fireworkeffect; //�Ҳɳ���
	bool firework; //�Ҳɳ���


	//����������� �Ŵ�����
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::InputContext mInputContext;
	Ogre::OverlaySystem* mOverlaySystem;
	OgreBites::SdkCameraMan* mCameraMan;  

	//�󺧵�
	OgreBites::Label * StageLabel;
	OgreBites::Label * BulletLabel;
	OgreBites::TextBox * Quest;
	

	Ogre::RenderWindow* mWindow;
	//Game ��
	Ogre::SceneManager* mSceneMgr;
	//����ȭ�� ��
	Ogre::SceneManager* StartSceneMgr;
	//����ȭ�� ��
	Ogre::SceneManager* OverSceneMgr;
	//Ŭ����ȭ�� ��
	Ogre::SceneManager* ClearSceneMgr;

	//����ȭ�� 2d��׶���
	Ogre::MaterialPtr Startmaterial;
	Ogre::Rectangle2D* Startrect;

	//����ȭ�� 2d��׶���
	Ogre::MaterialPtr Overmaterial;
	Ogre::Rectangle2D* Overrect;

	//Ŭ����ȭ�� 2d��׶���
	Ogre::MaterialPtr Clearmaterial;
	Ogre::Rectangle2D* Clearrect;

	//Ŭ����ȭ�� 2d��׶���

	//ī�޶�
	Ogre::Camera* mCamera;
	Ogre::Camera* StartCamera;
	Ogre::Camera* OverCamera;
	Ogre::Camera* ClearCamera;

	//OIS��
	OIS::InputManager* mInputMgr;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::MouseListener* mListner;

	//�ٿ���ڽ�
	Ogre::AxisAlignedBox CartBox;
	Ogre::AxisAlignedBox NPC1Box;
	Ogre::AxisAlignedBox NPC2Box;
	Ogre::AxisAlignedBox EnemyBox[3];
	Ogre::AxisAlignedBox EnemyBox_2[3];
	Ogre::AxisAlignedBox WallBox;
	Ogre::AxisAlignedBox UFOBox;


	// ��ƼŬ ����Ʈ �����ð��� ������ �ϱ� ����
	Ogre::Real Time[6];
	Ogre::Real Time2;
	bool fenemyeffect[3];
	bool fenemyeffect_2[3];
	bool fsnow;
	void fTime(Ogre::Real time, Ogre::Real time2, int i);



	// NPC���� ��ȣ�ۿ��� ���� BOOL������
	bool NPC1touch;
	bool NPC2touch;
	bool DestroyNpc1;
	bool DestroyNpc2;

	bool DestroyWall;
	void NPCFollow();



	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);


	//��� ��ȯ�� ����
	virtual void CreateSceneManager(void);
	virtual void CreateCamera(void);
	virtual void CreateViewports(void);
	virtual void createFrameListner(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	virtual void SetupViewport(Ogre::SceneManager *curr);

	//���ҽ� �ҷ����� �Լ�
	void setUpResources(void);
	bool configure(void);
	void loadResources(void);

	// ���� scene�� �̸�, start, game, over, clear����
	const char* CurrentScene;

	//��������1 Patrol
	Ogre::Vector3 pDistance[3];
	//��������2 Patrol
	Ogre::Vector3 pDistance_2[3];

	// Patrol �Լ�
	void Patrol(Ogre::Vector3 pDistance, Ogre::Vector3 EnemyLocation, Ogre::Vector3 EnemyVector, Ogre::Real eMove,Ogre::Real time, Ogre::SceneNode* Node, int eNum);
	void Patrol2(Ogre::Vector3 pDistance, Ogre::Vector3 EnemyLocation, Ogre::Vector3 EnemyVector, Ogre::Real eMove,Ogre::Real time, Ogre::SceneNode* Node, int eNum);

	// Input ������ ����
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);
	virtual bool processUnbufferInput(const Ogre::FrameEvent& evt);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	//Startȭ��, Overȭ��, clearȭ�� 2D ��� �̹��� ����
	virtual void ShowStart2Dbackground();
	virtual void ShowOver2Dbackground();
	virtual void ShowClear2Dbackground();

	//�Ѿ�
	struct BULLET
	{
		Ogre::Entity *bEntity;
		Ogre::SceneNode *bSceneNode;
		Ogre::AxisAlignedBox bBox;

	};
	//�Ѿ˰���
	int nBullet;
	// ��������2 ���� NewBullet
	Ogre::AxisAlignedBox NewBulletBox;
	bool DestroyNewBullet;

	// �Ѿ˵��� �����ϴ� list
	std::list<BULLET *>m_listBullet;

	// �Ѿ˵��� �������� �����ϴ� list
	std::list<Ogre::SceneNode *>m_listObject;
	
	// �Ѿ� �����Լ�
	void CreateBullet();
	// �Ѿ� �̵�, ���� �Լ�
	void UpdateBullet(float fTime);



	//����� patrol
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

	//����Ʈâ
	bool setOverlayCaption(const Ogre::String &elementName, const Ogre::String &caption);
	bool showOverlay(const Ogre::String &overlayName);
	bool ShowQuest;
	Ogre::Overlay* pOver;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
