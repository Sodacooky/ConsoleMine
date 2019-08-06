#include "pch.hpp"
#include "game.hpp"

CGame::CGame()
{
	for (int i = 0; i != 128; ++i)
	{
		m_vbMap.push_back({ 0,0,0 });
	}
	m_nDiscoveredBlockCount = 0;
	m_tBegin = 0;
	m_tEnd = 0;
}

int CGame::Start(void)
{
	while (true)
	{
		if (__StartMenu() == 1)
		{
			bool failed = true;
			if (__StartMain() == 1)
			{
				failed = false;
			}
			__Balance(failed);
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void CGame::__FlushMap(void)
{
	for (auto& block : m_vbMap)
	{
		block.nData = 0;
		block.nType = 1;
		block.bCovered = true;
	}
	m_nDiscoveredBlockCount = 0;
}

void CGame::__ExposeMap(void)
{
	for (auto& block : m_vbMap)
	{
		block.bCovered = false;
	}
}

void CGame::__CreateMines(void)
{
	srand((unsigned int)time(nullptr));

	int beginIndex = __TransPosToIndex(0, 0);
	int endIndex = __TransPosToIndex(WIDTH - 1, HEIGHT - 1);

	std::vector<int> vIndex;

	for (int i = 0; i != 32; ++i)
	{
		int tmp = rand() % 128;
		if (std::find(vIndex.begin(), vIndex.end(), tmp) == vIndex.end())
		{
			m_vbMap[tmp].nType = 2;
		}
		else
		{
			--i;
		}
	}
}

void CGame::__CreateNumbers(void)
{
	for (int index = 0; index != 128; ++index)
	{
		if (m_vbMap[index].nType == 1)
		{
			MyPoint ptThis;
			__TransIndexToPos(index, &ptThis);
			m_vbMap[index].nData = __ScanMines(ptThis);
		}
	}
}

void CGame::__PrintMap(void)
{
	for (int y = 0; y != HEIGHT; ++y)
	{
		for (int x = 0; x != WIDTH; ++x)
		{
			int index = __TransPosToIndex(x, y);
			if (m_vbMap[index].bCovered)
			{
				//未挖开
				std::cout << "? ";
			}
			else
			{
				//挖开的
				if (m_vbMap[index].nType == 1)
				{
					std::cout << m_vbMap[index].nData << " ";
				}
				else
				{
					std::cout << "M ";
				}
			}
		}
		std::cout << std::endl;
	}
}

int CGame::__ScanMines(MyPoint& ptThis)
{
	int count = 0;

	/*
	struct SITUATION
	{
		bool bTop = false;
		bool bBottom = false;
		bool bLeft = false;
		bool bRight = false;
	}situation;

	if (ptThis.x == 0)
	{
		situation.bLeft = true;
	}
	else if (ptThis.x >= WIDTH)
	{
		situation.bRight = true;
	}

	if (ptThis.y == 0)
	{
		situation.bTop = true;
	}
	else if (ptThis.x >= HEIGHT)
	{
		situation.bBottom = true;
	}
	*/

	if (__IsMine(__TransPosToIndex(ptThis.x - 1, ptThis.y - 1)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x, ptThis.y - 1)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x + 1, ptThis.y - 1)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x - 1, ptThis.y)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x + 1, ptThis.y)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x - 1, ptThis.y + 1)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x, ptThis.y + 1)))
	{
		++count;
	}
	if (__IsMine(__TransPosToIndex(ptThis.x + 1, ptThis.y + 1)))
	{
		++count;
	}
	return count;
}

void CGame::__WaitForPos(int* x, int* y)
{
	bool bIsFinished = false;
	while (!bIsFinished)
	{
		std::cout << "请输入要挖的从零开始数的坐标(x空格y): ";
		std::cin >> *x >> *y;
		//cache
		std::cin.get();
		if (__IsInRange(*x, 0, WIDTH - 1) && __IsInRange(*y, 0, HEIGHT - 1))
		{
			bIsFinished = true;
			continue;//break
		}
		else
		{
			std::cout << "不正确，";
		}
	}
}

bool CGame::__IsSafeDig(int x, int y)
{
	m_vbMap[__TransPosToIndex(x, y)].bCovered = false;
	if (__IsMine(__TransPosToIndex(x, y)))
	{
		return false;
	}
	++m_nDiscoveredBlockCount;
	return true;
}

int CGame::__TransPosToIndex(int x, int y)
{
	int tmpIndex = x + 16 * y;
	if (__IsInRange(tmpIndex, 0, 127))
	{
		return tmpIndex;
	}
	else
	{
		return -1;
	}
}

void CGame::__TransIndexToPos(int index, MyPoint* pt)
{
	pt->x = index % 16;
	pt->y = (index - pt->x) / 16;
	if (!__IsInRange(pt->x, 0, WIDTH - 1) || !__IsInRange(pt->y, 0, HEIGHT - 1))
	{
		throw("CGame::__TransIndexToPos() 转换错误!");//crash
	}
}

bool CGame::__IsInRange(int x, int begin, int end)
{
	if (x <= end && x >= begin)
	{
		return true;
	}
	return false;
}

bool CGame::__IsMine(int index)
{
	if (index == -1)
	{
		return false;
	}
	if (m_vbMap[index].nType == 2)
	{
		return true;
	}
	return false;
}

int CGame::__StartMenu(void)
{
	system("title 欢迎来玩迫真扫雷");
	std::cout << "发光 迫真扫雷 发光" << std::endl
		<< "按下键盘数字操作:" << std::endl
		<< "1.开始\n2.退出" << std::endl;
	switch (_getch())
	{
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	default:
		break;
	}
	return -1;
}

int CGame::__StartMain(void)
{
	__FlushMap();
	__CreateMines();
	__CreateNumbers();

	while (true)
	{
		int x, y;
		system("cls");
		__PrintMap();
		__WaitForPos(&x, &y);
		if (!__IsSafeDig(x, y))
		{
			return 2;
			//break;
		}
		if (m_nDiscoveredBlockCount >= 96)
		{
			return 1;
			//break;
		}
	}
	return -1;
}

void CGame::__Balance(bool bFailed)
{
	int foo;
	if (bFailed)
	{
		system("cls");
		__ExposeMap();
		__PrintMap();
		std::cout << "你挖到了炸弹!" << std::endl;
		foo = _getch();
		return;
	}
	int deltaT = (int)(m_tEnd - m_tBegin);
	std::cout << "你赢了！用时" << deltaT << "秒！" << std::endl;
	foo = _getch();
}