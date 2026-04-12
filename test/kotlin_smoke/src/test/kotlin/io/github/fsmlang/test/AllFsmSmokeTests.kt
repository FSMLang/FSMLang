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

// ── full_test18 (newMachine) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test18.newMachine as FT18FSM
import io.github.fsmlang.generated.full_test18.newMachineEvent as FT18Event
import io.github.fsmlang.generated.full_test18.newMachineEventStruct as FT18EventStruct

// ── full_test19 (top_level) [covers: 20] ──────────────────────────────────
import io.github.fsmlang.generated.full_test19.top_level as FT19FSM
import io.github.fsmlang.generated.full_test19.top_levelEvent as FT19Event
import io.github.fsmlang.generated.full_test19.top_levelEventStruct as FT19EventStruct

// ── full_test21 (top_level) [covers: 22, 23, 24, 25, 86] ─────────────────
import io.github.fsmlang.generated.full_test21.top_level as FT21FSM
import io.github.fsmlang.generated.full_test21.top_levelEvent as FT21Event
import io.github.fsmlang.generated.full_test21.top_levelEventStruct as FT21EventStruct

// ── full_test26 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test26.top_level as FT26FSM
import io.github.fsmlang.generated.full_test26.top_levelEvent as FT26Event
import io.github.fsmlang.generated.full_test26.top_levelEventStruct as FT26EventStruct

// ── full_test27 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test27.top_level as FT27FSM
import io.github.fsmlang.generated.full_test27.top_levelEvent as FT27Event
import io.github.fsmlang.generated.full_test27.top_levelEventStruct as FT27EventStruct

// ── full_test28 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test28.top_level as FT28FSM
import io.github.fsmlang.generated.full_test28.top_levelEvent as FT28Event
import io.github.fsmlang.generated.full_test28.top_levelEventStruct as FT28EventStruct

// ── full_test29 (top_level) [covers: 102, 103] ────────────────────────────
import io.github.fsmlang.generated.full_test29.top_level as FT29FSM
import io.github.fsmlang.generated.full_test29.top_levelEvent as FT29Event
import io.github.fsmlang.generated.full_test29.top_levelEventStruct as FT29EventStruct

// ── full_test30 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test30.top_level as FT30FSM
import io.github.fsmlang.generated.full_test30.top_levelEvent as FT30Event
import io.github.fsmlang.generated.full_test30.top_levelEventStruct as FT30EventStruct

// ── full_test31 (top_level) [covers: 33] ──────────────────────────────────
import io.github.fsmlang.generated.full_test31.top_level as FT31FSM
import io.github.fsmlang.generated.full_test31.top_levelEvent as FT31Event
import io.github.fsmlang.generated.full_test31.top_levelEventStruct as FT31EventStruct

// ── full_test32 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test32.top_level as FT32FSM
import io.github.fsmlang.generated.full_test32.top_levelEvent as FT32Event
import io.github.fsmlang.generated.full_test32.top_levelEventStruct as FT32EventStruct

// ── full_test34 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test34.top_level as FT34FSM
import io.github.fsmlang.generated.full_test34.top_levelEvent as FT34Event
import io.github.fsmlang.generated.full_test34.top_levelEventStruct as FT34EventStruct

// ── full_test35 (top_level) [covers: 36] ──────────────────────────────────
import io.github.fsmlang.generated.full_test35.top_level as FT35FSM
import io.github.fsmlang.generated.full_test35.top_levelEvent as FT35Event
import io.github.fsmlang.generated.full_test35.top_levelEventStruct as FT35EventStruct

// ── full_test37 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test37.top_level as FT37FSM
import io.github.fsmlang.generated.full_test37.top_levelEvent as FT37Event
import io.github.fsmlang.generated.full_test37.top_levelEventStruct as FT37EventStruct

// ── full_test38 (top_level) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test38.top_level as FT38FSM
import io.github.fsmlang.generated.full_test38.top_levelEvent as FT38Event
import io.github.fsmlang.generated.full_test38.top_levelEventStruct as FT38EventStruct

// ── full_test45 (newMachine) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test45.newMachine as FT45FSM
import io.github.fsmlang.generated.full_test45.newMachineEvent as FT45Event
import io.github.fsmlang.generated.full_test45.newMachineEventStruct as FT45EventStruct

// ── full_test46 (newMachine) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test46.newMachine as FT46FSM
import io.github.fsmlang.generated.full_test46.newMachineEvent as FT46Event
import io.github.fsmlang.generated.full_test46.newMachineEventStruct as FT46EventStruct

