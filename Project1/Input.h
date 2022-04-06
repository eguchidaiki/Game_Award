#pragma once

class Input final
{
private:
	static char keys[];
	static char oldkeys[];
	Input() {}
	Input(const Input&) = delete;
	~Input() {}
	Input operator=(const Input&) = delete;

public:
	static void Update();
	static bool isKey(int key);
	static bool isKeyTrigger(int key);
	static bool isKeyReturn(int key);
};