#include <stdio.h>
/* Test 1 */
/* David9km */

void quicksort(int nums[], int left, int right)
{
	int i = left;
	int j = right;
	int pivot = nums[(left+right) / 2];
	while(i <= j){
		while(nums[i] < pivot) {
			i++;
		}
		while(nums[j] > pivot) {
			j--;
		}
		if( i <= j) {
			int temp = nums[i];
			nums[i] = nums[j];
			nums[j] = temp;
			i++;
			j--;
		}
	}
	if (left < j) {
		quicksort(nums, left, j);
	}
	if(i < right) {
		quicksort(nums, i, right);
	}

}

int main (int argc, const char* argv[])
{

	int a[] = {7,2,9,18,54,79,101,8,9,21,34,66,1,0,102,19};
	int left = 0;
	int right = 15;
	quicksort(a, left, right);
	

};
