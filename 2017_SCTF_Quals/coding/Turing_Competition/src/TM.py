import json
def getInput():
    line = input('getInput> ')
    obj = json.loads(line)
    transitions = {}
    for c_state, char_head, n_state, char_push, mov_dir in obj['transition_function']:
        transitions[(c_state, char_head)] = (n_state, char_push, mov_dir)
    return TM(obj['initial_state'], obj['final_states'], transitions, obj['accepting_states']), line

class Tape():
    BLANK = ' '
    def __init__(self, tape):
        self.tape = dict(enumerate(tape))

    def __getitem__(self, idx):
        if idx in self.tape:
            return self.tape[idx]
        else:
            return self.BLANK

    def __setitem__(self, idx, v):
        self.tape[idx] = v

class OutOfStepException(Exception):
    pass
class IllegalDirectionException(Exception):
    pass

class TM():
    def __init__(self, init_state, final_states, transitions, accept_states):
        self.tape = Tape('')
        self.head = 0
        self.init_state = self.state = init_state
        self.final_states = set(final_states)
        self.accept_states = set(accept_states)
        self.transitions = transitions

    def step(self):
        char_at_head = self.tape[self.head]
        x = (self.state, char_at_head)
        if x in self.transitions:
            y = self.transitions[x]
            self.tape[self.head] = y[1]
            if y[2] == 'R':
                self.head += 1
            elif y[2] == 'L':
                self.head -= 1
            else:
                raise IllegalDirectionException()
            self.state = y[0]
            return True
        return False

    def run(self, tape):
        self.head = 0
        self.state = self.init_state
        self.tape = Tape(tape)

        count = 0
        while not self.state in self.final_states:
            if count >= 1000000:
                raise OutOfStepException()
            elif self.step():
                count = count + 1
            else:
                return False

        return self.state in self.accept_states

