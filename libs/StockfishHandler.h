#pragma once
#include <string>

class StockFish {
private:
	int depth;
public:
	StockFish(int);
	~StockFish(void);

	std::string getNextMove(std::string);
};