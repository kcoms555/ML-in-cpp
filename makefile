RUNNER_SOURCE = source/runner.cpp
TRAINER_SOURCE = source/trainer.cpp

RUNNER = bin/runner
TRAINER = bin/trainer

TARGET = $(RUNNER) $(TRAINER)

GCC = g++


all: $(TARGET)

$(RUNNER): $(RUNNER_SOURCE)
	$(GCC) -o $(RUNNER) $(RUNNER_SOURCE)

$(TRAINER): $(TRAINER_SOURCE)
	$(GCC) -o $(TRAINER) $(TRAINER_SOURCE)

clean:
	rm $(RUNNER) $(TRAINER)

.PHONY: all clean
