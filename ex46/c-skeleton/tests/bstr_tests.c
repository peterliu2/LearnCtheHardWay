/**
 * @file bstr_test.c
 * @brief better string test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-20
 */

#include "minunit.h"
#include <lcthw/bstrlib.h>

const char test[]   = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,./]";
const char test_1[] = "peter "; //"1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,./]";
const char test_search[]  = "uiop[]a";
const char test_find[]					=					"uiop[]a";
const char test_replace[]				=					"peterli";
const char test_bfindreplace_result[]   = "1234567890-=qwertypeterlisdfghjkl;'zxcvbnm,./]";

char *test_bfromcstr(void)
{
	bstring b = bfromcstr(test);
	mu_assert(strcmp((const char *)b->data, test) == 0, "Failed to string compare after bfromcstr().");
	mu_assert(b->slen == (int)strlen(test), "Wrong string length after bfromcstr().");
	mu_assert(bdestroy(b) == BSTR_OK, "destroy bstring failed.");
	return NULL;
}

char *test_blk2bstr(void)
{
	bstring b = blk2bstr(test, strlen(test));
	mu_assert(strcmp((const char *)b->data, test) == 0, "Failed to string compare after blk2bstr().");
	mu_assert(b->slen == (int)strlen(test), "Wrong string length after blk2bstr().");
	mu_assert(bdestroy(b) == BSTR_OK, "destroy bstring failed.");
	
	b = blk2bstr(test, strlen(test) / 2);
	mu_assert(strncmp((const char *)b->data, test, strlen(test) / 2) == 0, "Failed to string compare after blk2bstr().");
	mu_assert(b->slen == (int)strlen(test) / 2, "Wrong string length after blk2bstr().");
	mu_assert(bdestroy(b) == BSTR_OK, "destroy bstring failed.");
	return NULL;
}

char *test_bstrcpy(void)
{
	bstring b = bfromcstr(test);
	bstring copy = bstrcpy(b);
	mu_assert(strcmp((const char *)copy->data, test) == 0, "Failed to string compare after bstrcpy().");
	mu_assert(copy->slen == (int)strlen(test), "Wrong string length after bstrcpy().");
	mu_assert(bdestroy(b) == BSTR_OK, "destroy bstring failed.");
	mu_assert(bdestroy(copy) == BSTR_OK, "destroy bstring failed.");
	return NULL;
}

char *test_bassign(void)
{
	bstring b = bfromcstr(test);
	bstring assigned = bfromcstr("peter&&");
	mu_assert(bassign(assigned, b) == BSTR_OK, "Failed to bassign().");

	mu_assert(strcmp((const char *)assigned->data, test) == 0, "Failed to string compare after bassign().");
	mu_assert(assigned->slen == (int)strlen(test), "Wrong string length after bassign().");

	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() after bassign()");
	mu_assert(bdestroy(assigned) == BSTR_OK, "Failed to bdestroy() after bassign().");

	return NULL;
}

char *test_bassigncstr(void)
{
	bstring b = bfromcstr("rwere");
	mu_assert(bassigncstr(b, test) == BSTR_OK, "Failed to bassigncstr().");
	mu_assert(strcmp((const char *)b->data, test) == 0, "Failed to string compare after bassigncstr().");
	mu_assert(b->slen == (int)strlen(test), "Wrong string length after bassigncstr().");
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr bassigncstr().");
	return NULL;
}

char *test_bassignblk(void)
{
	bstring b = bfromcstr("");
	mu_assert(bassignblk(b, test, strlen(test)) == BSTR_OK, "Failed to bassignblk.");
	mu_assert(strcmp((const char *)b->data, test) == 0, "Wrong string compare after bassignblk().");
	mu_assert(b->slen == (int)strlen(test), "wrong string length after bassignblk().");
	
	mu_assert(bassignblk(b, test, strlen(test) / 2) == BSTR_OK, "Failed to bassignblk.");
	mu_assert(strncmp((const char *)b->data, test, strlen(test) / 2) == 0, "Wrong string compare after bassignblk().");
	mu_assert(b->slen == (int)strlen(test) / 2, "wrong string length after bassignblk().");
	
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr bassignblk().");
	return NULL;
}

