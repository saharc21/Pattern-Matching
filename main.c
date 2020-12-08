#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "slist.c"
#include "pattern_matching.c"

void print_pm(pm_t *pm);
void print_state(pm_state_t *state, int tabs, int* is_need, int is_get);
void print_tabs(int tabs, int* is_need);
unsigned char* shift(unsigned char* str, int len);
void search_and_destroy(pm_t *pm, char *s);
void _test1(pm_t *pm);
void _test2(pm_t *pm);
void _test3(pm_t *pm);
void _test4(pm_t *pm);
void _test5(pm_t *pm);
void _test6(pm_t *pm);
void _test7(pm_t *pm);
void _test8(pm_t *pm);
void _test9(pm_t *pm);
void _test10(pm_t *pm);
void _test11(pm_t *pm);
void _test12(pm_t *pm);
void _test13(pm_t *pm);

int main(int argc, char* argv[]) {
    pm_t *pm = (pm_t*)malloc(sizeof(pm_t));
    if(!pm) {
        return -1;
    }
    int _test = -1;
    if(argc == 2) {
        _test = atoi(argv[1]);
        
    }
    else {
        printf("this tester need get 1 argc:\ntest number\n");
        printf("test 1 to 7: standard test\n");
        printf("test 8 to 13: good luck\n");
        printf("the output save in log.txt file at the same location\n");
        printf("and comper with friends\n");
        printf("run the program again and not just enter the test number\n");
        printf("when test fail, search for _test[test number] function name\n");
    }
    
    FILE *fp;
    fp = fopen("log.txt", "w+");
    dup2(fileno(fp), fileno(stdout));
    
    switch (_test) {
        case 1:
            _test1(pm);
            break;
        case 2:
            _test2(pm);
            break;
        case 3:
            _test3(pm);
            break;
        case 4:
            _test4(pm);
            break;
        case 5:
            _test5(pm);
            break;
        case 6:
            _test6(pm);
            break;
        case 7:
            _test7(pm);
            break;
        case 8:
            _test8(pm);
            break;
        case 9:
            _test9(pm);
            break;
        case 10:
            _test10(pm);
            break;
        case 11:
            _test11(pm);
            break;
        case 12:
            _test12(pm);
            break;
        case 13:
            _test13(pm);
            break;
        default:
            break;
    }
    
    
    free(pm);
    fclose(fp);
    return 0;
}

void _test1(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"abcdefghijklmnopqrstuvwxyz", 26);
    pm_addstring(pm, (unsigned char*)"cbdefghijklmnopqrstuvwxyza", 26);
    pm_addstring(pm, (unsigned char*)"cdefghijklmnopqrstuvwxyzab", 26);
    pm_addstring(pm, (unsigned char*)"defghijklmnopqrstuvwxyzabc", 26);
    pm_addstring(pm, (unsigned char*)"efghijklmnopqrstuvwxyzabcd", 26);
    pm_addstring(pm, (unsigned char*)"fghijklmnopqrstuvwxyzabcde", 26);
    pm_addstring(pm, (unsigned char*)"ghijklmnopqrstuvwxyzabcdef", 26);
    pm_addstring(pm, (unsigned char*)"hijklmnopqrstuvwxyzabcdefg", 26);
    pm_addstring(pm, (unsigned char*)"ijklmnopqrstuvwxyzabcdefgh", 26);
    pm_addstring(pm, (unsigned char*)"jklmnopqrstuvwxyzabcdefghi", 26);
    pm_addstring(pm, (unsigned char*)"klmnopqrstuvwxyzabcdefghij", 26);
    pm_addstring(pm, (unsigned char*)"lmnopqrstuvwxyzabcdefghijk", 26);
    pm_addstring(pm, (unsigned char*)"mnopqrstuvwxyzabcdefghijkl", 26);
    pm_addstring(pm, (unsigned char*)"nopqrstuvwxyzabcdefghijklm", 26);
    pm_addstring(pm, (unsigned char*)"opqrstuvwxyzabcdefghijklmn", 26);
    pm_addstring(pm, (unsigned char*)"pqrstuvwxyzabcdefghijklmno", 26);
    pm_addstring(pm, (unsigned char*)"qrstuvwxyzabcdefghijklmnop", 26);
    pm_addstring(pm, (unsigned char*)"rstuvwxyzabcdefghijklmnopq", 26);
    pm_addstring(pm, (unsigned char*)"stuvwxyzabcdefghijklmnopqr", 26);
    pm_addstring(pm, (unsigned char*)"tuvwxyzabcdefghijklmnopqrs", 26);
    pm_addstring(pm, (unsigned char*)"uvwxyzabcdefghijklmnopqrst", 26);
    pm_addstring(pm, (unsigned char*)"vwxyzabcdefghijklmnopqrstu", 26);
    pm_addstring(pm, (unsigned char*)"wxyzabcdefghijklmnopqrstuv", 26);
    pm_addstring(pm, (unsigned char*)"xyzabcdefghijklmnopqrstuvw", 26);
    pm_addstring(pm, (unsigned char*)"yzabcdefghijklmnopqrstuvwx", 26);
    pm_addstring(pm, (unsigned char*)"zabcdefghijklmnopqrstuvwxy", 26);
    pm_makeFSM(pm);
    char *s = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxy";
    search_and_destroy(pm, s);
}

