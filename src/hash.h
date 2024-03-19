
unsigned int hash(const char* str, unsigned int capacity)
{
    unsigned int result = 0;
	unsigned int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        result += str[i];
    }

    result = result % capacity;

	return result;
}