// DO NOT UNCOMMENT ANYTHING FROM HERE!!
//=========================================
//=========== definitions etc. ============
//=========================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define VALIDATE(e, r) if(!(e)) return r;
#define NUM_DAYS 7

typedef int (*copy_info_f) (void* dest, const void* src); // copy func pointer typedef
typedef void (*free_info_f) (void* to_free); // free func pointer typedef
// day enum
typedef enum {
	SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
} day_t;

typedef struct datetime_t {
	size_t hour;
	size_t minute;
} datetime_t;

typedef struct shift_t {
	datetime_t start_time;
	void* employees_info; // generic array
	size_t n_employees_info; // how many cells are in the array
	size_t employees_info_size; // what is the size (in bytes) of each cell in the array
	copy_info_f copy_func;
	free_info_f free_func;
} shift_t;

typedef struct weekly_shifts_t {
	shift_t* shifts[NUM_DAYS]; // array with NUM_DAYS cells, each containing a shift_t pointer.
	size_t n_shifts[NUM_DAYS]; // array with NUM_DAYS cells, each containing a size_t number.
} weekly_shifts_t;


int sort_by_start(const void* a, const void* b) // compare function declaration. a and b will be converted to const shift_t*
{
	const shift_t* x = a;
	const shift_t* y = b;
	if (x->start_time.hour < y->start_time.hour) return -1;
	if (x->start_time.hour > y->start_time.hour) return 1;
	if (x->start_time.minute < y->start_time.minute) return -1;
	if (x->start_time.minute > y->start_time.minute) return 1;
	return 0;
}



/** Dynamically allocate a weekly_shifts_t struct. zero initialize fields.*/
weekly_shifts_t* new_weekly_shifts()
{
	weekly_shifts_t* res = calloc(1, sizeof(weekly_shifts_t));
	return res;
}

/**
 * Add a new shift to the weekly shifts.
 * @param wk pointer the the weekly shifts struct
 * @param day the day to which we add a shift.
 * @param start_time the start time of the shift
 * @param employees_info the array of employee information. NULL is invalid.
 * @param n_employees_info the number of cells in the employee_info array. 0 is invalid.
 * @param employees_info_size the size if bytes of a cell in the employee_info array. 0 is invalid.
 * @param copy_func pointer to a copy function that copies an employee info. NULL is invalid.
 * @param free_func pointer to a free function that frees an employee info NULL is invalid.
 * @return 1 if adding was succesful, 0 otherwise
 */
int add_shift(weekly_shifts_t* wk, day_t day, datetime_t start_time,
	void* employees_info, size_t n_employees_info, size_t employees_info_size,
	copy_info_f copy_func, free_info_f free_func)
{
	if (!wk || start_time.hour > 24 || start_time.minute > 59
		|| !employees_info || !n_employees_info || !employees_info_size
		|| !copy_func || !free_func) return 0;

	size_t size = ++wk->n_shifts[day];
	wk->shifts[day] = realloc(wk->shifts[day], size * sizeof(shift_t));

	char* info = calloc(n_employees_info, employees_info_size);
	char* info_source = employees_info;

	for (size_t i = 0; i < n_employees_info; ++i)
	{
		if (copy_func(
			info + i * employees_info_size,
			info_source + i * employees_info_size) == 0)
		{
			for (; i > 0; --i)
				free_func(info + (i - 1) * employees_info_size);
			--wk->n_shifts[day];
			free(info);
			return 0;
		}
	}

	shift_t shift = { start_time, info, n_employees_info, employees_info_size, copy_func, free_func };
	bool found = false;
	for (size_t i = 0; i < size - 1; ++i)
	{
		if (sort_by_start(&shift, wk->shifts[day] + i) > 0)
		{
			memcpy(wk->shifts[day] + i, wk->shifts[day] + i + 1, sizeof(shift_t) * (size - i - 1));
			wk->shifts[day][i] = shift;
			found = true;
			break;
		}
	}
	if (!found) wk->shifts[day][size - 1] = shift;
	return 1;
}

/**
 * Free the resources of the given weekly shifts and sets to NULL.
 * @param p_wk pointer to weekly_shifts_t pointer.
 */
void free_weekly_shifts(weekly_shifts_t** p_wk)
{
	if (!p_wk || !*p_wk) return;
	weekly_shifts_t* wk = *p_wk;
	for (size_t day = 0; day < NUM_DAYS; ++day)
	{
		for (size_t i = 0; i < wk->n_shifts[day]; ++i)
		{
			char* info = wk->shifts[day][i].employees_info;
			for (int j = 0; j < wk->shifts[day][i].n_employees_info; ++j)
				wk->shifts[day][i].free_func(info + j * wk->shifts[day][i].employees_info_size);
			free(info);
		}
		free(wk->shifts[day]);
	}
	free(wk);
	*p_wk = NULL;
}


// This function is for your tests. 
// If you wish to try some code, you may write it here and see the output in the first test.
// The test that calls this function will have no effect on the grade
void test_func() {

}

int copy_str_second_fail(void* dest, const void* src)
{
	static int run_count = 1;
	if (run_count > 1)
	{
		return 0;
	}
	char** d = (char**)dest;
	const char** s = (const char**)src;
	*d = malloc(strlen(*s) + 1);
	if (!*d)
	{
		return 0;
	}
	strcpy(*d, *s);
	++run_count;
	return 1;
}

void free_str(void* s)
{
	free(*(char**)s);
}

int a(int (*arr)[5])
{
	return 0;
}

int main() {
	a(NULL);
	weekly_shifts_t* wk = new_weekly_shifts();
	char* emp_info2[] = { "str1", "str2", "str3", "str4", "str5" };
	int res = add_shift(wk, MONDAY, (datetime_t) {
		12, 0
	}, emp_info2, 5, sizeof(char*), copy_str_second_fail, free_str);
	printf("%d", res);
	if (res != 0) {
		printf("\nadd should have failed!");
	}
	free_weekly_shifts(&wk);
	return 0;
}

