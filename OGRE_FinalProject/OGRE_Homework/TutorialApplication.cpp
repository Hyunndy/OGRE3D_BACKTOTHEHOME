/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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



#include "TutorialApplication.h"




TutorialApplication::TutorialApplication()
	: mRoot(0), mSceneMgr(0), StartSceneMgr(0), mTrayMgr(0), mOverlaySystem(0), mCameraMan(0), mAnimationState(0), mAnimationState2(0),
mResourcesCfg(Ogre::StringUtil::BLANK),
mPluginsCfg(Ogre::StringUtil::BLANK)
{
	ShowQuest=false;

	nBullet = 10;

	NPC1touch = false;	
	DestroyNpc1 = false;
	NPC2touch =false;
	DestroyNpc2=false;
	DestroyWall = false;
	CurrentScene = "start";
	DestroyNewBullet=false;
	fsnow = false;
	firework=false;

	Time2=0;
	for(int i=0; i<3; i++)
	{
		Patrolreach_x[i]=true;
		Patrolreach_y[i]=true;
		Patrolreach_x_2[i]=true;
		Patrolreach_y_2[i]=true;
		DestroyEnemy[i] = false;
		DestroyEnemy_2[i] = false;
		fenemyeffect[i]=false;
		fenemyeffect_2[i]=false;
		EnemyAnimationState[i] = 0;
		EnemyAnimationState_2[i] =0;
		KillEnemy_2[i]=0;

	}
	for(int i=0; i<6; i++)
		Time[i]=0;
	a=0;


}
TutorialApplication::~TutorialApplication()
{	

	delete mRoot;
	delete Startrect;
	delete Overrect;
	delete Clearrect;
	
	if (mOverlaySystem) delete mOverlaySystem;
	if(mCameraMan) delete mCameraMan;
	if(mTrayMgr) delete mTrayMgr;

	// Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
}


int TutorialApplication::Range(int a, int b)
{
	return (int)(rand() % (b+1));
}

void TutorialApplication::SetupViewport(Ogre::SceneManager *curr)
{

	mWindow->removeAllViewports();
	Ogre::Camera * cam;

	if(CurrentScene=="start")
		cam = curr->getCamera("startCam");
	else if(CurrentScene=="game")
		cam = curr->getCamera("gameCam");
	else if(CurrentScene=="over")
		cam = curr->getCamera("OverCam");
	else if(CurrentScene=="clear")
		cam = curr->getCamera("ClearCam");


	Ogre::Viewport * vp = mWindow->addViewport(cam);

	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void TutorialApplication::CreateSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "game");
	StartSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "start");
	OverSceneMgr= mRoot->createSceneManager(Ogre::ST_GENERIC, "over");
	ClearSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "clear");

	// �󺧻���� ���� mOverlaySystem
	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

		// ������ Ÿ�� ����
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
}

void TutorialApplication::CreateCamera()
{
	mCamera = mSceneMgr->createCamera("gameCam");
	StartCamera = StartSceneMgr->createCamera("startCam");
	OverCamera = OverSceneMgr->createCamera("OverCam");
	ClearCamera = ClearSceneMgr->createCamera("ClearCam");

	// �� ����� ���� ī�޶��
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void TutorialApplication::CreateViewports()
{
	SetupViewport(StartSceneMgr);
}

void TutorialApplication::createFrameListner(void)
{

//----------------------------------------------
// Initializing OIS
// InputManager�� ����� OIS�� �Է��� �޾ƿ´�.
//----------------------------------------------

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


	mInputMgr = OIS::InputManager::createInputSystem(pl);
	// Set false to true if you want buffered input
	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));



	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	mInputContext.mKeyboard = mKeyboard;
	mInputContext.mMouse = mMouse;

	//�� ����� ���� mTrayMgr
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);

//--------------------------------------------
// Shutting down OIS
// WindowEventListener�� ����ϱ� ���� ���
//---------------------------------------------

	// Set initial mouse clipping size
	windowResized(mWindow);

	// Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	// Register as a Frame listener
	mRoot->addFrameListener(this);


//--------------------------------------------
// Label��
//---------------------------------------------

	StageLabel = mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOP, "StageLabel", "1-heart fish Stage", 400);
	BulletLabel = mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOP, "BulletLabel", "Bullet :" + Ogre::StringConverter::toString(nBullet), 200);


}

void TutorialApplication::setUpResources(void)
{
//-----------------------------------------------------------------------
// Resources & plugin ���� ���
//-----------------------------------------------------------------------

	#ifdef _DEBUG
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif
		mRoot = new Ogre::Root(mPluginsCfg);
//-----------------------------------------------------------------------
// Parse *.cfg files
// ResourceGroupManager ���
//-----------------------------------------------------------------------
	
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);


	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}
}

bool TutorialApplication::configure(void)
{
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	return false;

	// The first parameter determines whether or not Ogre will create a RenderWindow for us.
	// ���⼭ false�ϸ� ���� �ߴ� ������ó�� ���� ������ ũ�� ���� ���� ����
	mWindow = mRoot->initialise(false, "TutorialApplication Render Window");
	mWindow = mRoot->createRenderWindow("Back to the home",1280 , 960, false, 0);

	// Set LOD level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	return true;
}

