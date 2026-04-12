##########################################
#
# Makefile for Kotlin smoke tests
#
# Drives:
#   1. Generate .kt for full_test144 (package test_fsm) — for TestFsmSmokeTest.kt
#   2. Generate .kt for all other eligible full_test directories (package full_testN)
#   3. Copy every generated .kt into the Gradle source tree
#   4. Run JUnit 5 via ./gradlew test
#

.PHONY: runtest clean

FSM      ?= $(OUTPUT_DIR)/fsm
PKG_BASE := io.github.fsmlang.generated
GRADLE_MAIN := src/main/kotlin/io/github/fsmlang/generated

# Accumulator lists populated by GEN_RULE invocations below
ALL_COPY_TARGETS :=
ALL_GEN_KT       :=
ALL_DEST_DIRS    :=

#── full_test144 (legacy — TestFsmSmokeTest.kt imports from package test_fsm) ─
FT144_KT   := ../full_test144/test_fsm.kt
FT144_DEST := $(GRADLE_MAIN)/test_fsm

$(FT144_KT): ../full_test144/test_fsm.fsm $(FSM)
	# fsm -tk produces <stem>.kt in the working directory; here that is test_fsm.kt
	cd ../full_test144 && $(FSM) -tk test_fsm.fsm

$(FT144_DEST)/test_fsm.kt: $(FT144_KT)
	@mkdir -p $(FT144_DEST)
	cp $< $@

#── macro: emit generate + copy rules for one .fsm file ──────────────────────
# Usage: $(eval $(call GEN_RULE,N,file.fsm))
# Generates ../full_testN/<stem>.kt and copies to $(GRADLE_MAIN)/full_testN/
define GEN_RULE
ALL_COPY_TARGETS += $(GRADLE_MAIN)/full_test$(1)/$(basename $(2)).kt
ALL_GEN_KT       += ../full_test$(1)/$(basename $(2)).kt
ALL_DEST_DIRS    += $(GRADLE_MAIN)/full_test$(1)

$(GRADLE_MAIN)/full_test$(1)/$(basename $(2)).kt: ../full_test$(1)/$(2) $$(FSM)
	@mkdir -p $(GRADLE_MAIN)/full_test$(1)
	# fsm -tk writes <stem>.kt alongside the .fsm in the working directory
	cd ../full_test$(1) && $$(FSM) -tk --kotlin-package=$(PKG_BASE).full_test$(1) $(2)
	cp ../full_test$(1)/$(basename $(2)).kt $$@

endef

