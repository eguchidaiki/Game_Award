#pragma once

class ActFlag final
{
public: //シングルトン化
	static ActFlag* Get();
private:
	ActFlag();
	ActFlag(const ActFlag*) = delete;
	~ActFlag();
	ActFlag operator=(const ActFlag*) = delete;

public: //メンバ関数
	// 決定
	bool Decision();
	// キャンセル
	bool Cancel();

	// 左に動く
	bool MoveLeft();
	// 右に動く
	bool MoveRight();
	// ジャンプ
	bool Jump();
	// 上に折れる
	bool FoldUp();
	// 下に折れる
	bool FoldDown();
	// 左に折れる
	bool FoldLeft();
	// 右に折れる
	bool FoldRight();
	// 上に開く
	bool OpenUp();
	// 下に開く
	bool OpenDown();
	// 左に開く
	bool OpenLeft();
	// 右に開く
	bool OpenRight();
};
