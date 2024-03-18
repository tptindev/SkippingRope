
unsigned int hash(const char* str, unsigned int capacity)
{
    unsigned int result = -1;
	unsigned int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        result += str[i];
        result = (result + str[i]) % capacity;
    }

	return result;
}