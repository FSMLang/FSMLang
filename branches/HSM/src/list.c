/*
  list.c

   implementation of the list module.  The list module provided general purpose list management.

    FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2002  Steven Stanton
    Copyright (C) 7/5/2022  Steven Stanton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Steven Stanton
    sstantin@pesticidesoftware.com

    For the latest on FSMLang: http://fsmlang.pesticidesoftware.com

    And, finally, your possession of this source code implies nothing.

*/

#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef struct _counter_str_ COUNTER_STR, *pCOUNTER_STR;
struct _counter_str_
{
   unsigned target;
   unsigned current;
};

static bool nth_record(pLIST_ELEMENT pmbr, void *data)
{
   pCOUNTER_STR pcs = (pCOUNTER_STR) data;

   /* returning true means we are done */
   return (pcs->current++ == pcs->target);
}

static bool match_member(pLIST_ELEMENT pmbr, void *data)
{
   /* returning true means we are done */
   return (pmbr->mbr == data);
}

pLIST init_list()
{
   return (pLIST)calloc(1,sizeof(LIST));
}

/**********************************************************************************************************************/
/**
 * @brief Iterate the given list, calling the action function on each record found.  Stop when the action function return true.
 * 
 * @author Steven Stanton (7/4/2022)
 * 
 * @param plist The list to iterate; NULL is not an error
 * @param action The action to perform on each list member; when this function returns true, the iteration will cease.
 * @param data Data to be passed to the action function.
 * 
 * @return a pointer to the record which caused processing to cease, or NULL.
 *
 * @ref_global None
 *
 * @mod_global None
 *
 * @thread_safe Yes
 *
 * 
 ***********************************************************************************************************************/
pLIST_ELEMENT iterate_list(pLIST plist, LIST_ITERATOR_FN action, void *data)
{
   pLIST_ELEMENT pfound = NULL;

   if (plist && action)
   {
      for (pLIST_ELEMENT mbr = plist->head;  mbr; mbr = mbr->next)
      {
         if ((*action)(mbr,data))
         {
            pfound = mbr;
            break;
         }
      }
   }

   return pfound;
}

/**********************************************************************************************************************/
/**
 * @brief move the src list to the dest list.
 * 
 * @author Steven Stanton (7/3/2022)
 * 
 * @param dest destination list
 * @param src source list
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 * 
 * @return pLIST pointer to the destination list, which will now contain the members of the source list.
 *
 * Expect src and dest to point to LIST structures having list integrity.
 * 
 * Move the members of the source list to the end of the destination list, returning a pointer to the destination list.
 *   The source list will be empty at the end of the operation.
 ***********************************************************************************************************************/
pLIST move_list(pLIST dest, pLIST src)
{
   if (dest)
   {
      /* this means dest */
      /* Leave all of the members where they are; simply update the list management record. */
      if (dest->tail)
      {
         /* this means the dest list is not empty */
         dest->tail->next  = src->head;
      }
      else
      {
         /* this means the dest list was empty */
         dest->head        = src->head;
      }
      dest->tail        = src->tail;
      dest->count       += src->count;

      /* Set up source for re-use. */
      memset(src,0,sizeof(LIST));

   }
   else
   {
      /* no destination yet, so return source */
      dest = src; 
   }

   return dest;
}

static bool unique_list_mover(pLIST_ELEMENT pelem, void *data)
{
   pLIST pdest = (pLIST) data;

   add_unique_to_list(pdest, pelem->mbr);

   return false;
}

/**********************************************************************************************************************/
/**
 * @brief move src to dest, without moving any member already in dest.
 * 
 * @author Steven Stanton (7/6/2022)
 * 
 * @param dest destination list
 * @param src source list
 * 
 *
 * @ref_global None
 *
 * @mod_global None
 *
 * @thread_safe yes
 * 
 * @return pLIST pointer to the destination list
 *
 ***********************************************************************************************************************/
pLIST move_list_unique(pLIST dest, pLIST src)
{
   if (dest)
   {
      iterate_list(src,unique_list_mover,dest);
   }
   else
   {
      dest = src;
   }
   return dest;
}

/**********************************************************************************************************************/
/**
 * @brief Free any list members, then free the list record.  All memory must have been malloced.
 * 
 * @author Steven Stanton (7/4/2022)
 * 
 * @param plist The list to be freed.
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 *
 * It is not an error to pass a null pointer.
 ***********************************************************************************************************************/
void free_list(pLIST plist)
{
   pLIST_ELEMENT pnext;

   if (plist)
   {
      {
         /* walk through the list, freeing as we go. */
         while (plist->head)
         {
            pnext = plist->head->next;
            free(plist->head);
            plist->head = pnext;
         }
      }
      free(plist);
   }

}

pLIST_ELEMENT add_to_list(pLIST plist, void *pmbr)
{
   pLIST_ELEMENT pelem = NULL;

   if (plist)
   {
      if (NULL != (pelem = (pLIST_ELEMENT) calloc(1, sizeof(LIST_ELEMENT))))
      {
         pelem->mbr = pmbr;

         if (NULL == plist->head)
         {
            plist->head  = plist->tail = pelem;
            plist->count = 1;
         }
         else
         {
            plist->tail->next = pelem;
            plist->tail       = pelem;
            plist->count++;
         }
      }
   }

   return pelem;
}

/**********************************************************************************************************************/
/**
 * @brief return a pointer to the nth member in the list
 * 
 * @author Steven Stanton (7/4/2022)
 * 
 * @param list list to search
 * @param target 0 based record number to find
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 * 
 * @return pointer to the member found, or NULL when the list is either NULL or empty, or when the desired record number exceeds the count of records.
 *
 * 
 ***********************************************************************************************************************/
void *find_nth_list_member(pLIST list, unsigned target)
{
   pLIST_ELEMENT pelem;
   COUNTER_STR finder;

   finder.current = 0;
   finder.target  = target;

   pelem = iterate_list(list,nth_record,&finder);

   return pelem ? pelem->mbr : NULL;
}

pLIST_ELEMENT add_unique_to_list(pLIST plist, void *pmbr)
{
   pLIST_ELEMENT pelem;

   if (NULL == (pelem = iterate_list(plist,match_member,pmbr)))
   {
      pelem = add_to_list(plist, pmbr);
   }

   return pelem;
}

