#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "fsm_priv.h"

typedef struct _comparitor_helper_ COMPARITOR_HELPER, *pCOMPARITOR_HELPER;
struct _comparitor_helper_ 
{
   pID_INFO id_infos;
   unsigned num_id_infos;
   unsigned which;
   bool     identical;
};

bool comparitor         (pLIST_ELEMENT,void*);
bool print_id_info_name (pLIST_ELEMENT,void*);

typedef bool (*TEST_FN)(void);

bool add_to_list_test(void);
bool join_lists      (void);
bool finder_test_0   (void);
bool finder_test_1   (void);
bool finder_test_2   (void);
bool finder_test_3   (void);
bool finder_test_4   (void);
bool finder_test_5   (void);
bool copy_test_1     (void);

TEST_FN tests[] = {
	add_to_list_test
	, join_lists
	, finder_test_0
	, finder_test_1
	, finder_test_2
	, finder_test_3
	, finder_test_4
	, finder_test_5
  , copy_test_1
	, NULL
};

ID_INFO id_infos_0[] =
{
	{
		.name = "0 - one"
	}
	, {
		.name = "0 - two"
	}
	, {
		.name = "0 - three"
	}
};

ID_INFO id_infos_1[] =
{
	{
		.name = "1 - one"
	}
	, {
		.name = "1 - two"
	}
	, {
		.name = "1 - three"
	}
};

unsigned id_info_count_0 = sizeof(id_infos_0)/sizeof(id_infos_0[0]);
unsigned id_info_count_1 = sizeof(id_infos_1)/sizeof(id_infos_1[0]);

bool print_id_info_name(pLIST_ELEMENT piilm, void *data)
{
	pID_INFO pid = (pID_INFO) piilm->mbr;

  (void) data;

	printf("%s\n", pid->name);
	return false;
}

bool comparitor(pLIST_ELEMENT pelem, void *data)
{
   pCOMPARITOR_HELPER pch = (pCOMPARITOR_HELPER) data;

   pch->identical = false;
   if (pelem->mbr == &pch->id_infos[pch->which++])
   {
      pch->identical = true;
   }

   return pch->identical;
}

bool add_to_list_test()
{
	unsigned id_info_counter;

	printf("\nadd_to_list\n");
 
	/* initialize the first list */
	pLIST pthe_list = init_list();

	/* load up the list */
	for (id_info_counter = 0; id_info_counter < id_info_count_0; id_info_counter++)
	{
		add_to_list(pthe_list, &id_infos_0[id_info_counter]);
	}
	
	printf("id info list count %u\n", pthe_list->count);

	iterate_list(pthe_list,print_id_info_name,pthe_list);

	return id_info_counter == pthe_list->count;

}

bool join_lists()
{
	unsigned id_info_counter_0;
	unsigned id_info_counter_1;
 
	printf("\njoin_lists\n");

	/* initialize the first list */
	pLIST pthe_first_list = init_list();

	/* load up the list */
	for (id_info_counter_0 = 0; id_info_counter_0 < id_info_count_0; id_info_counter_0++)
	{
		add_to_list(pthe_first_list, &id_infos_0[id_info_counter_0]);
	}
	
	printf("id info list count %u\n", pthe_first_list->count);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	/* initialize the second list */
	pLIST pthe_second_list = init_list();

	/* load up the list */
	for (id_info_counter_1 = 0; id_info_counter_1 < id_info_count_1; id_info_counter_1++)
	{
		add_to_list(pthe_second_list, &id_infos_1[id_info_counter_1]);
	}
	
	printf("id info list count %u\n", pthe_second_list->count);

	iterate_list(pthe_second_list,print_id_info_name,pthe_second_list);

	printf("join the lists\n");
	pLIST pdest = move_list(pthe_first_list, pthe_second_list);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	printf("pdest %p; pthe_first_list %p\n"
					, pdest
					, pthe_first_list
				);

	printf("pthe_second_list:\n\thead: %p\n\ttail: %p\n\tcount: %u\n"
					, pthe_second_list->head
					, pthe_second_list->tail
					, pthe_second_list->count
					);

	return (
						   (pdest == pthe_first_list)
						&& (pthe_second_list->head  == NULL)
						&& (pthe_second_list->tail  == NULL)
						&& (pthe_second_list->count == 0)
						&& (pthe_first_list->count  == (id_info_counter_0 + id_info_counter_1))
         );
}

