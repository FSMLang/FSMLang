package io.github.fsmlang.test

// ── full_test1 (testFSM) ───────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test1.testFSM as FT1FSM
import io.github.fsmlang.generated.full_test1.testFSMEvent as FT1Event
import io.github.fsmlang.generated.full_test1.testFSMEventStruct as FT1EventStruct

// ── full_test2 (test4FSM) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test2.test4FSM as FT2FSM
import io.github.fsmlang.generated.full_test2.test4FSMEvent as FT2Event
import io.github.fsmlang.generated.full_test2.test4FSMEventStruct as FT2EventStruct

// ── full_test3 (newMachine) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test3.newMachine as FT3FSM
import io.github.fsmlang.generated.full_test3.newMachineEvent as FT3Event
import io.github.fsmlang.generated.full_test3.newMachineEventStruct as FT3EventStruct

// ── full_test5 (newMachine) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test5.newMachine as FT5FSM
import io.github.fsmlang.generated.full_test5.newMachineEvent as FT5Event
import io.github.fsmlang.generated.full_test5.newMachineEventStruct as FT5EventStruct

// ── full_test6 (newMachine) [covers: 7] ───────────────────────────────────
import io.github.fsmlang.generated.full_test6.newMachine as FT6FSM
import io.github.fsmlang.generated.full_test6.newMachineEvent as FT6Event
import io.github.fsmlang.generated.full_test6.newMachineEventStruct as FT6EventStruct

// ── full_test8 (newMachine) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test8.newMachine as FT8FSM
import io.github.fsmlang.generated.full_test8.newMachineEvent as FT8Event
import io.github.fsmlang.generated.full_test8.newMachineEventStruct as FT8EventStruct

// ── full_test9 (newMachine) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test9.newMachine as FT9FSM
import io.github.fsmlang.generated.full_test9.newMachineEvent as FT9Event
import io.github.fsmlang.generated.full_test9.newMachineEventStruct as FT9EventStruct

// ── full_test10 (newMachine) [covers: 11] ─────────────────────────────────
import io.github.fsmlang.generated.full_test10.newMachine as FT10FSM
import io.github.fsmlang.generated.full_test10.newMachineEvent as FT10Event
import io.github.fsmlang.generated.full_test10.newMachineEventStruct as FT10EventStruct

// ── full_test13 (newMachine) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test13.newMachine as FT13FSM
import io.github.fsmlang.generated.full_test13.newMachineEvent as FT13Event
import io.github.fsmlang.generated.full_test13.newMachineEventStruct as FT13EventStruct

// ── full_test14 (newMachine) [covers: 15] ─────────────────────────────────
import io.github.fsmlang.generated.full_test14.newMachine as FT14FSM
import io.github.fsmlang.generated.full_test14.newMachineEvent as FT14Event
import io.github.fsmlang.generated.full_test14.newMachineEventStruct as FT14EventStruct

// ── full_test16 (newMachine) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test16.newMachine as FT16FSM
import io.github.fsmlang.generated.full_test16.newMachineEvent as FT16Event
import io.github.fsmlang.generated.full_test16.newMachineEventStruct as FT16EventStruct

// ── full_test17 (FlashDriver) ──────────────────────────────────────────────
import io.github.fsmlang.generated.full_test17.FlashDriver as FT17FSM
import io.github.fsmlang.generated.full_test17.FlashDriverEvent as FT17Event
import io.github.fsmlang.generated.full_test17.FlashDriverEventStruct as FT17EventStruct

// ── full_test74 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test74.test_fsm as FT74FSM
import io.github.fsmlang.generated.full_test74.test_fsmEvent as FT74Event
import io.github.fsmlang.generated.full_test74.test_fsmEventStruct as FT74EventStruct

// ── full_test75 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test75.test_fsm as FT75FSM
import io.github.fsmlang.generated.full_test75.test_fsmEvent as FT75Event
import io.github.fsmlang.generated.full_test75.test_fsmEventStruct as FT75EventStruct

// ── full_test81 (messenger) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test81.messenger as FT81FSM
import io.github.fsmlang.generated.full_test81.messengerEvent as FT81Event
import io.github.fsmlang.generated.full_test81.messengerEventStruct as FT81EventStruct

// ── full_test82 (messenger) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test82.messenger as FT82FSM
import io.github.fsmlang.generated.full_test82.messengerEvent as FT82Event
import io.github.fsmlang.generated.full_test82.messengerEventStruct as FT82EventStruct

