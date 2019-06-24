
struct Element
{
    int value;
    unsigned short props;
};

Element sorted_array[150000];
int size;

void init()
{
    for (int i = 0; i < 10; i++)
    {
        sorted_array[i].value = 0;
    }
    size = 0;
}

void add_number(int value, unsigned short props)
{
    if (size == 0)
    {
        sorted_array[0].value = value;
        sorted_array[0].props = props;
        size++;
        return;
    }

    for (int i = size; i >= 0; i--)
    {
        if (i > 0 && sorted_array[i - 1].value < value)
        {
            sorted_array[i] = sorted_array[i - 1];
        }
        else
        {
            sorted_array[i].value = value;
            sorted_array[i].props = props;
            break;
        }
    }
    size++;
}

void dump_array(int array[10], unsigned short mandatory_props)
{
    for (int i = 0, j = 0; i < size && j < 10; i++)
    {
        if ((sorted_array[i].props & mandatory_props) != mandatory_props) continue;
        array[j] = sorted_array[i].value;
        j++;
    }
}
