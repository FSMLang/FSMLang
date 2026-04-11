##########################################
#
# Makefile for Kotlin smoke tests
#
# Drives: generate .kt from full_test144, copy into Gradle project, run JUnit 5.
#

.PHONY: runtest clean

FSM ?= $(OUTPUT_DIR)/fsm

# Source FSM lives in the sibling full_test144 directory
FULL_TEST_DIR = ../full_test144
GENERATED_KT  = $(FULL_TEST_DIR)/test_fsm.kt

# Destination inside the Gradle source tree (main source set)
GRADLE_SRC_DIR = src/main/kotlin/io/github/fsmlang/generated/test_fsm

runtest: $(GENERATED_KT)
	mkdir -p $(GRADLE_SRC_DIR)
	cp $(GENERATED_KT) $(GRADLE_SRC_DIR)/
	./gradlew test
	@echo "kotlin smoke tests pass"

$(GENERATED_KT): $(FULL_TEST_DIR)/test_fsm.fsm $(FSM)
	cd $(FULL_TEST_DIR) && $(FSM) -tk test_fsm.fsm

clean:
	-@rm -f $(GENERATED_KT)
	-@rm -rf $(GRADLE_SRC_DIR)
	-@rm -rf build .gradle
