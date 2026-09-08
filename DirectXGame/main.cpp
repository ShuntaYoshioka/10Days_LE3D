#include "ClearScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include <Windows.h>

enum class Scene {

	kUnknow = 0,

kTitle,
kTutorial,
kGame,
kClear,
};

Scene scene = Scene::kUnknow;

using namespace KamataEngine;

void ChangeScene();

void UpdateScene();

void DrawScene();

GameScene* gameScene = nullptr;

TitleScene* titleScene = nullptr;

TutorialScene* tutorialScene = nullptr;

ClearScene* clearScene = nullptr;

// チュートリアルを一度見たか(見た後はタイトルから直接ゲームへ)
bool hasSeenTutorial = false;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"壁壊す");

	// DirectXCommon*インスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// デバッグテキスト(ClearSceneのスコア表示等で使用)の初期化
	DebugText::GetInstance()->Initialize();

	scene = Scene::kTitle;

	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {

		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		//ゲームシーンの更新
		ChangeScene();

		// 描画開始
		UpdateScene();
		dxCommon->PreDraw();

		//ゲームシーンの描画
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	//ゲームシーンの解放
	delete gameScene;
	delete clearScene;
	delete tutorialScene;


	//nullptrの代入
	gameScene = nullptr;
	clearScene = nullptr;
	tutorialScene = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}

void ChangeScene() {
	switch (scene) { 
		case Scene::kTitle:
		if (titleScene->isFinished()) {
			//旧scene開放
			delete titleScene;
			titleScene = nullptr;

			if (hasSeenTutorial) {
				//scene変化(2回目以降はチュートリアルを飛ばす)
				scene = Scene::kGame;
				//新scene生成と初期化
				gameScene = new GameScene;
				gameScene->Initialize();
			} else {
				//scene変化
				scene = Scene::kTutorial;
				//新scene生成と初期化
				tutorialScene = new TutorialScene;
				tutorialScene->Initialize();
			}
		}
		break;

		case Scene::kTutorial:
		if (tutorialScene->isFinished()) {
		//scene変化
			scene = Scene::kGame;
			hasSeenTutorial = true;
			//旧scene開放
			delete tutorialScene;
			tutorialScene = nullptr;
			//新scene生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

		case Scene::kGame:
		    // ゲームシーンの更新処理
		    if (gameScene) {

			    if (gameScene->isFinished()) {
				    if (gameScene->isclear_) {
					    //scene変化
					    scene = Scene::kClear;
					    uint32_t openedConnectionCount = gameScene->GetOpenedConnectionCount();
					    //旧scene開放
					    delete gameScene;
					    gameScene = nullptr;
					    //新scene生成と初期化
					    clearScene = new ClearScene;
					    clearScene->Initialize(openedConnectionCount);
				    } else {
					    scene = Scene::kTitle;
					    delete gameScene;
					    gameScene = nullptr;
					    titleScene = new TitleScene;
					    titleScene->Initialize();
				    }
			    }
		    }
		    break;

		case Scene::kClear:
			if (clearScene) {
				if (clearScene->isFinished()) {
					// NextSceneがStageSelect(WIP)の場合も現状はタイトルへ戻す
					scene = Scene::kTitle;
					delete clearScene;
					clearScene = nullptr;
					titleScene = new TitleScene;
					titleScene->Initialize();
				}
			}
			break;
	}

}

void UpdateScene() { 
	switch (scene) { 
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kTutorial:
		tutorialScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	}

}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene) {
			titleScene->Draw();
		}
		break;

	case Scene::kTutorial:
		if (tutorialScene) {
			tutorialScene->Draw();
		}
		break;

	case Scene::kGame:
		if (gameScene) {
			gameScene->Draw();
		}
		break;

	case Scene::kClear:
		if (clearScene) {
			clearScene->Draw();
		}
		break;
	}
}
