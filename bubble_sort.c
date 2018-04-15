#include <stdio.h>
/* Test 1 */
/* David9km */

int main (int argc, const char* argv[])
{

	int i;
	int j;
	int temp;
	int a[] = {7,2,9,18,54,79,101,8,9,21,34,66,1,0,102,19};
	for(i = 0; i < 16; i++)
	{
	    for(j = 0; j < 16; j++)
	    {
		if(a[j+1] < a[j])
		{
			temp= a[j];
			a[j] = a[j +1];
			a[j+1] = temp;		
		}
	    }
	}
	

};
