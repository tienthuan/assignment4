#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "regions.h"
#include "region_struct.h"
struct REGION
{
  rsize_t region_memory;
  Region *next;
  const char *name;
  Region *next_sub_region;
  Boolean allocated;
};

rsize_t roundUp(rsize_t numToRound)
{
    return (numToRound + 8 - 1) & -8;
}
//insert new region at bottom
Boolean insert(Region * region){
    Region *curr = top;
    while(curr->next != NULL){
        if(strcmp(region->name,curr->name) == 0){
            return FALSE;
        }
        curr = curr->next;
    }
    curr->next = region;
    return TRUE;
}
//make new region
Region * makeRegion(const char *region_name, rsize_t region_size){
    rsize_t memforRegion = roundUp(region_size);
    Region *region = malloc(memforRegion);
    region->name = region_name;
    region->region_memory = memforRegion;
    region->next = NULL;
    region->next_sub_region = NULL;
    region->allocated = FALSE;
    return region;
}
//initialize region
Boolean rinit(const char *region_name, rsize_t region_size){
   Region *region = makeRegion(region_name,region_size);
   Region *curr = top;
   if(NULL == top){
        top = region;
        chosen = region;
        return TRUE;
    }
    else{
        chosen = region;
        return insert(region);
    }
}
//choose region based on name
Boolean rchoose(const char *region_name){
    Region *curr = top;
    while(NULL != curr){
        if(strcmp(curr->name,region_name) == 0){
            chosen = curr;
            return TRUE;
        }
        curr = curr->next;
    }
    return FALSE;
}
//return name of current chosen block;
const char *rchosen(){
    if(NULL == chosen){
        return NULL;
    }
    else return chosen->name;
}
//from 1 big block make small subblocks in the chain
Region *makeBlock(Region *orig,rsize_t size){
 Region *new_region=(void*)((void*)orig+ size);
 new_region->region_memory=(orig->region_memory)-size;
 new_region->allocated=FALSE;
 new_region->next_sub_region=orig->next_sub_region;
 orig->region_memory=size;
 orig->allocated=TRUE;
 orig->next_sub_region=new_region;
}

//reallocate mem for first fit of the Node
void *ralloc(rsize_t block_size){
    rsize_t rounded_block_size = roundUp(block_size);
    assert(block_size > 0);//cannot be <=0
    void* output = NULL;
    Region *curr = chosen;
    if(NULL == chosen){
        return NULL;
    }
    while((NULL != (curr->next_sub_region)) && (((curr->allocated) == 1) || (curr->region_memory <= rounded_block_size)) ){
        curr = curr->next_sub_region;
    }
    printf("attempting to insert %hu into %hu\n",rounded_block_size, curr->region_memory);
    if(curr->region_memory == rounded_block_size){
        curr->allocated=1;
        output=curr++;
        return output;
    }
    else if(curr->region_memory>rounded_block_size+sizeof(Region)){
        makeBlock(curr,rounded_block_size);
        output=curr++;
        return output;
    }
    else{
        printf("returning NULL\n");// I AM RETURNING NULL BUT VSCODE DOES SOME WACKY STUFF AND NOW IDK
        output = NULL;
        return 0;
    }
}

//size of the block pointed to
rsize_t rsize(void *block_ptr){
   Region *curr = block_ptr;
   return curr->region_memory;
}
//set the free value of the current block to true
Boolean rfree(void *block_ptr){
    if(((void*)chosen<=block_ptr)&&(block_ptr<=(void*)(chosen + chosen->region_memory))){
        Region *sub_region = block_ptr;
        sub_region--;
        sub_region->allocated = 0;

        Region *curr,*prev;
        curr = chosen;
        //merging freeblocks into 1 big block
        while((curr->next)!=NULL){
            if((!curr->allocated) && (curr->next->allocated)){
                curr->region_memory += (curr->next->region_memory);
                curr->next=curr->next->next;
        }
                prev=curr;
                curr=curr->next_sub_region;
        }
        return TRUE;
    }

    return FALSE;
}
//destroy the region, removing it
void rdestroy(const char *region_name){
    Region *prev,*curr;
    curr = top;
    //if top Region is the one to be deleted
    if((curr!= NULL) && (strcmp(curr->name,region_name) == 0)){
        top = curr->next;
        chosen = curr->next;
        free(curr);
        curr = NULL;
        return;
    }
    while(NULL != curr->next){
        if(strcmp(curr->name,region_name) != 0){
            prev->next = curr->next;
            curr->next = NULL;
            free(curr);
            curr = NULL;
            return;
        }
        prev = curr;
        curr = curr -> next;
    }
}

void rdump(){
    Region *curr = top;
    while(NULL != curr){
        printf("region name: %s\n",curr->name);
        printf("region memory: %hu\n",curr->name);
        printf("is allocated: %d\n",curr->allocated);
        Region *sub_region = curr->next_sub_region;
        printf("sub_regions::\n");
        while(NULL != sub_region){
            printf("block position: %p\n",&sub_region);
            printf("block memory: %hu\n",sub_region->region_memory);
            printf("is allocated: %d\n",sub_region->allocated);
            sub_region = sub_region->next_sub_region;
            printf("....................\n");
        }
        curr = curr->next;
    printf("////////////////////////////\n");
    }
    
}

