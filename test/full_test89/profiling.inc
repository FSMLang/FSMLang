#include <stdio.h>
#ifdef FSM_VARIANT_C
#include <string.h>
#endif

#if !defined(SUB_MACHINE)
#define FSM_ENTRY(...) fsm_entry()
#define FSM_EXIT(...)  fsm_exit()

static void fsm_entry(void);
static void fsm_exit(void);

static void fsm_entry(void)
{
	printf("%s\n", __func__);
}

static void fsm_exit(void)
{
	printf("%s\n", __func__);
}

#endif

#define ACTION_ENTRY(A) action_entry((A))
#define ACTION_EXIT(A)  action_exit((A))

static void action_entry(FSM_TYPE_PTR);
static void action_exit(FSM_TYPE_PTR);

static void action_entry(FSM_TYPE_PTR pfsm)
{
#ifdef FSM_VARIANT_C
	if ((*pfsm->actionArray)[pfsm->event][pfsm->state].action != UFMN(noAction))
	{
		printf("%s: e=%u, s=%u\n"
	          , __func__
	          , pfsm->event
	          , pfsm->state
	         );
	}
#else
	printf("%s: e=%u, s=%u\n"
          , __func__
          , pfsm->event
          , pfsm->state
         );
#endif
}

static void action_exit(FSM_TYPE_PTR pfsm)
{
#ifdef FSM_VARIANT_C
	if ((*pfsm->actionArray)[pfsm->event][pfsm->state].action != UFMN(noAction))
	{
		printf("%s: e=%u, s=%u\n"
	          , __func__
	          , pfsm->event
	          , pfsm->state
			);
	}
#else
	printf("%s: e=%u, s=%u\n"
          , __func__
          , pfsm->event
          , pfsm->state
         );
#endif
}