// ── full_test47 (newMachine) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test47.newMachine as FT47FSM
import io.github.fsmlang.generated.full_test47.newMachineEvent as FT47Event
import io.github.fsmlang.generated.full_test47.newMachineEventStruct as FT47EventStruct

// ── full_test48 (newMachine) [covers: 49, 55] ─────────────────────────────
import io.github.fsmlang.generated.full_test48.newMachine as FT48FSM
import io.github.fsmlang.generated.full_test48.newMachineEvent as FT48Event
import io.github.fsmlang.generated.full_test48.newMachineEventStruct as FT48EventStruct

// ── full_test69 (anotherNewMachine + newMachine) [covers: 84, 85] ─────────
import io.github.fsmlang.generated.full_test69.anotherNewMachine as FT69AnotherNewMachine
import io.github.fsmlang.generated.full_test69.anotherNewMachineEvent as FT69AnotherNewMachineEvent
import io.github.fsmlang.generated.full_test69.anotherNewMachineEventStruct as FT69AnotherNewMachineEventStruct
import io.github.fsmlang.generated.full_test69.newMachine as FT69NewMachine
import io.github.fsmlang.generated.full_test69.newMachineEvent as FT69NewMachineEvent
import io.github.fsmlang.generated.full_test69.newMachineEventStruct as FT69NewMachineEventStruct

// ── full_test70 (newMachine) [covers: 72, 87] ─────────────────────────────
import io.github.fsmlang.generated.full_test70.newMachine as FT70FSM
import io.github.fsmlang.generated.full_test70.newMachineEvent as FT70Event
import io.github.fsmlang.generated.full_test70.newMachineEventStruct as FT70EventStruct

// ── full_test73 (test) ─────────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test73.test as FT73FSM
import io.github.fsmlang.generated.full_test73.testEvent as FT73Event
import io.github.fsmlang.generated.full_test73.testEventStruct as FT73EventStruct

// ── full_test74 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test74.test_fsm as FT74FSM
import io.github.fsmlang.generated.full_test74.test_fsmEvent as FT74Event
import io.github.fsmlang.generated.full_test74.test_fsmEventStruct as FT74EventStruct

// ── full_test75 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test75.test_fsm as FT75FSM
import io.github.fsmlang.generated.full_test75.test_fsmEvent as FT75Event
import io.github.fsmlang.generated.full_test75.test_fsmEventStruct as FT75EventStruct

// ── full_test76 (test_fsm) [covers: 110] ──────────────────────────────────
import io.github.fsmlang.generated.full_test76.test_fsm as FT76FSM
import io.github.fsmlang.generated.full_test76.test_fsmEvent as FT76Event
import io.github.fsmlang.generated.full_test76.test_fsmEventStruct as FT76EventStruct

// ── full_test77 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test77.test_fsm as FT77FSM
import io.github.fsmlang.generated.full_test77.test_fsmEvent as FT77Event
import io.github.fsmlang.generated.full_test77.test_fsmEventStruct as FT77EventStruct

// ── full_test78 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test78.test_fsm as FT78FSM
import io.github.fsmlang.generated.full_test78.test_fsmEvent as FT78Event
import io.github.fsmlang.generated.full_test78.test_fsmEventStruct as FT78EventStruct

// ── full_test79 (test_fsm) ─────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test79.test_fsm as FT79FSM
import io.github.fsmlang.generated.full_test79.test_fsmEvent as FT79Event
import io.github.fsmlang.generated.full_test79.test_fsmEventStruct as FT79EventStruct

// ── full_test81 (messenger) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test81.messenger as FT81FSM
import io.github.fsmlang.generated.full_test81.messengerEvent as FT81Event
import io.github.fsmlang.generated.full_test81.messengerEventStruct as FT81EventStruct

// ── full_test82 (messenger) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test82.messenger as FT82FSM
import io.github.fsmlang.generated.full_test82.messengerEvent as FT82Event
import io.github.fsmlang.generated.full_test82.messengerEventStruct as FT82EventStruct

// ── full_test83 (messenger) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test83.messenger as FT83FSM
import io.github.fsmlang.generated.full_test83.messengerEvent as FT83Event
import io.github.fsmlang.generated.full_test83.messengerEventStruct as FT83EventStruct

// ── full_test88 (test) ─────────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test88.test as FT88FSM
import io.github.fsmlang.generated.full_test88.testEvent as FT88Event
import io.github.fsmlang.generated.full_test88.testEventStruct as FT88EventStruct

