
int sorted_array[11];
int size;

void init()
{
	for (int i = 0; i < 10; i++)
	{
		sorted_array[i] = 0;
	}
	size = 0;
}


void add_number(int value)
{
	if (size == 0)
	{
		sorted_array[0] = value;
		size++;
		return;
	}
	
	for (int i = size; i >= 0; i--)
	{
		if (i > 0 && sorted_array[i - 1] < value)
		{
			sorted_array[i] = sorted_array[i - 1];
		}
		else
		{
			sorted_array[i] = value;
			break;
		}
	}

	if (size < 10) size++;
}

void dump_array(int array[10])
{
	for (int i = 0; i < size; i++)
	{
		array[i] = sorted_array[i];
	}
}
