package io.github.fsmlang.test

import io.github.fsmlang.generated.test_fsm.test_fsm
import io.github.fsmlang.generated.test_fsm.test_fsmEvent
import io.github.fsmlang.generated.test_fsm.test_fsmEventData
import io.github.fsmlang.generated.test_fsm.test_fsmEventStruct
import io.github.fsmlang.generated.test_fsm.test_fsmState
import io.github.fsmlang.generated.test_fsm.test_fsmUserHooks
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Test

/**
 * Smoke test for the Kotlin FSM generated from test/full_test144/test_fsm.fsm.
 *
 * Mirrors the single-instance drive sequence in test/full_test144/test.c and
 * the user hooks in test/full_test144/test_fsm-actions.c.
 */
class TestFsmSmokeTest {

    /**
     * Concrete hook implementation that mirrors test_fsm-actions.c.
     * e1_count accumulates the `i` field from every e1 payload.
     */
    private inner class TestHooks : test_fsmUserHooks() {
        var e1_count: Int = 0

        /** Mirror of test_fsm_grab_e1_data: accumulate payload.i */
        override fun grab_e1_data(
            fsm: test_fsm,
            payload: test_fsmEventData.E1Payload
        ): test_fsmEventData.E1Payload {
            e1_count += payload.i
            return payload
        }

        override fun act_on_e1(fsm: test_fsm): test_fsmEvent = test_fsmEvent.noEvent
        override fun act_on_e2(fsm: test_fsm): test_fsmEvent = test_fsmEvent.noEvent
        override fun act_on_e3(fsm: test_fsm): test_fsmEvent = test_fsmEvent.noEvent

        /** guard1 always returns s1 (mirrors C implementation) */
        override fun guard1(fsm: test_fsm): test_fsmState = test_fsmState.s1

        override fun enter_s1(fsm: test_fsm) {}
        override fun leave_s2(fsm: test_fsm) {}
    }

    /**
     * Port of the single-instance drive sequence from test/full_test144/test.c:
     *   - 4× e1 with i=1
     *   - e2
     *   - e3
     * Expected: e1_count == 4
     */
    @Test
    fun `drive sequence accumulates e1_count to 4`() {
        val hooks = TestHooks()
        val pfsm = test_fsm(hooks)

        repeat(4) {
            pfsm.dispatch(
                test_fsmEventStruct(
                    test_fsmEvent.e1,
                    test_fsmEventData.E1Payload(i = 1)
                )
            )
        }

        pfsm.dispatch(test_fsmEventStruct(test_fsmEvent.e2))
        pfsm.dispatch(test_fsmEventStruct(test_fsmEvent.e3))

        assertEquals(4, hooks.e1_count)
    }

    /**
     * SPEC.md "zero-init fallback": dispatching e1 without a payload must invoke
     * the translator with a default/zero payload (i = 0), leaving e1_count unchanged.
     */
    @Test
    fun `e1 without payload uses zero-init fallback and e1_count stays zero`() {
        val hooks = TestHooks()
        val pfsm = test_fsm(hooks)

        pfsm.dispatch(test_fsmEventStruct(test_fsmEvent.e1))

        assertEquals(0, hooks.e1_count)
    }
}
