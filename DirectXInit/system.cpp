#include "Game.h"
#include <stdio.h>
#define Stagebase (57.0f)

static float ball_y = 200.0f;
static float speed = 3;
static int ballnumber = 0;
int arrangenumber[100] = { -1 };
static int numcount = 0;


void Game::DrawScore() {
	DirectX::XMFLOAT3 pos = scoreboard.GetPos();
	DirectX::XMFLOAT3 size = scoreboard.GetSize();
	int keta = 0;
	do {
		scoreboard.numU = score % (int)pow(10, keta + 1) / (int)pow(10, keta);	//一桁切り出す
		scoreboard.SetPos(pos.x - (size.x - 10.0f) * keta, pos.y, pos.z);		//位置を設定
		scoreboard.Draw();		//スコアを描画
		keta++;
	} while (score >= (int)pow(10, keta));
	scoreboard.SetPos(pos.x, pos.y, pos.z);	//位置を元に戻す
}
void Game::Buttonsize() {
	float sizeval = 1.5f;
	static int flg;
	DirectX::XMFLOAT3 size = button.GetSize();

	if (flg == 0 && size.x > 130.0f) {
		flg = 1;
	}
	else if (flg == 1 && size.x < 110.0f) {
		flg = 0;
	}

	if (flg == 0) {
		size.x += sizeval;
		size.y += sizeval;
	}
	else {
		size.x -= sizeval;
		size.y -= sizeval;
	}

	if (size.x > 200.0f) {
		size.x = 200.0f;
	}
	else if (size.x < 50.0f) {
		size.x = 50.0f;
	}
	if (size.y > 150.0f) {
		size.y = 150.0f;
	}
	else if (size.y < 30.0f) {
		size.y = 30.0f;
	}

	button.SetSize(size.x, size.y, size.z);
}

void Game::StageRotation(int title) {
	float rotationspeed = 0.0f;
	if (input.GetKeyPress(VK_LEFT) || title == 1) {
		stage._angle += 1;
		rotationspeed += 1.0f;
	}
	else if (input.GetKeyPress(VK_RIGHT) || title == 2) {
		stage._angle -= 1;
		rotationspeed -= 1.0f;
	}
	else {
		rotationspeed = 0.0f;
	}
	stage.SetAngle(stage._angle);
	for (int i = 0; i < Ball_Max; i++) {
		if (ball[i].state == 2) {
			float x = ball[i].x;
			float y = ball[i].y;
			float angle = rotationspeed * (3.14 / 180);
			float x2 = x * cos(angle) - y * sin(angle);
			float y2 = x * sin(angle) + y * cos(angle);

			ball[i].x = x2;
			ball[i].y = y2;

			ball[i].SetPos(x2, y2, 0.0f);

		}
	}
}

void Game::Ballinit() {
	for (int i = 0; i < Ball_Max; i++) {
		ball[i].ballnum = -1;
		ball[i].SetPos(10000.0f, 10000.0f, 0.0f);
	}
}

void Game::BallSpawn(int MaxColor) {
	if (state == 0) {
		//ゲームの状態を「１：落下中」に変更
		state = 1;
		int r = rand() % MaxColor;	//7色からランダムで生成
		//int r = 0;	//デバッグ	赤のみ落下
		if (ball[ballnumber].state == 0) {

			ball[ballnumber].ballnum = ballnumber;

			ball[ballnumber].SetPos(0.0f, 200.0f, 0.0f);
			ball[ballnumber].ballcolor = r;
			ball[ballnumber].state = 1;	//ボールの状態を落下中にする
			switch (r) {
			case 0: ball[ballnumber].SetColor(1.0f, 0.0f, 0.0f, 0.5f);
				break;
			case 1: ball[ballnumber].SetColor(0.0f, 1.0f, 0.0f, 0.5f);
				break;
			case 2: ball[ballnumber].SetColor(0.0f, 0.0f, 1.0f, 0.5f);
				break;
			case 3: ball[ballnumber].SetColor(1.0f, 0.0f, 1.0f, 0.5f);
				break;
			case 4: ball[ballnumber].SetColor(1.0f, 1.0f, 0.0f, 0.5f);
				break;
			case 5: ball[ballnumber].SetColor(0.0f, 1.0f, 1.0f, 0.5f);
				break;
			case 6: ball[ballnumber].SetColor(1.0f, 1.0f, 1.0f, 0.5f);
				break;
			}
		}
	}
}

