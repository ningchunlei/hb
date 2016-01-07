#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <iterator>
#include <stack>
#include <cmath>
#include "chanlun.h"

using namespace std;

enum LineType {
	HIGH = 1, LOW = 2, NOTFIND = 3
};

enum QueKou {
	LEFT, RIGHT, QUEKOUNOTFIND
};

typedef struct _kLine{
	float high = -1;
	float low = -1;
	int index;
	int left = -1;
	int right = -1;
	bool avaliable = false;
	float top = -1;
	float buttom = -1;
	QueKou leftquekou = QUEKOUNOTFIND;
	QueKou rightquekou = QUEKOUNOTFIND;
	LineType type;
}kLine;

void leftHighMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){

	if (index == 0){
		return;
	}
	bool higherThanBefore = pKLine.high >= high[index - 1] ? true : false;
	bool smallThanBefore = pKLine.low <= low[index - 1] ? true : false;

	if (higherThanBefore && smallThanBefore){
		pKLine.low = low[index - 1];
		leftHighMerge(dataLen, high, low, index - 1, pKLine);
	}
	else if (!higherThanBefore){
		pKLine.avaliable = false;
	}
	else if (!smallThanBefore){
		pKLine.left = index - 1;
		if (pKLine.buttom == -1){
			pKLine.buttom = low[index - 1];
		}
		else if (pKLine.buttom > low[index - 1]){
			pKLine.buttom = low[index - 1];
		}
		pKLine.avaliable = true;
	}
}

void rightHighMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){
	if (index == dataLen - 1){
		return;
	}

	bool higherThanAfter = pKLine.high >= high[index + 1] ? true : false;
	bool smallThanAfter = pKLine.low <= low[index + 1] ? true : false;

	if (higherThanAfter && smallThanAfter){
		pKLine.low = low[index + 1];
		rightHighMerge(dataLen, high, low, index + 1, pKLine);
	}
	else if (!higherThanAfter){
		pKLine.avaliable = false;
	}
	else if (!smallThanAfter){
		pKLine.right = index + 1;
		if (pKLine.buttom == -1){
			pKLine.buttom = low[index + 1];
		}
		else if (pKLine.buttom > low[index + 1]){
			pKLine.buttom = low[index + 1];
		}
		pKLine.avaliable = true;
	}
}


void leftLowMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){

	if (index == 0){
		return;
	}
	bool higherThanBefore = pKLine.high >= high[index - 1] ? true : false;
	bool smallThanBefore = pKLine.low <= low[index - 1] ? true : false;

	if (higherThanBefore && smallThanBefore){
		pKLine.high = high[index - 1];
		leftLowMerge(dataLen, high, low, index - 1, pKLine);
	}
	else if (!smallThanBefore){
		pKLine.avaliable = false;
	}
	else if (!higherThanBefore){
		pKLine.left = index - 1;
		if (pKLine.top == -1){
			pKLine.top = high[index - 1];
		}
		else if (pKLine.top < high[index - 1]){
			pKLine.top = high[index - 1];
		}
		pKLine.avaliable = true;
	}
}

void rightLowMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){
	if (index == dataLen - 1){
		return;
	}

	bool higherThanAfter = pKLine.high >= high[index + 1] ? true : false;
	bool smallThanAfter = pKLine.low <= low[index + 1] ? true : false;

	if (higherThanAfter && smallThanAfter){
		pKLine.high = high[index + 1];
		rightLowMerge(dataLen, high, low, index + 1, pKLine);
	}
	else if (!smallThanAfter){
		pKLine.avaliable = false;
	}
	else if (!higherThanAfter){
		pKLine.right = index + 1;
		if (pKLine.top == -1){
			pKLine.top = high[index + 1];
		}
		else if (pKLine.top < high[index + 1]){
			pKLine.top = high[index + 1];
		}
		pKLine.avaliable = true;
	}
}

void mergeParameters(int dataLen, float* pOut, float* p1, float* p2, float* p3){
	pOut[0] = p1[0];
	pOut[1] = p2[0];
	pOut[2] = p3[0];
}

