/* Directions: complete the code by filling in "FILL" parts
 *   adding functions and variables is allowed
 *   do not change print and file load/save
 *   do not use your compiler-specific libraries
 * FILL parts: insertion and merge sort on linked lists
 */

/* Insertion sort and merge sort on Linked Lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////
// common functions for handling arrays
/////////////////////////////////////////////////////////////
long* readkeys_textfile( const char infile[], int *pN )
  // returns an array of long integers, with its size stored in
  // the memory indicated by integer pointer variable pN
  // the retured memory should freed by the caller
{
  long *A, tmp;
  int i;
  FILE *fp;

  // check for input file name
  if ( infile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return NULL;
  }

  // check for file existence
  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",infile);
    return NULL;
  }
  else {
    // check for number of elements
    if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
      fprintf(stderr, "cannot read number of elements %s\n",infile);
      return NULL;
    }
    else {
      A = (long*)malloc(sizeof(long)*(*pN));
      for (i=0; i<(*pN); i++) {
	if ( fscanf(fp, "%ld", &tmp) != 1 ) {
	  fprintf(stderr, "cannot read value at %d/%d\n",i+1,(*pN));
	  *pN = i;	// read data items
	  return A;
	}
	else A[i] = tmp;
      }
    }
    fclose(fp);
    return A;
  }
}

/////////////////////////////////////////////////////////////
// functions for handling linked lists
/////////////////////////////////////////////////////////////
struct LNode {	// update: item type chagned to long, variable name to key
  long key;
  struct LNode* next;
};

struct LNode* generateLNode(long key)
{
  struct LNode* tmp; //LNode 타입의 값을 저장하는 곳의 주소를 받는 tmp
  tmp = (struct LNode*) malloc(sizeof(struct LNode));
  tmp->key = key;
  tmp->next = NULL;	// bugfix on 190924
  return tmp;
}

struct LNode* insert_head(struct LNode* head, struct LNode* newPtr)
{ //기존 list 주소 , 새로 넣을 node 주소
  //NULL, newPtr
  if ( newPtr == NULL ) return head;	// Nothing to add
  else {
    newPtr->next = head;
    return newPtr;
  }
}

struct LNode* insert_next(struct LNode* prev, struct LNode* newPtr)
{
  //prev, cur
  //struct LNode* cur;	// unnecessary 190924
  if ( newPtr == NULL ) return prev;	// Nothing to add
  else if ( prev == NULL ) return insert_head(NULL, newPtr); //head 삽입은 예외의 경우로 고려
  else {
    //cur = prev->next;	// unnecessary, so removed, on 190924
    //newPtr->next = cur;	// unnecessary, so removed, on 190924
    newPtr->next = prev->next;
    prev->next=newPtr;
    return newPtr;
  }
}

void traverse(struct LNode *L)
{
  struct LNode *cur;
  printf("[");
  for (cur=L ; cur != NULL ; cur = cur->next )
    printf("%ld ",cur->key);
  printf("]\n");
}



void free_list_iterative (struct LNode *L)
{
  struct LNode *tmp;
  while ( L != NULL ) {
    tmp = L;
    L = L->next;	// every node access should be done before free()
    free(tmp);
  }
}

/////////////////////////////////////////////////////////////
// read a linked list from textfile
/////////////////////////////////////////////////////////////
struct LNode *array2linkedlist(long A[], int N)
{
  struct LNode *head, *cur, *prev;
  int i;

  head = NULL; prev = NULL;
  for (i=0; i<N; i++) {
    cur = generateLNode(A[i]); //cur==tmp, 실제 노드의 주소를 가지고 있음.
    prev = insert_next(prev,cur); //cur을 생성해서 삽입한뒤 prev로 업데이트 해준다. 그래야 linked list 구성
    if ( head == NULL ) head = prev;
  }

  return head;
}

struct LNode* readkeys_textfile_linkedlist( const char infile[], int* pN )
  // read text file of integers:
  // number_of_intergers integer1 integer2 ...
  // then convert it to a linked list
  // returns a list list of struct LNode, with its size stored in
  // the memory indicated by integer pointer variable pN
{
  struct LNode* head;
  long* A;

  A = readkeys_textfile(infile, pN);
  if ( A != NULL && *pN > 0 ) {
    head = array2linkedlist(A,*pN);
    free(A);
    return head;
  }
  else return NULL;	// failed
}

void savekeys_textfile_linkedlist( const char outfile[],
    struct LNode *head, int N )
  // save the given linked list to file whose name given by outfile[]
{
  FILE *fp;

  // check for output filename
  if ( outfile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return;
  }

  // check for file existence
  fp = fopen(outfile,"w");
  if ( !fp ) {
    fprintf(stderr, "cannot open file for write %s\n",outfile);
  }
  else {
    // save number of elements
    fprintf(fp, "%d\n",N);
    for (; head; head = head->next ) fprintf(fp, "%ld ", head->key);
    fprintf(fp,"\n");
    fclose(fp);
  }
}


/////////////////////////////////////////////////////////////
// linked list insertion sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */

struct LNode* list_insertion_sort ( struct LNode* head )
{
     // start from an empty list
    struct LNode* newHead = NULL;
    struct LNode *cur, *first, *forward, *tmp, *prev;



    for(first=head; first!=NULL; ){
         int i=0;
         first = first->next;
         newHead = insert_head(newHead, head);
         head = first;
         //새 list에 unsorted의 첫 노드를 집어넣음

         //정렬시작.
         cur = newHead;


         while((cur->next!=NULL) && (cur->key > cur->next->key)){
                forward = cur->next;// 현재 노드의 앞 노드 지정

                //이전 노드를 찾아서 현재 노드 다음 노드에 연결하기
                prev = newHead;
                while(prev!=cur && prev->next!=cur)
                    prev=prev->next;
                prev->next = cur->next;

                // 노드 간 교환
                tmp = forward->next;
                forward->next = cur;
                cur->next = tmp;
                i++;

                //처음 교환시 헤드 고정
                if(i==1) {
                    newHead = forward;
                }


         }
    }
    return newHead; //정렬이 끝난 list의 head를 돌려줘야함
}