bool finder_test_0()
{
	unsigned id_info_counter;
	unsigned record_to_find = 1;

	printf("\nfinder_test_0\n");

	/* initialize the first list */
	pLIST pthe_list = init_list();

	/* load up the list */
	for (id_info_counter = 0; id_info_counter < id_info_count_0; id_info_counter++)
	{
		add_to_list(pthe_list, &id_infos_0[id_info_counter]);
	}
	
	printf("id info list count %u\n", pthe_list->count);

	iterate_list(pthe_list,print_id_info_name,pthe_list);

	pID_INFO pid = (pID_INFO) find_nth_list_member(pthe_list, record_to_find);

	printf("pid: %p; &id_infos_0[%u]: %p; pid->name: %s\n"
					, pid
					, record_to_find
					, &id_infos_0[record_to_find]
					, pid->name
					);

	return pid == &id_infos_0[record_to_find];

}

bool finder_test_1()
{
	unsigned id_info_counter_0;
	unsigned id_info_counter_1;
	unsigned record_to_find = 4;
 
	printf("\nfinder_test_1\n");

	/* initialize the first list */
	pLIST pthe_first_list = init_list();

	/* load up the list */
	for (id_info_counter_0 = 0; id_info_counter_0 < id_info_count_0; id_info_counter_0++)
	{
		add_to_list(pthe_first_list, &id_infos_0[id_info_counter_0]);
		printf("&id_infos_0[%u]: %p\n"
						, id_info_counter_0
						, &id_infos_0[id_info_counter_0]
						);
	}
	
	printf("id info list count %u\n", pthe_first_list->count);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	/* initialize the second list */
	pLIST pthe_second_list = init_list();

	/* load up the list */
	for (id_info_counter_1 = 0; id_info_counter_1 < id_info_count_1; id_info_counter_1++)
	{
		add_to_list(pthe_second_list, &id_infos_1[id_info_counter_1]);
		printf("&id_infos_1[%u]: %p\n"
						, id_info_counter_1
						, &id_infos_1[id_info_counter_1]
						);
	}
	
	printf("id info list count %u\n", pthe_second_list->count);

	iterate_list(pthe_second_list,print_id_info_name,pthe_second_list);

	printf("join the lists\n");
	pLIST pdest = move_list(pthe_first_list, pthe_second_list);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	pID_INFO pid = (pID_INFO) find_nth_list_member(pdest, record_to_find);

	pID_INFO infos = (record_to_find > (id_info_counter_0 - 1)) ? id_infos_1 : id_infos_0;
	unsigned  offset = (record_to_find > (id_info_counter_0 - 1))
												? (record_to_find - id_info_counter_0)
												: record_to_find
												;

	printf("id_infos_0: %p; id_infos_1: %p; infos: %p\nid_info_counter_0: %u; offset: %u\n"
					, id_infos_0
					, id_infos_1
					, infos
					, id_info_counter_0
					, offset
					);

	printf("pid: %p; &infos[%u]: %p; pid->name: %s\n"
					, pid
					, offset
					, &infos[offset]
					, pid->name
					);

	return pid == &infos[offset];

}


bool finder_test_2()
{
	unsigned id_info_counter_0;
	unsigned id_info_counter_1;
	unsigned record_to_find = 3;
 
	printf("\nfinder_test_2\n");

	/* initialize the first list */
	pLIST pthe_first_list = init_list();

	/* load up the list */
	for (id_info_counter_0 = 0; id_info_counter_0 < id_info_count_0; id_info_counter_0++)
	{
		add_to_list(pthe_first_list, &id_infos_0[id_info_counter_0]);
		printf("&id_infos_0[%u]: %p\n"
						, id_info_counter_0
						, &id_infos_0[id_info_counter_0]
						);
	}
	
	printf("id info list count %u\n", pthe_first_list->count);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	/* initialize the second list */
	pLIST pthe_second_list = init_list();

	/* load up the list */
	for (id_info_counter_1 = 0; id_info_counter_1 < id_info_count_1; id_info_counter_1++)
	{
		add_to_list(pthe_second_list, &id_infos_1[id_info_counter_1]);
		printf("&id_infos_1[%u]: %p\n"
						, id_info_counter_1
						, &id_infos_1[id_info_counter_1]
						);
	}
	
	printf("id info list count %u\n", pthe_second_list->count);

	iterate_list(pthe_second_list,print_id_info_name,pthe_second_list);

	printf("join the lists\n");
	pLIST pdest = move_list(pthe_first_list, pthe_second_list);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	pID_INFO pid = (pID_INFO) find_nth_list_member(pdest, record_to_find);

	pID_INFO infos = (record_to_find > (id_info_counter_0 - 1)) ? id_infos_1 : id_infos_0;
	unsigned  offset = (record_to_find > (id_info_counter_0 - 1))
												? (record_to_find - id_info_counter_0)
												: record_to_find
												;

	printf("id_infos_0: %p; id_infos_1: %p; infos: %p\nid_info_counter_0: %u; offset: %u\n"
					, id_infos_0
					, id_infos_1
					, infos
					, id_info_counter_0
					, offset
					);

	printf("pid: %p; &infos[%u]: %p; pid->name: %s\n"
					, pid
					, offset
					, &infos[offset]
					, pid->name
					);

	return pid == &infos[offset];

}

