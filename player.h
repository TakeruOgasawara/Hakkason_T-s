//===========================
//
//�_�C���N�gX.player�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�}�N����`
#define MAX_PLAYER_MODEL (32)		//���f���̍ő吔
#define MAX_PLAYER_KEY (32)			//�L�[�̍ő吔
#define PLAYER_RADIUS	(30.0f)		//�����蔻��̔��a

//�����
typedef enum
{
	PLAYERENVIRONMENT_NORMAL = 0,
	PLAYERENVIRONMENT_CLOUD,
	PLAYERENVIRONMENT_MAX
}PLAYERENVIRONMENT;

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//�o�����
	PLAYERSTATE_NORMAL,				//�ʏ���
	PLAYERSTATE_MOVE,				//�ړ����
	PLAYERSTATE_DAMAGE,				//�_���[�W���
	PLAYERSTATE_DEATH,				//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̃��[�V����
typedef enum
{
	PLAYERMOTION_NORMAL = 0,		//�ҋ@���[�V����
	PLAYERMOTION_MOVE,				//�ړ����[�V����
	PLAYERMOTION_PUSH,				//�A�N�V�������[�V����
	PLAYERMOTION_JUMP,				//�W�����v���[�V����
	PLAYERMOTION_LAND,				//���n���[�V����
	PLAYERMOTION_AIR,				//�󒆃��[�V����
	PLAYERMOTION_DAMAGE,			//�_���[�W���[�V����
	PLAYERMOTION_DEATH,				//���S���[�V����
	PLAYERMOTION_TIMEUP,			//���Ԑ؂ꃂ�[�V����
	PLAYERMOTION_EASTEREGG0,		//���Q�b���_�������[�V����
	PLAYERMOTION_EASTEREGG1,		//�Q�b�_����`���܁`���Ӂ`��邹�ȁ`���C�����`���[�V����
	PLAYERMOTION_EASTEREGG2,		//�Q�b�_�������U�胂�[�V����
	PLAYERMOTION_EASTEREGG3,		//�Q�b�_�����U�胂�[�V����
	PLAYERMOTION_EASTEREGG4,		//�Q�b�_���������[�V����
	PLAYERMOTION_EASTEREGG5,		//�Q�b�_���I�y�����ۂ�����[�V����
	PLAYERMOTION_EASTEREGG6,		//�Q�b�_���ݯ���[�V����
	PLAYERMOTION_MAX
}PLAYERMOTION;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int nIdxModelParent;
}PlayerModel;

//�L�[�̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_PLAYER_MODEL];
	D3DXVECTOR3 rot[MAX_PLAYER_MODEL];
	int nFrame;
}KeyMotionPlayer;

//���[�V�����̍\����
typedef struct
{
	KeyMotionPlayer aKeyMotion[MAX_PLAYER_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}PlayerMotion;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 point;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	PLAYERSTATE state;
	PLAYERMOTION motionState;
	PLAYERMOTION motionStateOld;
	PlayerModel aPlayerModel[MAX_PLAYER_MODEL];
	PlayerMotion aPlayerMotion[PLAYERMOTION_MAX];
	PLAYERENVIRONMENT environment;
	D3DXMATERIAL pMatDef;
	int nNumModel;
	int nLife;
	int nMaxLife;
	int nTimerState;
	int nIdxLifeGauge;	//�Q�[�W�̔ԍ�
	int nIdxShadow;	//�e�̔ԍ�
	int nIdxBillboard;					//�r���{�[�h�̔ԍ�
	int nEasterTimer;
	float fJump;
	float fMove;
	float fAngle;
	float fJumpPow;
	bool bUse;
	bool bJump;
	bool bGod;
	bool bEaster;
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdatePlayerRanking(void);
void DrawPlayer(void);
void DrawPlayerShadow(void);
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
Player *GetPlayer(void);
void ReadFilePlayer(void);
void DeletePlayer(void);
void SetPosPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitPlayer(int nDamage);
PLAYERENVIRONMENT GetEnvironment(void);
void SoundPlayer(void);
void DebugPlayerMotion(void);

#endif // !_PLAYER_H_