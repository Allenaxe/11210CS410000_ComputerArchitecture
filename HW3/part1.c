#include <stdio.h>
#include <math.h>

int compare(int p, int q){
	if(p > q) return p + q;
	else return p;
}

int smod(int p , int q){
	int div,  divd;
	if(p > q) div = 2 + pow(2, p%4);
	else div = 4 + pow(2, q%4);
	div = div * 5;
	divd = p * 4 + q;
	return divd % div;
}

int main(){
   int x, y, z, ans;
   printf("input x: ");
   scanf("%d", &x);
   printf("input y: ");
   scanf("%d", &y);
   printf("input z: ");
   scanf("%d", &z);
   ans = smod(compare(x, y), z);
   printf("result = %d\n", ans);
   return 0;
}
