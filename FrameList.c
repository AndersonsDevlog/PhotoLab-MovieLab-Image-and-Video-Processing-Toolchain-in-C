/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2026                   */
/* FrameList.c: source file for frame list manipulations             */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "FrameList.h"
#include "Frame.h"

/* Create a new frame list */
FLIST *CreateFrameList(void)
{
    FLIST *list = (FLIST *)malloc(sizeof(FLIST));
    if (!list) {
        return NULL;
    }
    list->Length = 0;
    list->First = NULL;
    list->Last = NULL;
    return list;
}

/* Delete a frame list (and all entries) */
void DeleteFrameList(FLIST *list)
{
    assert(list);
    FENTRY *curr = list->First;
    FENTRY *next;

    while (curr != NULL) {
        next = curr->Next;
        
        if (curr->RGBFrame != NULL) {
            DeleteFrame(curr->RGBFrame);
        }
        if (curr->YUVFrame != NULL) {
            DeleteYUVFrame(curr->YUVFrame);
        }
        
        free(curr);
        curr = next;
    }
    free(list);
}

/* Insert a RGB frame to the frame list at the end */
void AppendRGBFrame(FLIST *list, Frame *RGBframe)
{
    assert(list);
    assert(RGBframe);

    FENTRY *newEntry = (FENTRY *)malloc(sizeof(FENTRY));
    assert(newEntry);

    newEntry->List = list;
    newEntry->RGBFrame = RGBframe;
    newEntry->YUVFrame = NULL;
    newEntry->Next = NULL;
    newEntry->Prev = list->Last;

    if (list->Last == NULL) {
        list->First = newEntry;
    } else {
        list->Last->Next = newEntry;
    }
    
    list->Last = newEntry;
    list->Length++;
}

/* Insert a YUV frame to the frame list at the end */
void AppendYUVFrame(FLIST *list, YUVFrame *YUVframe)
{
    assert(list);
    assert(YUVframe);

    FENTRY *newEntry = (FENTRY *)malloc(sizeof(FENTRY));
    assert(newEntry);

    newEntry->List = list;
    newEntry->YUVFrame = YUVframe;
    newEntry->RGBFrame = NULL; 
    newEntry->Next = NULL;
    newEntry->Prev = list->Last;

    if (list->Last == NULL) {
        list->First = newEntry;
    } else {
        list->Last->Next = newEntry;
    }
    
    list->Last = newEntry;
    list->Length++;
}

/* Crop a frame list: Keep frames from index 'start' to 'end' (inclusive) */
void CropFrameList(FLIST *list, unsigned int start, unsigned int end)
{
    assert(list);
    if (list->Length == 0) return;

    FENTRY *curr = list->First;
    FENTRY *next;
    unsigned int i = 0;

    while (curr != NULL) {
        next = curr->Next;
        
        if (i < start || i > end) {
            /* Unlink node */
            if (curr->Prev) curr->Prev->Next = curr->Next;
            if (curr->Next) curr->Next->Prev = curr->Prev;
            if (curr == list->First) list->First = curr->Next;
            if (curr == list->Last) list->Last = curr->Prev;

            /* Delete data and entry */
            if (curr->RGBFrame) DeleteFrame(curr->RGBFrame);
            if (curr->YUVFrame) DeleteYUVFrame(curr->YUVFrame);
            free(curr);
            list->Length--;
        }
        curr = next;
        i++;
    }
}

/* Fast forward a frame list: Keep every Nth frame (factor) */
void FastFrameList(FLIST *list, unsigned int factor)
{
    assert(list);
    if (factor <= 1) return;

    FENTRY *curr = list->First;
    FENTRY *next;
    unsigned int i = 0;

    while (curr != NULL) {
        next = curr->Next;
        
        if (i % factor != 0) {
            if (curr->Prev) curr->Prev->Next = curr->Next;
            if (curr->Next) curr->Next->Prev = curr->Prev;
            if (curr == list->First) list->First = curr->Next;
            if (curr == list->Last) list->Last = curr->Prev;

            if (curr->RGBFrame) DeleteFrame(curr->RGBFrame);
            if (curr->YUVFrame) DeleteYUVFrame(curr->YUVFrame);
            free(curr);
            list->Length--;
        }
        curr = next;
        i++;
    }
}

/* Reverse a frame list */
void ReverseFrameList(FLIST *list)
{
    assert(list);
    if (list->Length <= 1) return;

    FENTRY *curr = list->First;
    FENTRY *temp = NULL;

    /* Swap Next and Prev for all nodes */
    while (curr != NULL) {
        temp = curr->Prev;
        curr->Prev = curr->Next;
        curr->Next = temp;
        curr = curr->Prev; /* Move to the next node in old sequence */
    }

    /* Swap list head and tail */
    temp = list->First;
    list->First = list->Last;
    list->Last = temp;
}

/* Insert a RGB frame to the frame list in the front */
void PrependRGBFrame(FLIST *list, Frame *RGBframe)
{
    assert(list);
    assert(RGBframe);

    FENTRY *newEntry = (FENTRY *)malloc(sizeof(FENTRY));
    assert(newEntry);

    newEntry->List = list;
    newEntry->RGBFrame = RGBframe;
    newEntry->YUVFrame = NULL;
    newEntry->Prev = NULL;
    newEntry->Next = list->First;

    if (list->First == NULL) {
        /* List is empty */
        list->Last = newEntry;
    } else {
        list->First->Prev = newEntry;
    }
    
    list->First = newEntry;
    list->Length++;
}

/* EOF */
