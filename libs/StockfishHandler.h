#pragma once
#include <string>

class StockFish {
private:
	int depth;
public:
	StockFish(int);
	~StockFish(void);

	int mode = 2;

	std::string getNextMove(std::string);
};