bool finder_test_3()
{
	unsigned id_info_counter_0;
	unsigned id_info_counter_1;
	unsigned record_to_find = 0;
 
	printf("\nfinder_test_3\n");

	/* initialize the first list */
	pLIST pthe_first_list = init_list();

	/* load up the list */
	for (id_info_counter_0 = 0; id_info_counter_0 < id_info_count_0; id_info_counter_0++)
	{
		add_to_list(pthe_first_list, &id_infos_0[id_info_counter_0]);
		printf("&id_infos_0[%u]: %p\n"
						, id_info_counter_0
						, &id_infos_0[id_info_counter_0]
						);
	}
	
	printf("id info list count %u\n", pthe_first_list->count);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	/* initialize the second list */
	pLIST pthe_second_list = init_list();

	/* load up the list */
	for (id_info_counter_1 = 0; id_info_counter_1 < id_info_count_1; id_info_counter_1++)
	{
		add_to_list(pthe_second_list, &id_infos_1[id_info_counter_1]);
		printf("&id_infos_1[%u]: %p\n"
						, id_info_counter_1
						, &id_infos_1[id_info_counter_1]
						);
	}
	
	printf("id info list count %u\n", pthe_second_list->count);

	iterate_list(pthe_second_list,print_id_info_name,pthe_second_list);

	printf("join the lists\n");
	pLIST pdest = move_list(pthe_first_list, pthe_second_list);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	pID_INFO pid = (pID_INFO) find_nth_list_member(pdest, record_to_find);

	pID_INFO infos = (record_to_find > (id_info_counter_0 - 1)) ? id_infos_1 : id_infos_0;
	unsigned  offset = (record_to_find > (id_info_counter_0 - 1))
												? (record_to_find - id_info_counter_0)
												: record_to_find
												;

	printf("id_infos_0: %p; id_infos_1: %p; infos: %p\nid_info_counter_0: %u; offset: %u\n"
					, id_infos_0
					, id_infos_1
					, infos
					, id_info_counter_0
					, offset
					);

	printf("pid: %p; &infos[%u]: %p; pid->name: %s\n"
					, pid
					, offset
					, &infos[offset]
					, pid->name
					);

	return pid == &infos[offset];

}

