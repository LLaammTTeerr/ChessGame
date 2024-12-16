#pragma once
#include <string>

class StockFish {
private:
	int depth;
public:
	StockFish(int);
	~StockFish(void);

	void setMode(int);

	int mode = 3;

	std::string getNextMove(std::string);
};