void TutorialApplication::loadResources(void)
{
//-----------------------------------------------------------------------
// Configuring the RenderSystem
// DirectX�� OpenGL ���
// "ogre.cfg" Ȯ�� �� configuration dialog ���
//-----------------------------------------------------------------------

	// Initialize all of the resources found by OGRE.
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void TutorialApplication::ShowStart2Dbackground()
{

		Startmaterial = Ogre::MaterialManager::getSingleton().create("StartBackground", "General");
		Startmaterial->getTechnique(0)->getPass(0)->createTextureUnitState("gamestart.png");
		Startmaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		Startmaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		Startmaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

		Startrect = new Ogre::Rectangle2D(true);
		Startrect->setCorners(-1.0, 1.0, 1.0, -1.0);
		Startrect->setMaterial("StartBackground");

		// Render the background before everything else
		Startrect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		// Use infinite AAB to always stay visible
		Ogre::AxisAlignedBox aabInf;
		aabInf.setInfinite();
		Startrect->setBoundingBox(aabInf);

}

void TutorialApplication::ShowOver2Dbackground()
{

		Overmaterial = Ogre::MaterialManager::getSingleton().create("OverBackground", "General");
		Overmaterial->getTechnique(0)->getPass(0)->createTextureUnitState("gameover.png");
		Overmaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		Overmaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		Overmaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

		Overrect = new Ogre::Rectangle2D(true);
		Overrect->setCorners(-1.0, 1.0, 1.0, -1.0);
		Overrect->setMaterial("OverBackground");

		// Render the background before everything else
		Overrect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		// Use infinite AAB to always stay visible
		Ogre::AxisAlignedBox aabInf2;
		aabInf2.setInfinite();
		Overrect->setBoundingBox(aabInf2);

}
void TutorialApplication::ShowClear2Dbackground()
{
		Clearmaterial = Ogre::MaterialManager::getSingleton().create("ClearBackground", "General");
		Clearmaterial->getTechnique(0)->getPass(0)->createTextureUnitState("gameclear.png");
		Clearmaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		Clearmaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		Clearmaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

		Clearrect = new Ogre::Rectangle2D(true);
		Clearrect->setCorners(-1.0, 1.0, 1.0, -1.0);
		Clearrect->setMaterial("ClearBackground");

		// Render the background before everything else
		Clearrect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		// Use infinite AAB to always stay visible
		Ogre::AxisAlignedBox aabInf3;
		aabInf3.setInfinite();
		Clearrect->setBoundingBox(aabInf3);
}

bool TutorialApplication::setOverlayCaption(const Ogre::String &elementName, const Ogre::String &caption)
{
	 
	 Ogre::OverlayElement* elm =Ogre::OverlayManager::getSingleton().getOverlayElement(elementName);
	 
	if(!elm) return false;
	elm->setPosition(63,73);
	elm->setCaption(caption);
	return true;
}

bool TutorialApplication::showOverlay(const Ogre::String &overlayName)
{ 
	
 // Load up our UI and display it
 pOver = (Ogre::Overlay*)Ogre::OverlayManager::getSingleton().getByName(overlayName);  
 
 if (!pOver) return false;

 return true;	
}

bool TutorialApplication::go(void)
{

//-----------------------------------------------------------------------
// ���ҽ� ����(.cfg) ��� -> ������ ���� -> ���Ŵ��� ���� -> ī�޶� ����-> ����Ʈ ���� -> cfg���Ͽ� �ִ� ���ҽ��� �ʱ�ȭ
//-----------------------------------------------------------------------
	setUpResources();
	configure();
	CreateSceneManager();
	CreateCamera();
	CreateViewports();
	loadResources();

	
//-----------------------------------------------------------------------
// StartScene�� Render�Ǵ°͵�
//-----------------------------------------------------------------------
	if (CurrentScene == "start")
	{
		ShowStart2Dbackground();
		Ogre::SceneNode* Startbackground = StartSceneMgr->getRootSceneNode()->createChildSceneNode("StartBackground");
		Startbackground->attachObject(Startrect);
		Startmaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	}
//-----------------------------------------------------------------------
// Setting Up the Scene
// OverScene�� �ʿ��Ѱ͵� ���� ���Ƴֱ�
//-----------------------------------------------------------------------

	if(CurrentScene=="over")
	{
		mAnimationState->setEnabled(false);
		mAnimationState2->setEnabled(false);
		for(int i=0; i<3; i++)
		{
			EnemyAnimationState[i]->setEnabled(false);
			EnemyAnimationState_2[i]->setEnabled(false);
		}
	}
		ShowOver2Dbackground();	

		Ogre::SceneNode* Overbackground = OverSceneMgr->getRootSceneNode()->createChildSceneNode("OverBackground");
		Overbackground->attachObject(Overrect);
		Overmaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	
//-----------------------------------------------------------------------
// Setting Up the Scene
// ClearScene�� �ʿ��Ѱ͵� ���� ���Ƴֱ�
//-----------------------------------------------------------------------
	if(CurrentScene=="clear")
	{
		mSceneMgr->destroyLight("UFOLight");
		mAnimationState->setEnabled(false);
		mAnimationState2->setEnabled(false);
		for(int i=0; i<3; i++)
		{
			EnemyAnimationState[i]->setEnabled(false);
			EnemyAnimationState_2[i]->setEnabled(false);
		}
	}
		ShowClear2Dbackground();	

		Ogre::SceneNode* Clearbackground = ClearSceneMgr->getRootSceneNode()->createChildSceneNode("ClearBackground");
		Clearbackground->attachObject(Clearrect);
		Clearmaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	

//-----------------------------------------------------------------------
// Setting Up the Scene
// GameScene�� �ʿ��Ѱ͵� ���� ���Ƴֱ�
//-----------------------------------------------------------------------


	mCamera->setPosition(Ogre::Vector3(0, 580, 650));
	mCamera->lookAt(Ogre::Vector3(0, 0, -30));
	mCamera->pitch(Ogre::Degree(-3));
	mCamera->setNearClipDistance(5);

	//�ں��Ʈ
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));




	//�¾�
	Ogre::Light* MainLight = mSceneMgr->createLight("MainLight");
	MainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	MainLight->setDiffuseColour(Ogre::ColourValue(1,1,1));
	MainLight->setDirection(0,-1,-0.5);
	MainLight->setVisible(true);


	//��ī�̹ڽ�
	mSceneMgr->setSkyBox(true, "guswl/ClearSky", 5000);

	//Plane ��ü �ļ�
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y,-10);
	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 550,600,20,20,true,1,1,1,Ogre::Vector3::UNIT_Z);
	// 3,4��° ���� �����ϸ� �� ũ�� ����

	//Plane1
	Ogre::Entity* PlaneEnt = mSceneMgr->createEntity("PlaneEnt", "plane");
	PlaneEnt->setMaterialName("guswl/Snow");
	Ogre::SceneNode* PlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlaneNode");

	PlaneNode->attachObject(PlaneEnt);
	PlaneEnt->setCastShadows(FALSE);

	//Plane2
	Ogre::Entity* PlaneEnt2 = mSceneMgr->createEntity("PlaneEnt2", "plane");
	Ogre::SceneNode* PlaneNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlaneNode2");
	PlaneEnt2->setMaterialName("guswl/Snow");
	PlaneNode2->attachObject(PlaneEnt2);
	PlaneNode2->setPosition(0,0,-1100);
	PlaneEnt2->setCastShadows(FALSE);

	//Plane3 //��������2
	Ogre::Entity* PlaneEnt3 = mSceneMgr->createEntity("PlaneEnt3", "plane");
	Ogre::SceneNode* PlaneNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlaneNode3");
	PlaneEnt3->setMaterialName("guswl/Snow");
	PlaneNode3->attachObject(PlaneEnt3);
	PlaneNode3->setPosition(0,0,-1700);
	PlaneEnt3->setCastShadows(FALSE);
	
	//Plane4 // ��������2
	Ogre::Entity* PlaneEnt4 = mSceneMgr->createEntity("PlaneEnt4","plane");
	Ogre::SceneNode* PlaneNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlaneNode4");
	PlaneEnt4->setMaterialName("guswl/Snow");
	PlaneNode4->attachObject(PlaneEnt4);
	PlaneNode4->setPosition(0,0,-2800);
	PlaneEnt4->setCastShadows(FALSE);
	
	//Water ��ü ����
	Ogre::Plane Water(Ogre::Vector3::UNIT_Y,-10);
	Ogre::MeshManager::getSingleton().createPlane("water", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Water, 550,500,20,20,true,1,1,1,Ogre::Vector3::UNIT_Z);	

	//Water1
	Ogre::Entity* WaterEnt = mSceneMgr->createEntity("WaterEnt", "water");
	WaterEnt->setMaterialName("guswl/WaterStream");
	Ogre::SceneNode* WaterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WaterNode");
	WaterNode->attachObject(WaterEnt);
	WaterNode->setPosition(0,0,-550);
	WaterEnt->setCastShadows(FALSE);

	//Water2
	Ogre::Entity* WaterEnt2 = mSceneMgr->createEntity("WaterEnt2", "water");
	WaterEnt2->setMaterialName("guswl/WaterStream");
	Ogre::SceneNode* WaterNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WaterNode2");
	WaterNode2->attachObject(WaterEnt2);
	WaterNode2->setPosition(0,0,-2250);
	WaterEnt2->setCastShadows(FALSE);


	//�����
	Ogre::Entity* SnowmanEnt = mSceneMgr->createEntity("SnowmanEnt", "snowman.mesh");
	Ogre::SceneNode* SnowmanNode = PlaneNode->createChildSceneNode("SnowmanNode");
	SnowmanNode->attachObject(SnowmanEnt);
	SnowmanNode->setPosition(0,100,-1000);
	SnowmanNode->setScale(0.3,0.3,0.3);

	
	//����� ����Ʈ
	snow = mSceneMgr->createParticleSystem("Snow1", "guswl/PurpleFountain");
	snow2 = mSceneMgr->createParticleSystem("Snow2", "guswl/PurpleFountain");
	snow3 = mSceneMgr->createParticleSystem("Snow3", "guswl/PurpleFountain");
	Ogre::SceneNode* snowEffectNode = PlaneNode->createChildSceneNode("SnowEffect"); 
	Ogre::SceneNode* snowEffectNode2 =  PlaneNode->createChildSceneNode("SnowEffect2"); 
	Ogre::SceneNode* snowEffectNode3 =  PlaneNode->createChildSceneNode("SnowEffect3"); 
	snowEffectNode->setPosition(-180,0,-1000);
	snowEffectNode2->setPosition(180,0,-1000);
	snowEffectNode3->setPosition(0,0,-1000);

	//īƮ
	Ogre::Entity* BoyEnt = mSceneMgr->createEntity("CartEnt","Boy.mesh");
	Ogre::SceneNode* BoyNode = PlaneNode->createChildSceneNode("Cart");
	BoyNode->setScale(10,10,10);
	BoyNode->setPosition(0, 30, 250);
	//BoyNode->setPosition(0,30, -1100);
	BoyNode->yaw(Ogre::Degree(180),Ogre::Node::TS_LOCAL);
	BoyNode->attachObject(BoyEnt);
	//BoyEnt->setCastShadows(true);

	//ī�޶� �����Ǿ��ִ� ���
	Ogre::Entity* CameraEnt = mSceneMgr->createEntity("CameraEnt","sphere.mesh");
	Ogre::SceneNode* CameraNode = PlaneNode->createChildSceneNode("CameraNode");
	CameraNode->setScale(0,0,0);
	CameraNode->setPosition(0, 130, 280);
	CameraNode->attachObject(CameraEnt);
	//ī�޶� īƮ�� ���̴°�
	CameraNode->attachObject(mCamera);

	get_Distance(); // ī�޶�� ĳ���� ������ �Ÿ��� ����ϴ� �Լ�

	//NPC
	Ogre::Entity* NPCEnt = mSceneMgr->createEntity("NPC1Ent", "penguin.mesh");
	Ogre::SceneNode* NPCNode = PlaneNode->createChildSceneNode("NPC1");
	NPCNode->attachObject(NPCEnt);
	NPCNode->setPosition(200,40,-200);
	NPCNode->setScale(2,2,2);
	NPCNode->yaw(Ogre::Degree(-45), Ogre::Node::TS_LOCAL);


	mAnimationState = NPCEnt->getAnimationState("amuse");
	if(CurrentScene=="start")
	{
		
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
	}
	else
	{
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(false);
	}

	Ogre::Entity* NPCEnt2 = mSceneMgr->createEntity("NPC1Ent2", "penguin.mesh");
	Ogre::SceneNode* NPCNode2 = BoyNode->createChildSceneNode("NPC1_2");
	NPCNode2->setPosition(0,0,-7);
	NPCNode2->setScale(0.1,0.1,0.1);


	//����Ʈ ��ũ��
	showOverlay("App/Profile");
	setOverlayCaption("Profile/Text","Penguin:\nYour UFO is over there!!\nIf you want to cross the river,\nbring me 3 fishes!!!!\n\n**If you have 0 bullet,** \n**game will be over.**");

	//UFO
	Ogre::Entity* UFOEnt = mSceneMgr->createEntity("UFOEnt","ufo.mesh");
	Ogre::SceneNode* UFONode = PlaneNode4->createChildSceneNode("ufo");
	UFONode->setScale(150,150,150);
	UFONode->setPosition(0,50,0);
	UFONode->attachObject(UFOEnt);



	//�Ҳɳ��� ����Ʈ
	fireworkeffect = mSceneMgr->createParticleSystem("firework", "guswl/Fireworks");
	Ogre::SceneNode* fireworkNode = PlaneNode->createChildSceneNode("fireworkNode");
	fireworkNode->setPosition(0,15,-1500);
	mSceneMgr->getSceneNode("fireworkNode")->attachObject(fireworkeffect);	
	fireworkeffect->getEmitter(0)->setEmissionRate(0);
	

	//�����
	Ogre::Entity* EnemyEnt = mSceneMgr->createEntity("EnemyEnt", "fish.mesh");
	Ogre::SceneNode* EnemyNode = PlaneNode->createChildSceneNode("Enemy1");
	EnemyNode->setScale(10,10,10);
	EnemyNode->setPosition(20, 10, -490);
	EnemyAnimationState[0] = EnemyEnt->getAnimationState("swim");
	EnemyNode->attachObject(EnemyEnt);	

	//����� ����Ʈ
	enemyeffect[0] = mSceneMgr->createParticleSystem("Nimbus", "guswl/GreenyNimbus");	
	Ogre::SceneNode* EnemyEffectNode = PlaneNode->createChildSceneNode("EnemyEffect");

	//��������2 �����
	Ogre::Entity* EnemyEnt_2 = mSceneMgr->createEntity("EnemyEnt_2", "fish.mesh");
	Ogre::SceneNode* EnemyNode_2 = PlaneNode3->createChildSceneNode("Enemy1_2");
	EnemyNode_2->setScale(10,10,10);
	EnemyNode_2->setPosition(20, 10, -490);
	EnemyAnimationState_2[0] = EnemyEnt_2->getAnimationState("swim");
	EnemyNode_2->attachObject(EnemyEnt_2);		

	//��������2 ����� ����Ʈ
	enemyeffect_2[0] = mSceneMgr->createParticleSystem("Nimbus_2", "guswl/GreenyNimbus");	
	Ogre::SceneNode* EnemyEffectNode_2 = PlaneNode3->createChildSceneNode("EnemyEffect_2");

	//�����2
	Ogre::Entity* Enemy2Ent = mSceneMgr->createEntity("Enemy2Ent", "fish.mesh");
	Ogre::SceneNode* Enemy2Node = PlaneNode->createChildSceneNode("Enemy2");
	Enemy2Node->setScale(10,10,10);
	Enemy2Node->setPosition(20, 10, -490);
	EnemyAnimationState[1] = Enemy2Ent->getAnimationState("swim");
	Enemy2Node->attachObject(Enemy2Ent);


	//����� ����Ʈ2
	enemyeffect[1] = mSceneMgr->createParticleSystem("Nimbus2", "guswl/GreenyNimbus");	
	Ogre::SceneNode* Enemy2EffectNode = PlaneNode->createChildSceneNode("Enemy2Effect");

	//��������2 �����2
	Ogre::Entity* Enemy2Ent_2 = mSceneMgr->createEntity("Enemy2Ent_2", "fish.mesh");
	Ogre::SceneNode* Enemy2Node_2 = PlaneNode3->createChildSceneNode("Enemy2_2");
	Enemy2Node_2->setScale(10,10,10);
	Enemy2Node_2->setPosition(20, 10, -490);
	EnemyAnimationState_2[1] = Enemy2Ent_2->getAnimationState("swim");
	Enemy2Node_2->attachObject(Enemy2Ent_2);		

	//��������2 �����2 ����Ʈ
	enemyeffect_2[1] = mSceneMgr->createParticleSystem("Nimbus2_2", "guswl/GreenyNimbus");	
	Ogre::SceneNode* Enemy2EffectNode_2 = PlaneNode3->createChildSceneNode("Enemy2Effect_2");

	//�����3
	Ogre::Entity* Enemy3Ent = mSceneMgr->createEntity("Enemy3Ent", "fish.mesh");
	Ogre::SceneNode* Enemy3Node = PlaneNode->createChildSceneNode("Enemy3");
	Enemy3Node->setScale(10,10,10);
	Enemy3Node->setPosition(20, 10, -490);
	EnemyAnimationState[2] = Enemy3Ent->getAnimationState("swim");
	Enemy3Node->attachObject(Enemy3Ent);	

	//����� ����Ʈ3
	enemyeffect[2] = mSceneMgr->createParticleSystem("Nimbus3", "guswl/GreenyNimbus");	
	Ogre::SceneNode* Enemy3EffectNode = PlaneNode->createChildSceneNode("Enemy3Effect");

	//��������2 �����3
	Ogre::Entity* Enemy3Ent_2 = mSceneMgr->createEntity("Enemy3Ent_2", "fish.mesh");
	Ogre::SceneNode* Enemy3Node_2 = PlaneNode3->createChildSceneNode("Enemy3_2");
	Enemy3Node_2->setScale(10,10,10);
	Enemy3Node_2->setPosition(20, 10, -490);
	EnemyAnimationState_2[2] = Enemy3Ent_2->getAnimationState("swim");
	Enemy3Node_2->attachObject(Enemy3Ent_2);		

	//��������2 �����3 ����Ʈ
	enemyeffect_2[2] = mSceneMgr->createParticleSystem("Nimbus3_2", "guswl/GreenyNimbus");	
	Ogre::SceneNode* Enemy3EffectNode_2 = PlaneNode3->createChildSceneNode("Enemy3Effect_2");


	for(int i=0; i<3; i++)
	{
		if(CurrentScene=="start")
		{
			EnemyAnimationState[i]->setLoop(true);
			EnemyAnimationState[i]->setEnabled(true);
		}						
		else					
		{						
			EnemyAnimationState[i]->setLoop(false);
			EnemyAnimationState[i]->setEnabled(false);
		}
	}

	for(int i=0; i<3; i++)
	{
		if(CurrentScene=="start")
		{
			EnemyAnimationState_2[i]->setLoop(true);
			EnemyAnimationState_2[i]->setEnabled(true);
		}						
		else					
		{						
			EnemyAnimationState_2[i]->setLoop(false);
			EnemyAnimationState_2[i]->setEnabled(false);
		}
	}

	// ��������2 �Ա� ���� �Ѿ�
	Ogre::Entity * newBullet = mSceneMgr->createEntity("NewBulletEnt", "sphere.mesh");
	Ogre::SceneNode* newBulletNode = PlaneNode3->createChildSceneNode("newBulletNode");
	newBullet->setMaterialName("guswl/Green");


	newBulletNode->setPosition(0, 40, 140);
	newBulletNode->setScale(0.3,0.3,0.3);
	newBulletNode->attachObject(newBullet);


	//NPC2
	Ogre::Entity* NPC2Ent = mSceneMgr->createEntity("NPC2Ent", "penguin.mesh");
	Ogre::SceneNode* NPC2Node = PlaneNode3->createChildSceneNode("NPC2");
	NPC2Node->attachObject(NPC2Ent);
	NPC2Node->setPosition(225,40,-200);
	NPC2Node->setScale(2,2,2);
	NPC2Node->yaw(Ogre::Degree(-45), Ogre::Node::TS_LOCAL);


	mAnimationState2 = NPC2Ent->getAnimationState("amuse");
	if(CurrentScene=="start")
	{
		
		mAnimationState2->setLoop(true);
		mAnimationState2->setEnabled(true);
	}
	else
	{
		mAnimationState2->setLoop(false);
		mAnimationState2->setEnabled(false);
	}

	Ogre::Entity* NPC2Ent2 = mSceneMgr->createEntity("NPC2Ent2", "penguin.mesh");
	Ogre::SceneNode* NPC2Node2 = BoyNode->createChildSceneNode("NPC2_2");
	NPC2Node2->setPosition(0,0,-7);
	NPC2Node2->setScale(0.1,0.1,0.1);


