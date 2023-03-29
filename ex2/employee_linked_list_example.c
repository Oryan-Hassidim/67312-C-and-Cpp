#include "employee_linked_list_example.h"

Node* new_node(Node* next, const Employee* e);
void free_node(Node* node);
int parse_employee(char* buffer, Employee* e);
char* allocate_and_cpy(const char* src)
{
	char* dest = malloc(strlen(src) + 1);
	if (!dest)
	{
		return NULL;
	}
	strcpy(dest, src);
	return dest;
}


LinkedList* new_linked_list(void)
{
	//  LinkedList *l = malloc (sizeof (LinkedList));
	//  if (l)
	//    {
	//      l->head = NULL;
	//    }
	//  return l;
	return calloc(1, sizeof(LinkedList));
}
int push(LinkedList* list, const Employee* e)
{
	if (!list || !e)
	{
		return INVALID_ARGUMENT;
	}
	Node* new_head = new_node(list->head, e);
	if (!new_head)
	{
		return ALLOCATION_ERROR;
	}
	list->head = new_head;
	return SUCCESS;
}
Node* new_node(Node* next, const Employee* e)
{
	Node* node = malloc(sizeof(Node));
	if (!node)
	{
		return NULL;
	}
	node->next = next;
	int error = 0;
	if (!(node->data.first_name = allocate_and_cpy(e->first_name)))
	{
		free(node);
		return NULL;
	}
	if (!(node->data.last_name = allocate_and_cpy(e->last_name)))
	{
		free(node->data.first_name);
		free(node);
		return NULL;
	}
	if (!(node->data.email = allocate_and_cpy(e->email)))
	{
		free(node->data.first_name);
		free(node->data.last_name);
		free(node);
		return NULL;
	}
	node->data.id = e->id;
	return node;
}
const Employee* find(LinkedList* list, const Employee* e)
{
	if (!list || !e)
	{
		return NULL;
	}
	Node* cur_node = list->head;
	while (cur_node)
	{
		if ((!strcmp(cur_node->data.first_name, e->first_name)) && (!strcmp(cur_node->data.last_name, e->last_name)))
		{
			return &(cur_node->data);
		}
		cur_node = cur_node->next;
	}
	return NULL;
}

void free_linked_list(LinkedList* list)
{
	if (list)
	{
		Node* cur = list->head, * next = NULL;
		while (cur)
		{
			next = cur->next;
			free_node(cur);
			cur = next;
		}
		free(list);
	}
}
void free_node(Node* node)
{
	free(node->data.first_name);
	free(node->data.last_name);
	free(node->data.email);
	free(node);
}

int fill_linked_list(LinkedList* list, const char* filename)
{
	if (!list || !filename)
	{
		return INVALID_ARGUMENT;
	}
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		return FILE_OPEN_ERROR;
	}
	char buffer[MAX_LINE + 1];
	fgets(buffer, MAX_LINE + 1, fp);
	Employee e;

	while (fgets(buffer, MAX_LINE + 1, fp))
	{
		int error = parse_employee(buffer, &e);
		if (error)
		{
			return error;
		}
		if (push(list, &e))
		{
			return ALLOCATION_ERROR;
		}
	}
	fclose(fp);
	return SUCCESS;
}
int parse_employee(char* buffer, Employee* e)
{
	char* tok = NULL;
	if (!(tok = strtok(buffer, ",")))
	{
		return ID_PARSE_ERROR;
	}
	if (!((*e).id = (int)strtol(tok, NULL, 10)))
	{
		return ID_PARSE_ERROR;
	}
	if (!((*e).first_name = strtok(NULL, ",")))
	{
		return FIRST_NAME_PARSE_ERROR;
	}
	if (!((*e).last_name = strtok(NULL, ",")))
	{
		return LAST_NAME_PARSE_ERROR;
	}
	if (!((*e).email = strtok(NULL, ",")))
	{
		return EMAIL_PARSE_ERROR;
	}
	return SUCCESS;
}

int main()
{
	LinkedList* l = new_linked_list();
	fill_linked_list(l, "employees.csv");
	Node* cur = l->head;
	while (cur)
	{
		printf("%s->", cur->data.last_name);
		cur = cur->next;
	}
	free_linked_list(l);
	return EXIT_SUCCESS;
}