char *test_bconcat(void)
{
	bstring b0 = bfromcstr(test);
	bstring b1 = bfromcstr(test_1);

	mu_assert(bconcat(b0, b1) == BSTR_OK, "Failed to bconcat().");
	mu_assert(strncmp((const char *)b0->data, test, strlen(test)) == 0, "Wrong original part afetr bconcat().");
	mu_assert(strncmp((const char *)b0->data + strlen(test), test_1, strlen(test_1)) == 0, "Wrong concate part afetr bconcat().");
	mu_assert(b0->slen == (int)strlen(test) + (int)strlen(test_1), "Wrong concat string length after bconcat().");
	
	mu_assert(bdestroy(b0) == BSTR_OK, "Failed to bdestroy() afetr bconcat().");
	mu_assert(bdestroy(b1) == BSTR_OK, "Failed to bdestroy() afetr bconcat().");
	return NULL;
}

char *test_bstricmp(void)
{
	bstring b0 = bfromcstr(test);
	bstring b1 = bfromcstr(test);
	mu_assert(bstricmp(b0, b1) == 0,"Failed to bstricmp().");

	mu_assert(bdestroy(b1) == BSTR_OK, "Failed to bdestroy() afetr bstricmp().");
	b1 = bfromcstr(test_1);
	mu_assert(bstricmp(b0, b1) != 0,"Failed to bstricmp().");
	
	mu_assert(bdestroy(b0) == BSTR_OK, "Failed to bdestroy() afetr bstricmp().");
	mu_assert(bdestroy(b1) == BSTR_OK, "Failed to bdestroy() afetr bstricmp().");
	return NULL;
}

char *test_biseq(void)
{
	bstring b0 = bfromcstr(test);
	bstring b1 = bfromcstr(test);
	
	mu_assert(biseq(b0, b1) == 1, "Failed to equal test.");

	mu_assert(bdestroy(b0) == BSTR_OK, "Failed to bdestroy() afetr biseq().");
	b0 = bfromcstr(test_1);
	mu_assert(biseq(b0, b1) == 0, "Failed to not equal test.");

	mu_assert(bdestroy(b1) == BSTR_OK, "Failed to bdestroy() afetr biseq().");
	b1 = bfromcstr(test_1);
	mu_assert(biseq(b0, b1) == 1, "Failed to equal test.");
	
	mu_assert(bdestroy(b0) == BSTR_OK, "Failed to bdestroy() afetr biseq().");
	mu_assert(bdestroy(b1) == BSTR_OK, "Failed to bdestroy() afetr biseq().");
	return NULL;
}

char *test_binstr(void)
{
	bstring b = bfromcstr(test);
	bstring search = bfromcstr(test_1);
	mu_assert(binstr(b, 0, search) == BSTR_ERR, "Failed to not found on binstr().");
	
	mu_assert(bdestroy(search) == BSTR_OK, "Failed to bdestroy() afetr binstr().");
	search = bfromcstr(test_search);
	mu_assert(binstr(b, 0, search) == 18, "Failed to found on binstr().");
	mu_assert(binstr(b, 1, search) == 18, "Failed to found on binstr().");
	mu_assert(binstr(b, 10, search) == 18, "Failed to found on binstr().");
	mu_assert(binstr(b, 18, search) == 18, "Failed to found on binstr().");
	mu_assert(binstr(b, 19, search) == BSTR_ERR, "Failed to not found on binstr().");
		
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr binstr).");
	mu_assert(bdestroy(search) == BSTR_OK, "Failed to bdestroy() afetr binstr().");
	return NULL;
}

