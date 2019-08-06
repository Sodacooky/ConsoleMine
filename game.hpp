#pragma once

#define WIDTH 16
#define HEIGHT 8

class CGame
{
public:
	CGame();
	int Start(void);
private:
	struct Block
	{
		//type: 1-number 2-mine
		int nType;
		int nData;//for number
		bool bCovered;
	};
	struct MyPoint
	{
		int x, y;
	};
private:
	//game start time
	time_t m_tBegin;
	//game end time
	time_t m_tEnd;
	//map
	std::vector<Block> m_vbMap;
	//discovered count
	int m_nDiscoveredBlockCount;
private:
	void __FlushMap(void);
	void __ExposeMap(void);
	void __CreateMines(void);
	void __CreateNumbers(void);
	void __PrintMap(void);
	int __ScanMines(MyPoint& ptThis);

	void __WaitForPos(int* x, int* y);
	//如果挖到了炸弹返回假
	bool __IsSafeDig(int x, int y);
	//不存在这个点返回-1
	int __TransPosToIndex(int x, int y);
	//不存在这个index直接crash
	void __TransIndexToPos(int index, MyPoint* pt);

	bool __IsInRange(int x, int begin, int end);
	bool __IsMine(int index);

	//1 start 2 quit
	int __StartMenu(void);
	//1 win 2 failed
	int __StartMain(void);
	void __Balance(bool bFailed);
};