// ── full_test89 (test) ─────────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test89.test as FT89FSM
import io.github.fsmlang.generated.full_test89.testEvent as FT89Event
import io.github.fsmlang.generated.full_test89.testEventStruct as FT89EventStruct

// ── full_test91 (test) ─────────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test91.test as FT91FSM
import io.github.fsmlang.generated.full_test91.testEvent as FT91Event
import io.github.fsmlang.generated.full_test91.testEventStruct as FT91EventStruct

// ── full_test92 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test92.compact as FT92FSM
import io.github.fsmlang.generated.full_test92.compactEvent as FT92Event
import io.github.fsmlang.generated.full_test92.compactEventStruct as FT92EventStruct

// ── full_test93 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test93.compact as FT93FSM
import io.github.fsmlang.generated.full_test93.compactEvent as FT93Event
import io.github.fsmlang.generated.full_test93.compactEventStruct as FT93EventStruct

// ── full_test94 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test94.compact as FT94FSM
import io.github.fsmlang.generated.full_test94.compactEvent as FT94Event
import io.github.fsmlang.generated.full_test94.compactEventStruct as FT94EventStruct

// ── full_test95 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test95.compact as FT95FSM
import io.github.fsmlang.generated.full_test95.compactEvent as FT95Event
import io.github.fsmlang.generated.full_test95.compactEventStruct as FT95EventStruct

// ── full_test96 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test96.compact as FT96FSM
import io.github.fsmlang.generated.full_test96.compactEvent as FT96Event
import io.github.fsmlang.generated.full_test96.compactEventStruct as FT96EventStruct

// ── full_test97 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test97.compact as FT97FSM
import io.github.fsmlang.generated.full_test97.compactEvent as FT97Event
import io.github.fsmlang.generated.full_test97.compactEventStruct as FT97EventStruct

// ── full_test98 (compact) ──────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test98.compact as FT98FSM
import io.github.fsmlang.generated.full_test98.compactEvent as FT98Event
import io.github.fsmlang.generated.full_test98.compactEventStruct as FT98EventStruct

// ── full_test99 (test_empty_cells) ────────────────────────────────────────
import io.github.fsmlang.generated.full_test99.test_empty_cells as FT99FSM
import io.github.fsmlang.generated.full_test99.test_empty_cellsEvent as FT99Event
import io.github.fsmlang.generated.full_test99.test_empty_cellsEventStruct as FT99EventStruct

// ── full_test100 (test_empty_cells) ───────────────────────────────────────
import io.github.fsmlang.generated.full_test100.test_empty_cells as FT100FSM
import io.github.fsmlang.generated.full_test100.test_empty_cellsEvent as FT100Event
import io.github.fsmlang.generated.full_test100.test_empty_cellsEventStruct as FT100EventStruct

// ── full_test101 (top_level) ───────────────────────────────────────────────
import io.github.fsmlang.generated.full_test101.top_level as FT101FSM
import io.github.fsmlang.generated.full_test101.top_levelEvent as FT101Event
import io.github.fsmlang.generated.full_test101.top_levelEventStruct as FT101EventStruct

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

// ── full_test114 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test114.test_fsm as FT114FSM
import io.github.fsmlang.generated.full_test114.test_fsmEvent as FT114Event
import io.github.fsmlang.generated.full_test114.test_fsmEventStruct as FT114EventStruct

// ── full_test115 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test115.test_fsm as FT115FSM
import io.github.fsmlang.generated.full_test115.test_fsmEvent as FT115Event
import io.github.fsmlang.generated.full_test115.test_fsmEventStruct as FT115EventStruct

// ── full_test116 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test116.test_fsm as FT116FSM
import io.github.fsmlang.generated.full_test116.test_fsmEvent as FT116Event
import io.github.fsmlang.generated.full_test116.test_fsmEventStruct as FT116EventStruct

// ── full_test117 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test117.test_fsm as FT117FSM
import io.github.fsmlang.generated.full_test117.test_fsmEvent as FT117Event
import io.github.fsmlang.generated.full_test117.test_fsmEventStruct as FT117EventStruct

// ── full_test118 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test118.test_fsm as FT118FSM
import io.github.fsmlang.generated.full_test118.test_fsmEvent as FT118Event
import io.github.fsmlang.generated.full_test118.test_fsmEventStruct as FT118EventStruct