// ── full_test88 (test) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test88.test as FT88FSM
import io.github.fsmlang.generated.full_test88.testEvent as FT88Event
import io.github.fsmlang.generated.full_test88.testEventStruct as FT88EventStruct

// ── full_test99 (test_empty_cells) ────────────────────────────────────────
import io.github.fsmlang.generated.full_test99.test_empty_cells as FT99FSM
import io.github.fsmlang.generated.full_test99.test_empty_cellsEvent as FT99Event
import io.github.fsmlang.generated.full_test99.test_empty_cellsEventStruct as FT99EventStruct

// ── full_test109 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test109.test_fsm as FT109FSM
import io.github.fsmlang.generated.full_test109.test_fsmEvent as FT109Event
import io.github.fsmlang.generated.full_test109.test_fsmEventStruct as FT109EventStruct

// ── full_test111 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test111.test_fsm as FT111FSM
import io.github.fsmlang.generated.full_test111.test_fsmEvent as FT111Event
import io.github.fsmlang.generated.full_test111.test_fsmEventStruct as FT111EventStruct

// ── full_test112 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test112.test_fsm as FT112FSM
import io.github.fsmlang.generated.full_test112.test_fsmEvent as FT112Event
import io.github.fsmlang.generated.full_test112.test_fsmEventStruct as FT112EventStruct

// ── full_test113 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test113.test_fsm as FT113FSM
import io.github.fsmlang.generated.full_test113.test_fsmEvent as FT113Event
import io.github.fsmlang.generated.full_test113.test_fsmEventStruct as FT113EventStruct

// ── full_test135 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test135.test_fsm as FT135FSM
import io.github.fsmlang.generated.full_test135.test_fsmEvent as FT135Event
import io.github.fsmlang.generated.full_test135.test_fsmEventStruct as FT135EventStruct

// ── full_test136 (m1) ─────────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test136.m1 as FT136FSM
import io.github.fsmlang.generated.full_test136.m1Event as FT136Event
import io.github.fsmlang.generated.full_test136.m1EventStruct as FT136EventStruct

// ── full_test137 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test137.test_fsm as FT137FSM
import io.github.fsmlang.generated.full_test137.test_fsmEvent as FT137Event
import io.github.fsmlang.generated.full_test137.test_fsmEventStruct as FT137EventStruct

// ── full_test138 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test138.test_fsm as FT138FSM
import io.github.fsmlang.generated.full_test138.test_fsmEvent as FT138Event
import io.github.fsmlang.generated.full_test138.test_fsmEventStruct as FT138EventStruct

// ── full_test139 (FSMIssue) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test139.FSMIssue as FT139FSM
import io.github.fsmlang.generated.full_test139.FSMIssueEvent as FT139Event
import io.github.fsmlang.generated.full_test139.FSMIssueEventStruct as FT139EventStruct

// ── full_test142 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test142.test_fsm as FT142FSM
import io.github.fsmlang.generated.full_test142.test_fsmEvent as FT142Event
import io.github.fsmlang.generated.full_test142.test_fsmEventStruct as FT142EventStruct

// ── full_test143 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test143.test_fsm as FT143FSM
import io.github.fsmlang.generated.full_test143.test_fsmEvent as FT143Event
import io.github.fsmlang.generated.full_test143.test_fsmEventStruct as FT143EventStruct

import org.junit.jupiter.api.Test

/**
 * Smoke tests for Kotlin (-tk) output across all eligible full_test directories.
 *
 * Each test constructs the FSM with default hooks and dispatches one event to
 * verify that the generated code compiles and basic dispatch does not throw.
 *
 * Full behavioral parity with C tests is tracked in tests_skipped.md.
 * full_test144 is excluded here — it has full behavioral assertions in
 * TestFsmSmokeTest.kt.
 */
class AllFsmSmokeTests {

    @Test fun `full_test1 smoke`() {
        val fsm = FT1FSM()
        fsm.dispatch(FT1EventStruct(FT1Event.goodMessage))
    }

    @Test fun `full_test2 smoke`() {
        val fsm = FT2FSM()
        fsm.dispatch(FT2EventStruct(FT2Event.goodMessage))
    }

    @Test fun `full_test3 smoke`() {
        val fsm = FT3FSM()
        fsm.dispatch(FT3EventStruct(FT3Event.e1))
    }