void _test2(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"abcde", 5);
    pm_addstring(pm, (unsigned char*)"bcde", 4);
    pm_addstring(pm, (unsigned char*)"cde", 3);
    pm_addstring(pm, (unsigned char*)"df", 2);
    pm_makeFSM(pm);
    char *s = "abcdf";
    search_and_destroy(pm, s);
}

void _test3(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"a", 1);
    pm_addstring(pm, (unsigned char*)"aa", 2);
    pm_addstring(pm, (unsigned char*)"aaa", 3);
    pm_addstring(pm, (unsigned char*)"aaaa", 4);
    pm_makeFSM(pm);
    char *s = "aaaaaaaa";
    search_and_destroy(pm, s);
}

void _test4(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"bcd", 3);
    pm_addstring(pm, (unsigned char*)"abcd", 4);
    pm_addstring(pm, (unsigned char*)"bcde", 4);
    pm_addstring(pm, (unsigned char*)"abcde", 5);
    pm_makeFSM(pm);
    char *s = "abcde";
    search_and_destroy(pm, s);
}

void _test5(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"abcdef", 6);
    pm_addstring(pm, (unsigned char*)"bcde", 4);
    pm_addstring(pm, (unsigned char*)"cde", 3);
    pm_addstring(pm, (unsigned char*)"de", 2);
    pm_addstring(pm, (unsigned char*)"ef", 2);
    pm_makeFSM(pm);
    char *s = "abcdef";
    search_and_destroy(pm, s);
}

void _test6(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"aba", 3);
    pm_addstring(pm, (unsigned char*)"ba", 2);
    pm_addstring(pm, (unsigned char*)"a", 1);
    pm_makeFSM(pm);
    char *s = "xyzabacdef";
    search_and_destroy(pm, s);
}

void _test7(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"c", 1);
    pm_addstring(pm, (unsigned char*)"bsnfsnf", 0);
    pm_addstring(pm, (unsigned char*)"d", 1);
    pm_makeFSM(pm);
    char *s = "abcde";
    search_and_destroy(pm, s);
}

// not doing pm_makeFSM
void _test8(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"a", 1);
    char *s = "aaaaaaaa";
    search_and_destroy(pm, s);
}

// not doing pm_addstring
void _test9(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_makeFSM(pm);
    char *s = "aaaaaaaa";
    search_and_destroy(pm, s);
}

// sending NULL everywhere
void _test10(pm_t *pm) {
    pm_init(NULL);
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(NULL, (unsigned char*)"a", 1);
    pm_makeFSM(NULL);
    char *s = "aaaaaaaa";
    pm_fsm_search(NULL, (unsigned char*)s, strlen(s));
    pm_destroy(pm);
}

//the sending string to pm_fsm_search is NULL
void _test11(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"a", 1);
    pm_addstring(pm, (unsigned char*)"aa", 2);
    pm_addstring(pm, (unsigned char*)"aaa", 3);
    pm_addstring(pm, (unsigned char*)"aaaa", 4);
    pm_makeFSM(pm);
    pm_fsm_search(pm->zerostate, NULL, 17);
    pm_destroy(pm);
}