//-----------------------------------------------------------------------------------------------


	createFrameListner();

	// Start rendering loop
	mRoot->startRendering();
	

return true;
}

//-----------------------------------------------------------------------
// Patrol �Լ�
//-----------------------------------------------------------------------

void TutorialApplication::Patrol(Ogre::Vector3 pDistance, Ogre::Vector3 EnemyLocation, Ogre::Vector3 EnemyVector, Ogre::Real eMove,Ogre::Real time, Ogre::SceneNode* Node, int eNum)
{
	
	if( abs(pDistance.x)>=0 && abs(pDistance.x)<1) 
	{
		Patrolreach_x[eNum]=true;
		EnemyVector.x=0;	

	}
	else
	{
		if(PatrolLocation[eNum].x>EnemyLocation.x)
			EnemyVector.x+=eMove;

		else if(PatrolLocation[eNum].x<EnemyLocation.x)
			EnemyVector.x-=eMove;
		else
			EnemyVector.x =0;
	}

	if(abs(pDistance.z)>=0 && abs(pDistance.z)<1)
	{
		Patrolreach_y[eNum]=true;
		EnemyVector.z=0;


	}
	else
	{
		if(PatrolLocation[eNum].z>EnemyLocation.z)
			EnemyVector.z+=eMove;

		else if(PatrolLocation[eNum].z<EnemyLocation.z)
			EnemyVector.z-=eMove;
		else
			EnemyVector.z=0;
	}
	
	Node->translate(EnemyVector*time, Ogre::Node::TS_LOCAL);

}
void TutorialApplication::Patrol2(Ogre::Vector3 pDistance, Ogre::Vector3 EnemyLocation, Ogre::Vector3 EnemyVector, Ogre::Real eMove,Ogre::Real time, Ogre::SceneNode* Node, int eNum)
{
	
	if( abs(pDistance.x)>=0 && abs(pDistance.x)<1) 
	{
		Patrolreach_x_2[eNum]=true;
		EnemyVector.x=0;	

	}
	else
	{
		if(PatrolLocation_2[eNum].x>EnemyLocation.x)
			EnemyVector.x+=eMove;

		else if(PatrolLocation_2[eNum].x<EnemyLocation.x)
			EnemyVector.x-=eMove;
		else
			EnemyVector.x =0;
	}

	if(abs(pDistance.z)>=0 && abs(pDistance.z)<1)
	{
		Patrolreach_y_2[eNum]=true;
		EnemyVector.z=0;
	}
	else
	{
		if(PatrolLocation_2[eNum].z>EnemyLocation.z)
			EnemyVector.z+=eMove;

		else if(PatrolLocation_2[eNum].z<EnemyLocation.z)
			EnemyVector.z-=eMove;
		else
			EnemyVector.z=0;
	}
	
	Node->translate(EnemyVector*time, Ogre::Node::TS_LOCAL);

}

