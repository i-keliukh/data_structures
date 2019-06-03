#define MAX_SIZE	50000

int size = 0;
int array[MAX_SIZE];

void init() {
	size = 0;
}

void push_back(int value) {
	array[size] = value;
	size++;
}

int pop_back() {
	size--;
	return array[size];
}

int get_value(int index) {
	return array[index];
}