// add multi times the same pattern
void _test12(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"bcd", 3);
    pm_addstring(pm, (unsigned char*)"bcd", 3);
    pm_addstring(pm, (unsigned char*)"bcd", 3);
    pm_addstring(pm, (unsigned char*)"bcd", 3);
    pm_addstring(pm, (unsigned char*)"b", 1);
    pm_addstring(pm, (unsigned char*)"c", 1);
    pm_addstring(pm, (unsigned char*)"d", 1);
    pm_makeFSM(pm);
    char *s = "abcde";
    search_and_destroy(pm, s);
}

// pm_addstring, pm_makeFSM, pm_addstring, pm_makeFSM
void _test13(pm_t *pm) {
    if(pm_init(pm) == -1){
        printf("error init pm");
        exit(-1);
    }
    pm_addstring(pm, (unsigned char*)"bcd", 3);
    pm_makeFSM(pm);
    pm_addstring(pm, (unsigned char*)"b", 1);
    pm_addstring(pm, (unsigned char*)"c", 1);
    pm_addstring(pm, (unsigned char*)"d", 1);
    pm_makeFSM(pm);
    char *s = "abcdbcde";
    search_and_destroy(pm, s);
}

void search_and_destroy(pm_t *pm, char *s) {
    slist_t *list = pm_fsm_search(pm->zerostate, (unsigned char*)s, strlen(s));
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
    pm_destroy(pm);
}

unsigned char* shift(unsigned char* str, int len) {
    int i;
    unsigned char* temp = (unsigned char*)malloc(sizeof(unsigned char)* (len));
    for(i = 0; i < len-1; i++) {
        temp[i] = str[i+1];
    }
    temp[len-1] = str[0];
    return temp;
}

void print_match(slist_t *list) {
    slist_node_t *node = slist_head(list);
    while(node) {
        pm_match_t *match = (pm_match_t*)slist_data(node);
        printf("the \"%s\" start at %d, end at %d at %d state\n",match->pattern, match->start_pos, match->end_pos, match->fstate->id);
        node = slist_next(node);
    }
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
}

void print_pm(pm_t *pm) {
    if(!pm) {
        return;
    }
    printf("state(id, fail state id)\n");
    printf("(root)--------");
    int *b = (int*)malloc(sizeof(int)*100);
    int i;
    for(i = 0; i < 100; i++) {
        b[i] = 0;
    }
    print_state(pm->zerostate, 1, b, 0);
    free(b);
}

void print_state(pm_state_t *state, int tabs, int* is_need, int is_get) {
    slist_node_t *node = slist_head(state->_transitions);
    if(!node) {
        printf("\n");
        print_tabs(tabs, is_need);
        printf("\n");
        return;
    }
    int use_tabs = 0;
    while(node) {
        pm_labeled_edge_t *edge = (pm_labeled_edge_t*)slist_data(node);
        int is_out_state = slist_head(edge->state->output) ? 0 : -1;
        if(use_tabs != 0) {
            print_tabs(tabs, is_need);
            printf("%c---", edge->label);
        }
        
        else {
            printf("--|%c---", edge->label);
            use_tabs++;
        }
        
        int id = edge->state->id;
        int fail_id = edge->state->fail ? edge->state->fail->id : 0;
        is_out_state == 0 ? printf("(") : printf("-");
        printf("(");
        if(id < 10)
            printf(" ");
        printf("%d,", id);
        if(fail_id < 10)
            printf(" ");
        printf("%d)", fail_id);
        is_out_state == 0 ? printf(")") : printf("-");
        node = slist_next(node);
        if(node) {
            is_need[state->depth] = 1;
        }
        
        else {
            is_need[state->depth] = 0;
        }
        print_state(edge->state, tabs+1, is_need, is_get);
    }
    print_tabs(tabs, is_need);
    printf("\n");
    
    
}

void print_tabs(int tabs, int* is_need) {
    int i;
    for(i = 0; i < tabs; i++) {
        printf("\t\t\t\t");
        if(is_need[i] == 1) {
            printf("|");
        }
    }
}