//-----------------------------------------------------------------------
// ó���� ī�޶�-ĳ���� ������ �Ÿ��� ���ϱ� ���� �Լ�
//-----------------------------------------------------------------------

void TutorialApplication::get_Distance()
{
	Ogre::Vector3 CartLocation = mSceneMgr->getSceneNode("Cart")->getPosition();

	Ogre::Vector3 CameraLocation = mSceneMgr->getSceneNode("CameraNode")->getPosition();

	Ogre::Vector3 Distance2 = CartLocation-CameraLocation;

	Distance = Distance2.z;
}


//-----------------------------------------------------------------------
// �Ѿ� ���� �κ�
//-----------------------------------------------------------------------
void TutorialApplication::CreateBullet()
{
	  BULLET *pNewBullet = new BULLET;
	  memset(pNewBullet, 0, sizeof(BULLET));
	 
	  //�Ѿ� ���� ����
	  nBullet--;
	  //���ҵ� �Ѿ� ������ BulletLabel�� ���� ���ش�.
	  BulletLabel->setCaption("Bullet :" + Ogre::StringConverter::toString(nBullet));
	  
	  char szId[32] = "";
	  sprintf_s(szId, 32, "ID:%d", (int)pNewBullet);
	
	  Ogre::Vector3 CartLocation = mSceneMgr->getSceneNode("Cart")->getPosition();
	  pNewBullet->bEntity = mSceneMgr->createEntity(szId, "sphere.mesh");	  

	  pNewBullet->bEntity->setMaterialName("guswl/Green");
	  pNewBullet->bSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	 
	  pNewBullet->bBox = pNewBullet->bEntity->getWorldBoundingBox();
	  pNewBullet->bSceneNode->setScale(0.1f,0.1f,0.1f);
	  pNewBullet->bSceneNode->attachObject(pNewBullet->bEntity);  
	  pNewBullet->bSceneNode->setPosition(CartLocation);
	  
	  pNewBullet->bSceneNode->setOrientation(mSceneMgr->getSceneNode("Cart")->getOrientation());
	  

     m_listBullet.push_back(pNewBullet);
	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// �Ѿ� ���� �κ�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void TutorialApplication::UpdateBullet(float fTime)
{
	std::list<BULLET *>::iterator itor=m_listBullet.begin();

	Ogre::Vector3 Wall;
	Ogre::Vector3 BulletVecrtor(0.0f,0.0f,0.0f);

	Ogre::Real bSpeed  = 350;
	 

	while(itor != m_listBullet.end())
	{
		(*itor)->bBox = (*itor)->bEntity->getWorldBoundingBox();

		BulletVecrtor.z += bSpeed;
		(*itor)->bSceneNode->translate(BulletVecrtor * fTime, Ogre::Node::TS_LOCAL);
		
		BulletVecrtor.z=0.0f;

		Wall = (*itor)->bSceneNode->getPosition();

		if(Wall.x>230 || Wall.x<-230)
		{
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}
		
		else if((DestroyNpc1==false) && Wall.z <-1000)
		{
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}

		else if((DestroyNpc2==false && Wall.z <-2700))
		{
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}
		else if((*itor)->bBox.intersects(EnemyBox[0]) && (DestroyEnemy[0]==false))
		{					
			fenemyeffect[0]=true;
			mSceneMgr->getSceneNode("EnemyEffect")->attachObject(enemyeffect[0]);
			
			DestroyEnemy[0]=true;	

			EnemyAnimationState[0]->setEnabled(false);
			mSceneMgr->destroyEntity(mSceneMgr->getEntity("EnemyEnt"));
			mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("Enemy1"));
			
			
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}

		else if((*itor)->bBox.intersects(EnemyBox[1]) && (DestroyEnemy[1]==false))
		{					
			fenemyeffect[1]=true;
			mSceneMgr->getSceneNode("Enemy2Effect")->attachObject(enemyeffect[1]);
			
			DestroyEnemy[1]=true;	

			EnemyAnimationState[1]->setEnabled(false);
			mSceneMgr->destroyEntity(mSceneMgr->getEntity("Enemy2Ent"));
			mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("Enemy2"));
			
			
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}

		else if((*itor)->bBox.intersects(EnemyBox[2]) && (DestroyEnemy[2]==false))
		{					
			fenemyeffect[2]=true;
			mSceneMgr->getSceneNode("Enemy3Effect")->attachObject(enemyeffect[2]);
			
			DestroyEnemy[2]=true;	

			EnemyAnimationState[2]->setEnabled(false);
			mSceneMgr->destroyEntity(mSceneMgr->getEntity("Enemy3Ent"));
			mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("Enemy3"));
			
			
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}
		else if((*itor)->bBox.intersects(EnemyBox_2[0]) && (DestroyEnemy_2[0]==false))
		{	
			KillEnemy_2[0]++;

			if(KillEnemy_2[0]==2)
			{
				fenemyeffect_2[0]=true;
				mSceneMgr->getSceneNode("EnemyEffect_2")->attachObject(enemyeffect_2[0]);
				
				DestroyEnemy_2[0]=true;	

				EnemyAnimationState_2[0]->setEnabled(false);
				mSceneMgr->destroyEntity(mSceneMgr->getEntity("EnemyEnt_2"));
				mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("Enemy1_2"));
			}
			
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}
		else if((*itor)->bBox.intersects(EnemyBox_2[1]) && (DestroyEnemy_2[1]==false))
		{	
			KillEnemy_2[1]++;
			
			if(KillEnemy_2[1]==2)
			{
				fenemyeffect_2[1]=true;
				mSceneMgr->getSceneNode("Enemy2Effect_2")->attachObject(enemyeffect_2[1]);
				
				DestroyEnemy_2[1]=true;	

				EnemyAnimationState_2[1]->setEnabled(false);
				mSceneMgr->destroyEntity(mSceneMgr->getEntity("Enemy2Ent_2"));
				mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("Enemy2_2"));
			}
			
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}
		else if((*itor)->bBox.intersects(EnemyBox_2[2]) && (DestroyEnemy_2[2]==false))
		{		
			KillEnemy_2[2]++;

			if(KillEnemy_2[2]==2)
			{
				fenemyeffect_2[2]=true;
				mSceneMgr->getSceneNode("Enemy3Effect_2")->attachObject(enemyeffect_2[2]);
				
				DestroyEnemy_2[2]=true;	

				EnemyAnimationState_2[2]->setEnabled(false);
				mSceneMgr->destroyEntity(mSceneMgr->getEntity("Enemy3Ent_2"));
				mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("Enemy3_2"));
			}
			
			mSceneMgr->destroyEntity((*itor)->bEntity);
			mSceneMgr->destroySceneNode((*itor)->bSceneNode);
			itor = m_listBullet.erase(itor);
		}

		else
			itor++;
	}
}