// ── full_test119 (test_fsm) [covers: 124] ─────────────────────────────────
import io.github.fsmlang.generated.full_test119.test_fsm as FT119FSM
import io.github.fsmlang.generated.full_test119.test_fsmEvent as FT119Event
import io.github.fsmlang.generated.full_test119.test_fsmEventStruct as FT119EventStruct

// ── full_test120 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test120.test_fsm as FT120FSM
import io.github.fsmlang.generated.full_test120.test_fsmEvent as FT120Event
import io.github.fsmlang.generated.full_test120.test_fsmEventStruct as FT120EventStruct

// ── full_test121 (test_fsm) [covers: 145] ─────────────────────────────────
import io.github.fsmlang.generated.full_test121.test_fsm as FT121FSM
import io.github.fsmlang.generated.full_test121.test_fsmEvent as FT121Event
import io.github.fsmlang.generated.full_test121.test_fsmEventStruct as FT121EventStruct

// ── full_test122 (test_fsm) [covers: 146] ─────────────────────────────────
import io.github.fsmlang.generated.full_test122.test_fsm as FT122FSM
import io.github.fsmlang.generated.full_test122.test_fsmEvent as FT122Event
import io.github.fsmlang.generated.full_test122.test_fsmEventStruct as FT122EventStruct

// ── full_test123 (test_fsm) [covers: 147] ─────────────────────────────────
import io.github.fsmlang.generated.full_test123.test_fsm as FT123FSM
import io.github.fsmlang.generated.full_test123.test_fsmEvent as FT123Event
import io.github.fsmlang.generated.full_test123.test_fsmEventStruct as FT123EventStruct

// ── full_test125 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test125.test_fsm as FT125FSM
import io.github.fsmlang.generated.full_test125.test_fsmEvent as FT125Event
import io.github.fsmlang.generated.full_test125.test_fsmEventStruct as FT125EventStruct

// ── full_test126 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test126.test_fsm as FT126FSM
import io.github.fsmlang.generated.full_test126.test_fsmEvent as FT126Event
import io.github.fsmlang.generated.full_test126.test_fsmEventStruct as FT126EventStruct

// ── full_test127 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test127.test_fsm as FT127FSM
import io.github.fsmlang.generated.full_test127.test_fsmEvent as FT127Event
import io.github.fsmlang.generated.full_test127.test_fsmEventStruct as FT127EventStruct

// ── full_test128 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test128.test_fsm as FT128FSM
import io.github.fsmlang.generated.full_test128.test_fsmEvent as FT128Event
import io.github.fsmlang.generated.full_test128.test_fsmEventStruct as FT128EventStruct

// ── full_test129 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test129.test_fsm as FT129FSM
import io.github.fsmlang.generated.full_test129.test_fsmEvent as FT129Event
import io.github.fsmlang.generated.full_test129.test_fsmEventStruct as FT129EventStruct

// ── full_test130 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test130.test_fsm as FT130FSM
import io.github.fsmlang.generated.full_test130.test_fsmEvent as FT130Event
import io.github.fsmlang.generated.full_test130.test_fsmEventStruct as FT130EventStruct

