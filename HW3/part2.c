#include <stdio.h>

int cnt = 0;

void MoveTower(int disk, char source, char dest, char spare) {
    if(disk == 0) {
        // Move disk from source to dest
        printf("Move disk %d from %c to %c\n", disk, source, dest);
		++cnt;
	}
    else {
		// Move the smaller disk from source to spare
        MoveTower(disk - 1, source, spare, dest);

        // Move disk from source to dest
        printf("Move disk %d from %c to %c\n", disk, source, dest);
		++cnt;

		// Move the smaller disk from spare to dest
        MoveTower(disk - 1, spare, dest, source);
     }
}

int main() {
    int numDisks;
    printf("Please input the total number of disks: ");
    scanf("%d", &numDisks);
    MoveTower(numDisks - 1, 'A', 'B', 'C');

	printf("Total number of movement = %d\n", cnt);
    return 0;
} 