    @Test fun `full_test5 smoke`() {
        val fsm = FT5FSM()
        fsm.dispatch(FT5EventStruct(FT5Event.e1))
    }

    @Test fun `full_test6 smoke`() {
        val fsm = FT6FSM()
        fsm.dispatch(FT6EventStruct(FT6Event.e1))
    }

    @Test fun `full_test8 smoke`() {
        val fsm = FT8FSM()
        fsm.dispatch(FT8EventStruct(FT8Event.e1))
    }

    @Test fun `full_test9 smoke`() {
        val fsm = FT9FSM()
        fsm.dispatch(FT9EventStruct(FT9Event.e1))
    }

    @Test fun `full_test10 smoke`() {
        val fsm = FT10FSM()
        fsm.dispatch(FT10EventStruct(FT10Event.e1))
    }

    @Test fun `full_test13 smoke`() {
        val fsm = FT13FSM()
        fsm.dispatch(FT13EventStruct(FT13Event.e1))
    }

    @Test fun `full_test14 smoke`() {
        val fsm = FT14FSM()
        fsm.dispatch(FT14EventStruct(FT14Event.e1))
    }

    @Test fun `full_test16 smoke`() {
        val fsm = FT16FSM()
        fsm.dispatch(FT16EventStruct(FT16Event.e1))
    }

    @Test fun `full_test17 smoke`() {
        val fsm = FT17FSM()
        fsm.dispatch(FT17EventStruct(FT17Event.write))
    }

    @Test fun `full_test74 smoke`() {
        val fsm = FT74FSM()
        fsm.dispatch(FT74EventStruct(FT74Event.e1))
    }

    @Test fun `full_test75 smoke`() {
        val fsm = FT75FSM()
        fsm.dispatch(FT75EventStruct(FT75Event.e1))
    }

    @Test fun `full_test81 smoke`() {
        val fsm = FT81FSM()
        fsm.dispatch(FT81EventStruct(FT81Event.goodMessage))
    }

    @Test fun `full_test82 smoke`() {
        val fsm = FT82FSM()
        fsm.dispatch(FT82EventStruct(FT82Event.goodMessage))
    }

    @Test fun `full_test88 smoke`() {
        val fsm = FT88FSM()
        fsm.dispatch(FT88EventStruct(FT88Event.e1))
    }

    @Test fun `full_test99 smoke`() {
        val fsm = FT99FSM()
        fsm.dispatch(FT99EventStruct(FT99Event.e1))
    }

    @Test fun `full_test109 smoke`() {
        val fsm = FT109FSM()
        fsm.dispatch(FT109EventStruct(FT109Event.e1))
    }

    @Test fun `full_test111 smoke`() {
        val fsm = FT111FSM()
        fsm.dispatch(FT111EventStruct(FT111Event.e1))
    }

    @Test fun `full_test112 smoke`() {
        val fsm = FT112FSM()
        fsm.dispatch(FT112EventStruct(FT112Event.e1))
    }

    @Test fun `full_test113 smoke`() {
        val fsm = FT113FSM()
        fsm.dispatch(FT113EventStruct(FT113Event.e1))
    }

    @Test fun `full_test135 smoke`() {
        val fsm = FT135FSM()
        fsm.dispatch(FT135EventStruct(FT135Event.e1))
    }

    @Test fun `full_test136 smoke`() {
        val fsm = FT136FSM()
        fsm.dispatch(FT136EventStruct(FT136Event.e1))
    }

    @Test fun `full_test137 smoke`() {
        val fsm = FT137FSM()
        fsm.dispatch(FT137EventStruct(FT137Event.e1))
    }

    @Test fun `full_test138 smoke`() {
        val fsm = FT138FSM()
        fsm.dispatch(FT138EventStruct(FT138Event.e1))
    }

    @Test fun `full_test139 smoke`() {
        val fsm = FT139FSM()
        fsm.dispatch(FT139EventStruct(FT139Event.e1))
    }

    @Test fun `full_test142 smoke`() {
        val fsm = FT142FSM()
        fsm.dispatch(FT142EventStruct(FT142Event.e1))
    }

    @Test fun `full_test143 smoke`() {
        val fsm = FT143FSM()
        fsm.dispatch(FT143EventStruct(FT143Event.e1))
    }
}