void TutorialApplication::NPCFollow()
{		
	if(NPC1touch==TRUE)
	{
		if(DestroyNpc1==false)
		{		
			mAnimationState->setEnabled(false);
			mSceneMgr->destroyEntity(mSceneMgr->getEntity("NPC1Ent"));
			mSceneMgr->destroySceneNode("NPC1");
	
	
			mSceneMgr->getSceneNode("NPC1_2")->attachObject(mSceneMgr->getEntity("NPC1Ent2"));
			DestroyNpc1 = true;
		}
	}

	if(NPC2touch==TRUE)
	{
		if(DestroyNpc2==false)
		{
			firework=true;
			DestroyNpc2=true;
			mAnimationState2->setEnabled(false);
			mSceneMgr->destroyEntity(mSceneMgr->getEntity("NPC2Ent"));
			mSceneMgr->destroySceneNode("NPC2");

			mSceneMgr->getSceneNode("NPC2_2")->attachObject(mSceneMgr->getEntity("NPC2Ent2"));
			DestroyNpc2 = true;
			mSceneMgr->destroyLight("MainLight");
			//UFO�� ��
			Ogre::Light* UFOLight = mSceneMgr->createLight("UFOLight");
			UFOLight->setType(Ogre::Light::LT_POINT);
			UFOLight->setPosition(Ogre::Vector3(0,100, -2800));
			UFOLight->setDiffuseColour(1.0f, 0, 1.0f);
			mSceneMgr->getLight("UFOLight")->setVisible(true);


		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// OIS �κ�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


bool TutorialApplication::keyPressed(const OIS::KeyEvent& ke)
{	
	
	if (ke.key == OIS::KC_F1)
	{	
		if (CurrentScene == "start")
		{			
			CurrentScene = "game";
			SetupViewport(mSceneMgr);

		}
	}

	if (CurrentScene=="game")
	{
		if (ke.key == OIS::KC_SPACE)
		{
			if (NPC1touch == true)
			{
				if(DestroyEnemy[0]==true && DestroyEnemy[1]==true && DestroyEnemy[2]==true)
					NPCFollow();

				if(DestroyNpc1==false)
				{
					if(ShowQuest==false)
					{
						ShowQuest=true;
						pOver->show();

					}
					else 
					{
						pOver->hide();
						ShowQuest=false;
					}
				}


			}

			else if (NPC2touch==true)
			{
				if(DestroyEnemy_2[0]==true && DestroyEnemy_2[1]==true && DestroyEnemy_2[2]==true)
					NPCFollow();

				if(DestroyNpc2==false)
				{
					if(ShowQuest==false)
					{
						ShowQuest=true;
						pOver->show();

					}
					else 
					{
						pOver->hide();
						ShowQuest=false;
					}
				}
			}
			else
			{
			if(ShowQuest==true)
			{
				pOver->hide();
				ShowQuest=false;
			
			}
			}
		}
	}

	return true;
}
bool TutorialApplication::keyReleased(const OIS::KeyEvent& ke) {return true;}
bool TutorialApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;}
bool TutorialApplication::mouseMoved(const OIS::MouseEvent &arg) {return true;}
bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (CurrentScene=="game")
	{
		if (id == OIS::MB_Left)
		{
			//�Ѿ��� 5�߱����� �����ϱ� ���ؼ�
			if(nBullet>=1)
			CreateBullet();
		}
	}
	return true;
}

void TutorialApplication::InitPatrol(int number)
{	
	// patrol ó�� ������ ��ġ
	// ��������1 
	if(number==0)
	{
	p[0].x =20;
	p[0].y =10;
	p[0].z =-490;
	}
	if(number==1)
	{
	p[1].x = 20;
	p[1].y = 10;
	p[1].z = -490;
	}
	if(number==2)
	{
	p[2].x=20;
	p[2].y=10;
	p[2].z=-490;
	}

	a=Range(1,4);

	// ��ȭ ��
	int xValue;
	int zValue;


	if(number==0)
	{
		xValue=180;
		zValue=160;
	}

	if(number==1)
	{ //160 140
		xValue=160;
		zValue=140;
	}
	if(number==2)
	{ // 150 160
		xValue=150;
		zValue=150;
	}


	switch(a)
	{
		case 1:
			p[number].z -=zValue;
			PatrolLocation[number]=p[number];	
			break;

		case 2:
			p[number].x +=xValue;
			PatrolLocation[number]=p[number];
			break;

		case 3:
			p[number].z +=zValue;
			PatrolLocation[number]=p[number];	
			break;

		case 4:
			p[number].x -=xValue;
			PatrolLocation[number]=p[number];	
			break;
	}
	

}
void TutorialApplication::InitPatrol2(int number)
{	
	// patrol ó�� ������ ��ġ
	// ��������2
	if(number==0)
	{
	p[0].x =20;
	p[0].y =10;
	p[0].z =-490;
	}
	if(number==1)
	{
	p[1].x = 20;
	p[1].y = 10;
	p[1].z = -490;
	}
	if(number==2)
	{
	p[2].x=20;
	p[2].y=10;
	p[2].z=-490;
	}

	a=Range(1,4);

	// ��ȭ ��
	int xValue;
	int zValue;


	if(number==0)
	{
		xValue=180;
		zValue=160;
	}

	if(number==1)
	{
		xValue=160;
		zValue=140;
	}
	if(number==2)
	{
		xValue=150;
		zValue=150;
	}


	switch(a)
	{
		case 1:
			p[number].z -=zValue;
			PatrolLocation_2[number]=p[number];	
			break;

		case 2:
			p[number].x +=xValue;
			PatrolLocation_2[number]=p[number];
			break;

		case 3:
			p[number].z +=zValue;
			PatrolLocation_2[number]=p[number];	
			break;

		case 4:
			p[number].x -=xValue;
			PatrolLocation_2[number]=p[number];	
			break;
	}
	

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool TutorialApplication::processUnbufferInput(const Ogre::FrameEvent& evt)
{

	static bool mMouseDown = false;

	static Ogre::Real cMove = 150; // cart�� ���ǵ�
	static Ogre::Real cRotate = 10;
	static Ogre::Real CameraMove = 150;
	static Ogre::Real eMove = 130;
	static Ogre::Real eMove2 = 145;


	
	pDistance[0]= PatrolLocation[0]-EnemyLocation[0];
	pDistance[1]= PatrolLocation[1]-EnemyLocation[1];
	pDistance[2]= PatrolLocation[2]-EnemyLocation[2];


	pDistance_2[0]= PatrolLocation_2[0]-EnemyLocation_2[0];
	pDistance_2[1]= PatrolLocation_2[1]-EnemyLocation_2[1];
	pDistance_2[2]= PatrolLocation_2[2]-EnemyLocation_2[2];


	//ĳ����-ī�޶� ��ġ
	Ogre::Vector3 CartLocation = mSceneMgr->getSceneNode("Cart")->getPosition();
	Ogre::Vector3 CameraLocation = mSceneMgr->getSceneNode("CameraNode")->getPosition();
	Ogre::Vector3 cDistance = CartLocation-CameraLocation; // ī�޶� �Ÿ�


	Ogre::Vector3 CameraVector = Ogre::Vector3::ZERO;
	Ogre::Vector3 CartVector = Ogre::Vector3::ZERO;
	Ogre::Vector3 EnemyVector = Ogre::Vector3::ZERO;


	// �� Patrol
	//----------------------------------------------------------------------------------------------------------------------
	if(DestroyEnemy[0]==false)
	{
		EnemyLocation[0]= mSceneMgr->getSceneNode("Enemy1")->getPosition();
		mSceneMgr->getSceneNode("EnemyEffect")->setPosition(EnemyLocation[0]);
		Patrol(pDistance[0], EnemyLocation[0], EnemyVector, eMove, evt.timeSinceLastFrame, mSceneMgr->getSceneNode("Enemy1"),0);
	}
	if(DestroyEnemy[1]==false)
	{
		EnemyLocation[1]= mSceneMgr->getSceneNode("Enemy2")->getPosition();
		mSceneMgr->getSceneNode("Enemy2Effect")->setPosition(EnemyLocation[1]);
		Patrol(pDistance[1], EnemyLocation[1], EnemyVector, eMove, evt.timeSinceLastFrame, mSceneMgr->getSceneNode("Enemy2"),1);
	}
	if(DestroyEnemy[2]==false)
	{
		EnemyLocation[2]= mSceneMgr->getSceneNode("Enemy3")->getPosition();
		mSceneMgr->getSceneNode("Enemy3Effect")->setPosition(EnemyLocation[2]);
		Patrol(pDistance[2], EnemyLocation[2], EnemyVector, eMove, evt.timeSinceLastFrame, mSceneMgr->getSceneNode("Enemy3"),2);
	}

	if(DestroyEnemy_2[0]==false)
	{
		EnemyLocation_2[0]= mSceneMgr->getSceneNode("Enemy1_2")->getPosition();
		mSceneMgr->getSceneNode("EnemyEffect_2")->setPosition(EnemyLocation_2[0]);
		Patrol2(pDistance_2[0], EnemyLocation_2[0], EnemyVector, eMove2, evt.timeSinceLastFrame, mSceneMgr->getSceneNode("Enemy1_2"),0);
	}
	if(DestroyEnemy_2[1]==false)
	{
		EnemyLocation_2[1]= mSceneMgr->getSceneNode("Enemy2_2")->getPosition();
		mSceneMgr->getSceneNode("Enemy2Effect_2")->setPosition(EnemyLocation_2[1]);
		Patrol2(pDistance_2[1], EnemyLocation_2[1], EnemyVector, eMove2, evt.timeSinceLastFrame, mSceneMgr->getSceneNode("Enemy2_2"),1);
	}
	if(DestroyEnemy_2[2]==false)
	{
		EnemyLocation_2[2]= mSceneMgr->getSceneNode("Enemy3_2")->getPosition();
		mSceneMgr->getSceneNode("Enemy3Effect_2")->setPosition(EnemyLocation_2[2]);
		Patrol2(pDistance_2[2], EnemyLocation_2[2], EnemyVector, eMove2, evt.timeSinceLastFrame, mSceneMgr->getSceneNode("Enemy3_2"),2);
	}
	//----------------------------------------------------------------------------------------------------------------------

	//�ٿ�� �ڽ���
	CartBox = mSceneMgr->getEntity("CartEnt")->getWorldBoundingBox();
	if(DestroyWall==false)
		WallBox = mSceneMgr->getEntity("SnowmanEnt")->getWorldBoundingBox();
	if(DestroyNpc1==false)
		NPC1Box = mSceneMgr->getEntity("NPC1Ent")->getWorldBoundingBox();
	if(DestroyNpc2==false)
		NPC2Box = mSceneMgr->getEntity("NPC2Ent")->getWorldBoundingBox();

	if(DestroyNewBullet==false)
		NewBulletBox=mSceneMgr->getEntity("NewBulletEnt")->getWorldBoundingBox();

	UFOBox=mSceneMgr->getEntity("UFOEnt")->getWorldBoundingBox();


	// ĳ���Ϳ� NPC�� ��ġ
	if(CartBox.intersects(NPC1Box))
	{
		NPC1touch = true;
	}
	else
		NPC1touch=false;

	if(CartBox.intersects(NPC2Box))
	{
		NPC2touch = true;
	}
	else
		NPC2touch=false;	

	// ���1�� ����Ʈ�� �Ϸ��� ���¿��� ������� �ε����� ���
	if(DestroyNpc1==true)
	{
		if(CartBox.intersects(WallBox) && DestroyWall==false)
		{

			fsnow=true;
			mSceneMgr->getSceneNode("SnowEffect")->attachObject(snow);
			mSceneMgr->getSceneNode("SnowEffect2")->attachObject(snow2);
			mSceneMgr->getSceneNode("SnowEffect3")->attachObject(snow3);
			snow->getEmitter(0)->setEmissionRate(300);
			snow2->getEmitter(0)->setEmissionRate(300);
			snow3->getEmitter(0)->setEmissionRate(300);

			DestroyWall=true;

			mSceneMgr->destroyEntity(mSceneMgr->getEntity("SnowmanEnt"));
			mSceneMgr->destroySceneNode(mSceneMgr->getSceneNode("SnowmanNode"));
			StageLabel->setCaption("2-heart fish Stage");
			mSceneMgr->destroyEntity(mSceneMgr->getEntity("NPC1Ent2"));
			mSceneMgr->destroySceneNode("NPC1_2");
		}
	}

	// NewBullet�� ĳ���Ͱ� �ε����� ���
	if(CartBox.intersects(NewBulletBox) && DestroyNewBullet==false)
	{
		mSceneMgr->destroyEntity("NewBulletEnt");
		mSceneMgr->destroySceneNode("newBulletNode");
		DestroyNewBullet=true;
		nBullet+=8;
	  BulletLabel->setCaption("Bullet :" + Ogre::StringConverter::toString(nBullet));
	}

	//UFO�� �ε����� �� CLEARȭ������!
	if(CartBox.intersects(UFOBox))
	{
		CurrentScene="clear";
		SetupViewport(ClearSceneMgr);	
	}
	

	// Cart�� ������
	if(mKeyboard->isKeyDown(OIS::KC_W))
	{
		// ��鿡���� �¿� ������ ����
		if(CartLocation.x>240 || CartLocation.x<-240)
		{
			CartVector.z+=0;
			if(CartLocation.x>240)
				mSceneMgr->getSceneNode("Cart")->setPosition(239.9,CartLocation.y,CartLocation.z);
			else if(CartLocation.x<-240)
				mSceneMgr->getSceneNode("Cart")->setPosition(-239.9,CartLocation.y,CartLocation.z);
		}

		else if(DestroyNpc1==false && CartLocation.z<-300)
		{
			CartVector.z +=0;
			if(CartLocation.z<-300)
				mSceneMgr->getSceneNode("Cart")->setPosition(CartLocation.x, CartLocation.y, -299.9);
		}
		else if(DestroyNpc2==false && CartLocation.z<-2000)
		{
			CartVector.z +=0;
			if(CartLocation.z<-2000)
				mSceneMgr->getSceneNode("Cart")->setPosition(CartLocation.x, CartLocation.y, -1999.9);
		}
		else if(DestroyWall==false && CartLocation.z <-1000)
		{
			CartVector.z +=0;
			if(CartLocation.z<-1000)
				mSceneMgr->getSceneNode("Cart")->setPosition(CartLocation.x, CartLocation.y, -999.9);			
		}

		else
		CartVector.z+=cMove;		

		// ����� �ı� �� ��ƼŬ ���߰�
		if(CartLocation.z <-1200)
		{
			if(fsnow==true)
			{
				fsnow=false;
				snow->getEmitter(0)->setEmissionRate(0);
				snow2->getEmitter(0)->setEmissionRate(0);
				snow3->getEmitter(0)->setEmissionRate(0);
			}	
		}


		//-----------------------------------------------------------------------
		// ī�޶��� �̵��� �����ϴ� �κ�.
		// ī�޶�~ĳ���� ������ �Ÿ��� ���ؼ� �� �Ÿ��� �о����� ī�޶� ���󰡰�, �������� �ڷ� �������� �Ѵ�.
		//-----------------------------------------------------------------------
		if(cDistance.z >Distance)
			CameraVector.z+=CameraMove;
		else if(cDistance.z == Distance)
			CameraVector.z-=0;
		else
			CameraVector.z-=CameraMove;

	}

	if(mKeyboard->isKeyDown(OIS::KC_S))
	{
			// ��鿡���� �¿� ������ ����
		if(CartLocation.x>240 || CartLocation.x<-240)
		{
			CartVector.z-=0;
			if(CartLocation.x>240)
				mSceneMgr->getSceneNode("Cart")->setPosition(239.9,CartLocation.y,CartLocation.z);
			else if(CartLocation.x<-240)
				mSceneMgr->getSceneNode("Cart")->setPosition(-239.9,CartLocation.y,CartLocation.z);
		}

		else if(DestroyNpc1==false && CartLocation.z<-300)
		{
			CartVector.z -=0;
			if(CartLocation.z<-300)
				mSceneMgr->getSceneNode("Cart")->setPosition(CartLocation.x, CartLocation.y, -299.9);
		}

		else if(DestroyNpc2==false && CartLocation.z<-2000)
		{
			CartVector.z +=0;
			if(CartLocation.z<-2000)
				mSceneMgr->getSceneNode("Cart")->setPosition(CartLocation.x, CartLocation.y, -1999.9);
		}

		else if(DestroyWall==false && CartLocation.z <-1000)
		{
			CartVector.z +=0;
			if(CartLocation.z<-1000)
				mSceneMgr->getSceneNode("Cart")->setPosition(CartLocation.x, CartLocation.y, -999.9);			
		}
		else
			CartVector.z-=cMove;	

		//-----------------------------------------------------------------------
		// ī�޶��� �̵��� �����ϴ� �κ�.
		// ī�޶�~ĳ���� ������ �Ÿ��� ���ؼ� �� �Ÿ��� �о����� ī�޶� ���󰡰�, �������� �ڷ� �������� �Ѵ�.
		//-----------------------------------------------------------------------
		if(cDistance.z >Distance)
			CameraVector.z+=CameraMove;
		else if(cDistance.z == Distance)
			CameraVector.z-=0;
		else
			CameraVector.z-=CameraMove;


	}
	if(mKeyboard->isKeyDown(OIS::KC_A))
	{

		mSceneMgr->getSceneNode("Cart")->yaw(Ogre::Degree(cMove*evt.timeSinceLastFrame),Ogre::Node::TS_LOCAL);
		angle =Ogre::Degree(cMove*evt.timeSinceLastFrame);
	}

	if(mKeyboard->isKeyDown(OIS::KC_D))
	{

		mSceneMgr->getSceneNode("Cart")->yaw(Ogre::Degree(-cMove*evt.timeSinceLastFrame),Ogre::Node::TS_LOCAL);	

	}




	mSceneMgr->getSceneNode("Cart")->translate(CartVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);	
	mSceneMgr->getSceneNode("CameraNode")->translate(CameraVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);	

	
	return true;
}


void TutorialApplication::fTime(Ogre::Real time, Ogre::Real time2, int i)
{
	
	const Ogre::Real rTime = time*100;

	
	if(time2>rTime)
	{
		if(fenemyeffect[i]==true)
		{
			fenemyeffect[i]=false;
			enemyeffect[i]->getEmitter(0)->setEmissionRate(0);
		}

		if(fenemyeffect_2[i]==true)
		{
			fenemyeffect_2[i]=false;
			enemyeffect_2[i]->getEmitter(0)->setEmissionRate(0);
		}
	}
}

//-----------------------------------------------------------------------
// Setting up the FrameListner
// Call the capture method on all Keyboard, Mouse, Joystick
//-----------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed()) return false;

	mKeyboard->capture();
	mMouse->capture();

	
	if((DestroyEnemy[0]==false || DestroyEnemy[1]==false || DestroyEnemy[2]==false) && nBullet==0)
	{
		CurrentScene="over";
		SetupViewport(OverSceneMgr);	
	}

	if((DestroyEnemy_2[0]==false || DestroyEnemy_2[1]==false || DestroyEnemy_2[2]==false) && nBullet==0)
	{
		CurrentScene="over";
		SetupViewport(OverSceneMgr);	
	}

	//�Ѿ� ������
	UpdateBullet(evt.timeSinceLastFrame);

	if(Patrolreach_x[0]==true && Patrolreach_y[0]==true)
	{
			Patrolreach_x[0]=false;
			Patrolreach_y[0]=false;
			InitPatrol(0);

	}
	if(Patrolreach_x[1]==true && Patrolreach_y[1]==true)
	{
			Patrolreach_x[1]=false;
			Patrolreach_y[1]=false;
			InitPatrol(1);

	}
	if(Patrolreach_x[2]==true && Patrolreach_y[2]==true)
	{
			Patrolreach_x[2]=false;
			Patrolreach_y[2]=false;
			InitPatrol(2);

	}

		if(Patrolreach_x_2[0]==true && Patrolreach_y_2[0]==true)
		{
			Patrolreach_x_2[0]=false;
			Patrolreach_y_2[0]=false;	
			InitPatrol2(0);
		}

		if(Patrolreach_x_2[1]==true && Patrolreach_y_2[1]==true)
		{
			Patrolreach_x_2[1]=false;
			Patrolreach_y_2[1]=false;	
			InitPatrol2(1);
		}

		if(Patrolreach_x_2[2]==true && Patrolreach_y_2[2]==true)
		{
			Patrolreach_x_2[2]=false;
			Patrolreach_y_2[2]=false;	
			InitPatrol2(2);

		}


	if(fenemyeffect[0]==true)
	{
		Time[0] += evt.timeSinceLastFrame;
		fTime(evt.timeSinceLastFrame,Time[0],0);
	}
	if(fenemyeffect[1]==true)
	{
		Time[1] += evt.timeSinceLastFrame;
		fTime(evt.timeSinceLastFrame,Time[1],1);
	}
	if(fenemyeffect[2]==true)
	{
		Time[2] += evt.timeSinceLastFrame;
		fTime(evt.timeSinceLastFrame,Time[2],2);
	}

	if(fenemyeffect_2[0]==true)
	{
		Time[3] += evt.timeSinceLastFrame;
		fTime(evt.timeSinceLastFrame,Time[3],0);
	}
	if(fenemyeffect_2[1]==true)
	{
		Time[4] += evt.timeSinceLastFrame;
		fTime(evt.timeSinceLastFrame,Time[4],1);
	}
	if(fenemyeffect_2[2]==true)
	{
		Time[5] += evt.timeSinceLastFrame;
		fTime(evt.timeSinceLastFrame,Time[5],2);
	}

	if(DestroyEnemy[0]==false)
			EnemyBox[0] = mSceneMgr->getEntity("EnemyEnt")->getWorldBoundingBox();
	if(DestroyEnemy[1]==false)
			EnemyBox[1]= mSceneMgr->getEntity("Enemy2Ent")->getWorldBoundingBox();
	if(DestroyEnemy[2]==false)
			EnemyBox[2]= mSceneMgr->getEntity("Enemy3Ent")->getWorldBoundingBox();
	
	if(DestroyEnemy_2[0]==false)
			EnemyBox_2[0] = mSceneMgr->getEntity("EnemyEnt_2")->getWorldBoundingBox();
	if(DestroyEnemy_2[1]==false)
			EnemyBox_2[1]= mSceneMgr->getEntity("Enemy2Ent_2")->getWorldBoundingBox();
	if(DestroyEnemy_2[2]==false)
			EnemyBox_2[2]= mSceneMgr->getEntity("Enemy3Ent_2")->getWorldBoundingBox();	

	 if(firework==true)
{
	fireworkeffect->getEmitter(0)->setEmissionRate(300);
}


	if(CurrentScene=="game")
	{
		mAnimationState->addTime(evt.timeSinceLastFrame);
		mAnimationState2->addTime(evt.timeSinceLastFrame);
		for(int i=0; i<3; i++)
		{
			EnemyAnimationState[i]->addTime(evt.timeSinceLastFrame);
			EnemyAnimationState_2[i]->addTime(evt.timeSinceLastFrame);
		}
		mTrayMgr->frameRenderingQueued(evt);
		mCameraMan->frameRenderingQueued(evt);
		StageLabel->show();
		BulletLabel->show();
	}


	if (!processUnbufferInput(evt)) return false;
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

	return true;
}

//-----------------------------------------------------------------------
// Shutting down OIS
//-----------------------------------------------------------------------

void TutorialApplication::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void TutorialApplication::windowClosed(Ogre::RenderWindow* rw)
{
	if(rw == mWindow) {
	if(mInputMgr) {
	mInputMgr->destroyInputObject(mMouse);
	mInputMgr->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputMgr);
	delete mListner;
	mInputMgr = 0;
	}
	}
}




//-----------------------------------------------------------------------
// ������ �ʱ�ȭ, �����Լ�
//-----------------------------------------------------------------------

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	#endif

	#ifdef __cplusplus
	extern "C"
	{
	#endif
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
	#else
		int main(int argc, char *argv[])
		#endif
		{
	
			TutorialApplication app;
			srand((unsigned)time(NULL));
			try
			{			
				app.go();

			}

			catch(Ogre::Exception& e)
			{
				#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
					MessageBox(
					NULL,
					e.getFullDescription().c_str(),
					"An exception has occured!",
					MB_OK | MB_ICONERROR | MB_TASKMODAL);
				#else
					std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
				#endif
			}
		return 0;
		}

	#ifdef __cplusplus
	}
	#endif