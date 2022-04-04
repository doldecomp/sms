#pragma once
#include "dolphin.h"
#include "types.h"

class JSUPtrList;

class JSUPtrLink {
    public:
        JSUPtrLink(void *);
        ~JSUPtrLink();
        void *value;      // _0
        JSUPtrList *list; // _4
        JSUPtrLink *prev; // _8
        JSUPtrLink *next; // _C
};

class JSUPtrList {
    public:
        JSUPtrList(bool);
        ~JSUPtrList();
        void initiate();
        bool remove(JSUPtrLink *);
        bool append(JSUPtrLink *);
        bool prepend(JSUPtrLink *);
        bool insert(JSUPtrLink *, JSUPtrLink *);
        inline bool append_(JSUPtrLink *);
        inline bool prepend_(JSUPtrLink *);

        JSUPtrLink *first;       // _0
        JSUPtrLink *last;        // _4
        u32 length;              // _8
};

bool JSUPtrList::append_(JSUPtrLink *node) {
    bool result;

    result = (node->list == NULL);
    if (!result) {
        result = node->list->remove(node);
    }
    if (result) {
        if (this->length == 0) {
            node->list = this;
            node->prev = NULL;
            node->next = NULL;
            this->last = node;
            this->first = node;
            this->length = 1;
        } else {
            node->list = this;
            node->prev = this->last;
            node->next = NULL;
            this->last->next = node;
            this->last = node;
            this->length = this->length + 1;
        }
    }

    return result;
}

bool JSUPtrList::prepend_(JSUPtrLink *node) {
    bool result;

    result = (node->list == NULL);
    if (!result) {
        result = node->list->remove(node);
    }
    if (result) {
        if (this->length == 0) {
            node->list = this;
            node->prev = NULL;
            node->next = NULL;
            this->last = node;
            this->first = node;
            this->length = 1;
        } else {
            node->list = this;
            node->prev = NULL;
            node->next = this->first;
            this->first->prev = node;
            this->first = node;
            this->length = this->length + 1;
        }
    }

    return result;
}
