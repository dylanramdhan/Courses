#ifndef __BALANCED_H__
#define __BALANCED_H__

#include <vector>
#include <iostream>

struct Assignment {
	int diff;
	std::vector<int> ga; // Group A
	std::vector<int> gb; // Group B
	Assignment(int d, std::vector<int> va, std::vector<int> vb):
		diff(d), ga(va), gb(vb) {}
	void printAssignment() {
		std::cout << "Group A: ";
		for (int e: ga) {
			std::cout << e << " ";
		}
		std::cout << std::endl;
		std::cout << "Group B: ";
		for (int e: gb) {
			std::cout << e << " ";
		}
		std::cout << std::endl;
	}
};

Assignment balancedGroups(std::vector<int> const &birthdays);

#endif /* __BALANCED_H__*/