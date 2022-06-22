#include <stdio.h>
#include <stdlib.h>

struct point{
    int x;
    int high;
};

struct heap_t {
	int* arr;
	int count;
	int capacity;
};

struct heap_t heap_init(int capacity) {
	struct heap_t heap;

	heap.arr = malloc((capacity + 1) * sizeof(int));
	heap.capacity = capacity + 1;
	heap.count = 0;

	return heap;
}

void delete_max(struct heap_t* heap) {
	heap->arr[1] = heap->arr[heap->count];
	heap->count--;

	int current_index = 1;

	while (1) {
		if (current_index >= heap->count) {
			break;
		}

		int left_index = current_index * 2;
		int right_index = current_index * 2 + 1;
		int current = heap->arr[current_index];
		int left = heap->arr[left_index];
		int right = heap->arr[right_index];

		if (left < right && current < right && right_index <= heap->count) {
			heap->arr[right_index] = current;
			heap->arr[current_index] = right;

			current_index = right_index;
		} else if (left > right && current < left && left_index <= heap->count) {
			heap->arr[left_index] = current;
			heap->arr[current_index] = left;

			current_index = left_index;
		} else {
			break;
		}
	}
}

void insert(struct heap_t* heap, int value) {
	int current_index = heap->count + 1;

	if(heap->count == heap->capacity){
        heap->capacity = heap->capacity * 2;
        heap->arr = realloc(heap, heap->capacity * sizeof(int));
	}

	heap->arr[current_index] = value;
	heap->count++;


	while (1) {
		int parent_index = current_index / 2;
		int parent = heap->arr[parent_index];
		if (parent >= value) {
			break;
		}
		// swap
		// printf("swapping parent and value\n");
		heap->arr[current_index] = parent;
		heap->arr[current_index / 2] = value;

		current_index /= 2;
	}
}

int index_in_heap(struct heap_t* heap, int value){
    for(int i=1; i<=heap->count;i++){
        if(heap->arr[i] == value){
            return i;
        }
    }
}

void change_val(struct heap_t* heap, int value, int new_val){
    int i = index_in_heap(heap, value);
    heap->arr[i] = new_val;
    int parent_index = i / 2;
    int parent = heap->arr[parent_index];
    while (i != 1 && parent < heap->arr[i]){
       heap->arr[i] = parent;
       heap->arr[parent_index] = new_val;
       i = i / 2;
    }
}

void delete_value(struct heap_t* heap, int value){
    if(heap->arr[1] == value){
        delete_max(heap);
        return;
    }

    change_val(heap, value, heap->arr[1]+1);
    delete_max(heap);
}

void print_heap(struct heap_t* heap){
    for(int i=1;i<=heap->count;i++){
        printf("%d ", heap->arr[i]);
    }
}

void heapsort(int* arr, int size) {
	struct heap_t heap = heap_init(size);
	for (int i = 0; i < size; i++) {
		insert(&heap, arr[i]);
	}

	for (int i = 1; i <= size; i++) {
		printf("%d ", heap.arr[i]);
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		int max = heap.arr[1];
		arr[i] = max;
		delete_max(&heap);

		for (int i = 1; i <= heap.count; i++) {
			printf("%d ", heap.arr[i]);
		}
		printf("\n");
	}
}

struct point* init_point(int x, int high){
    struct point* new_point = malloc(sizeof(struct point));
    new_point->x = x;
    new_point->high = high;

    return new_point;
};

void sort_points(struct point** points, int count){
    for(int i=0;i<count-1;i++){
        for(int g=0;g<count-i-1;g++){
            if(points[g]->x > points[g+1]->x || (points[g]->x == points[g+1]->x && points[g]->high > points[g+1]->high)){
                struct point temp = *points[g];
                *points[g] = *points[g+1];
                *points[g+1] = temp;
            }
        }
    }
}

void print_points(struct point** points, int count){
    for(int i=0;i<count;i++){
        printf("[%d, %d]\n", points[i]->x, points[i]->high);
    }
}

int** getSkyline(int** buildings, int buildingsSize, int* buildingsColSize, int* returnSize, int** returnColumnSizes){
    if(buildingsSize == 0){
        return returnColumnSizes;
    }else if(buildingsSize == 1){
        int* ret = malloc(2 * sizeof(int));
        ret[0] = buildings[0][0];
        ret[1] = buildings[0][2];
        int* ret2 = malloc(2 * sizeof(int));
        ret2[0] = buildings[0][1];
        ret2[1] = 0;

        returnColumnSizes[(*returnSize)++] = ret;
        returnColumnSizes[(*returnSize)++] = ret2;

        return returnColumnSizes;
    }else{
        int g=0;
        struct point** points = malloc(2 * buildingsSize * sizeof(struct point));
        int points_count = 2 * buildingsSize;

        for(int i=0;i < buildingsSize;i++){
            points[g] = init_point(buildings[i][0], buildings[i][2] * -1);
            points[g+1] = init_point(buildings[i][1], buildings[i][2]);
            g = g + 2;
        }

        sort_points(points, points_count);

        //print_points(points, points_count);

        int prev_max_high = 0;

        struct heap_t max_heap = heap_init(10);
        insert(&max_heap, 0);

        for(int i=0;i<points_count;i++){
            if(points[i]->high > 0){
                delete_value(&max_heap, points[i]->high);
            }else{
                insert(&max_heap, points[i]->high * -1);
            }

            if(prev_max_high != max_heap.arr[1]){
                //printf("count %d\n", max_heap.arr[1]);
                int* ret = malloc(2 * sizeof(int));
                ret[0] = points[i]->x;
                ret[1] = max_heap.arr[1];

                returnColumnSizes[(*returnSize)] = ret;

                prev_max_high = max_heap.arr[1];
                (*returnSize)++;
            }
        }
    }

    return returnColumnSizes;
}

int main() {
	/*int buildingsSize = 2;

	int* building1 = malloc(sizeof(int) * 3);
	building1[0] = 0;
	building1[1] = 2;
	building1[2] = 3;
	int* building2 = malloc(sizeof(int) * 3);
	building2[0] = 2;
	building2[1] = 5;
	building2[2] = 3;

	int** buildings = malloc(sizeof(int*) * buildingsSize);
	buildings[0] = building1;
	buildings[1] = building2;*/

	// [2,9,10],[3,7,15],[5,12,12],[15,20,10],[19,24,8]]

	int buildingsSize = 5;

    int* building1 = malloc(sizeof(int) * 3);
	building1[0] = 2;
	building1[1] = 9;
	building1[2] = 10;
	int* building2 = malloc(sizeof(int) * 3);
	building2[0] = 3;
	building2[1] = 7;
	building2[2] = 15;
	int* building3 = malloc(sizeof(int) * 3);
	building3[0] = 5;
	building3[1] = 12;
	building3[2] = 12;
	int* building4 = malloc(sizeof(int) * 3);
	building4[0] = 15;
	building4[1] = 20;
	building4[2] = 10;
	int* building5 = malloc(sizeof(int) * 3);
	building5[0] = 19;
	building5[1] = 24;
	building5[2] = 8;

	int** buildings = malloc(sizeof(int*) * buildingsSize);
	buildings[0] = building1;
	buildings[1] = building2;
	buildings[2] = building3;
	buildings[3] = building4;
	buildings[4] = building5;

	int returnSize = 0;
	int** res = malloc(30 * sizeof(int*));
	res = getSkyline(buildings, buildingsSize, NULL, &returnSize, res);
	for(int i=0;i < returnSize;i++){
        printf("[%d, %d]\n", res[i][0], res[i][1]);
	}
}