void fenxing(int dataLen, float* pOut, float* high, float* low, float* bhan)
{
	float fenxing_baohan = bhan[0];
	float fenxing_quekou = bhan[1];
	float fenxing_tiaokongflag = bhan[2];

	fenxing_quekou = 1 + fenxing_quekou;

	vector<kLine> gvec;
	WCHAR wchar_pintf[1024];
	for (int i = 1; i < dataLen - 1; i++){
		bool higherThanBefore = high[i] >= high[i - 1] ? true : false;
		bool higherThanAfter = high[i] >= high[i + 1] ? true : false;

		bool smallThanBefore = low[i] <= low[i - 1] ? true : false;
		bool smallThanAfter = low[i] <= low[i + 1] ? true : false;

		if (higherThanBefore && higherThanAfter) {
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = HIGH;
			kline.top = high[i];
			float klow = -1;

			if (low[i] > high[i - 1] * fenxing_quekou) {
				kline.leftquekou = LEFT;
			}
			if (low[i] > high[i + 1] * fenxing_quekou){
				kline.rightquekou = RIGHT;
			}

			if (kline.leftquekou == LEFT){
				kline.avaliable = true;
				kline.left = i;
				kline.buttom = low[i];
			}
			else{
				leftHighMerge(dataLen, high, low, i, kline);
				klow = kline.low;
				kline.low = low[i];
			}

			if (kline.avaliable){
				if (kline.rightquekou == RIGHT){
					kline.avaliable = true;
					kline.right = i;
				}
				else{
					rightHighMerge(dataLen, high, low, i, kline);
				}

				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
			kline.low = kline.low > klow ? kline.low : klow;
		}
		if (smallThanBefore && smallThanAfter){
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = LOW;
			kline.buttom = low[i];
			float shigh = high[i];

			if (high[i] * fenxing_quekou < low[i - 1]) {
				kline.leftquekou = LEFT;
			}
			if (high[i] * fenxing_quekou < low[i + 1]){
				kline.rightquekou = RIGHT;
			}

			if (kline.leftquekou == LEFT){
				kline.avaliable = true;
				kline.left = i;
				kline.top = high[i];
			}
			else{
				leftLowMerge(dataLen, high, low, i, kline);
				shigh = kline.high;
				kline.high = high[i];
			}

			if (kline.avaliable){
				if (kline.rightquekou == RIGHT){
					kline.avaliable = true;
					kline.right = i;
				}
				else{
					rightLowMerge(dataLen, high, low, i, kline);
				}

				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
			kline.high = kline.high > shigh ? shigh : kline.high;
		}
	}

	memset(pOut, 0, dataLen);

	if (gvec.empty()){
		return;
	}
	vector<kLine>::iterator it = gvec.begin();
	LineType lineType = (*it).type;
	stack<kLine> kstack;
	kstack.push(*it);
	std::advance(it, 1);

	for (; it != gvec.end(); it++){
		kLine kline = kstack.top();
		kLine cur = *it;

		if (kline.type == HIGH){
			if (cur.type == HIGH){
				if (cur.high >= kline.high){
					kstack.pop();
					if (!kstack.empty()){
						kline = kstack.top();

						if (kline.buttom >= cur.buttom){
							continue;
						}
					}
					
					kstack.push(cur);
				}
				continue;
			}

			if (kline.top <= cur.top){
				continue;
			}

			if (kline.rightquekou == RIGHT || cur.leftquekou == LEFT){
				kstack.push(cur);
				lineType = cur.type;
				continue;
			}

			if (kline.right > cur.left || cur.index - kline.index < 3 || (cur.index - kline.index == 3 && low[kline.right] <= high[cur.left])){
				continue;
			}

			if (cur.index - kline.index == 3 && low[kline.right] > high[cur.left] && fenxing_tiaokongflag == 0){
				continue;
			}

			float stop = kline.top > cur.top ? cur.top : kline.top;
			float sbuttom = kline.buttom > cur.buttom ? kline.buttom : cur.buttom;

			float diff = stop - sbuttom;
			float ldiff = abs(diff) / (kline.top - kline.buttom);
			float rdiff = abs(diff) / (cur.top - cur.buttom);

			if ((diff <= 0 || (ldiff <= fenxing_baohan && rdiff <= fenxing_baohan)) && kline.high > cur.high && kline.low > cur.low){
				kstack.push(cur);
				lineType = cur.type;
			}
		}
		else{
			if (cur.type == LOW){
				if (cur.low <= kline.low){
					kstack.pop();
					if (!kstack.empty()){
						kline = kstack.top();

						if (kline.top <= cur.top){
							continue;
						}
					}
					
					kstack.push(cur);
				}
				continue;
			}

			if (kline.buttom >= cur.buttom){
				continue;
			}

			if (kline.rightquekou == RIGHT || cur.leftquekou == LEFT){
				kstack.push(cur);
				lineType = cur.type;
				continue;
			}

			if (kline.right > cur.left || cur.index - kline.index<3 || (cur.index - kline.index == 3 && high[kline.right] >= low[cur.left])){
				continue;
			}

			if (cur.index - kline.index == 3 && high[kline.right] < low[cur.left] && fenxing_tiaokongflag == 0){
				continue;
			}

			float stop = kline.top > cur.top ? cur.top : kline.top;
			float sbuttom = kline.buttom > cur.buttom ? kline.buttom : cur.buttom;

			float diff = stop - sbuttom;
			float ldiff = abs(diff) / (kline.top - kline.buttom);
			float rdiff = abs(diff) / (cur.top - cur.buttom);

			if ((diff <= 0 || (ldiff <= fenxing_baohan && rdiff <= fenxing_baohan)) && cur.high > kline.high && cur.low > kline.low){
				kstack.push(cur);
				lineType = cur.type;
			}

		}

	}

	while (!kstack.empty()){
		pOut[kstack.top().index] = kstack.top().type;
		kstack.pop();
	}

}

void fenxing_test(int dataLen, float* pOut, float* high, float* low, float* bhan)
{
	float fenxing_baohan = bhan[0];
	float fenxing_quekou = bhan[1];
	fenxing_quekou = 1 + fenxing_quekou;
	vector<kLine> gvec;
	WCHAR wchar_pintf[1024];

	swprintf(wchar_pintf, 1024, L"Length: %d", dataLen);
	OutputDebugString(wchar_pintf);

	for (int i = 1; i < dataLen - 2; i++){
		bool higherThanBefore = high[i] >= high[i - 1] ? true : false;
		bool higherThanAfter = high[i] >= high[i + 1] ? true : false;

		bool smallThanBefore = low[i] <= low[i - 1] ? true : false;
		bool smallThanAfter = low[i] <= low[i + 1] ? true : false;

		if (higherThanBefore && higherThanAfter) {
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = HIGH;
			kline.top = high[i];
			float klow = -1;

			if (low[i] > high[i - 1] * fenxing_quekou) {
				kline.leftquekou = LEFT;
			}
			if (low[i] > high[i + 1] * fenxing_quekou){
				kline.rightquekou = RIGHT;
			}

			if (kline.leftquekou == LEFT){
				kline.avaliable = true;
				kline.left = i;
				kline.buttom = low[i];
			}
			else{
				leftHighMerge(dataLen, high, low, i, kline);
				klow = kline.low;
				kline.low = low[i];
			}

			if (kline.avaliable){
				if (kline.rightquekou == RIGHT){
					kline.avaliable = true;
					kline.right = i;
				}
				else{
					rightHighMerge(dataLen, high, low, i, kline);
				}

				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
			kline.low = kline.low > klow ? kline.low : klow;
		}
		if (smallThanBefore && smallThanAfter){
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = LOW;
			kline.buttom = low[i];
			float shigh = high[i];

			if (high[i] * fenxing_quekou < low[i - 1]) {
				kline.leftquekou = LEFT;
			}
			if (high[i] * fenxing_quekou < low[i + 1]){
				kline.rightquekou = RIGHT;
			}

			if (kline.leftquekou == LEFT){
				kline.avaliable = true;
				kline.left = i;
				kline.top = high[i];
			}
			else{
				leftLowMerge(dataLen, high, low, i, kline);
				shigh = kline.high;
				kline.high = high[i];
			}

			if (kline.avaliable){
				if (kline.rightquekou == RIGHT){
					kline.avaliable = true;
					kline.right = i;
				}
				else{
					rightLowMerge(dataLen, high, low, i, kline);
				}

				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
			kline.high = kline.high > shigh ? shigh : kline.high;
		}
	}

	memset(pOut, 0, dataLen);

	if (gvec.empty()){
		return;
	}

	for (vector<kLine>::iterator it = gvec.begin(); it != gvec.end(); it++){
		pOut[(*it).index] = (*it).type;
	}

}


int searchRightHightFX(int start, vector<kLine> gvec, kLine& kline){
	if (start + 1 >= gvec.size()){
		return -1;
	}
	float tdiff = kline.high - gvec[start].high;
	float bdiff = kline.low - gvec[start + 1].high;

	if (tdiff>0 && bdiff>0){
		return start;
	}
	else if (tdiff >= 0 && bdiff <= 0){
		kline.low = gvec[start + 1].high;
		return searchRightHightFX(start + 2, gvec, kline);
	}
	else{
		return -1;
	}
}

int searchAllLowFX(int start, vector<kLine> gvec, int find, kLine& kline){
	if (start + 1 >= gvec.size()){
		return -1;
	}

	float tdiff = kline.high - gvec[start + 1].high;
	float bdiff = kline.low - gvec[start].high;

	if (tdiff == 0 && bdiff == 0){
		return searchAllLowFX(start + 2, gvec, find, kline);
	}
	else if (tdiff <= 0 && bdiff >= 0){
		kline.low = gvec[start].high;
		return searchAllLowFX(start + 2, gvec, find, kline);
	}
	else if (tdiff >= 0 && bdiff <= 0){
		kline.high = gvec[start + 1].high;
		return searchAllLowFX(start + 2, gvec, find, kline);
	}
	else if (tdiff>0 && bdiff>0){
		if (find == 1){
			kline.high = gvec[start + 1].high;
			kline.low = gvec[start].high;
			return searchAllLowFX(start + 2, gvec, find + 1, kline);
		}
		else if (find == 2){
			kline.high = gvec[start + 1].high;
			kline.low = gvec[start].high;
			return searchAllLowFX(start + 2, gvec, find, kline);
		}
	}
	else if (tdiff<0 && bdiff<0){
		if (find == 2){
			return start;
		}
		else if (find == 1){
			return -1;
		}
	}
	return -1;
}




int searchRightLowFX(int start, vector<kLine> gvec, kLine& kline){
	if (start + 1 >= gvec.size()){
		return -1;
	}
	float tdiff = kline.high - gvec[start + 1].high;
	float bdiff = kline.low - gvec[start].high;

	if (tdiff<0 && bdiff<0){
		return start;
	}
	else if (tdiff >= 0 && bdiff <= 0){
		kline.high = gvec[start + 1].high;
		return searchRightLowFX(start + 2, gvec, kline);
	}
	else{
		return -1;
	}
}

int searchAllHighFX(int start, vector<kLine> gvec, int find, kLine& kline){
	if (start + 1 >= gvec.size()){
		return -1;
	}

	float tdiff = kline.high - gvec[start].high;
	float bdiff = kline.low - gvec[start + 1].high;

	if (tdiff == 0 && bdiff == 0){
		return searchAllHighFX(start + 2, gvec, find, kline);
	}
	else if (tdiff <= 0 && bdiff >= 0){
		kline.high = gvec[start].high;
		return searchAllHighFX(start + 2, gvec, find, kline);
	}
	else if (tdiff >= 0 && bdiff <= 0){
		kline.low = gvec[start + 1].high;
		return searchAllHighFX(start + 2, gvec, find, kline);
	}
	else if (tdiff<0 && bdiff<0){
		if (find == 1){
			kline.high = gvec[start].high;
			kline.low = gvec[start + 1].high;
			return searchAllHighFX(start + 2, gvec, find + 1, kline);
		}
		else if (find == 2){
			kline.high = gvec[start].high;
			kline.low = gvec[start + 1].high;
			return searchAllHighFX(start + 2, gvec, find, kline);
		}
	}
	else if (tdiff>0 && bdiff>0){
		if (find == 2){
			return start;
		}
		else if (find == 1){
			return -1;
		}
	}
	return -1;
}


int searchLeftHighMerge(int start, vector<kLine> gvec, kLine& kline){
	if (start<0 || start - 1<0){
		return -1;
	}
	float tdiff = kline.high - gvec[start - 1].high;
	float bdiff = kline.low - gvec[start].high;

	if (tdiff <= 0 && bdiff >= 0){
		kline.high = gvec[start - 1].high;
		return searchLeftHighMerge(start - 2, gvec, kline);
	}
	else if (tdiff >= 0 && bdiff <= 0){
		kline.low = gvec[start].high;
		return searchLeftHighMerge(start - 2, gvec, kline);
	}
	else{
		return -1;
	}
}

int searchLeftLowMerge(int start, vector<kLine> gvec, kLine& kline){
	if (start - 1<0 || start<0){
		return -1;
	}

	float tdiff = kline.high - gvec[start].high;
	float bdiff = kline.low - gvec[start - 1].high;

	if (tdiff >= 0 && bdiff <= 0){
		kline.high = gvec[start].high;
		return searchLeftHighMerge(start - 2, gvec, kline);
	}
	else if (tdiff <= 0 && bdiff >= 0){
		kline.low = gvec[start - 1].high;
		return searchLeftHighMerge(start - 2, gvec, kline);
	}
	else{
		return -1;
	}
}



void xianduan(int dataLen, float* pOut, float* high, float* low, float* fb){
	vector<kLine> gvec;
	WCHAR wchar_pintf[1024];
	for (int i = 0; i < dataLen; i++){
		if (fb[i] == HIGH){
			kLine kline;
			kline.high = high[i];
			kline.index = i;
			kline.type = HIGH;
			gvec.push_back(kline);
			swprintf(wchar_pintf, 1024, L"value: %f type: %d", kline.high, kline.type);
			OutputDebugString(wchar_pintf);
		}
		else if (fb[i] == LOW){
			kLine kline;
			kline.high = low[i];
			kline.index = i;
			kline.type = LOW;
			gvec.push_back(kline);
			swprintf(wchar_pintf, 1024, L"value: %f type: %d", kline.high, kline.type);
			OutputDebugString(wchar_pintf);
		}
	}


	LineType lineType = NOTFIND;
	stack<kLine> pstack;
	int j = 0;
	int kk = 0;
	for (; kk < gvec.size(); kk++){
		kLine kline = gvec[kk];
		if (lineType == NOTFIND && kline.type == LOW){
			lineType = LOW;
			j = 1;
			continue;
		}
		else if (lineType == NOTFIND && kline.type == HIGH){
			lineType = HIGH;
			j = 1;
			continue;
		}
		j++;

		if (lineType == LOW && j == 4){
			if (!(gvec[kk - 2].high < gvec[kk].high && gvec[kk - 3].high < gvec[kk - 1].high)){
				lineType = NOTFIND;
				j = 0;
				kk = kk - 2;
				continue;
			}
			else{
				pstack.push(gvec[kk - 3]);
				j = kk - 3;
				break;
			}
		}
		if (lineType == HIGH && j == 4){
			if (!(gvec[kk - 2].high > gvec[kk].high && gvec[kk - 3].high > gvec[kk - 1].high)){
				lineType = NOTFIND;
				j = 0;
				kk = kk - 2;
				continue;
			}
			else{
				pstack.push(gvec[kk - 3]);
				j = kk - 3;
				break;
			}
		}

	}
	while (kk<(int)(gvec.size() - 3)){
		//着向上笔
		if (lineType == LOW){

			kLine tmp;
			tmp.high = gvec[kk].high;
			tmp.low = gvec[kk + 1].high;
			int start = searchRightHightFX(kk + 2, gvec, tmp);
			if (start == -1 || gvec[kk - 2].high>gvec[kk].high){
				kk = kk + 2;
			}
			else{
				//有缺口
				if (gvec[kk + 1].high > gvec[kk - 2].high){
					tmp.high = gvec[kk - 2].high;
					tmp.low = gvec[kk - 1].high;
					searchLeftHighMerge(kk - 3, gvec, tmp);
					if (gvec[kk + 1].high > tmp.high){
						tmp.high = gvec[kk + 2].high;
						tmp.low = gvec[kk + 1].high;
						start = searchAllLowFX(kk + 3, gvec, 1, tmp);
						if (start == -1){
							kk = kk + 2;
						}
						else{
							pstack.push(gvec[kk]);
							j = kk;
							kk = kk + 3;
							lineType = HIGH;
						}
					}
					else{
						pstack.push(gvec[kk]);
						j = kk;
						kk = kk + 3;
						lineType = HIGH;
					}
				}
				else{
					pstack.push(gvec[kk]);
					j = kk;
					kk = kk + 3;
					lineType = HIGH;
				}
			}
			/*
			if (gvec[kk + 1].high > gvec[kk - 2].high){
			kLine tmp;
			tmp.high = gvec[kk + 2].high;
			tmp.low = gvec[kk + 1].high;
			int start = searchAllLowFX(kk + 3, gvec, 1, tmp);
			if (start == -1){
			kk = kk + 2;
			}
			else{
			pstack.push(gvec[kk]);
			j = kk;
			kk = kk + 3;
			lineType = HIGH;
			}
			}
			else{
			kLine tmp;
			tmp.high = gvec[kk].high;
			tmp.low = gvec[kk + 1].high;
			int start = searchRightHightFX(kk + 2, gvec, tmp);
			if (start == -1){
			kk = kk + 2;
			}
			else{
			pstack.push(gvec[kk]);
			j = kk;
			kk = kk + 3;
			lineType = HIGH;
			}
			}*/
		}
		//找向下笔
		else if (lineType == HIGH){
			kLine tmp;
			tmp.high = gvec[kk + 1].high;
			tmp.low = gvec[kk].high;
			int start = searchRightLowFX(kk + 2, gvec, tmp);
			if (start == -1 || gvec[kk - 2].high<gvec[kk].high){
				kk = kk + 2;
			}
			else{
				if (gvec[kk + 1].high < gvec[kk - 2].high){
					tmp.high = gvec[kk - 1].high;
					tmp.low = gvec[kk - 2].high;
					searchLeftLowMerge(kk - 3, gvec, tmp);
					if (gvec[kk + 1].high < tmp.low){
						tmp.high = gvec[kk + 1].high;
						tmp.low = gvec[kk + 2].high;
						int start = searchAllHighFX(kk + 3, gvec, 1, tmp);
						if (start == -1){
							kk = kk + 2;
						}
						else{
							pstack.push(gvec[kk]);
							j = kk;
							kk = kk + 3;
							lineType = LOW;
						}
					}
					else{
						pstack.push(gvec[kk]);
						j = kk;
						kk = kk + 3;
						lineType = LOW;
					}
				}
				else{
					pstack.push(gvec[kk]);
					j = kk;
					kk = kk + 3;
					lineType = LOW;
				}
			}

			/*
			// 有缺口
			if (gvec[kk + 1].high < gvec[kk - 2].high){
			kLine tmp;
			tmp.high = gvec[kk + 1].high;
			tmp.low = gvec[kk + 2].high;
			int start = searchAllHighFX(kk + 3, gvec, 1, tmp);
			if (start == -1){
			kk = kk + 2;
			}
			else{
			pstack.push(gvec[kk]);
			j = kk;
			kk = kk + 3;
			lineType = LOW;
			}
			}
			else{
			kLine tmp;
			tmp.high = gvec[kk + 1].high;
			tmp.low = gvec[kk].high;
			int start = searchRightLowFX(kk + 2, gvec, tmp);
			if (start == -1){
			kk = kk + 2;
			}
			else{
			pstack.push(gvec[kk]);
			j = kk;
			kk = kk + 3;
			lineType = LOW;
			}
			}
			*/
		}
	}

	memset(pOut, 0, dataLen);
	while (!pstack.empty()){
		pOut[pstack.top().index] = pstack.top().type;
		pstack.pop();
	}

}




//加载的函数
PluginTCalcFuncInfo g_CalcFuncSets[] =
{
	{ 1, (pPluginFUNC)&fenxing },
	{ 2, (pPluginFUNC)&fenxing_test },
	{ 3, (pPluginFUNC)&xianduan },
	{ 4, (pPluginFUNC)&mergeParameters },
	{ 0, NULL },
};

//导出给TCalc的注册函数
bool RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if (*pFun == NULL)
	{
		(*pFun) = g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}
