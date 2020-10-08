template<class T> class stack
{
public:
	struct node
	{
		node* next;
		T data;
	};

	stack(): head(nullptr) {}

	void push(node* n)
	{
		n->next = head;
		head = n;
	}

	node* pop()
	{
		node* ret = head;
		if (ret != nullptr)
			head = ret->next;
		return ret;
	}

private:
	node* head;
};