void Game::FallBall() {
	bool hit = false;
	if (state == 1) {
		count++;
		int dropinterval = 0;
		float decreasespeed = 0.03f;
		if (input.GetKeyPress(VK_DOWN) && state == 1) {
			dropinterval = 0.5;
		}
		else {
			dropinterval = 1;	//加速
		}

		if (count > dropinterval) {
			count = 0;
			ball_y -= speed;
			ball[ballnumber].falltime++;

			speed -= decreasespeed;
			if (speed < 0.7f) {
				speed = 0.7f;
			}
		}

		ball[ballnumber].SetPos(0.0f, ball_y, 0.0f);
		ball[ballnumber].y = ball_y;

		for (int i = 0; i < Ball_Max; i++) {
			if (ballnumber != i) {
				hit = CircleHit(ball[ballnumber], ball[i]);
			}
			if (hit == true) {
				break;
			}
		}

		if (ball_y < Stagebase || hit == true) {
			if (ball[ballnumber].y < 160.0f) {
				ball_y = 200.0f;
				state = 0;
				ball[ballnumber].state = 2;	//ボールの状態を着地にする
				ballnumber++;
				speed = 3;
			}
			else {
				//gameover
				//scene = 0;
				state = 3;
			}
		}
	}

}

void Game::Gameinit() {
	for (int i = 0; i < Ball_Max; i++) {
		ball[i].x = 0.0f;
		ball[i].y = 500.0f;
	}
}

void Game::Arrange() {
	for (int i = 0; i < 100; i++) {
		arrangenumber[i] = -1;
	}
	for (int i = 0; i < Ball_Max; i++) {	//各ボールで探索
		if (ball[i].state == 2) {	//対象が着地済みの場合のみ探索開始
			numcount = 0;
			Arrange_Color(ball[i]);
			if (numcount > 4) {	//5個以上繋がっているなら消してスコアアップ
				for (int j = 0; j < 100; j++) {
					if (arrangenumber[j] != -1) {
						ball[arrangenumber[j]].x = 0.0f;
						ball[arrangenumber[j]].y = 500.0f;
						ball[arrangenumber[j]].SetPos(10000.0f, 10000.0f, 0.0f);
						ball[arrangenumber[j]].state = 0;
						score += numcount * numcount;
					}
				}
			}
			else if (numcount > 0 && numcount <= 4) {	//4個以下なら繋がっている数に応じて色が濃くなる
				float alpha = 0.5f;
				alpha = (numcount * 0.1f) + 0.5f;
				if (alpha > 1.0f) {
					alpha = 1.0f;
				}
				else if (alpha < 0.5f) {
					alpha = 0.5;
				}
				for (int j = 0; j < 100; j++) {
					if (arrangenumber[j] != -1) {
						ball[arrangenumber[j]].SetAlpha(alpha);
					}
				}
			}
		}
	}
}
void Game::Arrange_Color(Object targetball) {	//再帰プログラム		現在のボールと接触している同じ色のボールがあればそのボール番号を記録
	bool existing = false;
	for (int i = 0; i < Ball_Max; i++) {
		existing = false;
		if (ball[i].state == 2) {
			if (SerchCircleHit(targetball, ball[i]) == true) {	//対象のボールとそれ以外のボールが接触しているなら
				if (targetball.ballnum != ball[i].ballnum) {
					if (targetball.ballcolor == ball[i].ballcolor) {
						for (int j = 0; j < 100; j++) {		//現在の番号が既に配列に無いなら
							if (i == arrangenumber[j]) {
								existing = true;
							}
						}
						if (existing == false) {
							arrangenumber[numcount] = i;	//今の探索番号を記録
							numcount++;
							Arrange_Color(ball[i]);
						}
					}
				}
			}
		}
	}
}

bool Game::CircleHit(Object object1, Object object2) {
	float base = object1.x - object2.x;
	float height = object1.y - object2.y;
	float hypotenuse = pow(base, 2.0f) + pow(height, 2.0f);
	if (hypotenuse <= pow(object1.r + object2.r, 2.0f)) {
		return true;
	}
	else {
		return false;
	}
}
bool Game::SerchCircleHit(Object object1, Object object2) {	//ちょっと判定大きめ
	float base = object1.x - object2.x;
	float height = object1.y - object2.y;
	float hypotenuse = pow(base, 2.0f) + pow(height, 2.0f);
	if (hypotenuse <= pow((object1.r + 0.8f) + (object2.r + 0.8f), 2.0f)) {
		return true;
	}
	else {
		return false;
	}
}