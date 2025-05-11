#include "Game.h"
#include <time.h>

void Game::Init(HWND hWnd) {
	srand((unsigned)time(NULL));

	D3D_Create(hWnd);

	sound.Init();	//�T�E���h��������
	sound.Play(SOUND_LABEL_BGM000);	//BGM���Đ�

	stage.Init(L"asset/stage.png", 1, 1, 0);
	stage.SetPos(0.0f, 0.0f, 0.0f);	//�ʒu��ݒ�
	stage.SetSize(300.0f, 300.0f, 0.0f);	//�傫����ݒ�
	stage.SetAngle(0.0f);				//�p�x��ݒ�
	stage.SetColor(1.0f, 1.0f, 1.0f, 0.5f);	//�F��ݒ�

	for (int i = 0; i < 500; i++) {
		ball[i].Init(L"asset/ball.png", 1, 1, 0);
		ball[i].SetPos(0.0f, 0.0f, 0.0f);	//�ʒu��ݒ�
		ball[i].SetSize(20.0f, 20.0f, 0.0f);	//�傫����ݒ�
		ball[i].SetAngle(0.0f);				//�p�x��ݒ�
		ball[i].SetColor(1.0f, 0.0f, 0.0f, 1.0f);	//�F��ݒ�
	}

	scoreboard.Init(L"asset/score.png", 10, 1);	//���_�\����������
	scoreboard.SetPos(300.0f, 200.0f, 0.0f);	//�ʒu��ݒ�
	scoreboard.SetSize(40.0f, 40.0f, 0.0f);		//�傫����ݒ�

	Maxcolorvalue.Init(L"asset/score.png", 10, 1);	//���_�\����������
	Maxcolorvalue.SetPos(0.0f, 200.0f, 0.0f);	//�ʒu��ݒ�
	Maxcolorvalue.SetSize(40.0f, 40.0f, 0.0f);		//�傫����ݒ�

	scoreUI.Init(L"asset/UI.png", 10, 1);		//���_�\����������
	scoreUI.SetPos(225.0f, 200.0f, 0.0f);		//�ʒu��ݒ�
	scoreUI.SetSize(180.0f, 40.0f, 0.0f);		//�傫����ݒ�


	title.Init(L"asset/Ctitle.png");		//�^�C�g���\����������
	title.SetPos(0.0f, 0.0f, 0.0f);			//�ʒu��ݒ�
	title.SetSize(640.0f, 480.0f, 0.0f);	//�傫����ݒ�

	button.Init(L"asset/Button.png");		//�^�C�g���\����������
	button.SetPos(0.0f, -187.0f, 0.0f);		//�ʒu��ݒ�
	button.SetSize(120.0f, 80.0f, 0.0f);	//�傫����ݒ�

	result.Init(L"asset/GameOver.jpg");		//�^�C�g���\����������
	result.SetPos(0.0f, 0.0f, 0.0f);		//�ʒu��ݒ�
	result.SetSize(640.0f, 480.0f, 0.0f);	//�傫����ݒ�

	score = 0;	//���_��������
	scene = 0;	//�V�[����������
}

void Game::Draw() {
	// �Q�[���������s
	D3D_StartRender();

	switch (scene) {
	case 0:	//�^�C�g�����
		title.Draw();	//�^�C�g����`��
		Maxcolorvalue.numU = MaxColor;
		Maxcolorvalue.Draw();
		stage.Draw();
		button.Draw();
		break;

	case 1:	//�Q�[���{��
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
	scoreboard.Uninit();	//�X�R�A���I��
	scoreUI.Uninit();	//�F���̕\�����I��
	Maxcolorvalue.Uninit();	//�F���̕\�����I��
	title.Uninit();			//�^�C�g�����I��
	result.Uninit();
	sound.Uninit();			//�T�E���h���I��

	// DirectX�̉������
	D3D_Release();
}

void Game::Update() {

	input.Update();
	Buttonsize();

	switch (scene) {
	case 0:	//�^�C�g�����
		//�L�[���͂Ŗ{�҂�
		stage.SetPos(300.0f, -200.0f, 0.5f);
		stage.SetSize(800.0f, 800.0f, 0.0f);

		if (input.GetKeyTrigger(VK_RETURN)) {
			scene = 1;
			Gameinit();
			Ballinit();
			state = 0;	//�Q�[���̏�Ԃ�������
			score = 0;	//���_��������
		}
		else if (input.GetKeyTrigger(VK_UP) && MaxColor < 7) {
			MaxColor++;
		}
		else if (input.GetKeyTrigger(VK_DOWN) && MaxColor > 1) {
			MaxColor--;
		}
		StageRotation(1);
		break;
	case 1:	//�Q�[���{��

		//�Q�[���I�[�o�[�Ȃ�Q�[���̐i�s���~�߂�
		if (state == 3) {
			//�L�[���͂Ń^�C�g����ʂ֖߂�
			scene = 2;
			return;
		}
		else if (state == 0) {	//�������̃{�[�����Ȃ�
			stage.SetPos(0.0f, 0.0f, 0.5f);
			stage.SetSize(300.0f, 300.0f, 0.0f);
			BallSpawn(MaxColor);	//�{�[���̐���
		}
		else if(state == 1){	//�{�[��������
		StageRotation(0);	//�X�e�[�W�̉�]
		FallBall();		//�{�[���̗���
		Arrange();		//����������
		}
		if (input.GetKeyTrigger(VK_BACK)) {
			scene = 0;
			state = 0;
		}
		break;


	case 2:	//���U���g���
		if (input.GetKeyTrigger(VK_RETURN)) {
			scene = 0;
		}
		break;
	}

}