char *test_bfindreplace(void)
{
	bstring b = bfromcstr(test);
	bstring find = bfromcstr(test_find);
	bstring replace = bfromcstr(test_replace);

	mu_assert(bfindreplace(b, find, replace, 0) == BSTR_OK, "Failed to find replace on bfindreplace().");
	mu_assert(strcmp((const char *)b->data, test_bfindreplace_result) == 0, "Wrong bfindreplace() result.");

	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr bfindreplace().");
	b = bfromcstr(test);
	mu_assert(bfindreplace(b, find, replace, 10) == BSTR_OK, "Failed to find replace on bfindreplace().");
	mu_assert(strcmp((const char *)b->data, test_bfindreplace_result) == 0, "Wrong bfindreplace() result.");
	
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr bfindreplace().");
	mu_assert(bdestroy(find) == BSTR_OK, "Failed to bdestroy() afetr bfindreplace().");
	mu_assert(bdestroy(replace) == BSTR_OK, "Failed to bdestroy() afetr bfindreplace().");
	return NULL;
}

char *test_bsplit(void)
{
	struct bstrList *strlist = NULL;
	bstring b = bfromcstr("peter liu,, yahoo");
	mu_assert(b != NULL, "Failed to create bstring on bsplit().");
	strlist = bsplit(b, ' ');
	mu_assert(strlist->qty == 3, "Wrong split substring quy.");
	mu_assert(strcmp((const char *)strlist->entry[0]->data, "peter") == 0, "Wrong 1st split substring.");
	mu_assert(strcmp((const char *)strlist->entry[1]->data, "liu,,") == 0, "Wrong 2nd split substring.");
	mu_assert(strcmp((const char *)strlist->entry[2]->data, "yahoo") == 0, "Wrong 3nd split substring.");

	mu_assert(bstrListDestroy(strlist) == BSTR_OK, "Failed to destroy string list on bsplit().");
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr bsplit().");
	return NULL;
}

char *test_bformat(void)
{
	bstring b = bformat("Hello %s, %d", "peter", 12345);
	mu_assert(b != NULL, "Failed to bformat().");
	mu_assert(strcmp((const char *)b->data, "Hello peter, 12345") == 0, "Wrong bformat() data.");
	
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr bformat().");
	return NULL;
}

char *test_blength_bdata_bchar(void)
{
	int i = 0;
	
	bstring b = bfromcstr(test);
	mu_assert(b != NULL, "Failed to create bstring.");
	mu_assert(blength(b) == strlen(test), "Wrong string length on blength().");
	char *s = bdata(b);
	mu_assert(strcmp(s, test) == 0, "Wrong on bdada().");
	for (i = 0; i < blength(b); i++) {
		mu_assert(bchar(b, i) == test[i], "Wrong on bchar().");	
	}
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr blength().");
	
	b = bfromcstr(test_1);
	mu_assert(b != NULL, "Failed to create bstring.");
	mu_assert(blength(b) == strlen(test_1), "Wrong string length on blength().");
	s = bdata(b);
	mu_assert(strcmp(s, test_1) == 0, "Wrong on bdata().");
	for (i = 0; i < blength(b); i++) {
		mu_assert(bchar(b, i) == test_1[i], "Wrong on bchar().");	
	}
	mu_assert(bdestroy(b) == BSTR_OK, "Failed to bdestroy() afetr blength().");
	
	return NULL;
}

char *all_tests(void) 
{
	mu_suit_start();
	
	mu_run_test(test_bfromcstr);
	mu_run_test(test_blk2bstr);
	mu_run_test(test_bstrcpy);
	mu_run_test(test_bassign);
	mu_run_test(test_bassigncstr);
	mu_run_test(test_bassignblk);
	mu_run_test(test_bconcat);
	mu_run_test(test_bstricmp);
	mu_run_test(test_biseq);
	mu_run_test(test_binstr);
	mu_run_test(test_bfindreplace);
	mu_run_test(test_bsplit);
	mu_run_test(test_bformat);
	mu_run_test(test_blength_bdata_bchar);
	return NULL;
}

RUN_TESTS(all_tests);