/////////////////////////////////////////////////////////////
// linked list merge sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */

struct LNode* list_array_merge_sort( struct LNode** head_array, int N )
{
    //포인터 배열의 시작을 가져옴->head_array, element의 수 ->N
    //&head_array[i] = i번째 노드의 주소 , &head_array[i]->key == 노드의 key(정렬해야할 값)

  /* struct LNode* L=(*head_array);

    while(N>1){ //포인터 배열의 원소의 수가 1이 될때까지 반복

        int mid = (N-1)/2;
        for(int i=0; i<=mid; i){ // 정렬해야할 범위

            if(&head_array[i]->key<&head_array[N-i-1]->key){
                insert_next(&head_array[i], &head_array[N-i-1]);
            } // 앞에 있는 원소의 값보다 뒤에 있는 원소의 값이 큰 경우 앞에다 뒤를 붙힘.
            else{
                insert_next(&head_array[i-1], &head_array[N-i-1]);
            }
        }
        N=(N+1)/2;
    }*/


    return *head_array;//74가 정상적으로 출력력

}
/*while(N>1){ //포인터 배열의 원소의 수가 1이 될때까지 반복

        int mid = (N-1)/2;
        for(int i=0; i<=mid; i){ // 정렬해야할 범위
            if(&head_array[i]->key<&head_array[N-i-1]->key){
                insert_next(&head_array[i], &head_array[N-i-1]);
            } // 앞에 있는 원소의 값보다 뒤에 있는 원소의 값이 큰 경우 앞에다 뒤를 붙힘.
            else{
                insert_next(&head_array[i], &head_array[N-i-1]);
            }
        }
        N=(N+1)/2;
    }


    struct LNode *L1, *L2, *p;
    struct LNode* L=(*head_array);
    int i;

    //배열을 2개로 나누기
    p = L;
    L1 = L;
    for(i=0; i<N/2-1; i++){
        p=p->next;
    }
    L2 = p->next;

    list_array_merge_sort(&L1, N/2);
    list_array_merge_sort(&L2, (int)((N/2.0)+0.5));

    while(L1!=NULL || L2!=NULL){

    }


    */


/////////////////////////////////////////////////////////////

/* File name by Argument input -> scanf read
 * Input file name can be typed in the terminal
 * Update on 9/24: element size is changed to long,
 * to distinguish array index easily
 * in struct Node, item -> key, Node -> LNode
 * Update on 9/25: file reading is moved to a function readkeys_textfile()
 */
int main()
{
  int i, numElements;
  struct LNode *head;

  /* for file name, max length 1023 including path */
  char infile[1024], outfile[1024];
  int method;	// which sorting method

  // required for mergesort
  struct LNode** H; // 포인터의 주소를 가지는 포인터

  /* file name given by keyboard */
  printf("Input file name? ");
  scanf("%s",infile);
  printf("Output file name? ");
  scanf("%s",outfile);

  printf("Method (1: insertion, 2: merge)? ");
  scanf("%d",&method);

  /* read text file of integers:
   * number_of_intergers integer1 integer2 ...
   * then convert it to a linked list */
  head = readkeys_textfile_linkedlist(infile, &numElements);

  if ( head != NULL ) {
    // print all if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
    {
      traverse(head);
      printf("%d Loaded\n", numElements);
    }

    switch ( method ) {
      case 1:	// insertion sort
	head = list_insertion_sort(head);

	break;
      case 2:	// merge sort
	// change linked list to individual list items,
	// and save their pointers to an array of LNode* types
	H = (struct LNode**) malloc(sizeof(struct LNode*)*numElements);

	for (i=0; i<numElements; i++, head=head->next) H[i] = head; //포인터 배열 H

	// have to isolate list items later
	for (i=0; i<numElements; i++) H[i]->next=NULL;

    printf("merge\n");
	head = list_array_merge_sort(H,numElements);
	printf("merge\n");
	break;
      default:
	break;
    }

    // print out results, if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
      traverse(head);

    // save results
    savekeys_textfile_linkedlist(outfile, head, numElements);

    // once allocated, should be freed after use
    free_list_iterative(head);
  }

  return 0;
}


// the below list handling codes are not used.
// so commented out, but may use them
/*
struct LNode *emptylist()
{
  return NULL;
}

struct LNode *delete_head(struct LNode *head)
{
  struct LNode *cur;
  if ( head == NULL ) return NULL;	// empty, nothing to delete
  else {
    cur = head;
    head = head->next;
    cur->next = NULL;	 // not necessary, but just left
    free(cur);
    return head;
  }
}

struct LNode *delete_next(struct LNode *prev)
{
  struct LNode *cur;
  if ( prev == NULL ) return NULL;	// empty from prev, nothing
  else {
    cur = prev->next;
    prev->next = cur->next;
    cur->next = NULL;	// not necessary
    free(cur);
    //cur = prev->next;	// not necessary here
    return prev;
  }
}

struct LNode *lookup_recursive (long x, struct LNode *L)
{
  if (L == NULL) return NULL;
  else if (x == L->key) return L;
  else return lookup_recursive(x, L->next);
}

struct LNode *lookup_iterative (long x, struct LNode *L)
{
  struct LNode *cur;
  for (cur=L ; cur != NULL ; cur = cur->next ) {
    if ( x == cur->key ) return cur;
  }
  return NULL;
}
*/
