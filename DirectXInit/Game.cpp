#include "Game.h"
#include <time.h>

void Game::Init(HWND hWnd) {
	srand((unsigned)time(NULL));

	D3D_Create(hWnd);

	sound.Init();	//サウンドを初期化
	sound.Play(SOUND_LABEL_BGM000);	//BGMを再生

	stage.Init(L"asset/stage.png", 1, 1, 0);
	stage.SetPos(0.0f, 0.0f, 0.0f);	//位置を設定
	stage.SetSize(300.0f, 300.0f, 0.0f);	//大きさを設定
	stage.SetAngle(0.0f);				//角度を設定
	stage.SetColor(1.0f, 1.0f, 1.0f, 0.5f);	//色を設定

	for (int i = 0; i < 500; i++) {
		ball[i].Init(L"asset/ball.png", 1, 1, 0);
		ball[i].SetPos(0.0f, 0.0f, 0.0f);	//位置を設定
		ball[i].SetSize(20.0f, 20.0f, 0.0f);	//大きさを設定
		ball[i].SetAngle(0.0f);				//角度を設定
		ball[i].SetColor(1.0f, 0.0f, 0.0f, 1.0f);	//色を設定
	}

	scoreboard.Init(L"asset/score.png", 10, 1);	//得点表示を初期化
	scoreboard.SetPos(300.0f, 200.0f, 0.0f);	//位置を設定
	scoreboard.SetSize(40.0f, 40.0f, 0.0f);		//大きさを設定

	Maxcolorvalue.Init(L"asset/score.png", 10, 1);	//得点表示を初期化
	Maxcolorvalue.SetPos(0.0f, 200.0f, 0.0f);	//位置を設定
	Maxcolorvalue.SetSize(40.0f, 40.0f, 0.0f);		//大きさを設定

	scoreUI.Init(L"asset/UI.png", 10, 1);		//得点表示を初期化
	scoreUI.SetPos(225.0f, 200.0f, 0.0f);		//位置を設定
	scoreUI.SetSize(180.0f, 40.0f, 0.0f);		//大きさを設定


	title.Init(L"asset/Ctitle.png");		//タイトル表示を初期化
	title.SetPos(0.0f, 0.0f, 0.0f);			//位置を設定
	title.SetSize(640.0f, 480.0f, 0.0f);	//大きさを設定

	button.Init(L"asset/Button.png");		//タイトル表示を初期化
	button.SetPos(0.0f, -187.0f, 0.0f);		//位置を設定
	button.SetSize(120.0f, 80.0f, 0.0f);	//大きさを設定

	result.Init(L"asset/GameOver.jpg");		//タイトル表示を初期化
	result.SetPos(0.0f, 0.0f, 0.0f);		//位置を設定
	result.SetSize(640.0f, 480.0f, 0.0f);	//大きさを設定

	score = 0;	//得点を初期化
	scene = 0;	//シーンを初期化
}

void Game::Draw() {
	// ゲーム処理実行
	D3D_StartRender();

	switch (scene) {
	case 0:	//タイトル画面
		title.Draw();	//タイトルを描画
		Maxcolorvalue.numU = MaxColor;
		Maxcolorvalue.Draw();
		stage.Draw();
		button.Draw();
		break;

	case 1:	//ゲーム本編
		stage.Draw();
		for (int i = 0; i < Ball_Max; i++) {
			ball[i].Draw();
		}
		scoreUI.Draw();
		DrawScore();
		break;
	case 2:
		result.Draw();
		scoreUI.Draw();
		DrawScore();
		button.Draw();
		break;
	}



	D3D_FinishRender();
}

void Game::Uninit() {
	stage.Uninit();
	for (int i = 0; i < 500; i++) {
		ball[i].Uninit();
	}
	scoreboard.Uninit();	//スコアを終了
	scoreUI.Uninit();	//色数の表示を終了
	Maxcolorvalue.Uninit();	//色数の表示を終了
	title.Uninit();			//タイトルを終了
	result.Uninit();
	sound.Uninit();			//サウンドを終了

	// DirectXの解放処理
	D3D_Release();
}

void Game::Update() {

	input.Update();
	Buttonsize();

	switch (scene) {
	case 0:	//タイトル画面
		//キー入力で本編へ
		stage.SetPos(300.0f, -200.0f, 0.5f);
		stage.SetSize(800.0f, 800.0f, 0.0f);

		if (input.GetKeyTrigger(VK_RETURN)) {
			scene = 1;
			Gameinit();
			Ballinit();
			state = 0;	//ゲームの状態を初期化
			score = 0;	//得点を初期化
		}
		else if (input.GetKeyTrigger(VK_UP) && MaxColor < 7) {
			MaxColor++;
		}
		else if (input.GetKeyTrigger(VK_DOWN) && MaxColor > 1) {
			MaxColor--;
		}
		StageRotation(1);
		break;
	case 1:	//ゲーム本編

		//ゲームオーバーならゲームの進行を止める
		if (state == 3) {
			//キー入力でタイトル画面へ戻る
			scene = 2;
			return;
		}
		else if (state == 0) {	//落下中のボールがない
			stage.SetPos(0.0f, 0.0f, 0.5f);
			stage.SetSize(300.0f, 300.0f, 0.0f);
			BallSpawn(MaxColor);	//ボールの生成
		}
		else if(state == 1){	//ボール落下中
		StageRotation(0);	//ステージの回転
		FallBall();		//ボールの落下
		Arrange();		//揃った判定
		}
		if (input.GetKeyTrigger(VK_BACK)) {
			scene = 0;
			state = 0;
		}
		break;


	case 2:	//リザルト画面
		if (input.GetKeyTrigger(VK_RETURN)) {
			scene = 0;
		}
		break;
	}

}