#── single-fsm directories ────────────────────────────────────────────────────
$(eval $(call GEN_RULE,1,parser-test2.fsm))
$(eval $(call GEN_RULE,2,parser-test4.fsm))
$(eval $(call GEN_RULE,3,parser-test5.fsm))
$(eval $(call GEN_RULE,5,parser-test9.fsm))
$(eval $(call GEN_RULE,6,parser-test11.fsm))
$(eval $(call GEN_RULE,7,parser-test11.fsm))
$(eval $(call GEN_RULE,8,parser-test12.fsm))
$(eval $(call GEN_RULE,9,parser-test12.fsm))
$(eval $(call GEN_RULE,10,parser-test12.fsm))
$(eval $(call GEN_RULE,11,parser-test12.fsm))
$(eval $(call GEN_RULE,13,parser-test5.fsm))
$(eval $(call GEN_RULE,14,parser-test5.fsm))
$(eval $(call GEN_RULE,15,parser-test5.fsm))
$(eval $(call GEN_RULE,16,parser-test12.fsm))
$(eval $(call GEN_RULE,17,flashdriver.fsm))
$(eval $(call GEN_RULE,18,full-test18.fsm))
$(eval $(call GEN_RULE,19,top_level.fsm))
$(eval $(call GEN_RULE,20,top_level.fsm))
$(eval $(call GEN_RULE,21,top_level.fsm))
$(eval $(call GEN_RULE,22,top_level.fsm))
$(eval $(call GEN_RULE,23,top_level.fsm))
$(eval $(call GEN_RULE,24,top_level.fsm))
$(eval $(call GEN_RULE,25,top_level.fsm))
$(eval $(call GEN_RULE,26,top_level.fsm))
$(eval $(call GEN_RULE,27,top_level.fsm))
$(eval $(call GEN_RULE,28,top_level.fsm))
$(eval $(call GEN_RULE,29,top_level.fsm))
$(eval $(call GEN_RULE,30,top_level.fsm))
$(eval $(call GEN_RULE,31,top_level.fsm))
$(eval $(call GEN_RULE,32,top_level.fsm))
$(eval $(call GEN_RULE,33,top_level.fsm))
$(eval $(call GEN_RULE,34,top_level.fsm))
$(eval $(call GEN_RULE,35,top_level.fsm))
$(eval $(call GEN_RULE,36,top_level.fsm))
$(eval $(call GEN_RULE,37,top_level.fsm))
$(eval $(call GEN_RULE,38,top_level.fsm))
$(eval $(call GEN_RULE,45,top_level.fsm))
$(eval $(call GEN_RULE,46,top_level.fsm))
$(eval $(call GEN_RULE,47,top_level.fsm))
$(eval $(call GEN_RULE,48,top_level.fsm))
$(eval $(call GEN_RULE,49,top_level.fsm))
$(eval $(call GEN_RULE,55,top_level.fsm))
$(eval $(call GEN_RULE,70,top_level.fsm))
$(eval $(call GEN_RULE,72,top_level.fsm))
$(eval $(call GEN_RULE,73,test_fsm.fsm))
$(eval $(call GEN_RULE,74,test_fsm.fsm))
$(eval $(call GEN_RULE,75,test_fsm.fsm))
$(eval $(call GEN_RULE,76,test_fsm.fsm))
$(eval $(call GEN_RULE,77,test_fsm.fsm))
$(eval $(call GEN_RULE,78,test_fsm.fsm))
$(eval $(call GEN_RULE,79,test_fsm.fsm))
$(eval $(call GEN_RULE,81,parser-test2.fsm))
$(eval $(call GEN_RULE,82,parser-test2.fsm))
$(eval $(call GEN_RULE,83,parser-test2.fsm))
$(eval $(call GEN_RULE,86,top_level.fsm))
$(eval $(call GEN_RULE,87,top_level.fsm))
$(eval $(call GEN_RULE,88,test_fsm.fsm))
$(eval $(call GEN_RULE,89,test_fsm.fsm))
$(eval $(call GEN_RULE,91,test_fsm.fsm))
$(eval $(call GEN_RULE,92,compact.fsm))
$(eval $(call GEN_RULE,93,compact.fsm))
$(eval $(call GEN_RULE,94,compact.fsm))
$(eval $(call GEN_RULE,95,compact.fsm))
$(eval $(call GEN_RULE,96,compact.fsm))
$(eval $(call GEN_RULE,97,compact.fsm))
$(eval $(call GEN_RULE,98,compact.fsm))
$(eval $(call GEN_RULE,99,test_empty_cells.fsm))
$(eval $(call GEN_RULE,100,test_empty_cells.fsm))
$(eval $(call GEN_RULE,101,top_level.fsm))
$(eval $(call GEN_RULE,102,top_level.fsm))
$(eval $(call GEN_RULE,103,top_level.fsm))
$(eval $(call GEN_RULE,109,test_fsm.fsm))
$(eval $(call GEN_RULE,110,test_fsm.fsm))
$(eval $(call GEN_RULE,111,test_fsm.fsm))
$(eval $(call GEN_RULE,112,test_fsm.fsm))
$(eval $(call GEN_RULE,113,test_fsm.fsm))
$(eval $(call GEN_RULE,114,test_fsm.fsm))
$(eval $(call GEN_RULE,115,test_fsm.fsm))
$(eval $(call GEN_RULE,116,test_fsm.fsm))
$(eval $(call GEN_RULE,117,test_fsm.fsm))
$(eval $(call GEN_RULE,118,test_fsm.fsm))
$(eval $(call GEN_RULE,119,test_fsm.fsm))
$(eval $(call GEN_RULE,120,test_fsm.fsm))
$(eval $(call GEN_RULE,121,test_fsm.fsm))
$(eval $(call GEN_RULE,122,test_fsm.fsm))
$(eval $(call GEN_RULE,123,test_fsm.fsm))
$(eval $(call GEN_RULE,124,test_fsm.fsm))
$(eval $(call GEN_RULE,125,test_fsm.fsm))
$(eval $(call GEN_RULE,126,test_fsm.fsm))
$(eval $(call GEN_RULE,127,test_fsm.fsm))
$(eval $(call GEN_RULE,128,test_fsm.fsm))
$(eval $(call GEN_RULE,129,test_fsm.fsm))
$(eval $(call GEN_RULE,130,test_fsm.fsm))
$(eval $(call GEN_RULE,131,test_fsm.fsm))
$(eval $(call GEN_RULE,135,test_fsm.fsm))
$(eval $(call GEN_RULE,136,sanitized.fsm))
$(eval $(call GEN_RULE,137,test_fsm.fsm))
$(eval $(call GEN_RULE,138,test_fsm.fsm))
$(eval $(call GEN_RULE,139,issue.fsm))
$(eval $(call GEN_RULE,142,test_fsm.fsm))
$(eval $(call GEN_RULE,143,test_fsm.fsm))
$(eval $(call GEN_RULE,145,test_fsm.fsm))
$(eval $(call GEN_RULE,146,test_fsm.fsm))
$(eval $(call GEN_RULE,147,test_fsm.fsm))

#── multi-fsm directories (both .fsm files go into the same package) ─────────
$(eval $(call GEN_RULE,69,another.fsm))
$(eval $(call GEN_RULE,69,parser-test13.fsm))
$(eval $(call GEN_RULE,84,another.fsm))
$(eval $(call GEN_RULE,84,parser-test13.fsm))
$(eval $(call GEN_RULE,85,another.fsm))
$(eval $(call GEN_RULE,85,parser-test13.fsm))

#── top-level targets (must appear after all $(eval) calls) ──────────────────
runtest: $(FT144_DEST)/test_fsm.kt $(ALL_COPY_TARGETS)
	./gradlew test
	@echo "kotlin smoke tests pass"

clean:
	-@rm -f $(FT144_KT) $(ALL_GEN_KT)
	-@rm -rf $(FT144_DEST) $(sort $(ALL_DEST_DIRS))
	-@rm -rf build .gradle