bool finder_test_4()
{
	unsigned id_info_counter_0;
	unsigned id_info_counter_1;
	unsigned record_to_find = 2;
 
	printf("\nfinder_test_4\n");

	/* initialize the first list */
	pLIST pthe_first_list = init_list();

	/* load up the list */
	for (id_info_counter_0 = 0; id_info_counter_0 < id_info_count_0; id_info_counter_0++)
	{
		add_to_list(pthe_first_list, &id_infos_0[id_info_counter_0]);
		printf("&id_infos_0[%u]: %p\n"
						, id_info_counter_0
						, &id_infos_0[id_info_counter_0]
						);
	}
	
	printf("id info list count %u\n", pthe_first_list->count);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	/* initialize the second list */
	pLIST pthe_second_list = init_list();

	/* load up the list */
	for (id_info_counter_1 = 0; id_info_counter_1 < id_info_count_1; id_info_counter_1++)
	{
		add_to_list(pthe_second_list, &id_infos_1[id_info_counter_1]);
		printf("&id_infos_1[%u]: %p\n"
						, id_info_counter_1
						, &id_infos_1[id_info_counter_1]
						);
	}
	
	printf("id info list count %u\n", pthe_second_list->count);

	iterate_list(pthe_second_list,print_id_info_name,pthe_second_list);

	printf("join the lists\n");
	pLIST pdest = move_list(pthe_first_list, pthe_second_list);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	pID_INFO pid = (pID_INFO) find_nth_list_member(pdest, record_to_find);

	pID_INFO infos = (record_to_find > (id_info_counter_0 - 1)) ? id_infos_1 : id_infos_0;
	unsigned  offset = (record_to_find > (id_info_counter_0 - 1))
												? (record_to_find - id_info_counter_0)
												: record_to_find
												;

	printf("id_infos_0: %p; id_infos_1: %p; infos: %p\nid_info_counter_0: %u; offset: %u\n"
					, id_infos_0
					, id_infos_1
					, infos
					, id_info_counter_0
					, offset
					);

	printf("pid: %p; &infos[%u]: %p; pid->name: %s\n"
					, pid
					, offset
					, &infos[offset]
					, pid->name
					);

	return pid == &infos[offset];

}


bool finder_test_5()
{
	unsigned id_info_counter_0;
	unsigned id_info_counter_1;
	unsigned record_to_find = 8;
 
	printf("\nfinder_test_5\n");

	/* initialize the first list */
	pLIST pthe_first_list = init_list();

	/* load up the list */
	for (id_info_counter_0 = 0; id_info_counter_0 < id_info_count_0; id_info_counter_0++)
	{
		add_to_list(pthe_first_list, &id_infos_0[id_info_counter_0]);
		printf("&id_infos_0[%u]: %p\n"
						, id_info_counter_0
						, &id_infos_0[id_info_counter_0]
						);
	}
	
	printf("id info list count %u\n", pthe_first_list->count);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	/* initialize the second list */
	pLIST pthe_second_list = init_list();

	/* load up the list */
	for (id_info_counter_1 = 0; id_info_counter_1 < id_info_count_1; id_info_counter_1++)
	{
		add_to_list(pthe_second_list, &id_infos_1[id_info_counter_1]);
		printf("&id_infos_1[%u]: %p\n"
						, id_info_counter_1
						, &id_infos_1[id_info_counter_1]
						);
	}
	
	printf("id info list count %u\n", pthe_second_list->count);

	iterate_list(pthe_second_list,print_id_info_name,pthe_second_list);

	printf("join the lists\n");
	pLIST pdest = move_list(pthe_first_list, pthe_second_list);

	iterate_list(pthe_first_list,print_id_info_name,pthe_first_list);

	pID_INFO pid = (pID_INFO) find_nth_list_member(pdest, record_to_find);

	return pid == NULL;

}

bool copy_test_1(void)
{
   pLIST pdest = init_list();
   pLIST psrc = init_list();
   pLIST ptst;
   COMPARITOR_HELPER ch;

   printf("\ncopy_test_1\n");

   /* load up the list */
   for (unsigned id_info_counter_0 = 0;
         id_info_counter_0 < id_info_count_0;
         id_info_counter_0++
       )
   {
     add_to_list(psrc, &id_infos_0[id_info_counter_0]);
   }

   printf("the source list:\n");
   printf("id info list count %u\n", psrc->count);

   iterate_list(psrc,print_id_info_name,NULL);

   ptst = copy_list(pdest, psrc);

   printf("the dest list:\n");
   printf("id info list count %u\n", pdest->count);
   iterate_list(pdest,print_id_info_name,NULL);

   ch.id_infos     = id_infos_0;
   ch.num_id_infos = id_info_count_0;
   ch.which        = 0;
   ch.identical    = false;

   iterate_list(pdest, comparitor, &ch);

   return (ptst == pdest)
          && ch.identical
          && (pdest->count == psrc->count)
          && (pdest->count == id_info_count_0)
          ;
   
}

int main(int argc, char **argv)
{
	int result = 0;

	printf("%s: hello, world\n",argv[0]);

	for (TEST_FN *test_fn = tests; *test_fn; test_fn++)
	{
		if (!(*test_fn)())
		{
			result = 1;
			break;
		}
	}

	printf("%s\n", result ? "Test failed" : "All tests pass");

	return result;
}