// ── full_test131 (test_fsm) ────────────────────────────────────────────────
import io.github.fsmlang.generated.full_test131.test_fsm as FT131FSM
import io.github.fsmlang.generated.full_test131.test_fsmEvent as FT131Event
import io.github.fsmlang.generated.full_test131.test_fsmEventStruct as FT131EventStruct

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

    @Test fun `full_test18 smoke`() {
        val fsm = FT18FSM()
        fsm.dispatch(FT18EventStruct(FT18Event.e1))
    }

    @Test fun `full_test19 smoke`() {
        val fsm = FT19FSM()
        fsm.dispatch(FT19EventStruct(FT19Event.e1))
    }

    @Test fun `full_test21 smoke`() {
        val fsm = FT21FSM()
        fsm.dispatch(FT21EventStruct(FT21Event.e1))
    }

    @Test fun `full_test26 smoke`() {
        val fsm = FT26FSM()
        fsm.dispatch(FT26EventStruct(FT26Event.e1))
    }

    @Test fun `full_test27 smoke`() {
        val fsm = FT27FSM()
        fsm.dispatch(FT27EventStruct(FT27Event.e1))
    }

    @Test fun `full_test28 smoke`() {
        val fsm = FT28FSM()
        fsm.dispatch(FT28EventStruct(FT28Event.e1))
    }

    @Test fun `full_test29 smoke`() {
        val fsm = FT29FSM()
        fsm.dispatch(FT29EventStruct(FT29Event.e1))
    }

    @Test fun `full_test30 smoke`() {
        val fsm = FT30FSM()
        fsm.dispatch(FT30EventStruct(FT30Event.e1))
    }

    @Test fun `full_test31 smoke`() {
        val fsm = FT31FSM()
        fsm.dispatch(FT31EventStruct(FT31Event.e1))
    }

    @Test fun `full_test32 smoke`() {
        val fsm = FT32FSM()
        fsm.dispatch(FT32EventStruct(FT32Event.e1))
    }

    @Test fun `full_test34 smoke`() {
        val fsm = FT34FSM()
        fsm.dispatch(FT34EventStruct(FT34Event.e1))
    }

    @Test fun `full_test35 smoke`() {
        val fsm = FT35FSM()
        fsm.dispatch(FT35EventStruct(FT35Event.e1))
    }

    @Test fun `full_test37 smoke`() {
        val fsm = FT37FSM()
        fsm.dispatch(FT37EventStruct(FT37Event.e1))
    }

    @Test fun `full_test38 smoke`() {
        val fsm = FT38FSM()
        fsm.dispatch(FT38EventStruct(FT38Event.e1))
    }

    @Test fun `full_test45 smoke`() {
        val fsm = FT45FSM()
        fsm.dispatch(FT45EventStruct(FT45Event.e1))
    }

    @Test fun `full_test46 smoke`() {
        val fsm = FT46FSM()
        fsm.dispatch(FT46EventStruct(FT46Event.e1))
    }

    @Test fun `full_test47 smoke`() {
        val fsm = FT47FSM()
        fsm.dispatch(FT47EventStruct(FT47Event.e1))
    }

    @Test fun `full_test48 smoke`() {
        val fsm = FT48FSM()
        fsm.dispatch(FT48EventStruct(FT48Event.e1))
    }

    @Test fun `full_test69 smoke`() {
        val fsm1 = FT69AnotherNewMachine()
        fsm1.dispatch(FT69AnotherNewMachineEventStruct(FT69AnotherNewMachineEvent.e1))
        val fsm2 = FT69NewMachine()
        fsm2.dispatch(FT69NewMachineEventStruct(FT69NewMachineEvent.e1))
    }

    @Test fun `full_test70 smoke`() {
        val fsm = FT70FSM()
        fsm.dispatch(FT70EventStruct(FT70Event.e1))
    }

    @Test fun `full_test73 smoke`() {
        val fsm = FT73FSM()
        fsm.dispatch(FT73EventStruct(FT73Event.e1))
    }

    @Test fun `full_test74 smoke`() {
        val fsm = FT74FSM()
        fsm.dispatch(FT74EventStruct(FT74Event.e1))
    }

    @Test fun `full_test75 smoke`() {
        val fsm = FT75FSM()
        fsm.dispatch(FT75EventStruct(FT75Event.e1))
    }

    @Test fun `full_test76 smoke`() {
        val fsm = FT76FSM()
        fsm.dispatch(FT76EventStruct(FT76Event.e1))
    }

    @Test fun `full_test77 smoke`() {
        val fsm = FT77FSM()
        fsm.dispatch(FT77EventStruct(FT77Event.e1))
    }

    @Test fun `full_test78 smoke`() {
        val fsm = FT78FSM()
        fsm.dispatch(FT78EventStruct(FT78Event.e1))
    }

    @Test fun `full_test79 smoke`() {
        val fsm = FT79FSM()
        fsm.dispatch(FT79EventStruct(FT79Event.e1))
    }

    @Test fun `full_test81 smoke`() {
        val fsm = FT81FSM()
        fsm.dispatch(FT81EventStruct(FT81Event.goodMessage))
    }

    @Test fun `full_test82 smoke`() {
        val fsm = FT82FSM()
        fsm.dispatch(FT82EventStruct(FT82Event.goodMessage))
    }

    @Test fun `full_test83 smoke`() {
        val fsm = FT83FSM()
        fsm.dispatch(FT83EventStruct(FT83Event.goodMessage))
    }

    @Test fun `full_test88 smoke`() {
        val fsm = FT88FSM()
        fsm.dispatch(FT88EventStruct(FT88Event.e1))
    }

    @Test fun `full_test89 smoke`() {
        val fsm = FT89FSM()
        fsm.dispatch(FT89EventStruct(FT89Event.e1))
    }

    @Test fun `full_test91 smoke`() {
        val fsm = FT91FSM()
        fsm.dispatch(FT91EventStruct(FT91Event.e1))
    }

    @Test fun `full_test92 smoke`() {
        val fsm = FT92FSM()
        fsm.dispatch(FT92EventStruct(FT92Event.e1))
    }

    @Test fun `full_test93 smoke`() {
        val fsm = FT93FSM()
        fsm.dispatch(FT93EventStruct(FT93Event.e1))
    }

    @Test fun `full_test94 smoke`() {
        val fsm = FT94FSM()
        fsm.dispatch(FT94EventStruct(FT94Event.e1))
    }

    @Test fun `full_test95 smoke`() {
        val fsm = FT95FSM()
        fsm.dispatch(FT95EventStruct(FT95Event.e1))
    }

    @Test fun `full_test96 smoke`() {
        val fsm = FT96FSM()
        fsm.dispatch(FT96EventStruct(FT96Event.e1))
    }

    @Test fun `full_test97 smoke`() {
        val fsm = FT97FSM()
        fsm.dispatch(FT97EventStruct(FT97Event.e1))
    }

    @Test fun `full_test98 smoke`() {
        val fsm = FT98FSM()
        fsm.dispatch(FT98EventStruct(FT98Event.e1))
    }

    @Test fun `full_test99 smoke`() {
        val fsm = FT99FSM()
        fsm.dispatch(FT99EventStruct(FT99Event.e1))
    }

    @Test fun `full_test100 smoke`() {
        val fsm = FT100FSM()
        fsm.dispatch(FT100EventStruct(FT100Event.e1))
    }

    @Test fun `full_test101 smoke`() {
        val fsm = FT101FSM()
        fsm.dispatch(FT101EventStruct(FT101Event.e1))
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

    @Test fun `full_test114 smoke`() {
        val fsm = FT114FSM()
        fsm.dispatch(FT114EventStruct(FT114Event.e1))
    }

    @Test fun `full_test115 smoke`() {
        val fsm = FT115FSM()
        fsm.dispatch(FT115EventStruct(FT115Event.e1))
    }

    @Test fun `full_test116 smoke`() {
        val fsm = FT116FSM()
        fsm.dispatch(FT116EventStruct(FT116Event.e1))
    }

    @Test fun `full_test117 smoke`() {
        val fsm = FT117FSM()
        fsm.dispatch(FT117EventStruct(FT117Event.e1))
    }

    @Test fun `full_test118 smoke`() {
        val fsm = FT118FSM()
        fsm.dispatch(FT118EventStruct(FT118Event.e1))
    }

    @Test fun `full_test119 smoke`() {
        val fsm = FT119FSM()
        fsm.dispatch(FT119EventStruct(FT119Event.e1))
    }

    @Test fun `full_test120 smoke`() {
        val fsm = FT120FSM()
        fsm.dispatch(FT120EventStruct(FT120Event.e1))
    }

    @Test fun `full_test121 smoke`() {
        val fsm = FT121FSM()
        fsm.dispatch(FT121EventStruct(FT121Event.e1))
    }

    @Test fun `full_test122 smoke`() {
        val fsm = FT122FSM()
        fsm.dispatch(FT122EventStruct(FT122Event.e1))
    }

    @Test fun `full_test123 smoke`() {
        val fsm = FT123FSM()
        fsm.dispatch(FT123EventStruct(FT123Event.e1))
    }

    @Test fun `full_test125 smoke`() {
        val fsm = FT125FSM()
        fsm.dispatch(FT125EventStruct(FT125Event.e1))
    }

    @Test fun `full_test126 smoke`() {
        val fsm = FT126FSM()
        fsm.dispatch(FT126EventStruct(FT126Event.e1))
    }

    @Test fun `full_test127 smoke`() {
        val fsm = FT127FSM()
        fsm.dispatch(FT127EventStruct(FT127Event.e1))
    }

    @Test fun `full_test128 smoke`() {
        val fsm = FT128FSM()
        fsm.dispatch(FT128EventStruct(FT128Event.e1))
    }

    @Test fun `full_test129 smoke`() {
        val fsm = FT129FSM()
        fsm.dispatch(FT129EventStruct(FT129Event.e1))
    }

    @Test fun `full_test130 smoke`() {
        val fsm = FT130FSM()
        fsm.dispatch(FT130EventStruct(FT130Event.e1))
    }

    @Test fun `full_test131 smoke`() {
        val fsm = FT131FSM()
        fsm.dispatch(FT131EventStruct(FT131Event.e1))
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
